#include "DebugDraw.h"

static void SetSDLColor(SDL_Renderer* r, const b2Color& c)
{
    SDL_SetRenderDrawColor(r,(Uint8)(c.r * 255),(Uint8)(c.g * 255),(Uint8)(c.b * 255),255);
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    SetSDLColor(m_renderer, color);

    for (int i = 0; i < vertexCount; ++i)
    {
        SDL_Point p1 = ToScreen(vertices[i]);
        SDL_Point p2 = ToScreen(vertices[(i + 1) % vertexCount]);
        SDL_RenderDrawLine(m_renderer, p1.x, p1.y, p2.x, p2.y);
    }
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    DrawPolygon(vertices, vertexCount, color);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    SetSDLColor(m_renderer, color);

    SDL_Point c = ToScreen(center);
    int r = (int)Scale(radius);

    int x = r;
    int y = 0;
    int decision = 1 - x;

    while (y <= x) {
        SDL_RenderDrawPoint(m_renderer, c.x + x, c.y + y);
        SDL_RenderDrawPoint(m_renderer, c.x + y, c.y + x);
        SDL_RenderDrawPoint(m_renderer, c.x - y, c.y + x);
        SDL_RenderDrawPoint(m_renderer, c.x - x, c.y + y);
        SDL_RenderDrawPoint(m_renderer, c.x - x, c.y - y);
        SDL_RenderDrawPoint(m_renderer, c.x - y, c.y - x);
        SDL_RenderDrawPoint(m_renderer, c.x + y, c.y - x);
        SDL_RenderDrawPoint(m_renderer, c.x + x, c.y - y);

        y++;
        if (decision <= 0) {
            decision += 2 * y + 1;
        } else {
            x--;
            decision += 2 * (y - x) + 1;
        }
    }
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2&, const b2Color& color)
{
    DrawCircle(center, radius, color);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    SetSDLColor(m_renderer, color);

    SDL_Point a = ToScreen(p1);
    SDL_Point b = ToScreen(p2);

    SDL_RenderDrawLine(m_renderer, a.x, a.y, b.x, b.y);
}

void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
    SetSDLColor(m_renderer, color);

    SDL_Point pt = ToScreen(p);
    int r = (int)(size / 2);

    SDL_Rect rect{ pt.x - r, pt.y - r, r * 2, r * 2 };
    SDL_RenderFillRect(m_renderer, &rect);
}
