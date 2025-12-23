#include "utils/schema.hpp"
#include "utils/strings.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace dm {

// ====================================================================================================
// | ENCODING & DECODING |
// =======================

    Schema Schema::decode(const std::string& string) {
        Schema schema;
    
        std::istringstream stream(string);
        schema._input(stream);
        
        return schema;
    }
    
    std::string Schema::encode(const Schema& schema) {
        std::ostringstream stream; 
        schema._output(stream);
        
        return stream.str();
    }

// ====================================================================================================
// | LOGISTICS |
// =============

    bool Schema::read(const std::string& filePath) {
        // attempt to open the file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            return false;
        }
        
        this->_input(file);
        
        // failed to read file
        if (file.bad()) {
            return false;
        }
        
        file.close();
        return true;
    }
    
    bool Schema::write(const std::string& filePath) const {
        // attempt to open the file
        std::ofstream file(filePath);
        if (!file.is_open()) {
            return false;
        }
        
        this->_output(file);
        
        // failed to write to file
        if (file.bad()) {
            return false;
        }
        
        file.flush();
        
        // failed to flush file
        if (file.bad()) {
            return false;
        }
        
        file.close();
        return true;
    }
    
    void Schema::_input(std::istream& stream) {
        bool awaitingSection = true;
        std::string header;
        std::string line;
        while (std::getline(stream, line)) {
            // trim the line
            line = strings::trim(line);
        
            // if empty line encountered, begin a new section
            if (line == "") {
                awaitingSection = true;
                continue;
            }
            
            // if awaiting a section begin a new one
            if (awaitingSection) {
                // set the header to the upper-case line
                header = strings::toUpperCase(line);
                
                // add a new vector of entries under the header (if it doesn't exist)
                if (this->count(header) == 0) {
                    this->operator[](header) = std::vector<std::string>();
                }
                
                awaitingSection = false;
                continue;
            }
            
            // else append the line to the entries under the header
            else {
                this->operator[](header).push_back(line);
            }
        }
    }
    
    void Schema::_output(std::ostream& stream) const {
        std::string newLine;        
        for (const Section& section : *this) {
            // separate rows using a newline
            stream << newLine;
            newLine = "\n\n";
            
            // append the header
            stream << section.first;
            
            // append each entry
            for (std::string entry : section.second) {
                stream << "\n" << entry;
            }
        }
    }
}
