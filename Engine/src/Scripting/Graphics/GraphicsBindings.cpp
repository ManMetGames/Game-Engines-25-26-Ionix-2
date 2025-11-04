#include "Scripting/Graphics/GraphicsBindings.h"
#include "Architecture/TextureManager/TextureManager.h"

namespace IonixEngine {

    void RegisterGraphicsBindings(sol::state& lua) {
        auto texture = [](std::string filePath, std::string alias) {
            TextureManager::Get().AddTexture(filePath, alias);
            };

        lua["Texture"] = lua.create_table_with(
            "add_texture", texture
        );
    }

}