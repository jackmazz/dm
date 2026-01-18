#include "actor.hpp"
#include "asset.hpp"
#include "config.hpp"
#include "stage.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "utils/cache.hpp"
#include "utils/schema.hpp"
#include "utils/string-tools.hpp"

#include <cstddef>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#define _PROPERTIES_SECTION_HEADER "[PROPERTIES]"
#define _MARKERS_SECTION_HEADER "[MARKERS]"
#define _EFFECTS_SECTION_HEADER "[EFFECTS]"
#define _UNITS_SECTION_HEADER "[UNITS]"

namespace dm {

// ================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Stage::Stage(void)
        : Stage(0, "", 0, 0)
    {}

    Stage::Stage(
        unsigned long id, 
        const std::string& filePath,
        std::size_t rowCount, 
        std::size_t columnCount
    ) 
        : Stage(
            id, filePath, "", 
            rowCount, columnCount, 
            "", ""
        )
    {}

    Stage::Stage(
        unsigned long id, 
        const std::string& filePath,
        const std::string& name, 
        std::size_t rowCount, 
        std::size_t columnCount, 
        const std::string& markers, 
        const std::string& effects
    ) 
        : Asset(id, filePath, name)
    {
        this->_rowCount = rowCount;
        this->_columnCount = columnCount;

        std::size_t k = 0; // marker & effect index
        for (std::size_t i = 0; i < rowCount; i++) {
            for (std::size_t j = 0; j < columnCount; j++) {
                // use default marker if not available for index k
                char marker = '\0';
                if (k < markers.length()) {
                    marker = markers[k];
                }

                // use floor effect if not available for index k
                char effect = DM_TILE_FLOOR_EFFECT;
                if (k < effects.length()) {
                    effect = effects[k];
                }

                // append tile to tile vector
                this->_tiles.emplace_back(this, i, j, marker, effect);
                k++;
            }
        }
    }

// ================================================================================================
// | ACCESSORS |
// =============

    unsigned long Stage::getTypeId(void) const {
        return DM_STAGE_TYPE_ID;
    }
    
    std::string Stage::getTypeName(void) const {
        return DM_STAGE_TYPE_NAME;
    }
    
    bool Stage::checkType(const Asset* asset) {
        if (asset == nullptr) {
            return false;
        }
    
        return asset->getTypeId() == DM_STAGE_TYPE_ID;
    }
    
    bool Stage::checkType(const Asset::Contact& contact) {
        return contact.typeId == DM_STAGE_TYPE_ID;
    }
    
    const Stage* Stage::cast(const Asset* asset) {
        if (asset == nullptr) {
            return nullptr;
        }
        
        // if the type id matches, we can safely cast
        if (Stage::checkType(asset)) {
            return static_cast<const Stage*>(asset);
        }
        
        return nullptr;
    }
    
    Stage* Stage::cast(Asset* asset) {
        return const_cast<Stage*>(
            Stage::cast(static_cast<const Asset*>(asset))
        );
    }

    std::size_t Stage::getSize(void) const {
        return this->getRowCount() * this->getColumnCount();
    }

    std::size_t Stage::getRowCount(void) const {
        return this->_rowCount;
    }

    std::size_t Stage::getColumnCount(void) const {
        return this->_columnCount;
    }
    
    bool Stage::isInBounds(Tile::Position position) const {
        return this->isInBounds(position.row, position.column);
    }

    bool Stage::isInBounds(std::size_t row, std::size_t column) const {
        return row < this->getRowCount() && column < this->getColumnCount();
    }
    
    const Tile* Stage::getTile(const Tile::Position& position) const {
        return this->getTile(position.row, position.column);
    }

    const Tile* Stage::getTile(std::size_t row, std::size_t column) const {
        // return the null pointer if row & column are out-of-bounds
        if (!this->isInBounds(row, column)) {
            return nullptr;
        }
        
        // access the tile using a 1-dimensional index
        return &this->_tiles[this->_linearIndex(row, column)];
    }
    
    Tile* Stage::getTile(const Tile::Position& position) {
        return this->getTile(position.row, position.column);
    }

    Tile* Stage::getTile(std::size_t row, std::size_t column) {
        return const_cast<Tile*>(
            static_cast<const Stage*>(this)->getTile(row, column)
        );
    }
    
    /*
    std::vector<const Actor*> Stage::getActors(void) const {
        // accumulate all actors which are currently loaded
        std::vector<const Actor*> actors;
        for (unsigned long id : this->_actorState) { WRONG
            const Actor* actor = Actor::search(id);
            if (actor != nullptr) {
                actors.push_back(actor);
            }
        }

        return actors;
    }

    std::vector<Actor*> Stage::getActors(void) {
        // accumulate all actors which are currently loaded
        std::vector<Actor*> actors;
        for (unsigned long id : this->_actorState) { WRONG
            Actor* actor = Actor::search(id);
            if (actor != nullptr) {
                actors.push_back(actor);
            }
        }

        return actors;
    }
    */
    
    std::set<Asset::Contact> Stage::getContacts(void) const {
        std::set<Asset::Contact> contacts;
        for (Stage::_LinkMapping link : this->_links) {
            contacts.insert(link.first);
        }
        
        return contacts;
    }
    
    const Tile* Stage::getLinkedTile(const Asset::Contact& contact) const {
        if (!this->isLinked(contact)) {
            return nullptr;
        }
        
        Tile::Position position = this->_links[contact];
        
        // make sure the contact's position is in bounds
        if (!this->isInBounds(position)) {
            return nullptr;
        }
        
        return this->getTile(position);
    }
    
    Tile* Stage::getLinkedTile(const Asset::Contact& contact) {
        return const_cast<Tile*>(
            static_cast<const Stage*>(this)->getLinkedTile(contact)
        );
    }
    
    bool Stage::isLinked(const Unit* unit) const {
        if (unit == nullptr) {
            return false;
        }
        
        return this->isLinked(unit->getContact());
    }
    
    bool Stage::isLinked(const Asset::Contact& contact) const {
        return this->_links.count(contact) > 0;
    }

// ================================================================================================
// | MODIFIERS |
// =============

    void Stage::link(const Unit* unit) {
        if (unit == nullptr || !unit->isPlaced()) {
            return;
        }
        
        this->link(
            unit->getContact(), 
            unit->getTile()->getPosition()
        );
    }
    
    void Stage::link(
        const Asset::Contact& contact,
        const Tile::Position& position
    ) {
        this->_links[contact] = position;
    }
    
    void Stage::unlink(const Unit* unit) {
        if (unit == nullptr) {
            return;
        }
        
        this->unlink(unit->getContact());
    }
    
    void Stage::unlink(const Asset::Contact& contact) {
        this->_links.erase(contact);
    }
    
    void Stage::unlinkAll(void) {
        this->_links.clear();
    }

// ================================================================================================
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

    Schema Stage::toSchema(void) const {
        Schema schema;
        std::vector<std::string> properties;
        std::vector<std::string> markers;
        std::vector<std::string> effects;
        std::vector<std::string> links;

        // append properties
        properties.push_back("id = " + std::to_string(this->getId()));
        properties.push_back("name = " + this->getName());
        properties.push_back("row-count = " + std::to_string(this->getRowCount()));
        properties.push_back("column-count = " + std::to_string(this->getColumnCount()));

        // append each row of markers & effects
        for (std::size_t i = 0; i < this->getRowCount(); i++) {
            std::string markerRow;
            std::string effectRow;

            for (std::size_t j = 0; j < this->getColumnCount(); j++) {
                const Tile* tile = this->getTile(i, j);
                markerRow += std::string(1, tile->getMarker());
                effectRow += std::string(1, tile->getEffect());
            }

            markers.push_back(markerRow);
            effects.push_back(effectRow);
        }

        // append each asset id
        for (Stage::Link link : this->_links) {
            links.push_back(
                link.first.typeName + ", "
                    + std::to_string(link.first.typeId) + ", "
                    + std::to_string(link.first.id) + ", "
                    + link.first.filePath + ", "
                    + std::to_string(link.second.row) + ", "
                    + std::to_string(link.second.column)
            );
        }

        // map headers to entries
        schema[_PROPERTIES_SECTION_HEADER] = properties;
        schema[_MARKERS_SECTION_HEADER] = markers;
        schema[_EFFECTS_SECTION_HEADER] = effects;
        schema[_UNITS_SECTION_HEADER] = links;

        return schema;
    }

// ================================================================================================
// | LOGISTICS |
// =============

    Cache<Stage, DM_STAGE_CACHE_CAP> Stage::_cache;

    Stage* Stage::fetch(unsigned long id) {
        return Stage::_cache.search(id);
    }

    bool Stage::isLoaded(unsigned long id) {
        return Stage::_cache.contains(id);
    }

    Stage* Stage::load(const std::string& filePath) {    
        // attempt to read the schema
        // try to find the file in the state directory first
        // if not found there, read from the template
        Schema schema;
        if (
            !schema.read(std::string(DM_STATE_DIR) + filePath) ||
            !schema.read(std::string(DM_ASSET_DIR) + filePath)
        ) {
            return nullptr;
        }

        unsigned long id;
        std::string name;
        std::size_t rowCount;
        std::size_t columnCount;
        std::string markers = "";
        std::string effects = "";
        std::vector<Stage::Link> links;

        for (const Schema::Section& section : schema) {
            // process the properties section
            if (section.first == _PROPERTIES_SECTION_HEADER) {
                std::map<std::string, std::string> keyValues 
                    = strings::parseIni(section.second, "=");

                // attempt to parse the loaded properties
                try {
                    id = stoul(keyValues["id"], nullptr, 16);
                    name = keyValues["name"];
                    rowCount = stoul(keyValues["row-count"]);
                    columnCount = stoul(keyValues["column-count"]);
                }

                // make sure missing or invalid properties don't cause a crash
                catch (const std::invalid_argument& exception) {
                    return nullptr;
                } catch (const std::out_of_range& exception) {
                    return nullptr;
                }
            }

            // process the markers section
            else if (section.first == _MARKERS_SECTION_HEADER) {
                for (const std::string& entry : section.second) {
                    markers += entry;
                }
            }

            // process the modifers section
            else if (section.first == _EFFECTS_SECTION_HEADER) {
                for (const std::string& entry : section.second) {
                    effects += entry;
                }
            }

            // process the units section
            else if (section.first == _UNITS_SECTION_HEADER) {
                std::vector<std::vector<std::string>> rows 
                    = strings::parseDsv(section.second, ",", 6);

                for (const std::vector<std::string>& entry : rows) {
                    // attempt to parse the asset id
                    try {
                        Asset::Contact contact;
                        Tile::Position position;
                        
                        contact.typeName = entry[0];
                        contact.typeId = stoul(
                            entry[1],
                            nullptr, 16
                        );
                        
                        contact.id = stoul(
                            entry[2],
                            nullptr, 16
                        );
                        contact.filePath = entry[3];
                        
                        position.row = stoul(entry[4]);
                        position.column = stoul(entry[5]);
                        
                        links.emplace_back(contact, position);
                    }

                    // make sure invalid information doesn't cause a crash
                    catch (const std::invalid_argument& exception) {
                        return nullptr;
                    }
                }
            }
        }

        // attempt to store the stage
        Stage* stage = Stage::_cache.store(
            id, filePath, name, 
            rowCount, columnCount, 
            markers, effects
        );
        if (stage == nullptr) {
            return nullptr;
        }
        
        // link the units to the stage
        for (Stage::Link link : links) {
            stage->link(link.first, link.second);
            
            // try to find the loaded unit
            Unit* unit = nullptr;
            if (Actor::checkType(link.first)) {
                unit = Actor::fetch(link.first.id);
            }
            if (unit == nullptr) {
                continue;
            }
            
            // make sure the unit can be placed in bounds
            // disregard the unit's previous position
            if (!stage->isInBounds(
                link.second.row, 
                link.second.column
            )) {
                continue;
            }
            
            // place the unit
            // disregard the unit's previous position
            unit->setTile(stage->getTile(
                link.second.row, 
                link.second.column
            ));
        }
        
        return stage;
    }

    Stage* Stage::select(unsigned long id) {
        return Stage::_cache.select(id);
    }

    bool Stage::unload(unsigned long id) {
        Stage* stage = Stage::_cache.search(id);
        if (stage == nullptr) {
            return false;
        }
    
        // remove any loaded actors from the stage
        for (Actor *actor : stage->getActors()) {
            // remove the actor, but don't unlink
            actor->discard();
        }
        
        return Stage::_cache.remove(id);
    }

// ================================================================================================
// | UTILITIES |
// =============

    std::size_t Stage::_linearIndex(
        std::size_t row, 
        std::size_t column
    ) const {
        // convert the 2-dimensional row/column to a 1-dimensional index
        return row * this->getColumnCount() + column;
    }
    
        
    bool Stage::_compareContacts(
        const Actor::Contact& contactA,
        const Actor::Contact& contactB
    ) {
        return contactA.id < contactB.id;
    }
}

