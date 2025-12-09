#include "TiledMap.hpp"
#include "TiledObject.hpp"
#include "Architecture/JSON/JSONDeserialize.hpp"
#include <cstdio>
#include <sstream>

namespace IonixEngine {

TiledObjectLayer::TiledObjectLayer(JSONDeserialize* json) {
    bool ok = true;
    ok = json->BeginObject(); if (!ok) { return; }

    ok = json->BeginField("draworder"); if (!ok) { return; }
    ok = json->GetString(&drawOrder); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("id"); if (!ok) { return; }
    ok = json->GetInt(&id); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("name"); if (!ok) { return; }
    ok = json->GetString(&name); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("objects"); if (!ok) { return; }
    ok = json->BeginArray();
    if (ok) {
        do {
            json->BeginElement();
            objects.push_back(TiledObject(json));
            json->EndElement();
        } while (json->HasNext());
    }
    ok = json->EndArray(); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("opacity"); if (!ok) { return; }
    ok = json->GetFloat(&opacity); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("properties"); if (!ok) { return; }
    ok = json->BeginArray();
    if (ok) {
        do {
            json->BeginElement();
            properties.push_back(TiledProperty(json));
            json->EndElement();
        } while (json->HasNext());
    }
    ok = json->EndArray(); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("type"); if (!ok) { return; }
    ok = json->GetString(&type); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("visible"); if (!ok) { return; }
    ok = json->GetBool(&visible); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("x"); if (!ok) { return; }
    ok = json->GetFloat(&position.x); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("y"); if (!ok) { return; }
    ok = json->GetFloat(&position.y); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    
    ok = json->EndObject(); if (!ok) { return; }
}

std::string TiledObjectLayer::ToString() {
    std::stringstream stream;
    stream << "\tObject layer\n";
    stream << "\t\tDraw Order: " << drawOrder << "\n";
    stream << "\t\tID: " << id << "\n";
    stream << "\t\tName: " << name << "\n";
    stream << "\t\tObjects: " << "\n";
    for (TiledObject& object : objects) {
        stream << object.ToString() << "\n";
    }
    stream << "\t\tOpacity: " << opacity << "\n";
    stream << "\t\tProperties:\n";
    for (TiledProperty& property : properties) {
        stream << property.ToString() << "\n";
    }
    stream << "\t\tType: " << type << "\n";
    stream << "\t\tVisible: " << visible << "\n";
    stream << "\t\tPosition: [" << position.x << ", " << position.y << "]\n";

    return stream.str();
}

TiledTileLayer::TiledTileLayer(JSONDeserialize* json) {
    bool ok = true;
    ok = json->BeginObject(); if (!ok) { return; }

    ok = json->BeginField("data"); if (!ok) { printf("Could not find tile data\n"); return; }
    ok = json->BeginArray();
    if (ok) {
        do {
            int value;
            json->BeginElement();
            if (json->GetInt(&value)) {
                data.push_back(value);
            }
            json->EndElement();
        } while (json->HasNext());
    } else {
        printf("[Tiled] failed to begin data array\n");
    }
    ok = json->EndArray(); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    printf("[Tiled] Finished data array\n");

    ok = json->BeginField("height"); if (!ok) { return; }
    ok = json->GetFloat(&size.y); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    printf("[Tiled] Parsed height\n");

    ok = json->BeginField("id"); if (!ok) { return; }
    ok = json->GetInt(&id); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    printf("[Tiled] Parsed id\n");

    ok = json->BeginField("name"); if (!ok) { return; }
    ok = json->GetString(&name); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    printf("[Tiled] Parsed name\n");

    ok = json->BeginField("opacity"); if (!ok) { return; }
    ok = json->GetFloat(&opacity); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    printf("[Tiled] Parsed opacity\n");

    ok = json->BeginField("type"); if (!ok) { return; }
    ok = json->GetString(&type); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    printf("[Tiled] Parsed type\n");

    ok = json->BeginField("visible"); if (!ok) { return; }
    ok = json->GetBool(&visible); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    printf("[Tiled] Parsed visible\n");

    ok = json->BeginField("width"); if (!ok) { return; }
    ok = json->GetFloat(&size.x); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    printf("[Tiled] Parsed width\n");

    ok = json->BeginField("x"); if (!ok) { return; }
    ok = json->GetFloat(&position.x); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    printf("[Tiled] Parsed x\n");

    ok = json->BeginField("y"); if (!ok) { return; }
    ok = json->GetFloat(&position.x); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    printf("[Tiled] Parsed y\n");

    ok = json->EndObject(); if (!ok) { printf("Failed to finish tile layer\n"); return; }
    printf("[Tiled] Successfully loaded layer: %s\n", name.c_str());
}

TiledTileset::TiledTileset(JSONDeserialize* json) {
    bool ok = true;
    ok = json->BeginObject(); if (!ok) { return; };

    ok = json->BeginField("columns"); if (!ok) { return; }
    ok = json->GetInt(&columns); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("firstgid"); if (!ok) { return; }
    ok = json->GetInt(&firstGID); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("image"); if (!ok) { return; }
    ok = json->GetString(&imagePath); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("imageheight"); if (!ok) { return; }
    ok = json->GetFloat(&imageSize.y); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("imagewidth"); if (!ok) { return; }
    ok = json->GetFloat(&imageSize.x); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("margin"); if (!ok) { return; }
    ok = json->GetInt(&margin); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("name"); if (!ok) { return; }
    ok = json->GetString(&name); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("spacing"); if (!ok) { return; }
    ok = json->GetInt(&spacing); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("tilecount"); if (!ok) { return; }
    ok = json->GetInt(&tileCount); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("tileheight"); if (!ok) { return; }
    ok = json->GetFloat(&tileSize.y); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("tilewidth"); if (!ok) { return; }
    ok = json->GetFloat(&tileSize.x); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->EndObject(); if (!ok) { return; };
}

std::string TiledTileset::ToString() {
    std::stringstream stream;

    stream << "\t\tTileset:\n";
    stream << "\t\t\tColumns: " << columns << "\n";
    stream << "\t\t\tImage: " << imagePath << "\n";
    stream << "\t\t\tImage Size: [ " << imageSize.x << ", " << imageSize.y << " ]\n";
    stream << "\t\t\tMargin: " << margin << "\n";
    stream << "\t\t\tName: " << name << "\n";
    stream << "\t\t\tSpacing: " << spacing << "\n";
    stream << "\t\t\tTile Count: " << tileCount << "\n";
    stream << "\t\t\tTile Size: [ " << tileSize.x << ", " << tileSize.y << " ]\n";

    return stream.str();
}

int TiledTileset::Rows() {
    return tileCount / columns;
}

TiledLayer::TiledLayer(JSONDeserialize* json) {
    isTile = json->StringInRange("data", 16);
    if (isTile) {
        tileLayer = TiledTileLayer(json);
    } else {
        objectLayer = TiledObjectLayer(json);
    }
}

TiledMap::TiledMap(JSONDeserialize* json) {
    bool ok = true;
    ok = json->Begin(); if (!ok) { return; }

    ok = json->BeginField("compressionlevel"); if (!ok) { return; }
    ok = json->GetInt(&compressionLevel); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("height"); if (!ok) { return; }
    ok = json->GetFloat(&size.y); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("infinite"); if (!ok) { return; }
    ok = json->GetBool(&infinite); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("layers"); if (!ok) { return; }
    ok = json->BeginArray(); if (!ok) { return; }
    if (ok) {
        do {
            json->BeginElement();
            layers.push_back(TiledLayer(json));
            json->EndElement();
        } while (json->HasNext());
    }
    ok = json->EndArray(); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("nextlayerid"); if (!ok) { return; }
    ok = json->GetInt(nullptr); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("nextobjectid"); if (!ok) { return; }
    ok = json->GetInt(nullptr); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("orientation"); if (!ok) { return; }
    ok = json->GetString(&orientation); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("renderorder"); if (!ok) { return; }
    ok = json->GetString(&renderOrder); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("tiledversion"); if (!ok) { return; }
    ok = json->GetString(&tiledVersion); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("tileheight"); if (!ok) { return; }
    ok = json->GetFloat(&tileSize.y); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("tilesets"); if (!ok) { return; }
    ok = json->BeginArray(); if (!ok) { return; }
    if (ok) {
        do {
            json->BeginElement();
            tilesets.push_back(TiledTileset(json));
            json->EndElement();
        } while (json->HasNext());
    }
    ok = json->EndArray(); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("tilewidth"); if (!ok) { return; }
    ok = json->GetFloat(&tileSize.x); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("type"); if (!ok) { return; }
    ok = json->GetString(&type); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("version"); if (!ok) { return; }
    ok = json->GetString(&version); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("width"); if (!ok) { return; }
    ok = json->GetFloat(&size.x); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->End(); if (!ok) { return; }

}

size_t TiledMap::GetTilemapIdx(TiledTileLayer& layer) {
    size_t idx = 0;
    if (layer.data.size() == 0) { return 0; }
    int tile = layer.data[0];
    for (; idx < tilesets.size(); idx++) {
        TiledTileset* tileset = &tilesets[idx];
        if (tileset->firstGID >= tile && tileset->firstGID + tileset->tileCount <= tile) {
            return idx;
        }
    }
    return 0;
}

std::string TiledMap::ToString() {
    std::stringstream stream;
    stream << "Map data:\n";
    stream << "\tCompression: " << compressionLevel << "\n";
    stream << "\tInfinite: " << infinite << "\n";
    stream << "\tSize: [ " << size.x << ", " << size.y << "\n";
    stream << "\tTile Size: [ " << tileSize.x << ", " << tileSize.y << "\n";
    stream << "\tLayers:\n";
    for (TiledLayer& layer : layers) {
        if (layer.isTile) {
            stream << "\tTile layer: " << layer.tileLayer.name << "\n";
        } else {
            stream << "\tLayer: \n" << layer.objectLayer.ToString() << "\n";
        }
    }
    stream << "\tOrientation: " << orientation << "\n";
    stream << "\tRender Order: " << renderOrder << "\n";
    stream << "\tTiled Version: " << tiledVersion << "\n";

    for (TiledTileset& tileset : tilesets) {
        stream << "\tTileset: \n" << tileset.ToString() << "\n";
    }

    stream << "\tType: " << type << "\n";
    stream << "\tVersion: " << version << "\n";

    return stream.str();
}

}
