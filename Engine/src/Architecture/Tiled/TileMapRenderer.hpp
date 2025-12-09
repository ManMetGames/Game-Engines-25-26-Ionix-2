#include "Architecture/ECS/Component.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Tiled/TiledMap.hpp"

namespace IonixEngine {

class TileMapRenderer : public Component {
private:
    SDL_Texture* image;
    std::vector<std::pair<SDL_Rect, SDL_Rect>> tiles;
    b2Vec2 tileSize;
public:
    TileMapRenderer(Entity* entity, const TiledTileLayer& tileLayer, const TiledTileset& tileSet);

    virtual void Render(RenderData* data) override;
};

};
