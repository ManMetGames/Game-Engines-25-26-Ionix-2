#include "StringUtils.hpp"

namespace IonixEngine {
    void ReplaceAll(std::string& str, const std::string& pattern, const std::string& to) {
        if (pattern.empty()) { return; }
        size_t startPos = 0;
        while((startPos = str.find(pattern, startPos)) != std::string::npos) {
            str.replace(startPos, pattern.length(), to);
            startPos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }
}
