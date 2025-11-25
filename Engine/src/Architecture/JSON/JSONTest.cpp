#include "JSONDeserialize.hpp"
#include <fstream>
#include <sstream>
#include <string>

using namespace IonixEngine;

int main(int argc, char **argv) {
    std::ifstream inputJson = std::ifstream("./test.json");
    std::stringstream inputData;
    inputData << inputJson.rdbuf();
    inputJson.close();
    std::string json = inputData.str();
    printf("Input Json:\n%s\n", json.c_str());

    JSONDeserialize deserialize = JSONDeserialize(json);
    int test = 0;
    float floatvalue = 0;
    bool boolvalue = false;
    std::string stringvalue;
    if (!deserialize.Start()) { printf("Could not start parsing\n"); }
    if (!deserialize.GetInt("int", &test)) { printf("Could not parse 'int'\n"); }
    else { printf("Successfully parsed int 'int' as %d\n", test); }
    if (!deserialize.GetFloat("float", &floatvalue)) { printf("Could not parse 'float'\n"); }
    else { printf("Successfully parsed float 'float' as %f\n", floatvalue); }
    if (!deserialize.GetBool("bool", &boolvalue)) { printf("Could not parse 'bool'\n"); }
    else { printf("Successfully parsed bool 'bool' as %b\n", boolvalue); }
    if (!deserialize.GetString("string", &stringvalue)) { printf("Could not parse 'bool'\n"); }
    else { printf("Successfully parsed string 'string' as %s\n", stringvalue.c_str()); }
    if (!deserialize.End()) { printf("More json to parse\n"); }
}
