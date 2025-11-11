#include "JSONDeserialize.hpp"
#include <string>

using namespace IonixEngine;

int main(int argc, char **argv) {
    std::string json_ = std::string("{ test: 10, }");

    JSONDeserialize deserialize = JSONDeserialize(json_);
    int test = 0;
    if (!deserialize.Start()) { printfln("Could not start parsing"); }
    if (!deserialize.GetInt("test", &test)) { printfln("Could not parse test"); }
    else { printfln("Successfully parsed int 'test' as %d", test); }
    if (!deserialize.End()) { printfln("More json to parse"); }
    json_.pop_back();
}
