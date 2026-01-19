#pragma once

#include "utils/form.hpp"

#include <cstddef>
#include <ostream>
#include <string>

namespace dm {
    class Asset {
    
        // ========================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
        public:
            Asset(void);
            
            Asset(
                unsigned long primeId,
                const std::string& filePath
            );
            
            Asset(
                unsigned long primeId, 
                const std::string& filePath,
                const std::string& name
            );
            
            virtual ~Asset(void) = default;
        
        // ========================================================================================
        // | ACCESSORS |
        // =============
        
        public:
            virtual unsigned long getTypeId(void) const = 0;
            virtual std::string getTypeName(void) const = 0;
        
            Asset::Contact getContact(void) const;
            unsigned long getFormId(void) const;
            std::string getFilePath(void) const;
            
            std::string getName(void) const;
            
            std::size_t getPriority(void) const;
            bool isActive(void) const;            
        
        // ========================================================================================
        // | MODIFIERS |
        // =============
        
        public:
            void setName(const std::string& name);
            
            void setPriority(std::size_t priority);
            void shiftPriority(std::size_t shift);
            void deactivate(void);
        
        // ========================================================================================
        // | COMPARATORS |
        // ===============
        
        public:
            bool equals(const Asset& other) const;
            std::size_t compare(const Asset& other) const;
        
        // ========================================================================================
        // | CONVERTERS |
        // ==============
        
        public:
            virtual std::string toString(void) const = 0;
            virtual Form toForm(void) const = 0;
        
        // ========================================================================================
        // | LOGISTICS |
        // =============
        
        public:
            bool save(void) const;
        
        // ========================================================================================
        // | MEMBERS |
        // ===========
        
        private:
            unsigned long _primeId; // unique id
            std::string _filePath; // path to read & write
            std::string _name; // display name
            
            std::size_t _priority; // #of stores until this entry is removed
    };
    
// ================================================================================================
// | OPERATORS |
// =============

    bool operator==(
        const Asset& assetA,
        const Asset& assetB
    );
    
    bool operator<(
        const Asset& assetA,
        const Asset& assetB
    );
    
    bool operator<=(
        const Asset& assetA,
        const Asset& assetB
    );
    
    bool operator>(
        const Asset& assetA,
        const Asset& assetB
    );

    bool operator>=(
        const Asset& assetA,
        const Asset& assetB
    );
    
    std::ostream& operator<<(
        std::ostream& stream, 
        const Asset& asset
    );
}

