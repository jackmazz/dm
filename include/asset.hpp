#pragma once

#include "utils/schema.hpp"

#include <cstddef>
#include <string>
#include <utility>

namespace dm {
    class Asset {
        
        // ========================================================================================
        // | TYPES |
        // =========
        
        public:
            using Contact = std::pair<unsigned long, std::string>;
        
        // ========================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
        public:
            Asset(void);
            
            Asset(
                unsigned long id, 
                const std::string& filePath
            );
            
            Asset(
                unsigned long id, 
                const std::string& filePath, 
                const std::string& name
            );
            
            virtual ~Asset(void) = default;
        
        // ========================================================================================
        // | ACCESSORS |
        // =============
        
        public:
            Asset::Contact getContact(void) const;
        
            unsigned long getId(void) const;
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
        // | CONVERTERS |
        // ==============
        
        public:
            virtual std::string toString(void) const = 0;
            virtual Schema toSchema(void) const = 0;
        
        // ========================================================================================
        // | LOGISTICS |
        // =============
        
        public:
            bool save(void) const;
        
        // ========================================================================================
        // | MEMBERS |
        // ===========
        
        private:
            unsigned long _id; // unique id
            std::string _filePath; // path to read & write
            std::string _name; // display name
            
            std::size_t _priority; // #of stores until this entry is removed
    };
    
// ================================================================================================
// | OPERATORS |
// =============
    
    std::ostream& operator<<(
        std::ostream& stream, 
        const Asset& asset
    );
}

