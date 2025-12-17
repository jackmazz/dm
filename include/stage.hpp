#pragma once

#include "config.hpp"
#include "tile.hpp"
#include "utils/cache.hpp"

#include <cstddef>
#include <set>
#include <string>
#include <vector>

namespace dm {    
    class Stage : public CacheEntry {
        public:
            /* constructors & destructors */
                explicit Stage(void);
                
                explicit Stage(
                    unsigned long id,
                    const std::string& filePath,
                    std::size_t rowCount, 
                    std::size_t columnCount
                );
                
                explicit Stage(
                    unsigned long id,
                    const std::string& filePath,
                    const std::string& name,
                    std::size_t rowCount, 
                    std::size_t columnCount,
                    const std::string& markers,
                    const std::string& modifiers
                );
                
                ~Stage(void);
            
            /* accessors */
                std::string filePath(void);
                
                std::string name(void);
                
                std::size_t size(void);
                std::size_t rowCount(void);
                std::size_t columnCount(void);
                bool inBounds(std::size_t row, std::size_t column);
                
                Tile* tileAt(std::size_t row, std::size_t column);
                
                std::vector<Actor*> occupants(void);
                std::size_t occupantCount(void);
                bool containsOccupant(Actor *actor);
            
            /* modifiers */
                void setName(const std::string& name);
                
                void addOccupant(Actor *occupant);
                void removeOccupant(Actor *occupant);
            
            /* converters */
                std::string toString(void);
            
            /* logistics */
                static Stage* load(const std::string& filePath);
                static Stage* select(unsigned long id);
                static bool unload(Stage* stage);
        
        private:        
            /* members */
                static Cache<Stage, STAGE_CACHE_CAP> _cache;    // stage storage
                
                unsigned long _id;                              // unique id
                std::string _filePath;                          // path to load & unload
                
                std::string _name;                              // display name
            
                std::size_t _rowCount;                          // grid #of rows
                std::size_t _columnCount;                       // grid #of columns
                
                std::vector<Tile> _tiles;                       // tile storage
                
                std::set<std::pair<                             // actor information storage
                    unsigned long, 
                    std::string
                >> _occupantInfo;
    };
}

