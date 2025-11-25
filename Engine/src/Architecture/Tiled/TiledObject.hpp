#pragma once

#include "JSON/JSONDeserialize.hpp"
#include "b2_math.h"
#include <cstdint>
#include <string>
#include <vector>
namespace IonixEngine {

b2Vec2 FromJson(JSONDeserialize* json);

class TiledProperty {
public:
    std::string name;
    std::string type;
    std::string value;

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
    std::vector<b2Vec2> polygon; // Will be present on all polygon colliders ???
    TiledObject(JSONDeserialize* json);
    std::string ToString();
};

};
