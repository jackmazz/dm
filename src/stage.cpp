#include "actor.hpp"
#include "config.hpp"
#include "stage.hpp"
#include "tile.hpp"
#include "utils/cache.hpp"
#include "utils/dmo-utils.hpp"
#include "utils/string-utils.hpp"

#include <cstddef>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define _PROPERTIES_SECTION_HEADER "[PROPERTIES]"
#define _MARKERS_SECTION_HEADER "[MARKERS]"
#define _MODIFIERS_SECTION_HEADER "[MODIFIERS]"
#define _ACTORS_SECTION_HEADER "[ACTORS]"

namespace dm {

// ====================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Stage::Stage(void) : Stage(0, "", 0, 0) {}
    
    Stage::Stage(
        unsigned long id,
        const std::string& filePath,
        std::size_t rowCount, 
        std::size_t columnCount
    ) : Stage(id, filePath, "", rowCount, columnCount, "", "") {}
    
    Stage::Stage(
        unsigned long id,
        const std::string& filePath,
        const std::string& name,
        std::size_t rowCount, 
        std::size_t columnCount,
        const std::string& markers,
        const std::string& modifiers
    ) : Asset(id, filePath, name) {
        this->_rowCount = rowCount;
        this->_columnCount = columnCount;
        
        std::size_t k = 0; // marker & modifier index
        for (std::size_t i = 0; i < rowCount; i++) {
            for (std::size_t j = 0; j < columnCount; j++) {                
                // use default marker if not available for index k
                char marker = '\0';
                if (k < markers.length()) {
                    marker = markers[k];
                }
                
                // use floor modifier if not available for index k
                char modifier = DM_TILE_MODIFIER_FLOOR;
                if (k < modifiers.length()) {
                    modifier = modifiers[k];
                }
                
                // append tile to tile vector
                this->_tiles.emplace_back(
                    this,
                    i, j,
                    marker,
                    modifier
                );
                k++;
            }
        }
    }
    
    Stage::~Stage(void) {}

// ====================================================================================================
// | ACCESSORS |
// =============
    
    std::size_t Stage::getSize(void) const {
        return this->getRowCount() * this->getColumnCount();
    }
    
    std::size_t Stage::getRowCount(void) const {
        return this->_rowCount;
    }
    
    std::size_t Stage::getColumnCount(void) const {
        return this->_columnCount;
    }
    
    bool Stage::isInBounds(std::size_t row, std::size_t column) const {
        return row < this->getRowCount() && column < this->getColumnCount();
    }
    
    const Tile* Stage::getTile(std::size_t row, std::size_t column) const {        
        // return the null pointer if row & column are out-of-bounds
        if (!this->isInBounds(row, column)) {
            return nullptr;
        }
        
        std::size_t index = row * this->getColumnCount() + column; // 1-dimensional index
        return &this->_tiles[index];
    }
    
    Tile* Stage::getTile(std::size_t row, std::size_t column) {        
        return const_cast<Tile*>(
            static_cast<const Stage*>(this)->getTile(row, column)
        );
    }

// ====================================================================================================
// | MODIFIERS |
// =============

    void Stage::addActor(Actor *actor) {
        if (actor != nullptr) {
            this->_actorInfo.emplace(actor->getId(), actor->getFilePath());
        }
    }
    
    void Stage::removeActor(Actor *actor) {
        this->_actorInfo.erase(std::make_pair(
            actor->getId(), 
            actor->getFilePath()
        ));
    }
    
// ====================================================================================================
// | CONVERTERS |
// ==============

    std::string Stage::toString(void) const {
        std::string string;
        std::string newLine;
        
        for (std::size_t i = 0; i < this->getRowCount(); i++) {
            // separate rows using a newline
            string += newLine;
            newLine = "\n";
            
            for (std::size_t j = 0; j < this->getColumnCount(); j++) {
                string += this->getTile(i, j)->toString();
            }
        }
        
        return string;
    }
    
    DMO Stage::toDMO(void) const {
        DMO dmo;
        std::vector<std::string> properties;
        std::vector<std::string> markers;
        std::vector<std::string> modifiers;
        std::vector<std::string> actorInfo;
        
        // append properties
        properties.push_back("id = " + std::to_string(stage->getId()));
        properties.push_back("name = " + stage->getName());
        properties.push_back("row-count = " + std::to_string(stage->getRowCount()));
        properties.push_back("column-count = " + std::to_string(stage->getColumnCount()));
        
        // append each row of markers & modifiers
        for (std::size_t i = 0; i < stage->getRowCount(); i++) {
            std::string markerRow;
            std::string modifierRow;
        
            for (std::size_t j = 0; j < stage->getColumnCount(); j++) {
                Tile* tile = stage->getTile(i, j);
                markerRow += std::string(1, tile->getMarker());
                modifierRow += std::string(1, tile->getModifier());
            }
            
            markers.push_back(markerRow);
            modifiers.push_back(modifierRow);
        }
        
        // append each row of actor info
        for (std::pair<unsigned long, std::string> info : stage->_actorInfo) {
            actorInfo.push_back(std::to_string(info.first) + ", " + info.second);
        }
        
        // map headers to entries
        dmo[_PROPERTIES_SECTION_HEADER] = properties;
        dmo[_MARKERS_SECTION_HEADER] = markers;
        dmo[_MODIFIERS_SECTION_HEADER] = modifiers;
        dmo[_ACTORS_SECTION_HEADER] = actorInfo;
        
        return dmo;
    }
    
// ====================================================================================================
// | LOGISTICS |
// =============
    
    Cache<Stage, STAGE_CACHE_CAP> Stage::_cache;

    Stage* Stage::load(const std::string& filePath) {
        // attempt to read into a dmo
        DMO dmo;
        if (!dmoRead(filePath)) {
            return nullptr;
        }
        
        unsigned long id;
        std::string name;
        std::size_t rowCount;
        std::size_t columnCount;
        std::string markers = "";
        std::string modifiers = "";
        std::vector<std::pair<unsigned long, std::string>> actorInfo;
        
        for (const DMOSection& section : dmo) {
            const std::string& header = section.first;
            const std::vector<std::string>& entries = section.second;
        
            // process the properties section
            if (header == _PROPERTIES_SECTION_HEADER) {
                std::unordered_map<std::string, std::string> properties;
                for (const std::string& entry : entries) {
                    std::vector<std::string> split = splitString(entry, "=", 2);
                    if (split.size() != 2) {
                        continue;
                    }
                    
                    std::string key = trimString(split[0]);
                    std::string value = trimString(split[1]);
                    properties[key] = value;
                }
                
                // attempt to parse the loaded properties
                try {
                    id = stol(properties["id"]);
                    name = properties["name"];
                    rowCount = stol(properties["row-count"]);
                    columnCount = stol(properties["column-count"]);
                }
                
                // make sure missing or invalid properties don't cause a crash
                catch (const std::invalid_argument& exception) {
                    return nullptr;
                } catch (const std::out_of_range& exception) {
                    return nullptr;
                }
            }
            
            // process the markers section
            else if (header == _MARKERS_SECTION_HEADER) {
                for (const std::string& entry: entries) {
                    markers += entry;
                }
            }
            
            // process the modifers section
            else if (header == _MODIFIERS_SECTION_HEADER) {
                for (const std::string& entry: entries) {
                    modifiers += entry;
                }
            }
            
            // process the actor information section
            else if (header == _ACTORS_SECTION_HEADER) {
                for (const std::string& entry: entries) {
                    std::vector<std::string> split = splitString(entry, ",", 2);
                    if (split.size() != 2) {
                        continue;
                    }
                    
                    // attempt to parse the actor information
                    try {
                        unsigned long id = stol(trimString(split[0]));
                        std::string filePath = trimString(split[1]);
                        
                        actorInfo.emplace_back(id, filePath);
                    }
                    
                    // make sure invalid information doesn't cause a crash
                    catch (const std::invalid_argument& exception) {
                        return nullptr;
                    }
                }
            }
        }
        
        // unload a stage if the cache exceeds it's capacity
        if (Stage::_cache.isFull()) {
            Stage::unload(Stage::_cache.getFront());
        }
        
        // store this stage
        Stage* stage = Stage::_cache.store(
            id,
            filePath,
            name,
            rowCount,
            columnCount,
            markers,
            modifiers
        );
        
        for (std::pair<unsigned long, std::string> pair : actorInfo) {
            // attempt to select the actor, load if necessary
            Actor* actor = Actor::select(pair.first);
            if (actor == nullptr) {
                actor = Actor::load(pair.second);
                if (actor == nullptr) {
                    continue;
                }
            }
            
            stage->addActor(actor);
        }
        
        return stage;
    }

    Stage* Stage::select(unsigned long id) {
        return Stage::_cache.select(id);
    }
    
    bool Stage::unload(Stage* stage) {
        if (stage == nullptr) {
            return false;
        }
        
        // convert the stage to a DMO
        DMO dmo = stage->toDMO();
        
        // write the dmo, return whether the write was successful
        return dmoWrite(stage->getFilePath());
    }
}

