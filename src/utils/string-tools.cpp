#include "utils/string-tools.hpp"

#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace dm::stringTools {

// ================================================================================================
// | FORMATTING |
// ==============

    std::string trim(const std::string& string) {
        std::size_t start = 0;
        std::size_t end = string.length();

        // iterate from the start to find the first space-esque character
        while (start < end && std::isspace(string[start])) {
            start++;
        }

        // iterate from the end to find the last space-esque character
        while (end > start && std::isspace(string[end - 1])) {
            end--;
        }

        // create a substring to omit all space-esque characters
        return string.substr(start, end - start);
    }

    std::string toLowerCase(const std::string& string) {
        // create a copy of the input string
        std::string copy = string;

        // convert each character to be unsigned and lower-case
        for (char& c : copy) {
            c = std::toupper(static_cast<unsigned char>(c));
        }

        return copy;
    }
    
    std::string toUpperCase(const std::string& string) {
        // create a copy of the input string
        std::string copy = string;

        // convert each character to be unsigned and upper-case
        for (char& c : copy) {
            c = std::toupper(static_cast<unsigned char>(c));
        }

        return copy;
    }

// ================================================================================================
// | PARSING |
// ===========

    std::vector<std::string> split(
        const std::string& string, 
        const std::string& delimiter
    ) {
        return stringTools::split(string, delimiter, 0);
    }

    std::vector<std::string> split(
        const std::string& string, 
        const std::string& delimiter, 
        std::size_t maxSplits
    ) {
        std::vector<std::string> splits;
        std::string split;

        // split until the max limit is reached, or the max limit is 0 (unlimited)
        std::size_t start = 0;
        std::size_t end = 0;
        while (maxSplits == 0 || splits.size() + 1 < maxSplits) {
            // find the index of the delimiter
            end = string.find(delimiter, start);
            if (end == std::string::npos) {
                break;
            }

            // create a substring from the start until the delimiter
            split = string.substr(start, end - start);

            // update the start index
            start = end + delimiter.length();

            // append the substring
            splits.push_back(split);
        }

        // append any remaining characters
        if (start <= string.length()) {
            splits.push_back(string.substr(start));
        }

        return splits;
    }
    

    std::vector<std::vector<std::string>> parseDsv(
        std::vector<std::string> strings, 
        const std::string& delimiter, 
        std::size_t columnCount
    ) {
        std::vector<std::vector<std::string>> rows;
        for (const std::string& string : strings) {
            // separate values by the delimiter
            std::vector<std::string> splits = split(
                string, 
                delimiter, 
                columnCount
            );
            
            // trim the values
            for (std::size_t i = 0; i < splits.size(); i++) {
                splits[i] = stringTools::trim(splits[i]);
            }
            
            // append the row if it contains all the required columns
            if (columnCount == 0 || splits.size() == columnCount) {
                rows.push_back(splits);
            }
        }
        
        return rows;
    }

    std::map<std::string, std::string> parseIni(
        std::vector<std::string> strings, 
        const std::string& associator
    ) {
        std::map<std::string, std::string> keyValues;
        for (const std::string& string : strings) {
            // separate the key & value by the delimiter
            std::vector<std::string> split = stringTools::split(
                string, 
                associator, 2
            );
            
            // insert the key-value if a delimiter was found
            if (split.size() == 2) {
                std::string key = stringTools::trim(split[0]);
                std::string value = stringTools::trim(split[1]);
                keyValues[key] = value;
            }
        }

        return keyValues;
    }
}

