#include "utils/asset.hpp"

#include <cstddef>
#include <string>

namespace dm {

// ====================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================
    
    Asset::Asset(void) : Asset(0, "") {}
    
    Asset::Asset(
        unsigned long id, 
        const std::string& filePath
    ) 
        : Asset(id, filePath, "")
    {}
    
    Asset::Asset(
        unsigned long id, 
        const std::string& filePath,
        const std::string& name
    ) {
        this->_id = id;
        this->_filePath = filePath;
        
        this->setName(name);
        this->setPriority(0);
    }

// ====================================================================================================
// | ACCESSORS |
// =============
    
    unsigned long Asset::getId(void) const {
        return this->_id;
    }
    
    std::string Asset::getFilePath(void) const {
        return this->_filePath;
    }
    
    std::string Asset::getName(void) const {
        return this->_name;
    }
    
    std::size_t Asset::getPriority(void) const {
        return this->_priority;
    }
    
    bool Asset::isActive(void) const {
        return this->getPriority() > 0;
    }

// ====================================================================================================
// | MODIFIERS |
// =============

    void Asset::setName(const std::string& name) {
        this->_name = name;
    }

    void Asset::setPriority(std::size_t priority) {
        this->_priority = priority;
    }
    
    void Asset::shiftPriority(std::size_t shift) {
        this->setPriority(this->getPriority() + shift);
    }
    
    void Asset::deactivate(void) {
        this->setPriority(0);
    }
    
// ====================================================================================================
// | LOGISTICS |
// =============
    
    bool Asset::save(void) const {
        return this->toSchema().write(this->getFilePath());
    }
}

