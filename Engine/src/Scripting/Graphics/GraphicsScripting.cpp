#include "Scripting/Graphics/GraphicsScripting.h"
#include "Architecture/TextureManager/TextureManager.h"
#include "GraphicsScripting.h"
#include <Graphics/SpriteComponent.h>

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

        auto getCurrentFrame = [](SpriteComponent* spriteComponent ) -> int{
            return spriteComponent->getCurrentFrame();
        };

        auto getEndFrame = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getEndFrame();
            };


        auto getRows = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getRows();
            };

        auto getColumns = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getCols();
            };

        auto getWidth = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getSpriteWidth();
            };

        auto getHeight = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getSpriteHeight();
            };

        auto getImageWidth = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getWidth();
            };

        auto getImageHeight = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getHeight();
            };

        auto getZedOrder = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getZedOrder();
            };

        auto getTotalFrames = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getTotalFrames();
            };

        auto getCurrentColumn = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getCurrentCol();
            };

        auto getCurrentRow = [](SpriteComponent* spriteComponent) -> int {
            return spriteComponent->getCurrentRow();
            };

        auto setCurrentFrame = [](SpriteComponent* spriteComponent, int x) {
            spriteComponent->setCurrentFrame(x);
            };

        auto setEndFrame = [](SpriteComponent* spriteComponent, int x) {
            spriteComponent->setEndFrame(x);
            };


        auto setRows = [](SpriteComponent* spriteComponent, int x) {
            spriteComponent->setRows(x);
            };

        auto setColumns = [](SpriteComponent* spriteComponent, int x) {
            spriteComponent->setCols(x);
            };

        auto setWidth = [](SpriteComponent* spriteComponent, int x) {
            spriteComponent->setSpriteWidth(x);
            };

        auto setHeight = [](SpriteComponent* spriteComponent, int x) {
            spriteComponent->setSpriteHeight(x);
            };

        auto setImageWidth = [](SpriteComponent* spriteComponent, int x) {
            spriteComponent->setWidth(x);
            };

        auto setImageHeight = [](SpriteComponent* spriteComponent, int x) {
            spriteComponent->setHeight(x);
            };

        auto setZedOrder = [](SpriteComponent* spriteComponent, int x) {
            spriteComponent->setZedOrder(x);
            };

        auto getPlaybackMode = [](SpriteComponent* spriteComponent) -> int{
            return spriteComponent->getPlaybackMode();
        };

        auto setPlaybackMode = [](SpriteComponent* spriteComponent, int playbackMode) {
            spriteComponent->setPlaybackMode(static_cast<IonixEngine::playbackOptions>(playbackMode));
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
            "set_zed_order", setZedOrder,
            "get_playback_mode", getPlaybackMode,
            "set_playback_mode", setPlaybackMode
        );
    }

}