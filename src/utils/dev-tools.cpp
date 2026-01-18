#include "utils/dev-tools.hpp"

#include <cstdlib>
#include <string>

namespace dm {

// ================================================================================================
// | GLOBALS |
// ===========

    bool _debugModeEnabled = true;

// ================================================================================================
// | ASSERTIONS |
// ==============
    
    void assertTrue(
        const std::string& testName,
        const std::string& message,
        bool condition
    ) {
        if (!condition) {
            std::cerr << "[Error] " << testName << " failed: " 
                << message << "\n";
            std::abort();
        }
    }
    
    void assertFalse(
        const std::string& testName,
        const std::string& message,
        bool condition
    ) {
        if (condition) {
            std::cerr << "[Error] " << testName << " failed: " 
                << message << "\n";
            std::abort();
        }
    }

// ================================================================================================
// | LOGGING |
// ===========

    void setDebugModeEnabled(bool enabled) {
        _debugModeEnabled = enabled;
    }
    
    bool isDebugModeEnabled(void) {
        return _debugModeEnabled;
    }
}

