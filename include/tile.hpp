#pragma once

#include <cstddef>
#include <ostream>
#include <string>
#include <utility>

#define DM_TILE_FLOOR_EFFECT 'f'
#define DM_TILE_BARRIER_EFFECT 'b'

namespace dm {
    // forward declarations
    class Unit;
    class Stage;

    class Tile {
    
        // ========================================================================================
        // | TYPES |
        // =========
        
        public:
            struct Position {
                std::size_t row;
                std::size_t column;
            };
    
        // ========================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
        public:
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
                char effect
            );
            
            ~Tile(void) = default;
        
        // ========================================================================================
        // | ACCESSORS |
        // =============
        
        public:
            const Stage* getParent(void) const;
            Stage* getParent(void);
            bool hasParent(void) const;
            
            Tile::Position getPosition(void) const;
            std::size_t getRow(void) const;
            std::size_t getColumn(void) const;

            char getMarker(void) const;
            char getEffect(void) const;
            bool isBlocked(void) const;

            const Unit* getOccupant(void) const;
            Unit* getOccupant(void);
            bool isOccupied(void) const;
        
        // ========================================================================================
        // | MODIFIERS |
        // =============
        
        public:
            void setMarker(char marker);
            void setEffect(char modifer);

            void setOccupant(Unit* occupant);
            void discardOccupant(void);
        
        // ========================================================================================
        // | CONVERTERS |
        // ==============
        
        public:
            std::string toString(void) const;
        
        // ========================================================================================
        // | MEMBERS |
        // ===========
        
        private:
            Stage* _parent; // where the tile is stored

            std::size_t _row; // row index of the tile
            std::size_t _column; // column index of the tile

            char _marker; // display character
            char _effect; // whether the tile is a floor, barrier, door, etc.

            Unit* _occupant; // unit occupying the tile
    };
    
// ================================================================================================
// | OPERATORS |
// =============
    
    std::ostream& operator<<(
        std::ostream& stream, 
        const Tile& tile
    );
}

