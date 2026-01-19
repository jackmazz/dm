#include "utils/form.hpp"
#include "utils/string-tools.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace dm {

// ================================================================================================
// | ENCODING & DECODING |
// =======================

    Form Form::decode(const std::string& string) {
        Form form;
        
        std::istringstream stream(string);
        form._input(stream);

        return form;
    }

    std::string Form::encode(const Form& form) {
        std::ostringstream stream;
        form._output(stream);

        return stream.str();
    }

// ================================================================================================
// | LOGISTICS |
// =============

    bool Form::read(const std::string& filePath) {
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

    bool Form::write(const std::string& filePath) const {
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

    void Form::_input(std::istream& stream) {
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

    void Form::_output(std::ostream& stream) const {
        std::string newLine;
        for (const Form::Section& section : *this) {
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

// ========================================================================================
// | OPERATORS |
// =============

    std::ostream& operator<<(
        std::ostream& stream, 
        const Form& form
    ) {
        stream << Form::encode(form);
        return stream;
    }
}

