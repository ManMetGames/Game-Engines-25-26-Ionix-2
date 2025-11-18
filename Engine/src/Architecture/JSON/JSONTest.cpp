#include "JSONDeserialize.hpp"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace IonixEngine;

struct Object {
    int intvalue;
    std::string stringvalue;
};

int main(int argc, char **argv) {
    std::ifstream inputJson = std::ifstream("./test.json");
    std::stringstream inputData;
    inputData << inputJson.rdbuf();
    inputJson.close();
    std::string json = inputData.str();
    printf("Input Json:\n%s\n", json.c_str());

    JSONDeserialize deserialize = JSONDeserialize(json);
    int intvalue = 0;
    float floatvalue = 0;
    bool boolvalue = false;
    std::string stringvalue;
    if (!deserialize.Start()) { printf("Could not start parsing\n"); }

    if (!deserialize.BeginField("int")) { printf("Could not find field 'int'\n"); }
    if (!deserialize.GetInt(&intvalue)) { printf("Could not parse 'int'\n"); }
    if (!deserialize.EndField()) { printf("Could not find end field 'int'\n"); }

    if (!deserialize.BeginField("float")) { printf("Could not find field 'float'\n"); }
    if (!deserialize.GetFloat(&floatvalue)) { printf("Could not parse 'float'\n"); }
    if (!deserialize.EndField()) { printf("Could not find end field 'float'\n"); }

    if (!deserialize.BeginField("bool")) { printf("Could not find field 'bool'\n"); }
    if (!deserialize.GetBool(&boolvalue)) { printf("Could not parse 'bool'\n"); }
    if (!deserialize.EndField()) { printf("Could not find end field 'bool'\n"); }

    if (!deserialize.BeginField("string")) { printf("Could not find field 'string'\n"); }
    if (!deserialize.GetString(&stringvalue)) { printf("Could not parse 'string'\n"); }
    if (!deserialize.EndField()) { printf("Could not find end field 'string'\n"); }

    std::vector<int> array;
    if (!deserialize.BeginField("array")) { printf("Did not find array\n"); }
    if (!deserialize.BeginArray()) { printf("Array was empty/null\n"); }
    {
        int value = 0;
    do {
        if (!deserialize.BeginElement()) { printf("Could not start element\n"); break; }
        if (!deserialize.GetInt(&value)) { 
            printf("Failed to parse element\n");
            break;
        } else {
            array.push_back(value);
        }
        if (!deserialize.EndElement()) { printf("Could not end element\n"); break; }
    } while (deserialize.HasNext());
    }
    if (!deserialize.EndArray()) { printf("Did not succesfully end array\n"); }
    if (!deserialize.EndField()) { printf("Did not find end of array field\n"); }

    Object object;
    if (!deserialize.BeginField("object")) { printf("Did not find Object 'object'\n"); }
    if (!deserialize.BeginObject()) { printf("Did not find start of object"); }

    if (!deserialize.BeginField("int")) { printf("Could not find field 'int'\n"); }
    if (!deserialize.GetInt(&object.intvalue)) { printf("Could not parse 'object.int'\n"); }
    if (!deserialize.EndField()) { printf("Could not find end field 'int'\n"); }

    if (!deserialize.BeginField("string")) { printf("Could not find field 'string'\n"); }
    if (!deserialize.GetString(&object.stringvalue)) { printf("Could not parse 'string'\n"); }
    if (!deserialize.EndField()) { printf("Could not find end field 'string'\n"); }

    if (!deserialize.EndObject()) { printf("Did not find end of object"); }
    if (!deserialize.EndField()) { printf("Did not find end of field 'object'"); }

    if (!deserialize.End()) { printf("More json to parse\n"); }

    std::stringstream arrayString;
    std::copy(array.begin(), array.end(), std::ostream_iterator<int>(arrayString, ","));

    std::stringstream objectString;
    objectString << "{ int: " << object.intvalue << ", string: \"" << object.stringvalue << " }";
    printf("[Deserialized]: int: %d, float: %.1f, bool: %d, string: %s, array: %s, object: %s\n", intvalue, floatvalue, boolvalue, stringvalue.c_str(), arrayString.str().c_str(), objectString.str().c_str());
}
