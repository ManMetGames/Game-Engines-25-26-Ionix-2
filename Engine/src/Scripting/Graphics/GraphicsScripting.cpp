#include "Scripting/Graphics/GraphicsScripting.h"
#include "Architecture/TextureManager/TextureManager.h"
#include "GraphicsScripting.h"
#include <Graphics/AnimatedSpriteComponent.h>

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

        auto current_frame = [](AnimatedSpriteComponent* spriteComponent ) -> int{
            spriteComponent->getCurrentFrame();
        };

        auto end_frame = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getEndFrame();
            };


        auto rows = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getRows();
            };

        auto columns = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getCols();
            };

        auto width = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getSpriteWidth();
            };

        auto height = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getSpriteHeight();
            };

        auto zed_order = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getZedOrder();
            };

        auto total_frames = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getTotalFrames();
            };

        auto current_column = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getCurrentCol();
            };

        auto current_row = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getCurrentRow();
            };

        
        lua["Texture"] = lua.create_table_with(
            "add_texture", texture
        );
    }

}