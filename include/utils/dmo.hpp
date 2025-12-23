#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace dm {
    class DMO : public std::unordered_map<std::string, std::vector<std::string>> {
        public:
            /* types */
                using Section = std::pair<const std::string, std::vector<std::string>>;
            
            /* encoding & decoding */
                void decode(const std::string& string);
                std::string encode();
            
            /* logistics */
                bool read(const std::string& filePath);
                bool write(const std::string& filePath);
        
        private:
            /* logistics */
                void _input(std::istream& stream);
                void _output(std::ostream& stream);
    };
}

