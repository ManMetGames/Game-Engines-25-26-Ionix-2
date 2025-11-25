#include "TiledMap.hpp"
#include "Tiled/TiledObject.hpp"
#include "JSON/JSONDeserialize.hpp"

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

TiledLayer::TiledLayer(JSONDeserialize* json) {
    isTile = json->StringInRange("\"data\"", 10);
    if (isTile) {
        // TODO: This is a tile layer, implementation will be merged later
        // tileLayer = TiledTileLayer(json);
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

    ok = json->End(); if (!ok) { return; }
}

}
