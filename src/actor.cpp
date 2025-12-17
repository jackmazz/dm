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
    ) : CacheEntry(id) {
        this->_filePath = filePath;
        this->_placement = nullptr;
        
        this->setName(name);
        this->setMarker(marker);
    }
    
    Actor::~Actor(void) {}
    
// ====================================================================================================
// | ACCESSORS |
// =============
    
    std::string Actor::filePath(void) {
        return this->_filePath;
    }
    
    std::string Actor::name(void) {
        return this->_name;
    }
    
    char Actor::marker(void) {
        return this->_marker;
    }
    
    Tile* Actor::placement(void) {
        return this->_placement;
    }
    
    bool Actor::isPlaced(void) {
        return this->placement() != nullptr;
    }
    
// ====================================================================================================
// | MODIFIERS |
// =============

    void Actor::setName(const std::string& name) {
        this->_name = name;
    }
    
    void Actor::setMarker(char marker) {
        this->_marker = marker;
    }

    void Actor::setPlacement(Tile* placement) {        
        /* transition if:
            - moving to a new stage
            - moving from nothing to a stage
            - moving from a stage to nothing
        */
        
        bool transit = (
            this->placement() == nullptr || placement == nullptr
        ) || (
            this->placement()->parent() != placement->parent()
        );
        
        this->setPlacement(placement, transit);
    }
    
    void Actor::setPlacement(Tile* placement, bool transit) {
        // avoid infinite loop
        if (this->placement() == placement) {
            return;
        }

        // change the actor's tile
        Tile* prevPlacement = this->placement();
        this->_placement = placement;
        
        // if the actor was placed, set old tile's occupant to the null pointer
        if (prevPlacement != nullptr) {
            prevPlacement->setOccupant(nullptr, transit);
        }
        
        // if the actor is now placed, set new tile's occupant to the actor
        if (this->isPlaced()) {
            placement->setOccupant(this, transit);
        }
    }
    
// ====================================================================================================
// | CONVERTERS |
// ==============

    std::string Actor::toString(void) {
        return std::string(1, this->marker());
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
        
        Tile* placement = nullptr;
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
                            placement = stage->tileAt(row, column);
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
            Actor::unload(Actor::_cache.front());
        }
        
        // store the actor
        Actor* actor = Actor::_cache.store(
            id,
            filePath,
            name,
            marker
        );
        actor->setPlacement(placement);
        
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
        properties.push_back("id = " + std::to_string(actor->id()));
        properties.push_back("name = " + actor->name());
        properties.push_back("marker = " + std::string(1, actor->marker()));
        if (actor->isPlaced()) {
            properties.push_back("stage-id = " + std::to_string(actor->placement()->parent()->id()));
            properties.push_back("row = " + std::to_string(actor->placement()->row()));
            properties.push_back("column = " + std::to_string(actor->placement()->column()));
        }
        
        // map headers to entries
        object[_PROPERTIES_SECTION_HEADER] = properties;
        
        // remove the actor
        actor->setPlacement(nullptr, false);
        
        // write the object, return whether the write was successful
        return object.write(actor->filePath());
    }
}

