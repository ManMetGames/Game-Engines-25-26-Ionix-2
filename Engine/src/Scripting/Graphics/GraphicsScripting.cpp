#include "Scripting/Graphics/GraphicsScripting.h"
#include "Architecture/TextureManager/TextureManager.h"
#include "GraphicsScripting.h"
#include "Architecture/Application.h"
#include "Architecture/Scene.h"
#include <Graphics/SpriteComponent.h>
#include <Graphics/Camera.h>

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

        auto setColor = [](SpriteComponent* spriteComponent, int r, int g, int b) {
            spriteComponent->setColor(static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b));
        };

        auto emitParticle = [](uint32_t textureHash,
                               int renderLayer,
                               float x,
                               float y,
                               float vx,
                               float vy,
                               float lifetime,
                               float startSize,
                               float endSize) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }

            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetParticleSystem().Emit(
                textureHash,
                renderLayer,
                x,
                y,
                vx,
                vy,
                lifetime,
                startSize,
                endSize,
                255, 255, 255, 255,
                255, 255, 255, 0,
                7,
                0.0f, 0.0f,
                0.0f
            );
        };

        //camera
		auto Camera = [](float startX, float startY, int renderLayer) {
			return new IonixEngine::Camera(startX, startY, renderLayer);
			};

		auto initializeCamera = [](IonixEngine::Camera* camera) {
			camera->Init();
			};

        auto SetZoom = [](SDL_Renderer* renderer, int zoom) {
            Application::Get().currentCam->SetZoom(renderer, zoom);
            };

        auto ClearBackground = [](SDL_Renderer* renderer) {
            Application::Get().currentCam->ClearBackground(renderer);
            };

        auto SetColor = [](Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
            Application::Get().currentCam->SetColor(r, g, b, a);
            };
        auto MoveCamera = [](float deltaX, float deltaY, bool moveCamDelta) {
            Application::Get().currentCam->MoveCamera(deltaX, deltaY, moveCamDelta);
            };
		auto switchcamera = []() {
			Application::Get().currentCam->SwitchCamera();
			};
		auto rotateCamera = [](float angle) {
			Application::Get().currentCam->Rotate(angle);
			};
        auto rotateEnity = [](Entity* e, float angle) {
            Application::Get().currentCam->RotateEntity(e, angle);
            };
		auto renderToScreen = [](SDL_Renderer* renderer, float posX, float posY, float sizeX, float sizeY) {
			Application::Get().currentCam->RenderToScreen(renderer, posX, posY, sizeX, sizeY);
			};

	

        auto setAnimation = [](SpriteComponent* spriteComponent, int rows, int cols, int spriteW, int spriteH) {
            spriteComponent->setAnimation(rows, cols, spriteW, spriteH);
            };

        auto setTickRate = [](SpriteComponent* spriteComponent, float x) {
            spriteComponent->setTickRate(x);
            };

        auto getTickRate = [](SpriteComponent* spriteComponent) -> float{
            return spriteComponent->getTickRate();
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
            "image_width", getImageWidth,
            "image_height", getImageHeight,
            "zed_order", getZedOrder,
            "total_frames", getTotalFrames,
            "current_column", getCurrentColumn,
            "current_row", getCurrentRow,
            "get_tick_rate", getTickRate,
            "set_current_frame", setCurrentFrame,
            "set_end_frame", setEndFrame,
            "set_rows", setRows,
            "set_columns", setColumns,
            "set_width", setWidth,
            "set_height", setHeight,
            "set_image_width", setImageWidth,
            "set_image_height", setImageHeight,
            "set_zed_order", setZedOrder,
            "get_playback_mode", getPlaybackMode,
            "set_playback_mode", setPlaybackMode,
            "set_color", setColor,
            "set_animation", setAnimation,
            "set_tick_rate", setTickRate
        );
       
        lua["Particles"] = lua.create_table_with(
            "emit", emitParticle
        );

        lua["Camera"] = lua.create_table_with(
            "create_camera", Camera,
			"initialize_camera", initializeCamera,
            "set_zoom", SetZoom,
            "clear_background", ClearBackground,
            "Set_color", SetColor,
			"move_camera", MoveCamera,
			"switch_camera", switchcamera,
			"rotate_camera", rotateCamera,
			"rotate_entity", rotateEnity,
			"render_to_screen", renderToScreen
        );
    }

}