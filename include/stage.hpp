#pragma once

#include "config.hpp"
#include "tile.hpp"
#include "utils/asset.hpp"
#include "utils/dmo.hpp"
#include "utils/cache.hpp"

#include <cstddef>
#include <set>
#include <string>
#include <vector>

namespace dm {    
    class Stage : public Asset {
        public:
            /* constructors & destructors */
                Stage(void);
                
                Stage(
                    unsigned long id,
                    const std::string& filePath,
                    std::size_t rowCount, 
                    std::size_t columnCount
                );
                
                Stage(
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
                std::size_t getSize(void) const;
                std::size_t getRowCount(void) const;
                std::size_t getColumnCount(void) const;
                bool isInBounds(std::size_t row, std::size_t column) const;
                
                const Tile* getTile(std::size_t row, std::size_t column) const;
                Tile* getTile(std::size_t row, std::size_t column);
            
            /* modifiers */                
                void addActor(Actor *actor);
                void removeActor(Actor *actor);
            
            /* converters */
                std::string toString(void) const override;
                DMO toDMO(void) const override;
            
            /* logistics */
                static Stage* load(const std::string& filePath);
                static Stage* select(unsigned long id);
                static bool unload(unsigned long id);
        
        private:        
            /* members */
                static Cache<               // stage storage
                    Stage, 
                    STAGE_CACHE_CAP
                > _cache;
            
                std::size_t _rowCount;      // grid #of rows
                std::size_t _columnCount;   // grid #of columns
                
                std::vector<Tile> _tiles;   // tile storage
                
                std::set<std::pair<         // actor information storage
                    unsigned long, 
                    std::string
                >> _actorInfo;
    };
}

