#pragma once

#include <cstddef>
#include <string>

namespace dm {
    class Position {
    
        // ========================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
        public:
            Position(void);
            
            Position(
                std::size_t row,
                std::size_t column
            );
            
            ~Position(void) = default;
        
        // ========================================================================================
        // | ACCESSORS |
        // =============
            
        public:
            std::size_t getRow(void) const;
            std::size_t getColumn(void) const;
        
        // ========================================================================================
        // | MODIFIERS |
        // =============
        
        public:
            void setRow(std::size_t row);
            void setColumn(std::size_t column);
        
        // ========================================================================================
        // | COMPARATORS |
        // ===============
        
        public:
            bool equals(const Position& other) const;
            std::size_t compare(const Position& other) const;
        
        // ========================================================================================
        // | CONVERTERS |
        // ==============
        
        public:
            std::string toString(void) const;
        
        // ========================================================================================
        // | MEMBERS |
        // ===========
    
        private:
            std::size_t row;
            std::size_t column;
    };
    
// ================================================================================================
// | OPERATORS |
// =============
    
    bool operator==(
        const Position& positionA,
        const Position& positionB
    );
    
    bool operator<(
        const Position& positionA,
        const Position& positionB
    );
    
    bool operator<=(
        const Position& positionA,
        const Position& positionB
    );
    
    bool operator>(
        const Position& positionA,
        const Position& positionB
    );

    bool operator>=(
        const Position& positionA,
        const Position& positionB
    );
    
    std::ostream& operator<<(
        std::ostream& stream, 
        const Position& position
    );
}

