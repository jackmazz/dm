#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace dm {
    std::string stringTrim(const std::string& string);
    
    std::vector<std::string> stringSplit(
        const std::string& string, 
        const std::string& delimiter, 
        std::size_t maxSplits = 0
    );
    
    std::string stringToUpperCase(const std::string& string);
    std::string stringToLowerCase(const std::string& string);
}

