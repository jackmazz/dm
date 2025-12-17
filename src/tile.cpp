#include "actor.hpp"
#include "stage.hpp"
#include "tile.hpp"

#include <cstddef>
#include <string>

namespace dm {

// ====================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Tile::Tile(void) : Tile(nullptr, 0, 0) {}
    
    Tile::Tile(
        Stage* parent,
        std::size_t row,
        std::size_t column
    ) : Tile(parent, row, column, '\0', DM_TILE_MODIFIER_FLOOR) {}
    
    Tile::Tile(
        Stage* parent,
        std::size_t row,
        std::size_t column,
        char marker,
        char modifier
    ) {
        this->_parent = parent;
        this->_row = row;
        this->_column = column;
        this->_occupant = nullptr;
        
        this->setMarker(marker);
        this->setModifier(modifier);
    }
    
    Tile::~Tile(void) {}

// ====================================================================================================
// | ACCESSORS |
// =============    

    Stage* Tile::parent(void) {
        return this->_parent;
    }
    
    std::size_t Tile::row(void) {
        return this->_row;
    }
    
    std::size_t Tile::column(void) {
        return this->_column;
    }
    
    char Tile::marker(void) {
        return this->_marker;
    }
    
    char Tile::modifier(void) {
        return this->_modifier;
    }
    
    bool Tile::isBlocked(void) {
        return this->modifier() == DM_TILE_MODIFIER_BARRIER;
    }
    
    Actor* Tile::occupant(void) {
        return this->_occupant;
    }
    
    bool Tile::isOccupied(void) {
        return this->occupant() != nullptr;
    }

// ====================================================================================================
// | MODIFIERS |
// =============    

    void Tile::setMarker(char marker) {
        this->_marker = marker;
    }
    
    void Tile::setModifier(char modifier) {
        this->_modifier = modifier;
    }
    
    void Tile::setOccupant(Actor* occupant) {
        this->setOccupant(occupant, true);
    }
    
    void Tile::setOccupant(Actor* occupant, bool transit) {
        // avoid infinite loop
        if (this->occupant() == occupant) {
            return;
        }
        
        // change the tile's occupant
        Actor* prevOccupant = this->occupant();
        this->_occupant = occupant;
        
        // if the tile was occupied, set old occupant's placement to the null pointer
        if (prevOccupant != nullptr) {
            prevOccupant->setPlacement(nullptr);
            if (transit) {
                this->parent()->removeOccupant(prevOccupant);
            }
        }
        
        // if the tile is now occupied, set new occupants's placement to the tile
        if (this->isOccupied()) {
            occupant->setPlacement(this);
            if (transit) {
                this->parent()->addOccupant(occupant);
            }
        }
    }
    
// ====================================================================================================
// | CONVERTERS |
// ==============

    std::string Tile::toString(void) {
        if (this->isOccupied()) {
            return this->occupant()->toString();
        } else {
            return std::string(1, this->marker());
        }
    }
}

