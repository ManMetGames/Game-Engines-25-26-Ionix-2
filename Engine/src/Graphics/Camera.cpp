#include "Camera.h"
#include <iostream>
#include "Architecture/Application.h"

namespace IonixEngine
{
    Camera::Camera(float startX, float startY, float startZoom, int height, int width, bool isFocused)
		: x(startX), y(startY), zoom(startZoom), h(height), w(width), isFocused(isFocused)
    {
		EntityID id = Application::Get().layerScene->GetScene()->CreateEntity();
		Entity* camEntity = Application::Get().layerScene->GetScene()->GetEntityFromID(id);
		Application::Get().layerGraphics->AddCamera(*camEntity);
    }

    void Camera::handleInput(float deltaTime)
    {
        if (!isFocused) return;
        const float speed = 3.0f;

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

        if (Application::Get().layerInput->m_Input->IsKeyDown(SDL_SCANCODE_C)) {
            //Application::Get().layerScene->GetCameras()
        }

        if (zoom < 0.2f) zoom = 0.2f;
        if (zoom > 5.0f) zoom = 5.0f;

        std::vector<Entity>& entities = Application::Get().layerScene->GetEntities();

        for (auto it = entities.begin(); it != entities.end(); ++it) {
            it->position.y += y;
            it->position.x += x;
        }
    }
}