#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace dm::stringTools {

// ================================================================================================
// | FORMATTING |
// ==============

    std::string trim(const std::string& string);

    std::string toLowerCase(const std::string& string);
    std::string toUpperCase(const std::string& string);

// ================================================================================================
// | PARSING |
// ===========

    std::vector<std::string> split(
        const std::string& string, 
        const std::string& delimiter
    );
    
    std::vector<std::string> split(
        const std::string& string, 
        const std::string& delimiter, 
        std::size_t maxSplits
    );

    std::vector<std::vector<std::string>> parseDsv(
        std::vector<std::string> strings, 
        const std::string& delimiter, 
        std::size_t columnCount = 0
    );

    std::map<std::string, std::string> parseIni(
        std::vector<std::string> strings, 
        const std::string& associator
    );
}

