#include "TileMapRenderer.hpp"
#include "ECS/Component.hpp"
#include "Architecture/TextureManager/TextureManager.h"
#include "Graphics/QueueRenderer.h"
#include <filesystem>

namespace IonixEngine {

SDL_Rect RectV(const b2Vec2& position, const b2Vec2& size) { 
    return SDL_Rect {
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(size.x),
        static_cast<int>(size.y)
    };
}

SDL_Rect RectVCentred(const b2Vec2& position, const b2Vec2& size) { 
    return SDL_Rect {
        static_cast<int>(position.x - size.x * 0.5f),
        static_cast<int>(position.y - size.y * 0.5f),
        static_cast<int>(size.x),
        static_cast<int>(size.y)
    };
}

TileMapRenderer::TileMapRenderer(Entity* entity, const TiledTileLayer& tileLayer, const TiledTileset& tileSet) : Component(entity, false, true, false) {
    uint32_t hash = TextureManager::Hash(std::filesystem::path(tileSet.imagePath).stem().string());
    image = TextureManager::Get().GetRawTexture(hash);

    float x = tileLayer.position.x;
    float y = tileLayer.position.y;

    tileSize = tileSet.tileSize;

    for (int tile : tileLayer.data) {
        b2Vec2 destPos = b2Vec2 { static_cast<float>(tile % tileSet.columns) * tileSet.tileSize.x, tile / static_cast<float>(tileSet.columns) * tileSet.tileSize.y };
        tiles[b2Vec2 { x, y }] = RectV(destPos, tileSize);
        x += tileSet.tileSize.x;
        if (x >= tileLayer.size.x * tileSet.tileSize.x) {
            x = tileLayer.position.x;
            y += tileSet.tileSize.y;
        }
    }
}

void TileMapRenderer::Render(RenderData* data) {
    for (std::pair<b2Vec2, SDL_Rect> tile : tiles) {
        RenderCall call = RenderCall {
            image, RectVCentred(tile.first, tileSize), tile.second
        };
        data->queue->AddToQueue(call);
    }
}

};
