#include "JSONDeserialize.hpp"
#include <cstdarg>
#include <cstdio>
#include <string>
#include <string_view>

namespace IonixEngine {

void printfln(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf(format, args);
    va_end(args);
    printf("\n");
}

JSONDeserialize::JSONDeserialize(const std::string& json) {
    data = std::string(json);
    pos = 0;
    brackets = std::stack<char>();
    objectContext = std::stack<size_t>();
}

void JSONDeserialize::Reset() {
    pos = 0;
    while (!brackets.empty()) { brackets.pop(); }
    while (!objectContext.empty()) { objectContext.pop(); }
}

bool JSONDeserialize::Start() {
    printfln("[JSON Deserialize] Begining parsing json data: %s", data.c_str());
    Reset();
    pos = data.find('{', 0);
    return pos != data.npos;
}

bool JSONDeserialize::GetInt(const std::string& fieldname, int* out) {
    size_t newPos = pos;
    if (data[pos] != '"') {
        newPos = data.find('"', pos);
        if (newPos == data.npos) { printfln("[JSON Deserialize] Could not find start of field after char %zu", pos); return false; }
        pos = newPos;
    }
    newPos = data.find(std::basic_string_view(fieldname.c_str()).data(), pos);
    if (newPos == data.npos) { printfln("[JSON Deserialize] Could not find fieldname: %s after char %zu", fieldname.c_str(), pos); return false; }
    pos = newPos;
    newPos = data.find(':', pos);
    if (newPos == data.npos) { printfln("[JSON Deserialize] Malformed JSON after char %zu, expected ':'", pos); return false; }
    pos = newPos;
    newPos = data.find(",", 0);
    if (newPos == data.npos) { printfln("[JSON Deserialize] Malformed JSON after char %zu, expected int", pos); return false; }
    printfln("Found end of integer at pos: %zu, started at %zu -> length: %zu", newPos, pos, newPos - pos);
    char* valid;
    std::string substr = data.substr(pos, newPos - pos).c_str();
    printfln("Looking at substr: %s", substr.c_str());
    int value = strtol(substr.c_str(), &valid, 10);
    if (valid) {
        *out = value;
    } else {
        printfln("[JSON Deserialize] Could not parse int");
        return false;
    }
    pos = newPos;
    return true;
}

bool JSONDeserialize::End() {
    bool foundEnd = true;
    for (size_t i = data.find('}', pos); i != data.npos && i < data.length(); i++) {
        if (std::isspace(data[foundEnd])) { continue; }
        else { foundEnd = false; break; }
    }
    if (!foundEnd) {
        printfln("[JSON] Deserializer found more data but End() was called...");
    }
    Reset();
    return foundEnd;
}


}
