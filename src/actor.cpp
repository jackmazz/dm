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
#include <vector>

#define _PROPERTIES_SECTION_HEADER "[PROPERTIES]"

namespace dm {

// ====================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Actor::Actor(void) : Actor(0, "") {}
    
    Actor::Actor (
        unsigned long id,
        const std::string& filePath
    ) : Actor(id, filePath, "", '\0') {}
    
    Actor::Actor (
        unsigned long id,
        const std::string& filePath,
        const std::string& name,
        char marker
    ) : Asset(id, filePath, name) {
        this->_tile = nullptr;
        this->setMarker(marker);
    }
    
    Actor::~Actor(void) {}
    
// ====================================================================================================
// | ACCESSORS |
// =============
    
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
            tile != prevTile
        ) && ((
            (tile == nullptr) != (prevTile == nullptr)
        ) || (
            tile->getParent() != prevTile->getParent()
        ));
        
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
    
// ====================================================================================================
// | LOGISTICS |
// =============

    Cache<Actor, ACTOR_CACHE_CAP> Actor::_cache;
    
    Actor* Actor::load(const std::string& filePath) {
        // attempt to read into an object
        Object object;
        if (!object.read(filePath)) {
            return nullptr;
        }
        
        Tile* tile = nullptr;
        unsigned long id;
        std::string name;
        char marker;
        
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
                
                    // marker length must be 1
                    if (properties["marker"].length() != 1) {
                        return nullptr;
                    }
                    marker = properties["marker"][0];
                    
                    // if the actor was placed, get it's tile
                    if (properties.count("stage-id") > 0) {
                        std::size_t stageId = stol(properties["stage-id"]);
                        std::size_t row = stol(properties["row"]);
                        std::size_t column = stol(properties["column"]);
                        
                        Stage* stage = Stage::select(stageId);
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
        
        // unload an actor if the cache exceeds it's capacity
        if (Actor::_cache.isFull()) {
            Actor::unload(Actor::_cache.getFront());
        }
        
        // store the actor
        Actor* actor = Actor::_cache.store(
            id,
            filePath,
            name,
            marker
        );
        actor->setTile(tile);
        
        return actor;
    }
    
    Actor* Actor::select(unsigned long id) {
        return Actor::_cache.select(id);
    }
    
    bool Actor::unload(Actor* actor) {
        if (actor == nullptr) {
            return false;
        }
    
        Object object;
        std::vector<std::string> properties;
        
        // append properties
        properties.push_back("id = " + std::to_string(actor->getId()));
        properties.push_back("name = " + actor->getName());
        properties.push_back("marker = " + std::string(1, actor->getMarker()));
        if (actor->isPlaced()) {
            properties.push_back("stage-id = " + std::to_string(
                actor
                ->getTile()
                ->getParent()
                ->getId()
            ));
            
            properties.push_back("row = " + std::to_string(
                actor
                ->getTile()
                ->getRow()
            ));
            
            properties.push_back("column = " + std::to_string(
                actor
                ->getTile()
                ->getColumn()
            ));
        }
        
        // map headers to entries
        object[_PROPERTIES_SECTION_HEADER] = properties;
        
        // remove the actor
        actor->setTile(nullptr, false);
        
        // write the object, return whether the write was successful
        return object.write(actor->getFilePath());
    }
}

