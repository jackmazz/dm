#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dm {
    class Schema : public std::unordered_map<std::string, std::vector<std::string>> {        
        public:
            /* types */
                using Section = std::pair<const std::string, std::vector<std::string>>;
        
            /* encoding & decoding */
                static Schema decode(const std::string& string);
                static std::string encode(const Schema& schema);
            
            /* logistics */
                bool read(const std::string& filePath);
                bool write(const std::string& filePath) const;
        
        private:
            /* logistics */
                void _input(std::istream& stream);
                void _output(std::ostream& stream) const;
    };
}

