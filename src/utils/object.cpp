#include "utils/object.hpp"
#include "utils/string-utils.hpp"

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace dm {
    bool Object::read(const std::string& filePath) {
        // remove all key-value pairs
        this->clear();
    
        // attempt to open the file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            return false;
        }
        
        bool awaitingSection = true;
        std::string header;
        std::string line;
        while (getline(file, line)) {
            // trim the line
            line = trimString(line);
        
            // if empty line encountered, begin a new section
            if (line == "") {
                awaitingSection = true;
                continue;
            }
            
            // if awaiting a section begin a new one
            if (awaitingSection) {
                // set the header to the upper-case line
                header = stringToUpperCase(line);
                
                // add a new vector of entries under the header
                // replace entries if they already exist under the header
                this->operator[](header) = std::vector<std::string>();
                
                awaitingSection = false;
                continue;
            }
            
            // else append the line to the entries under the header
            else {
                this->operator[](header).push_back(line);
            }
        }
        
        // failed to read file
        if (file.bad()) {
            return false;
        }
        
        file.close();
        return true;
    }
    
    bool Object::write(const std::string& filePath) {
        // attempt to open the file
        std::ofstream file(filePath);
        if (!file.is_open()) {
            return false;
        }
        
        for (const ObjectSection& section : *this) {
            const std::string& header = section.first;
            const std::vector<std::string>& entries = section.second;
            
            // write the header
            file << header << "\n";
            
            // write each entry
            for (const std::string& entry : entries) {
                file << entry << "\n";
            }
            
            // write a blank line to end the section
            file << "\n";
        }
        
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
}
