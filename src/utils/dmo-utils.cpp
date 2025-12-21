#include "utils/dmo-utils.hpp"
#include "utils/string-utils.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace dm {
    void dmoDecode(DMO& dmo, const std::string& string) {
        _input(std::istringstream(string));
    }
    
    std::string dmoEncode(const DMO& dmo) {
        std::string string;
        std::string newLine;
        for (const DMOSection& section : dmo) {
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

    bool dmoRead(DMO& dmo, const std::string& filePath) {    
        // attempt to open the file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            return false;
        }
        
        _input(dmo, file);
        
        // failed to read file
        if (file.bad()) {
            return false;
        }
        
        file.close();
        return true;
    }
    
    bool dmoWrite(DMO& dmo, const std::string& filePath) {
        // attempt to open the file
        std::ofstream file(filePath);
        if (!file.is_open()) {
            return false;
        }
        
        _output(dmo, file);
        
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
    
    static void _input(DMO& dmo, std::istream& stream) {
        // remove all sections
        dmo.clear();
    
        bool awaitingSection = true;
        std::string header;
        std::string line;
        while (std::getline(stream, line)) {
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
                
                // add a new vector of entries under the header (if it doesn't exist)
                if (dmo.count(header) == 0) {
                    dmo[header] = std::vector<std::string>();
                }
                
                awaitingSection = false;
                continue;
            }
            
            // else append the line to the entries under the header
            else {
                dmo[header].push_back(line);
            }
        }
    }
    
    static void _output(DMO& dmo, std::ostream& stream) {
        std::string newLine;        
        for (const DMOSection& section : dmo) {
            // separate rows using a newline
            stream << newLine;
            newLine = "\n\n";
            
            // append the header
            string << section.first;
            
            // append each entry
            for (std::string entry : section.second) {
                string << "\n" << entry;
            }
        }
    }
}
