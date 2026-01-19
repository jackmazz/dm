#include "utils/position.hpp"

#include <cmath>
#include <ostream>
#include <string>
#include <utility>

namespace dm {

// ================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Position::Position(void)
        : Position(0, 0)
    {}
            
    Position::Position(
        std::size_t row,
        std::size_t column
    ) {
        this->setRow(row);
        this->setColumn(column);
    }
    
// ================================================================================================
// | ACCESSORS |
// =============

    std::size_t Position::getRow(void) const {
        return this->_row;
    }
    
    std::size_t Position::getColumn(void) const {
        return this->_column;
    }

// ================================================================================================
// | MODIFIERS |
// =============

    void Position::setRow(std::size_t row) {
        this->_row = row;
    }
    
    void Position::setColumn(std::size_t column) {
        this->_column = column;
    }
    
// ================================================================================================
// | COMPARATORS |
// ===============

    bool Position::equals(const Position& other) const {
        return this->compare(other) == 0;
    }
    
    std::size_t Position::compare(const Position& other) const {        
        // order by row then column
        if (this->getRow() != other.getRow()) {
            return this->getRow() - other.getRow();
        }
        return this->getColumn() - other.getColumn();;
    }

// ========================================================================================
// | CONVERTERS |
// ==============

    std::string Position::toString(void) const {
        return std::string("(")
            + std::to_string(this->getRow()) + ", "
            + std::to_string(this->getColumn()) + ")";
    }

// ================================================================================================
// | OPERATORS |
// =============
    
    bool operator==(
        const Position& positionA,
        const Position& positionB
    ) {
        return positionA.equals(positionB);
    }
    
    bool operator<(
        const Position& positionA,
        const Position& positionB
    ) { 
        return positionA.compare(positionB) < 0;
    }
    
    bool operator<=(
        const Position& positionA,
        const Position& positionB
    ) {
        return positionA.compare(positionB) <= 0;
    }
    
    bool operator>(
        const Position& positionA,
        const Position& positionB
    ) {
        return positionA.compare(positionB) > 0;
    }

    bool operator>=(
        const Position& positionA,
        const Position& positionB
    ) {
        return positionA.compare(positionB) >= 0;
    }
    
    std::ostream& operator<<(
        std::ostream& stream, 
        const Position& position
    ) {
        stream << position.toString();
        return stream;
    }
}

