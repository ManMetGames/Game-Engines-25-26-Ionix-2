#pragma once

#include <cstddef>
#include <limits>
#include <stack>
#include <string>
namespace IonixEngine {

class JSONDeserialize {
    std::string data = std::string();
    size_t pos = 0;
    std::stack<char> brackets = std::stack<char>();
    std::stack<size_t> objectContext = std::stack<size_t>();

    void Reset();
    size_t AdvanceToField(const std::string& fieldname);
    size_t AdvanceToNonWhitespace(size_t maxDistance = std::numeric_limits<size_t>().max());
    bool ValidEndObject();

public:
    JSONDeserialize(const std::string& json);

    // Start/Stop parsing
    bool Begin();
    bool End();

    // Fields
    bool BeginField(const std::string& fieldname);
    bool EndField();

    // Arrays
    bool BeginArray();
    bool HasNext();
    bool BeginElement();
    bool EndElement();
    bool EndArray();

    // Members
    bool GetInt(int* out);
    bool GetFloat(float* out);
    bool GetDouble(double* out);
    bool GetString(std::string* out);
    bool GetBool(bool* out);
    
    bool BeginObject();
    bool EndObject();

    // Utility
    bool StringInRange(const std::string& str, size_t range);
};

}
