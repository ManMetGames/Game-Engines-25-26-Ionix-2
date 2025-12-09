#ifdef TILED_TEST
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
    printf("\tSize: [ %.1f, %.1f ]\n", map.size.x, map.size.y);
    printf("\tTile Size: [ %.1f, %.1f ]\n", map.tileSize.x, map.tileSize.y);
    printf("\tLayers:\n");
    for (TiledLayer& layer : map.layers) {
        if (layer.isTile) { continue; }
        printf("\tLayer: \n%s\n", layer.objectLayer.ToString().c_str());
    }
    printf("\tOrientation: %s\n", map.orientation.c_str());
    printf("\tRender Order: %s\n", map.renderOrder.c_str());
    printf("\tTiled Version: %s\n", map.tiledVersion.c_str());

    for (TiledTileset& tileset : map.tilesets) {
        printf("\tTileset: \n%s\n", tileset.ToString().c_str());
    }

    printf("\tType: %s\n", map.type.c_str());
    printf("\tVersion: %s\n", map.version.c_str());

}
#else
void LoadTileMap() {};
#endif
