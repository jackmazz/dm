#include "actor.hpp"
#include "config.hpp"
#include "stage.hpp"
#include "utils/cache.hpp"

#include <cstddef>

namespace dm {

// ====================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================
    
    CacheEntry::CacheEntry(unsigned long id) {
        this->_id = id;
        this->_priority = 0;
    }
    
    CacheEntry::~CacheEntry() {}

// ====================================================================================================
// | ACCESSORS |
// =============
    
    unsigned long CacheEntry::id() {
        return this->_id;
    }
    
    std::size_t CacheEntry::priority(void) {
        return this->_priority;
    }
}

