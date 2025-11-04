#include "Camera.h"
#include <iostream>

namespace IonixEngine
{
	Camera::Camera(float startX, float startY, float startZoom)
		: x(startX), y(startY), zoom(startZoom)
	{ }

    void Camera::handleInput(float deltaTime)
    {
        const float speed = 100.0f;
        const Uint8* keystates = SDL_GetKeyboardState(NULL);

        if (keystates[SDL_SCANCODE_W]) y += speed * deltaTime;
        if (keystates[SDL_SCANCODE_S]) y -= speed * deltaTime;
        if (keystates[SDL_SCANCODE_A]) x += speed * deltaTime;
        if (keystates[SDL_SCANCODE_D]) x -= speed * deltaTime;
        handleEvent(keystates);
    }

    void Camera::handleEvent(const Uint8* keystates)
    {
        // Mouse wheel zoom
        if (keystates[SDL_SCANCODE_Y])
        {   
            zoom *= 1.1f;
        }
        else if (keystates[SDL_SCANCODE_U]) {
            zoom *= 0.9f;
        }
        if (zoom < 0.2f) zoom = 0.2f;
        if (zoom > 5.0f) zoom = 5.0f;
    }

    SDL_Rect Camera::apply(const SDL_Rect& worldRect) const
    {
        SDL_Rect screenRect;
        screenRect.x = static_cast<int>((worldRect.x - x) * zoom);
        screenRect.y = static_cast<int>((worldRect.y - y) * zoom);
        screenRect.w = static_cast<int>(worldRect.w * zoom);
        screenRect.h = static_cast<int>(worldRect.h * zoom);
        std::cout << screenRect.x << std::endl;
        return screenRect;
	}
}