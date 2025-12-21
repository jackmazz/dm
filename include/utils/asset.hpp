#pragma once

#include "utils/dmo-utils.hpp"

#include <cstddef>
#include <string>

namespace dm {
    class Asset {
        public:
            /* constructors & destructors */
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
                
                ~Asset(void);
            
             /* accessors */
                unsigned long getId(void) const;
                std::string getFilePath(void) const;
                std::string getName(void) const;
                
                std::size_t getPriority(void) const;
            
            /* modifiers */
                void setName(const std::string& name);
            
                void setPriority(std::size_t priority);
            
            /* converters */
                virtual std::string toString(void) const;
                virtual DMO toDMO(void) const;
        
        private:            
            /* members */
                unsigned long _id;      // unique id
                std::string _filePath;  // path to read & write
                std::string _name;      // display name
                
                std::size_t _priority;  // #of stores until this entry is removed
    };
}

