#pragma once

#include "config.hpp"
#include "tile.hpp"
#include "utils/asset.hpp"
#include "utils/dmo.hpp"
#include "utils/cache.hpp"

#include <string>

namespace dm {    
    class Actor : public Asset {
        public:
            /* constructors & destructors */
                Actor(void);
                
                Actor (
                    unsigned long id,
                    const std::string& filePath
                );
                
                Actor (
                    unsigned long id,
                    const std::string& filePath,
                    const std::string& name,
                    char marker
                );
                
                ~Actor(void);
            
            /* accessors */                
                char getMarker(void) const;
                
                const Tile* getTile(void) const;
                Tile* getTile(void);
                bool isPlaced(void) const;
            
            /* modifiers */
                void setTile(Tile* tile);
                void setTile(Tile* tile, bool transit);
                
                void setMarker(char marker);
            
            /* converters */
                std::string toString(void) const override;
                DMO toDMO(void) const override;
            
            /* logistics */
                static Actor* load(const std::string& filePath);
                static Actor* select(unsigned long id);
                static bool unload(unsigned long id);
        
        private:
            /* members */
                static Cache<           // actor storage
                    Actor, 
                    ACTOR_CACHE_CAP
                > _cache;
                
                char _marker;           // display character
                
                Tile* _tile;            // tile the actor occupies
    };
}

