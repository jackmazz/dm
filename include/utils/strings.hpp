#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace dm::strings {
    /* formatting */
        std::string trim(const std::string& string);
        
        std::string toUpperCase(const std::string& string);
        std::string toLowerCase(const std::string& string);
    
    /* parsing */
        std::vector<std::string> split(
            const std::string& string, 
            const std::string& delimiter, 
            std::size_t maxSplits = 0
        );
        
        std::vector<std::vector<std::string>> parseDSV (
            std::vector<std::string> strings,
            const std::string& delimiter,
            std::size_t maxValues = 0
        );
        
        std::unordered_map<std::string, std::string> parseIni(
            std::vector<std::string> strings,
            const std::string& delimiter
        );
}

