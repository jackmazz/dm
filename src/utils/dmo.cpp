#include "utils/dmo.hpp"
#include "utils/strings.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace dm {

// ====================================================================================================
// | ENCODING & DECODING |
// =======================

    void DMO::decode(const std::string& string) {
        std::istringstream stream(string);
        this->_input(stream);
    }
    
    std::string DMO::encode() {
        std::string string;
        std::string newLine;
        for (const DMO::Section& section : *this) {
            // separate rows using a newline
            string += newLine;
            newLine = "\n\n";
            
            // append the header
            string += section.first;
            
            // append each entry
            for (std::string entry : section.second) {
                string += "\n" + entry;
            }
        }
        
        return string;
    }

// ====================================================================================================
// | LOGISTICS |
// =============

    bool DMO::read(const std::string& filePath) {
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
    
    bool DMO::write(const std::string& filePath) {
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
    
    void DMO::_input(std::istream& stream) {
        // remove all sections
        this->clear();
    
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
    
    void DMO::_output(std::ostream& stream) {
        std::string newLine;        
        for (const DMO::Section& section : *this) {
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
