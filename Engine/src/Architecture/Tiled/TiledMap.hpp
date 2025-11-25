#pragma once

#include "TiledObject.hpp"
#include "b2_math.h"
#include "JSON/JSONDeserialize.hpp"
#include <cstdint>
#include <string>

namespace IonixEngine {

class TiledObjectLayer {
public:
    std::string drawOrder;
    int32_t id;
    std::string name;
    std::vector<TiledObject> objects;
    float opacity;
    std::vector<TiledProperty> properties; // Not sure if I need to parse these
    std::string type;
    bool visible;
    b2Vec2 position;

    TiledObjectLayer() = default;
    TiledObjectLayer(JSONDeserialize* json);
    std::string ToString();
};

// Functionality will be merged in later, for now just remove tile layers
class TiledTileLayer {
public:
    bool isTile;
    std::vector<int> data;
    int32_t id;
    std::string name;
    float opacity;
    std::string type;
    bool visible;
    b2Vec2 postion;
    b2Vec2 size;

    TiledTileLayer() = default;
    TiledTileLayer(JSONDeserialize* json);
};

class TiledLayer {
public:
    bool isTile;
    TiledObjectLayer objectLayer;
    TiledTileLayer tileLayer;

    TiledLayer(JSONDeserialize* json);
};

class TiledMap {
public:
    bool infinite;
    std::vector<TiledLayer> layers;
    b2Vec2 size;
    int32_t compressionLevel;

    TiledMap(JSONDeserialize* json);
};

};
