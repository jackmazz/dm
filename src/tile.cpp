#include "stage.hpp"
#include "tile.hpp"
#include "unit.hpp"

#include <cstddef>
#include <ostream>
#include <string>
#include <utility>

namespace dm {

// ================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Tile::Tile(void) 
        : Tile(nullptr, 0, 0)
    {}

    Tile::Tile(
        Stage* parent, 
        std::size_t row, 
        std::size_t column
    ) 
        : Tile(
            parent, row, column, 
            '\0', DM_TILE_FLOOR_EFFECT
        )
    {}

    Tile::Tile(
        Stage* parent, 
        std::size_t row, 
        std::size_t column, 
        char marker, 
        char effect
    ) {
        this->_parent = parent;
        this->_row = row;
        this->_column = column;
        this->_occupant = nullptr;

        this->setMarker(marker);
        this->setEffect(effect);
    }

// ================================================================================================
// | ACCESSORS |
// =============

    const Stage* Tile::getParent(void) const {
        return this->_parent;
    }

    Stage* Tile::getParent(void) {
        return const_cast<Stage*>(
            static_cast<const Tile*>(this)->getParent()
        );
    }
    
    bool Tile::hasParent(void) const {
        return this->getParent() != nullptr;
    }
    
    Tile::Position Tile::getPosition(void) const {
        Tile::Position position;
        position.row = this->getRow();
        position.column = this->getColumn();
        return position;
    }

    std::size_t Tile::getRow(void) const {
        return this->_row;
    }

    std::size_t Tile::getColumn(void) const {
        return this->_column;
    }

    char Tile::getMarker(void) const {
        return this->_marker;
    }

    char Tile::getEffect(void) const {
        return this->_effect;
    }

    bool Tile::isBlocked(void) const {
        return this->getEffect() == DM_TILE_BARRIER_EFFECT;
    }

    const Unit* Tile::getUnit(void) const {
        return this->_unit;
    }

    Unit* Tile::getUnit(void) {
        return const_cast<Unit*>(
            static_cast<const Tile*>(this)->getUnit()
        );
    }

    bool Tile::isOccupied(void) const {
        return this->getUnit() != nullptr;
    }

// ================================================================================================
// | MODIFIERS |
// =============

    void Tile::setMarker(char marker) {
        this->_marker = marker;
    }

    void Tile::setEffect(char effect) {
        this->_effect = effect;
    }
    
    void Tile::setUnit(Unit* occupant) {
        // if removing the current unit ...
        if (occupant == nullptr && this->isOccupied()) {
            Unit* previous = this->getUnit();
            this->_occupant = nullptr;
            
            // update the old occupant (if not already updated)
            if (previous->getTile() == this) {
                previous->setTile(nullptr);
            }
            
            // unlink the old occupant from the parent
            if (this->hasParent()) {
                this->getParent()->unlink(previous.getId());
            }
        } 
        
        // if setting (not replacing) the occupant ...
        else if (occupant != nullptr && !this->isOccupied()) {
            this->_occupant = occupant;
            
            // update the new occupant (if not already updated)
            if (occupant->getTile() != this) {
                occupant->setTile(this);
            }
            
            // link the old occupant to the parent
            if (this->hasParent()) {
                this->getParent()->link(occupant.getId());
            }
        }
    }
    
    void Tile::discardOccupant(void) {
        if (!this->isOccupied()) {
            return;
        }
        
        Unit* previous = this->getOccupant();
        this->_occupant = nullptr;
        
        // update the old occupant (if not already updated)
        if (previous->getTile() == this) {
            previous->discard();
        }
    }

// ================================================================================================
// | CONVERTERS |
// ==============

    std::string Tile::toString(void) const {
        if (this->isOccupied()) {
            return this->getOccupabt()->toString();
        } else {
            return std::string(1, this->getMarker());
        }
    }

// ================================================================================================
// | OPERATORS |
// =============
        
    std::ostream& operator<<(
        std::ostream& stream, 
        const Tile& tile
    ) {
        stream << tile.toString();
        return stream;
    }
}

