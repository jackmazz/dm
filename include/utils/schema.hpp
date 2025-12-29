#pragma once

#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dm {
    class Schema : public std::unordered_map<std::string, std::vector<std::string>> {
        
        // ========================================================================================
        // | TYPES |
        // =========
        
        public:
            using Section = std::pair<const std::string, std::vector<std::string>>;
        
        // ========================================================================================
        // | ENCODING & DECODING |
        // =======================
        
        public:
            static Schema decode(const std::string& string);
            static std::string encode(const Schema& schema);
        
        // ========================================================================================
        // | LOGISTICS |
        // =============
        
        public:
            bool read(const std::string& filePath);
            bool write(const std::string& filePath) const;
        
        private:
            void _input(std::istream& stream);
            void _output(std::ostream& stream) const;
    };

// ================================================================================================
// | OPERATORS |
// =============
    
    std::ostream& operator<<(
        std::ostream& stream, 
        const Schema& schema
    );
}

