#include "Scripting/Graphics/GraphicsScripting.h"
#include "Architecture/TextureManager/TextureManager.h"
#include "GraphicsScripting.h"

namespace IonixEngine {

    GraphicsScripting* GraphicsScripting::s_Instance = nullptr;

    GraphicsScripting& GraphicsScripting::Get() {
        if (!s_Instance)
            s_Instance = new GraphicsScripting();
        return *s_Instance;
    }

    void IonixEngine::GraphicsScripting::Init(sol::state& lua)
    {
        auto texture = [](std::string filePath, std::string alias) {
            TextureManager::Get().AddTexture(filePath, alias);
            };

        lua["Texture"] = lua.create_table_with(
            "add_texture", texture
        );
    }

}