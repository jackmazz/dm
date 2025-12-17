#include "utils/string-utils.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace dm {
    std::string trimString(const std::string& string) {
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

    std::vector<std::string> splitString(
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

    std::string stringToUpperCase(const std::string& string) {
        // create a copy of the input string
        std::string copy = string;
        
        // convert each character to be unsigned and upper-case
        for (char& c: copy) {
            c = std::toupper(static_cast<unsigned char>(c));
        }
        
        return copy;
    }
    
    std::string stringToLowerCase(const std::string& string) {
        // create a copy of the input string
        std::string copy = string;
        
        // convert each character to be unsigned and lower-case
        for (char& c: copy) {
            c = std::toupper(static_cast<unsigned char>(c));
        }
        
        return copy;
    }
}

