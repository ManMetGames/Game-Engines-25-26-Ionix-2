#pragma once

#include <cstddef>
#include <stack>
#include <string>
namespace IonixEngine {

class JSONDeserialize {
    std::string data = std::string();
    size_t pos = 0;
    std::stack<char> brackets = std::stack<char>();
    std::stack<size_t> objectContext = std::stack<size_t>();

    void Reset();

public:
    JSONDeserialize(const std::string& json);

    // Start/Stop parsing
    bool Start();
    bool End();

    // Arrays
    bool BeginArray(const std::string& fieldname);
    bool HasNext();
    bool EndArray();

    // Members
    bool GetInt(const std::string& fieldname, int* out);
    bool GetFloat(const std::string& fieldname, float* out);
    bool GetDouble(const std::string& fieldname, double* out);
    bool GetString(const std::string& fieldname, std::string* out);
    bool GetBool(const std::string& fieldname, bool* out);
    
    bool BeginObject(const std::string& fieldname);
    bool EndObject();
};

}
