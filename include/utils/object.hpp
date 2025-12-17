#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace dm {
    typedef std::pair<const std::string, std::vector<std::string>> ObjectSection;

    class Object : public std::unordered_map<std::string, std::vector<std::string>> {
        public:
            bool read(const std::string& filePath);
            bool write(const std::string& filePath);
    };
}

