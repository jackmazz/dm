#include "asset.hpp"
#include "config.hpp"

#include <cstddef>
#include <ostream>
#include <string>

namespace dm {

// ================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Asset::Asset(void)
        : Asset(0, "")
    {}

    Asset::Asset(
        unsigned long primeId,
        const std::string& filePath
    )
        : Asset(primeId, filePath, "")
    {}

    Asset::Asset(
        unsigned long primeId,
        const std::string& filePath,
        const std::string& name
    ) {
        this->_primeId = primeId;
        this->_filePath = filePath;
        
        this->setName(name);
        this->setPriority(0);
    }

// ================================================================================================
// | ACCESSORS |
// =============

    Asset::Contact Asset::getContact(void) const {
        Asset::Contact contact;
        contact.typeId = this->getTypeId();
        contact.typeName = this->getTypeName();
        contact.primeId = this->getPrimeId();
        contact.filePath = this->getFilePath();
        return contact;
    }

    unsigned long Asset::getPrimeId(void) const {
        return this->_primeId;
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

// ================================================================================================
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

// ================================================================================================
// | COMPARATORS |
// ===============

    bool Asset::equals(const Asset& other) const {
        return this->compare(other) == 0;
    }

    std::size_t Asset::compare(const Asset& other) const {
        return this->getPrimeId() - other.getPrimeId();
    }

// ================================================================================================
// | LOGISTICS |
// =============

    bool Asset::save(void) const {
        // write the form to the state directory
        return this->toForm().write(
            std::string(DM_STATE_DIR) + this->getFilePath()
        );
    }
    
// ================================================================================================
// | OPERATORS |
// =============

    bool operator==(
        const Asset& assetA,
        const Asset& assetB
    ) {
        return assetA.equals(assetB);
    }
    
    bool operator<(
        const Asset& assetA,
        const Asset& assetB
    ) { 
        return assetA.compare(assetB) < 0;
    }
    
    bool operator<=(
        const Asset& assetA,
        const Asset& assetB
    ) {
        return assetA.compare(assetB) <= 0;
    }
    
    bool operator>(
        const Asset& assetA,
        const Asset& assetB
    ) {
        return assetA.compare(assetB) > 0;
    }

    bool operator>=(
        const Asset& assetA,
        const Asset& assetB
    ) {
        return assetA.compare(assetB) >= 0;
    }
    
    std::ostream& operator<<(
        std::ostream& stream, 
        const Asset& asset
    ) {
        stream << asset.toString();
        return stream;
    }
}

