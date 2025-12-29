#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace dm::strings {

// ================================================================================================
// | FORMATTING |
// ==============

    std::string trim(const std::string& string);

    std::string toUpperCase(const std::string& string);
    std::string toLowerCase(const std::string& string);

// ================================================================================================
// | PARSING |
// ===========

    std::vector<std::string> split(
        const std::string& string, 
        const std::string& delimiter, 
        std::size_t maxSplits = 0
    );

    std::vector<std::vector<std::string>> parseDsv(
        std::vector<std::string> strings, 
        const std::string& delimiter, 
        std::size_t columnCount = 0
    );

    std::unordered_map<std::string, std::string> parseIni(
        std::vector<std::string> strings, 
        const std::string& delimiter
    );
}

