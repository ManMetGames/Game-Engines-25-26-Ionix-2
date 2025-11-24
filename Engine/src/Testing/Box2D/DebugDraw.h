#pragma once
#include <Box2D.h>
#include <SDL.h>

class DebugDraw : public b2Draw
{
public:
    DebugDraw(SDL_Renderer* renderer): m_renderer(renderer)
    {}

    void SetRenderer(SDL_Renderer* renderer) { m_renderer = renderer; }

    SDL_Point ToScreen(const b2Vec2& vec) const {
        const float SCALE = 100.0f; // pixels per meter (adjust to your world scale)
        const int SCREEN_HEIGHT = 640; // or use your actual window height

        return {
            static_cast<int>(vec.x * SCALE),
            static_cast<int>(vec.y * SCALE)
        };
    }

    float Scale(float meters) const {
        
        return meters * m_scale;
    }

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2&, const b2Color& color) override;
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
    void DrawTransform(const b2Transform& xf) override {}
    void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

private:
    SDL_Renderer* m_renderer;
    float m_scale;
};
