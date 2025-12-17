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
                Stage* parent(void);
                
                std::size_t row(void);
                std::size_t column(void);
                          
                char marker(void);
                char modifier(void);
                bool isBlocked(void);
                
                Actor* occupant(void);
                bool isOccupied(void);
            
            /* modifiers */
                void setMarker(char marker);
                void setModifier(char modifer);
                
                void setOccupant(Actor* occupant);
                void setOccupant(Actor* occupant, bool transit);
            
            /* converters */
                std::string toString(void);
        
        private:
            /* members */
                Stage* _parent;         // where the tile is stored
                
                std::size_t _row;       // row index of the tile
                std::size_t _column;    // column index of the tile
                
                char _marker;           // display character
                char _modifier;         // whether the tile is a floor, barrier, door, etc.
                
                Actor* _occupant;       // actor occupying the tile
    };
}

