#pragma once

#include <cstddef>
#include <ostream>
#include <string>

namespace dm {
    class Contact {
    
        // ========================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
        public:
            Contact(void);
            
            Contact(
                unsigned long typeId,
                const std::string& typeName,
                unsigned long primeId, 
                const std::string& filePath
            );
            
            ~Contact(void) = default;
        
        // ========================================================================================
        // | ACCESSORS |
        // =============
        
        public:
            unsigned long getTypeId(void) const;
            std::string getTypeName(void) const;
            
            unsigned long getPrimeId(void) const;
            std::string getFilePath(void) const;
        
        // ========================================================================================
        // | COMPARATORS |
        // ===============
        
        public:
            bool equals(const Contact& other) const;
            std::size_t compare(const Contact& other) const;
        
        // ========================================================================================
        // | CONVERTERS |
        // ==============
        
        public:
            std::string toString(void) const;
    
        // ========================================================================================
        // | MEMBERS |
        // ===========
    
        private:
            unsigned long _typeId; // type specifier
            std::string _typeName; // visual type specifier
            
            unsigned long _primeId; // unique id
            std::string _filePath; // path to load & unload
    };

// ================================================================================================
// | OPERATORS |
// =============
    
    bool operator==(
        const Contact& contactA,
        const Contact& contactB
    );
    
    bool operator<(
        const Contact& contactA,
        const Contact& contactB
    );
    
    bool operator<=(
        const Contact& contactA,
        const Contact& contactB
    );
    
    bool operator>(
        const Contact& contactA,
        const Contact& contactB
    );

    bool operator>=(
        const Contact& contactA,
        const Contact& contactB
    );
    
    std::ostream& operator<<(
        std::ostream& stream, 
        const Contact& contact
    );
}
