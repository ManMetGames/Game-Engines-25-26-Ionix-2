#include "TileMapRenderer.hpp"
#include "ECS/Component.hpp"
#include "Architecture/TextureManager/TextureManager.h"
#include "Graphics/QueueRenderer.h"

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
    uint32_t hash = TextureManager::HashFromPath(tileSet.imagePath);
    image = TextureManager::Get().GetRawTexture(hash);

    float x = tileLayer.position.x;
    float y = tileLayer.position.y;

    tileSize = tileSet.tileSize;

    for (int tile : tileLayer.data) {
        tile -= tileSet.firstGID; // For some reason tile ids start at 1
        if (tile != 0) {
            b2Vec2 src = b2Vec2 { static_cast<float>(tile % tileSet.columns) * tileSet.tileSize.x, tile / static_cast<float>(tileSet.columns) * tileSet.tileSize.y };
            tiles.push_back(std::make_pair(RectVCentred(b2Vec2{ x, y }, tileSize), RectV(src, tileSize)));
        }
        x += tileSet.tileSize.x;
        if (x >= tileLayer.size.x * tileSet.tileSize.x) {
            x = tileLayer.position.x;
            y += tileSet.tileSize.y;
        }
    }
}

void TileMapRenderer::Render(RenderData* data) {
    for (std::pair<SDL_Rect, SDL_Rect> tile : tiles) {
        RenderCall call = RenderCall {
            image, tile.first, tile.second
        };
        data->queue->AddToQueue(call);
    }
}

};
