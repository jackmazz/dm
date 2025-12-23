#pragma once

#include <cstddef>
#include <string>

#define DM_TILE_MODIFIER_FLOOR 'f'
#define DM_TILE_MODIFIER_BARRIER 'b'

namespace dm {
    // forward declarations
    class Actor;
    class Stage;

    class Tile {
        public:        
            /* constructors & destructors */
                Tile(void);
                
                Tile(
                    Stage* parent,
                    std::size_t row,
                    std::size_t column
                );
                
                Tile(
                    Stage* parent,
                    std::size_t row,
                    std::size_t column,
                    char marker,
                    char modifier
                );
                
                ~Tile(void);
            
            /* accessors */
                const Stage* getParent(void) const;
                Stage* getParent(void);
                
                std::size_t getRow(void) const;
                std::size_t getColumn(void) const;
                          
                char getMarker(void) const;
                char getModifier(void) const;
                bool isBlocked(void) const;
                
                const Actor* getActor(void) const;
                Actor* getActor(void);
                bool isOccupied(void) const;
            
            /* modifiers */
                void setMarker(char marker);
                void setModifier(char modifer);
                
                void setActor(Actor* actor, bool transit=true);
            
            /* converters */
                std::string toString(void) const;
        
        private:
            /* members */
                Stage* _parent;         // where the tile is stored
                
                std::size_t _row;       // row index of the tile
                std::size_t _column;    // column index of the tile
                
                char _marker;           // display character
                char _modifier;         // whether the tile is a floor, barrier, door, etc.
                
                Actor* _actor;          // actor occupying the tile
    };
}

