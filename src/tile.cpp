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
        this->_actor = nullptr;
        
        this->setMarker(marker);
        this->setModifier(modifier);
    }
    
    Tile::~Tile(void) {}

// ====================================================================================================
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
    
    std::size_t Tile::getRow(void) const {
        return this->_row;
    }
    
    std::size_t Tile::getColumn(void) const {
        return this->_column;
    }
    
    char Tile::getMarker(void) const {
        return this->_marker;
    }
    
    char Tile::getModifier(void) const {
        return this->_modifier;
    }
    
    bool Tile::isBlocked(void) const {
        return this->getModifier() == DM_TILE_MODIFIER_BARRIER;
    }
    
    const Actor* Tile::getActor(void) const {
        return this->_actor;
    }
    
    Actor* Tile::getActor(void) {
        return const_cast<Actor*>(
            static_cast<const Tile*>(this)->getActor()
        );
    }
    
    bool Tile::isOccupied(void) const {
        return this->getActor() != nullptr;
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
    
    void Tile::setActor(Actor* actor) {
        this->setActor(actor, true);
    }
    
    void Tile::setActor(Actor* actor, bool transit) {
        Actor* prevActor = this->getActor();
    
        // avoid infinite loop
        if (actor == prevActor) {
            return;
        }
        
        // change this tile's actor
        this->_actor = actor;
        
        // if this tile was occupied, set the old actor's tile to the null pointer
        if (prevActor != nullptr) {
            prevActor->setTile(nullptr);
            
            // if transitioning, update the parent's actor list
            if (transit) {
                this->getParent()->removeActor(prevActor);
            }
        }
        
        // if this tile is now occupied, set the new actor's tile to this tile
        if (this->isOccupied()) {
            actor->setTile(this);
            
            // if transitioning, update the parent's actor list
            if (transit) {
                this->getParent()->addActor(actor);
            }
        }
    }
    
// ====================================================================================================
// | CONVERTERS |
// ==============

    std::string Tile::toString(void) const {
        if (this->isOccupied()) {
            return this->getActor()->toString();
        } else {
            return std::string(1, this->getMarker());
        }
    }
}

