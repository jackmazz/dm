#pragma once

#include "config.hpp"
#include "utils/cache.hpp"

#include <string>

namespace dm {
    // forward declarations
    class Tile;
    
    class Actor : public CacheEntry {
        public:
            /* constructors & destructors */
                explicit Actor(void);
                
                explicit Actor (
                    unsigned long id,
                    const std::string& filePath
                );
                
                explicit Actor (
                    unsigned long id,
                    const std::string& filePath,
                    const std::string& name,
                    char marker
                );
                
                ~Actor(void);
            
            /* accessors */
                std::string filePath(void);
                
                std::string name(void);
                
                char marker(void);
                
                Tile* placement(void);
                bool isPlaced(void);
            
            /* modifiers */
                void setName(const std::string& name);
            
                void setPlacement(Tile* placement);
                void setPlacement(Tile* placement, bool transit);
                
                void setMarker(char marker);
            
            /* converters */
                std::string toString(void);
            
            /* logistics */
                static Actor* load(const std::string& filePath);
                static Actor* select(unsigned long id);
                static bool unload(Actor* actor);
        
        private:
            /* members */
                static Cache<Actor, ACTOR_CACHE_CAP> _cache;    // actor storage
                
                std::string _filePath;                          // path to load & unload
                
                std::string _name;                              // display name
                char _marker;                                   // display character
                
                Tile* _placement;                               // tile the actor occupies
    };
}

