#include "TiledObject.hpp"
#include "b2_math.h"
#include "JSON/JSONDeserialize.hpp"
#include <sstream>
#include <string>

namespace IonixEngine {

b2Vec2 FromJson(JSONDeserialize *json) {
    b2Vec2 vector;
    json->BeginObject();
    json->BeginField("x");
    json->GetFloat(&vector.x);
    json->EndField();
    json->BeginField("y");
    json->GetFloat(&vector.y);
    json->EndField();
    json->EndObject();
    return vector;
}

TiledProperty::TiledProperty(JSONDeserialize* json) {
    bool ok = true;
    ok = json->BeginObject(); if (!ok) { return; }
    ok = json->BeginField("name"); if (!ok) { return; }
    ok = json->GetString(&name); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("type"); if (!ok) { return; }
    ok = json->GetString(&type); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("value"); if (!ok) { return; }
    ok = json->GetString(&value); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }
    ok = json->EndObject(); if (!ok) { return; }
}

std::string TiledProperty::ToString() {
    std::stringstream stream;
    stream << "\t\t\tProperty:\n";
    stream << "\t\t\t\tName: " << name << "\n";
    stream << "\t\t\t\tType: " << type << "\n";
    stream << "\t\t\t\tValue: " << value;

    return stream.str();
}

TiledObject::TiledObject(JSONDeserialize* json) {
    bool ok = true;
    ok = json->BeginObject(); if (!ok) { return; }
    ok = json->BeginField("height"); if (!ok) { return; }
    ok = json->GetFloat(&size.y); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("id"); if (!ok) { return; }
    ok = json->GetInt(&id); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    bool isPolygon = false;
    isPolygon = json->BeginField("name");
    if (!isPolygon) {
        ok = json->GetString(nullptr); if (!ok) { return; }
        ok = json->EndField(); if (!ok) { return; }

        ok = json->BeginField("properties"); if (!ok) { return; }
        ok = json->BeginArray(); if (!ok) { return; }
        if (ok) {
            do {
                json->BeginElement();
                properties.push_back(TiledProperty(json));
                json->EndElement();
            } while (json->HasNext());
        }
        ok = json->EndArray(); if (!ok) { return; }
        ok = json->EndField(); if (!ok) { return; }
    } else {
        ok = json->BeginField("polygon"); if (!ok) { return; }

        ok = json->BeginArray();
        if (ok) {
            do {
                json->BeginElement();
                polygon.push_back(FromJson(json));
                json->EndElement();
            } while (json->HasNext());
        }
        ok = json->EndArray();
        ok = json->EndField(); if (!ok) { return; }
    }

    ok = json->BeginField("rotation"); if (!ok) { return; }
    ok = json->GetFloat(&rotation); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("type"); if (!ok) { return; }
    ok = json->GetString(nullptr); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("visible"); if (!ok) { return; }
    ok = json->GetBool(&visible); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("width"); if (!ok) { return; }
    ok = json->GetFloat(&size.x); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("x"); if (!ok) { return; }
    ok = json->GetFloat(&position.x); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->BeginField("y"); if (!ok) { return; }
    ok = json->GetFloat(&position.y); if (!ok) { return; }
    ok = json->EndField(); if (!ok) { return; }

    ok = json->EndObject(); if (!ok) { return; }
}

std::string TiledObject::ToString() {
    std::stringstream stream;

    stream << "\t\tTiledObject:\n";
    stream << "\t\t\tID: " << id << "\n";
    stream << "\t\t\tPosition: [ " << position.x << ", " << position.y << "]\n";
    stream << "\t\t\tSize: [ " << size.x << ", " << size.y << "]\n";
    stream << "\t\t\tRotation:" << rotation << "\n";
    stream << "\t\t\tVisible:" << visible << "\n";
    if (polygon.size() > 0) {
        stream << "\t\t\tProperties:\n";
        for (TiledProperty& property : properties) {
            stream << property.ToString() << "\n";
        }
    }
    if (polygon.size() > 0) {
        stream << "\t\t\tPolygons:\n";
        for (b2Vec2 vert : polygon) {
            stream << "\t\t\t\t[ " << vert.x << ", " << vert.y << "]\n";
        }
    }

    return stream.str();
}

}
