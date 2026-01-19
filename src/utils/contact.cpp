#include "utils/contact.hpp"

#include <cstddef>
#include <ostream>
#include <string>

namespace dm {

// ================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Contact::Contact(void)
        : Contact(0, "", 0, "");
    {}
                
    Contact::Contact(
        unsigned long typeId,
        std::string& typeName,
        unsigned long primeId, 
        const std::string& filePath
    ) {
        this->_typeId = typeId;
        this->_typeName = typeName;
        this->_primeId = primeId;
        this->_filePath = filePath;
    }

// ================================================================================================
// | ACCESSORS |
// =============

    unsigned long Contact::getTypeId(void) const {
        return this->_typeId;
    }

    std::string Contact::getTypeName(void) const {
        return this->_typeName;
    }

    unsigned long Contact::getPrimeId(void) const {
        return this->_primeId;
    }

    std::string Contact::getFilePath(void) const {
        return this->_filePath;
    }

// ================================================================================================
// | COMPARATORS |
// ===============

    bool Contact::equals(const Contact& other) const {
        return this->compare(other) == 0;
    }

    std::size_t Contact::compare(const Contact& other) const {
        return this->getPrimeId() - other.getPrimeId();
    }

// ================================================================================================
// | CONVERTERS |
// ==============

    std::string Contact::toString(void) const {
        return std::string("(")
            + this->getTypeName() + ", "
            + std::to_string(this->getTypeId()) + ", "
            + std::to_string(this->getPrimeId()) + ", "
            + this->getFilePath() + ")";
    }

// ================================================================================================
// | OPERATORS |
// =============

    bool operator==(
        const Contact& contactA,
        const Contact& contactB
    ) {
        return contactA.equals(contactB);
    }

    bool operator<(
        const Contact& contactA,
        const Contact& contactB
    ) {
        return contactA.compare(contactB) < 0;
    }

    bool operator<=(
        const Contact& contactA,
        const Contact& contactB
    ) {
        return contactA.compare(contactB) <= 0;
    }

    bool operator>(
        const Contact& contactA,
        const Contact& contactB
    ) {
        return contactA.compare(contactB) > 0;
    }

    bool operator>=(
        const Contact& contactA,
        const Contact& contactB
    ) {
        return contactA.compare(contactB) >= 0;
    }

    std::ostream& operator<<(
        std::ostream& stream, 
        const Contact& contact
    ) {
        stream << contact.toString();
        return stream;
    }
}

