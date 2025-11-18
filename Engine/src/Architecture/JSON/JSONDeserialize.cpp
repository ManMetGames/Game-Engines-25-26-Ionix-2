#include "JSONDeserialize.hpp"
#include <cstddef>
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

bool JSONDeserialize::GetInt(int* out) {
    size_t newPos = pos;

    newPos = data.find('\n', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected int\n", pos); return false; }
    char* valid;
    std::string substr = data.substr(pos + 1, newPos - pos - 1).c_str();
    int value = strtol(substr.c_str(), &valid, 10);
    if (valid) {
        *out = value;
    } else {
        printf("[JSON Deserialize] Could not parse int from string: %s", substr.c_str());
        return false;
    }
    pos = newPos;
    return true;
}

bool JSONDeserialize::GetFloat(float* out) {
    size_t newPos = pos;

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

bool JSONDeserialize::GetDouble(double* out) {
    size_t newPos = pos;

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

bool JSONDeserialize::GetBool(bool* out) {
    size_t newPos = pos;

    newPos = data.find('\n', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected bool\n", pos); return false; }
    std::string substr = data.substr(pos + 1, newPos - pos - 1).c_str();
    if (substr.find("true",  substr.length(), 0) != substr.npos) { *out = true; }
    if (substr.find("false", substr.length(), 0) != substr.npos) { *out = false; } else { printf("[JSON Deserialize] Could not parse bool\n"); return false; }
    pos = newPos;
    return true;
}

bool JSONDeserialize::GetString(std::string* out) {
    size_t newPos = pos;

    size_t startPos = data.find('"', pos);
    if (startPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected '\"'\n", pos); return false; }
    size_t endPos = data.find('"', startPos + 1) - 1;
    if (endPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected ending '\"'\n", startPos); return false; }
    *out = data.substr(startPos + 1, endPos - startPos);
    newPos = data.find('\n', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected string\n", pos); return false; }
    return true;
}

bool JSONDeserialize::BeginField(const std::string& fieldname) {
    size_t newPos = AdvanceToField(fieldname);
    if (newPos == data.npos) { return false; }
    pos = newPos;
    objectContext.push(objectContext.size());
    return true;
}

bool JSONDeserialize::EndField() {
    return ValidEndObject();
}

bool JSONDeserialize::BeginArray() {
    size_t newPos = pos;

    newPos = data.find('[', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find start of array after char %zu\n", pos); return false; }
    pos = newPos;
    if (data[pos + 1] == ']') {
        return false;
    }
    brackets.push('[');
    objectContext.push(objectContext.size());
    pos++;

    return true;
}

bool JSONDeserialize::HasNext() {
    if (data[pos - 1] == ',') {
        return true;
    } else {
        return false;
    }
}

bool JSONDeserialize::BeginElement() {
    size_t newPos = AdvanceToNonWhitespace();
    if (newPos == data.npos) { return false; }
    pos = newPos;
    return true;
}

bool JSONDeserialize::EndElement() {
    if (data[pos] == '\n') {
        return true;
    }
    size_t newPos = data.find('\n', pos);
    if (newPos < pos) {
        printf("[JSON Deserialize] Could not find new line after element at char %zu\n", pos);
        return false;
    }
    pos = newPos - 1;
    printf("[JSON Deserialize] Finishing element - remaining string: %s", data.substr(pos).c_str());
    return true;
}

bool JSONDeserialize::EndArray() {
    return ValidEndObject();
}

bool JSONDeserialize::BeginObject() {
    size_t newPos = pos;
    newPos = data.find('{', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find start of object after char %zu", pos); return false; }

    brackets.push('{');
    objectContext.push(objectContext.size());
    pos = newPos;

    return true;
}

bool JSONDeserialize::EndObject() {
    size_t newPos = data.find('}', pos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find closing bracket of object\n"); return false; }
    pos = newPos;
    if (brackets.top() != '{') { printf("[JSON Deserialize] Most recent unresolved bracket was not '{'\n"); return false; }
    brackets.pop();
    return ValidEndObject();
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

size_t JSONDeserialize::AdvanceToField(const std::string& fieldname) {
    size_t newPos = pos;
    if (data[newPos] != '"') {
        newPos = data.find('"', newPos);
        if (newPos == data.npos) { printf("[JSON Deserialize] Could not find start of field after char %zu\n", newPos); return data.npos; }
    }
    newPos = data.find(std::basic_string_view(fieldname.c_str()).data(), newPos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Could not find fieldname: %s after char %zu\n", fieldname.c_str(), newPos); return data.npos; }
    newPos = data.find(':', newPos);
    if (newPos == data.npos) { printf("[JSON Deserialize] Malformed JSON after char %zu, expected ':'\n", newPos); return data.npos; }

    return newPos;
}

size_t JSONDeserialize::AdvanceToNonWhitespace(size_t maxDistance) {
    for (size_t i = pos + 1; i < data.size(); i++) {
        if (!isspace(data[i])) {
            return i - 1;
        }
    }
    return data.npos;
}

bool JSONDeserialize::ValidEndObject() {
    size_t size = objectContext.top();
    objectContext.pop();
    return objectContext.size() == size;
}

}
