#pragma once

#include <cstddef>
#include <string>

#include "utils/schema.hpp"

namespace dm {
    class Asset {
    
        public:
        // ====================================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
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
        
        public:
        // ====================================================================================================
        // | ACCESSORS |
        // =============
        
            unsigned long getId(void) const;
            std::string getFilePath(void) const;
            std::string getName(void) const;
            
            std::size_t getPriority(void) const;
            bool isActive(void) const;
        
        public:
        // ====================================================================================================
        // | MODIFIERS |
        // =============
        
            void setName(const std::string& name);
            
            void setPriority(std::size_t priority);
            void shiftPriority(std::size_t shift);
            void deactivate(void);
        
        public:
        // ====================================================================================================
        // | CONVERTERS |
        // ==============
        
            virtual std::string toString(void) const = 0;
            virtual Schema toSchema(void) const = 0;
        
        public:
        // ====================================================================================================
        // | LOGISTICS |
        // =============
        
            bool save(void) const;

        private:
        // ====================================================================================================
        // | MEMBERS |
        // ===========
        
            unsigned long _id; // unique id
            std::string _filePath; // path to read & write
            std::string _name; // display name
            
            std::size_t _priority; // #of stores until this entry is removed
    };
}

