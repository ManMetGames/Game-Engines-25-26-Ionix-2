#include "JSONDeserialize.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>

namespace IonixEngine {

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
    Reset();
    pos = data.find('{', 0);
    return pos != data.npos;
}

bool JSONDeserialize::GetInt(const std::string& fieldname, int* out) {
    size_t newPos = pos;
    if (data[pos] != '"') {
        newPos = data.find('"', pos);
        if (newPos == data.npos) { printf("[JSON Deserialize] Could not find start of field after char %zu\n", pos); return false; }
        pos = newPos;
    }
    newPos = data.find(std::basic_string_view(fieldname.c_str()).data(), pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find fieldname: %s after char %zu\n", fieldname.c_str(), pos); return false; }
    pos = newPos;
    newPos = data.find(':', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected ':'\n", pos); return false; }
    pos = newPos;
    newPos = data.find('\n', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected int\n", pos); return false; }
    char* valid;
    std::string substr = data.substr(pos + 1, newPos - pos - 1).c_str();
    int value = strtol(substr.c_str(), &valid, 10);
    if (valid) {
        *out = value;
    } else {
        printf("[JSON Deserialize] Could not parse int");
        return false;
    }
    pos = newPos;
    return true;
}

bool JSONDeserialize::GetFloat(const std::string& fieldname, float* out) {
    size_t newPos = pos;
    if (data[pos] != '"') {
        newPos = data.find('"', pos);
        if (newPos == data.npos) { printf("[JSON Deserialize] Could not find start of field after char %zu\n", pos); return false; }
        pos = newPos;
    }
    newPos = data.find(std::basic_string_view(fieldname.c_str()).data(), pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find fieldname: %s after char %zu\n", fieldname.c_str(), pos); return false; }
    pos = newPos;
    newPos = data.find(':', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected ':'\n", pos); return false; }
    pos = newPos;
    newPos = data.find('\n', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected float\n", pos); return false; }
    char* valid;
    std::string substr = data.substr(pos + 1, newPos - pos - 1).c_str();
    float value = strtof(substr.c_str(), &valid);
    if (valid) {
        *out = value;
    } else {
        printf("[JSON Deserialize] Could not parse float");
        return false;
    }
    pos = newPos;
    return true;
}

bool JSONDeserialize::GetDouble(const std::string& fieldname, double* out) {
    size_t newPos = pos;
    if (data[pos] != '"') {
        newPos = data.find('"', pos);
        if (newPos == data.npos) { printf("[JSON Deserialize] Could not find start of field after char %zu\n", pos); return false; }
        pos = newPos;
    }
    newPos = data.find(std::basic_string_view(fieldname.c_str()).data(), pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find fieldname: %s after char %zu\n", fieldname.c_str(), pos); return false; }
    pos = newPos;
    newPos = data.find(':', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected ':'\n", pos); return false; }
    pos = newPos;
    newPos = data.find('\n', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected double\n", pos); return false; }
    char* valid;
    std::string substr = data.substr(pos + 1, newPos - pos - 1).c_str();
    double value = strtod(substr.c_str(), &valid);
    if (valid) {
        *out = value;
    } else {
        printf("[JSON Deserialize] Could not parse double");
        return false;
    }
    pos = newPos;
    return true;
}

bool JSONDeserialize::GetBool(const std::string& fieldname, bool* out) {
    size_t newPos = pos;
    if (data[pos] != '"') {
        newPos = data.find('"', pos);
        if (newPos == data.npos) { printf("[JSON Deserialize] Could not find start of field after char %zu\n", pos); return false; }
        pos = newPos;
    }
    newPos = data.find(std::basic_string_view(fieldname.c_str()).data(), pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find fieldname: %s after char %zu\n", fieldname.c_str(), pos); return false; }
    pos = newPos;
    newPos = data.find(':', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected ':'\n", pos); return false; }
    pos = newPos;
    newPos = data.find('\n', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected bool\n", pos); return false; }
    std::string substr = data.substr(pos + 1, newPos - pos - 1).c_str();
    if (substr.find("true",  substr.length(), 0) != substr.npos) { *out = true; }
    if (substr.find("false", substr.length(), 0) != substr.npos) { *out = false; } else { printf("[JSON Deserialize] Could not parse bool"); return false; }
    pos = newPos;
    return true;
}

bool JSONDeserialize::GetString(const std::string& fieldname, std::string* out) {
    size_t newPos = pos;
    if (data[pos] != '"') {
        newPos = data.find('"', pos);
        if (newPos == data.npos) { printf("[JSON Deserialize] Could not find start of field after char %zu\n", pos); return false; }
        pos = newPos;
    }
    newPos = data.find(std::basic_string_view(fieldname.c_str()).data(), pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find fieldname: %s after char %zu\n", fieldname.c_str(), pos); return false; }
    pos = newPos;
    newPos = data.find(':', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected ':'\n", pos); return false; }
    pos = newPos;
    size_t startPos = data.find('"', pos);
    if (startPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected '\"'\n", pos); return false; }
    size_t endPos = data.find('"', startPos + 1);
    if (endPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected ending '\"'\n", startPos); return false; }
    *out = data.substr(startPos, endPos - startPos);
    newPos = data.find('\n', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected string\n", pos); return false; }
    return true;
}

bool JSONDeserialize::BeginArray(const std::string& fieldname) {
    size_t newPos = pos;
    if (data[pos] != '"') {
        newPos = data.find('"', pos);
        if (newPos == data.npos) { printf("[JSON Deserialize] Could not find start of field after char %zu\n", pos); return false; }
        pos = newPos;
    }
    newPos = data.find(std::basic_string_view(fieldname.c_str()).data(), pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find fieldname: %s after char %zu\n", fieldname.c_str(), pos); return false; }
    pos = newPos;
    newPos = data.find(':', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected ':'\n", pos); return false; }
    pos = newPos;

    newPos = data.find('[', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find start of array after char %zu", pos); return false; }
    pos = newPos;
    if (data[pos + 1] == ']') {
        return false;
    }

    return true;
}

bool JSONDeserialize::EndArray() {
    size_t value = objectContext.top();
    objectContext.pop();
    return value = objectContext.size();
}

bool JSONDeserialize::HasNext() {
    return data[pos + 1] == ',';
}

bool JSONDeserialize::End() {
    bool foundEnd = true;
    for (size_t i = data.find('}', pos); i != data.npos && i < data.length(); i++) {
        if (std::isspace(data[foundEnd])) { continue; }
        else { foundEnd = false; break; }
    }
    if (!foundEnd) {
        printf("[JSON] Deserializer found more data but End() was called...\n");
    }
    Reset();
    return foundEnd;
}


}
