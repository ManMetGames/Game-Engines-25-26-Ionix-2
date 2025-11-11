#include "Camera.h"
#include <iostream>
#include "Architecture/Application.h"

namespace IonixEngine
{
    Camera::Camera(float startX, float startY, float startZoom,
        int height, int width, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
        : x(startX), y(startY), zoom(startZoom), h(height), w(width),
        bg_r(r), bg_g(g), bg_b(b), bg_a(a)
    {
    }

    void Camera::ClearBackground(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, bg_a);
        SDL_RenderClear(renderer);
    }

    void Camera::handleInput(float deltaTime)
    {
        const float speed = 1.0f;

        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_W)) y = -speed * deltaTime;
        else if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_S)) y = speed * deltaTime;
        else if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_A)) x = -speed * deltaTime;
        else if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_D)) x = speed * deltaTime;
        else { x = 0; y = 0; }
        
        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_U)) {
            zoom *= 0.9f;
        }
        else if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_I))
        {
            zoom *= 1.1f;
        }
        if (zoom < 0.2f) zoom = 0.2f;
        if (zoom > 5.0f) zoom = 5.0f;

        auto& entities = Application::Get().layerScene->GetEntities();
        for (auto& entity : entities)
        {
            entity.position.x += x;
            entity.position.y += y;
        }
    }

    void Camera::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        bg_r = r;
        bg_g = g;
        bg_b = b;
        bg_a = a;
    }
}