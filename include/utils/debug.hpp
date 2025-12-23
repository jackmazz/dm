#pragma once

#include <format>
#include <iostream>
#include <sstream>
#include <string>

namespace dm {
    template <typename... Args>
    void debugPrint(double id, Args&&... args) {
        std::ostringstream stream;
        
        stream << "debug-" << id;
        (stream << ... << args);
        
        std::cout << stream.str() << "\n";
    }
}

