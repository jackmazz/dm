#include "actor.hpp"
#include "asset.hpp"
#include "config.hpp"
#include "stage.hpp"
#include "unit.hpp"
#include "utils/cache.hpp"
#include "utils/schema.hpp"
#include "utils/string-tools.hpp"

#include <cstddef>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#define _PROPERTIES_SECTION_HEADER "[PROPERTIES]"

namespace dm {

// ================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Actor::Actor(void)
        : Actor(0, "")
    {}

    Actor::Actor(
        unsigned long id,
        const std::string& filePath
    )
        : Actor(id, filePath, "", '\0')
    {}

    Actor::Actor(
        unsigned long id, 
        const std::string& filePath,
        const std::string& name, 
        char marker
    )
        : Unit(id, filePath, name, marker)
    {}

// ================================================================================================
// | ACCESSORS |
// =============

    unsigned long Actor::getTypeId(void) const {
        return DM_ACTOR_TYPE_ID;
    }
    
    std::string Actor::getTypeName(void) const {
        return DM_ACTOR_TYPE_NAME;
    }
    
    bool Actor::checkType(const Asset* asset) {
        if (asset == nullptr) {
            return false;
        }
    
        return asset->getTypeId() == DM_ACTOR_TYPE_ID;
    }
    
    bool Actor::checkType(const Asset::Contact& contact) {
        return contact.typeId == DM_ACTOR_TYPE_ID;
    }
    
    const Actor* Actor::cast(const Asset* asset) {
        if (asset == nullptr) {
            return nullptr;
        }
        
        // if the type id matches, we can safely cast
        if (Actor::checkType(asset)) {
            return static_cast<const Actor*>(asset);
        }
        
        return nullptr;
    }
    
    Actor* Actor::cast(Asset* asset) {
        return const_cast<Actor*>(
            Actor::cast(static_cast<const Asset*>(asset))
        );
    }
    
// ================================================================================================
// | CONVERTERS |
// ==============
    
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
    
// ================================================================================================
// | LOGISTICS |
// =============

    Cache<Actor, DM_ACTOR_CACHE_CAP> Actor::_cache;

    Actor* Actor::fetch(unsigned long id) {
        return Actor::_cache.search(id);
    }

    bool Actor::isLoaded(unsigned long id) {
        return Actor::_cache.contains(id);
    }

    Actor* Actor::load(const std::string& filePath) {        
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

        Tile* tile = nullptr;
        unsigned long id;
        std::string name;
        char marker;

        for (const Schema::Section& section : schema) {
            // process the properties section
            if (section.first == _PROPERTIES_SECTION_HEADER) {
                std::map<std::string, std::string> keyValues =
                    strings::parseIni(section.second, "=");

                // attempt to parse the loaded properties
                try {
                    id = stoul(
                        keyValues["id"], 
                        nullptr, 16
                    );
                    
                    name = keyValues["name"];

                    // marker length must be 1
                    if (keyValues["marker"].length() != 1) {
                        return nullptr;
                    }
                    marker = keyValues["marker"][0];

                    // if the actor was placed, get it's tile
                    if (keyValues.count("stage-id") > 0) {
                        std::size_t stageId = stoul(
                            keyValues["stage-id"], 
                            nullptr, 16
                        );
                        
                        std::size_t row = stoul(keyValues["row"]);
                        std::size_t column = stoul(keyValues["column"]);
                        
                        // set the tile if the stage is loaded
                        Stage* stage = Stage::fetch(stageId);
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
            id, filePath, name, 
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
        Actor* actor = Actor::_cache.search(id);
        if (actor == nullptr) {
            return false;
        }
    
        // remove the actor, but don't unlink
        actor->discard();
        
        return Actor::_cache.remove(id);
    }
}
    
