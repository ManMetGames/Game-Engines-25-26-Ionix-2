#include "TiledMap.hpp"
#include <fstream>
#include <sstream>

using namespace IonixEngine;

int main(void) {
    std::ifstream json = std::ifstream("./../../../../Tiled/TiledObjects.json");
    std::stringstream jsonStr;
    jsonStr << json.rdbuf();
    std::string jsonData = jsonStr.str();
    JSONDeserialize jsonDeserialize = JSONDeserialize(jsonData);
    TiledMap map = TiledMap(&jsonDeserialize);

    printf("Map data:\n");
    printf("\tCompression: %d\n", map.compressionLevel);
    printf("\tInfinite: %d\n", map.infinite);
    printf("\tSize: %.1f, %.1f\n", map.size.x, map.size.y);
    printf("\tLayers:\n");
    for (TiledLayer& layer : map.layers) {
        if (layer.isTile) { continue; }
        printf("\t\tLayer: %s\n", layer.objectLayer.ToString().c_str());
    }
}
