#pragma once
#include <box2d.h>
#include <SDL.h>

class DebugDraw : public b2Draw
{
public:
    DebugDraw(SDL_Renderer* renderer, float scale = 30.0f)
        : m_renderer(renderer), m_scale(scale)
    {}

    void SetRenderer(SDL_Renderer* renderer) { m_renderer = renderer; }

    SDL_Point ToScreen(const b2Vec2& vec) const {
        float scale = 100.0f;
        return {
            static_cast<int>(vec.x * scale),
            static_cast<int>(vec.y * scale)
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
