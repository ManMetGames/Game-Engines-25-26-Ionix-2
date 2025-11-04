#include "Camera.h"
#include <iostream>
#include "Architecture/Application.h"

namespace IonixEngine
{
    Camera::Camera(float startX, float startY, float startZoom, int height, int width)
        : x(startX), y(startY), zoom(startZoom), h(height), w(width)
	{ }

    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    void Camera::handleInput(float deltaTime)
    {
        const float speed = 100.0f;

        if (keystates[SDL_SCANCODE_W]) y += speed * deltaTime;
        if (keystates[SDL_SCANCODE_S]) y -= speed * deltaTime;
        if (keystates[SDL_SCANCODE_A]) x += speed * deltaTime;
        if (keystates[SDL_SCANCODE_D]) x -= speed * deltaTime;
        handleEvent();
    }

    void Camera::handleEvent()
    {
        if (keystates[SDL_SCANCODE_U]) {
            zoom *= 0.9f;
        }
        else if (keystates[SDL_SCANCODE_I])
        {
            zoom *= 1.1f;
        }
        if (zoom < 0.2f) zoom = 0.2f;
        if (zoom > 5.0f) zoom = 5.0f;
    }

    void Camera::renderTexture(int renderX, int renderY, SDL_Rect* clip, SDL_Renderer* renderer)
    {
        // Create a rectangle for the texture
        SDL_Rect renderQuad = { 
            static_cast<int>(renderX - x), 
            static_cast<int>(renderY - y), 
            64, 
            64 
        }; // 64x64 is the size of the object

        float actualClientWidth = Application::Get().GetWindow().m_Data.Width;
        std::vector<Entity>& entities = Application::Get().layerScene->GetEntities();

		std::cout << "Camera Position: (" << x << ", " << y << "), Zoom: " << zoom << std::endl;

        // Render to the screen
        SDL_RenderFillRect(renderer, &renderQuad);
    }

    SDL_Rect Camera::apply(const SDL_Rect& worldRect) const
    {
        SDL_Rect screenRect;
        screenRect.x = static_cast<int>((worldRect.x - x) * zoom);
        screenRect.y = static_cast<int>((worldRect.y - y) * zoom);
        screenRect.w = static_cast<int>(worldRect.w * zoom);
        screenRect.h = static_cast<int>(worldRect.h * zoom);
        std::cout << zoom << std::endl;
        return screenRect;
	}
}