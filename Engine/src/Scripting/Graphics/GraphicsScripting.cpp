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

        auto getCurrentFrame = [](AnimatedSpriteComponent* spriteComponent ) -> int{
            spriteComponent->getCurrentFrame();
        };

        auto getEndFrame = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getEndFrame();
            };


        auto getRows = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getRows();
            };

        auto getColumns = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getCols();
            };

        auto getWidth = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getSpriteWidth();
            };

        auto getHeight = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getSpriteHeight();
            };

        auto getZedOrder = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getZedOrder();
            };

        auto getTotalFrames = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getTotalFrames();
            };

        auto getCurrentColumn = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getCurrentCol();
            };

        auto getCurrentRow = [](AnimatedSpriteComponent* spriteComponent) -> int {
            spriteComponent->getCurrentRow();
            };

        auto setCurrentFrame = [](AnimatedSpriteComponent* spriteComponent, int x) -> int {
            spriteComponent->setCurrentFrame(x);
            };

        auto setEndFrame = [](AnimatedSpriteComponent* spriteComponent, int x) -> int {
            spriteComponent->setEndFrame(x);
            };


        auto setRows = [](AnimatedSpriteComponent* spriteComponent, int x) -> int {
            spriteComponent->setRows(x);
            };

        auto setColumns = [](AnimatedSpriteComponent* spriteComponent, int x) -> int {
            spriteComponent->setCols(x);
            };

        auto setWidth = [](AnimatedSpriteComponent* spriteComponent, int x) -> int {
            spriteComponent->setSpriteWidth(x);
            };

        auto setHeight = [](AnimatedSpriteComponent* spriteComponent, int x) -> int {
            spriteComponent->setSpriteHeight(x);
            };

        auto setZedOrder = [](AnimatedSpriteComponent* spriteComponent, int x) -> int {
            spriteComponent->setZedOrder(x);
            };

        
        lua["Texture"] = lua.create_table_with(
            "add_texture", texture
        );

        lua["Sprite"] = lua.create_table_with(
            "current_frame", getCurrentFrame,
            "end_frame", getEndFrame,
            "rows", getRows,
            "columns", getColumns,
            "width", getWidth,
            "height", getHeight,
            "zed_order", getZedOrder,
            "total_frames", getTotalFrames,
            "current_column", getCurrentColumn,
            "current_row", getCurrentRow,
            "set_current_frame", setCurrentFrame,
            "set_end_frame", setEndFrame,
            "set_rows", setRows,
            "set_columns", setColumns,
            "set_width", setWidth,
            "set_height", setHeight,
            "set_zed_order", setZedOrder
        );
    }

}