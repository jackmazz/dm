#include "asset.hpp"
#include "tile.hpp"
#include "unit.hpp"

#include <string>
#include <vector>

namespace dm {

// ================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Unit::Unit(void)
        : Unit(0, "")
    {}

    Unit::Unit(
        unsigned long id,
        const std::string &filePath
    )
        : Unit(id, filePath, "", '\0')
    {}

    Unit::Unit(
        unsigned long id, 
        const std::string &filePath,
        const std::string& name, 
        char marker
    )
        : Asset(id, filePath, name)
    {
        this->_tile = nullptr;
        this->setMarker(marker);
    }

// ================================================================================================
// | ACCESSORS |
// =============

    char Unit::getMarker(void) const {
        return this->_marker;
    }

    const Tile* Unit::getTile(void) const {
        return this->_tile;
    }

    Tile* Unit::getTile(void) {
        return const_cast<Tile*>(
            static_cast<const Unit*>(this)->getTile()
        );
    }

    bool Unit::isPlaced(void) const {
        return this->getTile() != nullptr;
    }

// ================================================================================================
// | MODIFIERS |
// =============

    void Unit::setMarker(char marker) {
        this->_marker = marker;
    }
    
    void Unit::setTile(Tile* tile) {
        // if removing the unit...
        if (tile == nullptr && this->isPlaced()) {
            Tile* previous = this->getTile();
            this->_tile = nullptr;
            
            if (previous->getOccupant() == this) {
                previous->setOccupant(nullptr);
            }
        } 
        
        // if placing or moving the unit to an unoccupied space...
        else if (tile != nullptr && !tile->isOccupied()) {
            Tile* previous = this->getTile();
            this->_tile = tile;
            
            if (previous != nullptr && previous->getOccupant() == this) {
                previous->setOccupant(nullptr);
            }
            
            if (tile != nullptr && tile->getOccupant() != this) {
                tile->setOccupant(this);
            }
        }
    }
    
    void Unit::discard(void) {
        if (!this->isPlaced()) {
            return;
        }
        
        Tile* previous = this->getTile();
        this->_tile = nullptr;
        
        // update the old tile (if not already updated)
        if (previous->getOccupant() == this) {
            previous->discardOccupant();
        }
    }

// ================================================================================================
// | CONVERTERS |
// ==============

    std::string Unit::toString(void) const {
        return std::string(1, this->getMarker());
    }
}

