#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace dm {

// ================================================================================================
// | ASSERTIONS |
// ==============

    template <typename T>
    void assertEquals(
        const std::string& testName,
        const std::string& message,
        const T& expected, 
        const T& actual
    ) {
        if (expected != actual) {
            std::cout << "[Error] " << testName << " failed: "
                << message
                << " ("
                    << "expected != actual"
                << ") -> (" 
                    << expected << " != " << actual
                << ")" << "\n";
            std::abort();
        }
    }
    
    void assertTrue(
        const std::string& testName,
        const std::string& message,
        bool condition
    );

// ================================================================================================
// | LOGGING |
// ===========

    void setDebugModeEnabled(bool enabled);
    bool isDebugModeEnabled(void);

    template <typename... Args>
    void debugPrint(double id, Args&&... args) {
        if (!isDebugModeEnabled()) {
            return;
        }
    
        std::ostringstream stream;

        stream << "[Debug-" << id << "]";
        ((stream << " " << std::forward<Args>(args)), ...);

        std::cout << stream.str() << "\n";
    }
}

