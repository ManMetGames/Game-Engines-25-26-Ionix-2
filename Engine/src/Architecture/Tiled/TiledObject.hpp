#pragma once

#include "Architecture/JSON/JSONDeserialize.hpp"
#include "b2_math.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
namespace IonixEngine {

b2Vec2 FromJson(JSONDeserialize* json);

enum PropertyType { Bool, Int, Float, String };

static std::unordered_map<std::string, PropertyType> typeLookup = {
    { "bool", Bool },
    { "int", Int },
    { "float", Float },
    { "string", String }
};

class TiledProperty {
public:
    std::string name;
    std::string type;
    bool boolValue;
    int intValue;
    float floatValue;
    std::string stringValue;

    TiledProperty(JSONDeserialize* json);
    std::string ToString();
};

class TiledObject {
public:
    std::vector<TiledProperty> properties;
    b2Vec2 position;
    b2Vec2 size;
    int32_t id;
    float rotation;
    bool visible;
    std::string name;
    std::vector<b2Vec2> polygon; // Will be present on all polygon colliders ???

    TiledObject(JSONDeserialize* json);
    std::string ToString();
};

};
