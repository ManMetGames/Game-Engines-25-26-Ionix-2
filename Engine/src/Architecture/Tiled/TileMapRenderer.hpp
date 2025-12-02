#include "Architecture/ECS/Component.hpp"
#include "SDL_render.h"
#include "Tiled/TiledMap.hpp"
#include <unordered_map>

namespace IonixEngine {

class TileMapRenderer : public Component {
private:
    SDL_Texture* image;
    std::unordered_map<b2Vec2, SDL_Rect> tiles;
    b2Vec2 tileSize;
public:
    TileMapRenderer(Entity* entity, const TiledTileLayer& tileLayer, const TiledTileset& tileSet);

    virtual void Render(RenderData* data) override;
};

};
