#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace dm.DMO {
    typedef std::unordered_map<std::string, std::vector<std::string>> DMO;
    typedef std::pair<std::string, std::vector<std::string>> DMOSection;
    
    void dmoDecode(DMO& dmo, const std::string& string);
    std::string dmoEncode(const DMO& dmo);
    
    bool dmoRead(DMO& dmo, const std::string& filePath);
    bool dmoWrite(const DMO& dmo, const std::string& filePath);
}

