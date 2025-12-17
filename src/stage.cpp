#include "actor.hpp"
#include "config.hpp"
#include "stage.hpp"
#include "tile.hpp"
#include "utils/cache.hpp"
#include "utils/object.hpp"
#include "utils/string-utils.hpp"

#include <cstddef>
#include <deque>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define _PROPERTIES_SECTION_HEADER "[PROPERTIES]"
#define _MARKERS_SECTION_HEADER "[MARKERS]"
#define _MODIFIERS_SECTION_HEADER "[MODIFIERS]"
#define _OCCUPANTS_SECTION_HEADER "[OCCUPANTS]"

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
    ) : CacheEntry(id) {
        this->_filePath = filePath;
        this->_rowCount = rowCount;
        this->_columnCount = columnCount;
        
        this->setName(name);
        
        std::size_t k = 0; // marker & isBarrier index
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
    
    std::string Stage::filePath(void) {
        return this->_filePath;
    }
    
    std::string Stage::name(void) {
        return this->_name;
    }
    
    std::size_t Stage::size(void) {
        return this->rowCount() * this->columnCount();
    }
    
    std::size_t Stage::rowCount(void) {
        return this->_rowCount;
    }
    
    std::size_t Stage::columnCount(void) {
        return this->_columnCount;
    }
    
    bool Stage::inBounds(std::size_t row, std::size_t column) {
        return row < this->rowCount() && column < this->columnCount();
    }
    
    Tile* Stage::tileAt(std::size_t row, std::size_t column) {        
        // return the null pointer if row & column are out-of-bounds
        if (!this->inBounds(row, column)) {
            return nullptr;
        }
        
        std::size_t index = row * this->columnCount() + column; // 1-dimensional index
        return &this->_tiles[index];
    }
    
    std::vector<Actor*> Stage::occupants(void) {
        std::vector<Actor*> occupants;
        for (std::pair<unsigned long, std::string> info : this->_occupantInfo) {
            Actor* occupant = Actor::select(info.first);
            if (occupant != nullptr) {
                occupants.push_back(occupant);
            }
        }
        
        return occupants;
    }
    
    std::size_t Stage::occupantCount(void) {
        return this->_occupantInfo.size();
    }
    
    bool Stage::containsOccupant(Actor *actor) {
        if (actor == nullptr) {
            return false;
        }
    
        std::pair<unsigned long, std::string> info(
            actor->id(), 
            actor->filePath()
        );
        
        return this->_occupantInfo.find(info) != this->_occupantInfo.end();
    }

// ====================================================================================================
// | MODIFIERS |
// =============

    void Stage::setName(const std::string& name) {
        this->_name = name;
    }
    
    void Stage::addOccupant(Actor *occupant) {
        if (occupant != nullptr) {
            this->_occupantInfo.emplace(occupant->id(), occupant->filePath());
        }
    }
    
    void Stage::removeOccupant(Actor *occupant) {
        std::pair<unsigned long, std::string> info(
            occupant->id(), 
            occupant->filePath()
        );
        
        this->_occupantInfo.erase(info);
    }
    
// ====================================================================================================
// | CONVERTERS |
// ==============

    std::string Stage::toString(void) {
        std::string str;
        for (std::size_t i = 0; i < this->rowCount(); i++) {
            for (std::size_t j = 0; j < this->columnCount(); j++) {
                str += this->tileAt(i, j)->toString();
            }
            
            // separate rows using a newline
            if (i < this->rowCount()-1) {
                str += "\n";
            }
        }
        
        return str;
    }
    
// ====================================================================================================
// | LOGISTICS |
// =============
    
    Cache<Stage, STAGE_CACHE_CAP> Stage::_cache;

    Stage* Stage::load(const std::string& filePath) {
        // attempt to read into an object
        Object object;
        if (!object.read(filePath)) {
            return nullptr;
        }
        
        unsigned long id;
        std::string name;
        std::size_t rowCount;
        std::size_t columnCount;
        std::string markers = "";
        std::string modifiers = "";
        std::vector<std::pair<unsigned long, std::string>> actorInfo;
        
        for (const ObjectSection& section : object) {
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
            
            // process the occupant information section
            else if (header == _OCCUPANTS_SECTION_HEADER) {
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
            Stage::unload(Stage::_cache.front());
        }
        
        // store the stage
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
            Actor* occupant = Actor::select(pair.first);
            if (occupant == nullptr) {
                occupant = Actor::load(pair.second);
                if (occupant == nullptr) {
                    continue;
                }
            }
            
            stage->addOccupant(occupant);
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
    
        Object object;
        std::vector<std::string> properties;
        std::vector<std::string> markers;
        std::vector<std::string> modifiers;
        std::vector<std::string> occupantInfo;
        
        // append properties
        properties.push_back("id = " + std::to_string(stage->id()));
        properties.push_back("name = " + stage->name());
        properties.push_back("row-count = " + std::to_string(stage->rowCount()));
        properties.push_back("column-count = " + std::to_string(stage->columnCount()));
        
        // append each row of markers & modifiers
        for (std::size_t i = 0; i < stage->rowCount(); i++) {
            std::string markerRow;
            std::string modifierRow;
        
            for (std::size_t j = 0; j < stage->columnCount(); j++) {
                Tile* tile = stage->tileAt(i, j);
                markerRow += std::string(1, tile->marker());
                modifierRow += std::string(1, tile->modifier());
            }
            
            markers.push_back(markerRow);
            modifiers.push_back(modifierRow);
        }
        
        // append each row of actor info
        for (std::pair<unsigned long, std::string> info : stage->_occupantInfo) {
            occupantInfo.push_back(std::to_string(info.first) + ", " + info.second);
        }
        
        // map headers to entries
        object[_PROPERTIES_SECTION_HEADER] = properties;
        object[_MARKERS_SECTION_HEADER] = markers;
        object[_MODIFIERS_SECTION_HEADER] = modifiers;
        object[_OCCUPANTS_SECTION_HEADER] = occupantInfo;
        
        // write the object, return whether the write was successful
        return object.write(stage->filePath());
    }
}

