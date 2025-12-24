#include "actor.hpp"
#include "config.hpp"
#include "stage.hpp"
#include "tile.hpp"
#include "utils/asset.hpp"
#include "utils/cache.hpp"
#include "utils/schema.hpp"
#include "utils/strings.hpp"

#include <cstddef>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define _PROPERTIES_SECTION_HEADER "[PROPERTIES]"

namespace dm {

// ====================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Actor::Actor(void) : Actor(0, "") {}

    Actor::Actor(
        unsigned long id, 
        const std::string& filePath
    ) : Actor(id, filePath, "", '\0') {}

    Actor::Actor(
        unsigned long id, 
        const std::string& filePath, 
        const std::string& name, 
        char marker
    ) : Asset(id, filePath, name) {
        this->_tile = nullptr;
        this->setMarker(marker);
    }

// ====================================================================================================
// | ACCESSORS |
// =============

    Actor::Contact Actor::getContact(void) const {
        return std::make_pair(
            this->getId(), 
            this->getFilePath()
        );
    }

    char Actor::getMarker(void) const {
        return this->_marker;
    }

    const Tile* Actor::getTile(void) const {
        return this->_tile;
    }

    Tile* Actor::getTile(void) {
        return const_cast<Tile*>(
            static_cast<const Actor*>(this)->getTile()
        );
    }

    bool Actor::isPlaced(void) const {
        return this->getTile() != nullptr;
    }

// ====================================================================================================
// | MODIFIERS |
// =============

    void Actor::setMarker(char marker) {
        this->_marker = marker;
    }

    void Actor::setTile(Tile* tile) {
        /* transition if:
            - moving to a new stage
            - moving from nothing to a stage
            - moving from a stage to nothing
        */

        Tile* prevTile = this->getTile();
            bool transit = (
                tile != prevTile && (
                    (tile == nullptr) != (prevTile == nullptr) ||
                    tile->getParent() != prevTile->getParent()
                )
            );

            this->setTile(tile, transit);
    }

    void Actor::setTile(Tile* tile, bool transit) {
        Tile* prevTile = this->getTile();

        // avoid infinite loop
        if (tile == prevTile) {
            return;
        }

        // change this actor's tile
        this->_tile = tile;

        // if this actor was placed, set old tile's actor to the null pointer
        if (prevTile != nullptr) {
            prevTile->setActor(nullptr, transit);
        }

        // if this actor is now placed, set new tile's actor to this actor
        if (this->isPlaced()) {
            tile->setActor(this, transit);
        }
    }

// ====================================================================================================
// | CONVERTERS |
// ==============

    std::string Actor::toString(void) const {
        return std::string(1, this->getMarker());
    }

    Schema Actor::toSchema(void) const {
        Schema schema;
        std::vector<std::string> properties;

        // append properties
        properties.push_back("id = " + std::to_string(this->getId()));
        properties.push_back("name = " + this->getName());
        properties.push_back("marker = " + std::string(1, this->getMarker()));
        
        if (this->isPlaced()) {
            properties.push_back("stage-id = "
                + std::to_string(this->getTile()->getParent()->getId()
            ));
            
            properties.push_back("row = " 
                + std::to_string(this->getTile()->getRow()
            ));
            
            properties.push_back("column = " 
                + std::to_string(this->getTile()->getColumn()
            ));
        }

        // map headers to entries
        schema[_PROPERTIES_SECTION_HEADER] = properties;

        return schema;
    }

// ====================================================================================================
// | LOGISTICS |
// =============

    Cache<Actor, DM_ACTOR_CACHE_CAP> Actor::_cache;

    Actor* Actor::get(unsigned long id) {
        return Actor::_cache.get(id);
    }

    bool Actor::contains(unsigned long id) {
        return Actor::_cache.contains(id);
    }

    Actor* Actor::load(const std::string& filePath) {
        std::string fullPath = DM_ACTOR_DIR+"/" + filePath;
    
        // attempt to read the schema
        Schema schema;
        if (!schema.read(fullPath)) {
            return nullptr;
        }

        Tile* tile = nullptr;
        unsigned long id;
        std::string name;
        char marker;

        for (const Schema::Section& section : schema) {
            // process the properties section
            if (section.first == _PROPERTIES_SECTION_HEADER) {
                std::unordered_map<std::string, std::string> keyValues =
                    strings::parseIni(section.second, "=");

                // attempt to parse the loaded properties
                try {
                    id = stol(keyValues["id"]);
                    name = keyValues["name"];

                    // marker length must be 1
                    if (keyValues["marker"].length() != 1) {
                        return nullptr;
                    }
                    marker = keyValues["marker"][0];

                    // if the actor was placed, get it's tile
                    if (keyValues.count("stage-id") > 0) {
                        std::size_t stageId = stol(keyValues["stage-id"]);
                        std::size_t row = stol(keyValues["row"]);
                        std::size_t column = stol(keyValues["column"]);
                        
                        Stage* stage = Stage::get(stageId);
                        if (stage != nullptr) {
                            tile = stage->getTile(row, column);
                        }
                    }
                }

                // make sure missing or invalid properties don't cause a crash
                catch (const std::invalid_argument& exception) {
                    return nullptr;
                } catch (const std::out_of_range& exception) {
                    return nullptr;
                }
            }
        }

        // attempt to store the actor
        Actor* actor = Actor::_cache.store(
            id, fullPath, name, 
            marker
        );
        if (actor == nullptr) {
            return nullptr;
        }

        // place the actor
        actor->setTile(tile);

        return actor;
    }

    Actor* Actor::select(unsigned long id) {
        return Actor::_cache.select(id);
    }

    bool Actor::unload(unsigned long id) {
        return Actor::_cache.remove(id);
    }
}

