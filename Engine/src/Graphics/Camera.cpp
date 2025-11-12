#include "Camera.h"
#include <iostream>
#include "Architecture/Application.h"

namespace IonixEngine
{
    Camera::Camera(float startX, float startY, float startZoom, int height, int width, bool isFocused)
		: x(startX), y(startY), xOffset(startX), yOffset(startY), zoom(startZoom), h(height), w(width), isFocused(isFocused)
    {
    }

    void Camera::Init() 
    {
        Application::Get().layerGraphics->m_Cameras.push_back(this);
        std::vector<Camera*> cams = Application::Get().layerGraphics->m_Cameras;
        for (auto it = cams.begin(); it != cams.end(); ++it) 
        {
            if(*it == this)
            {
				camIndex = cams.size() - 1;
            }
        }
    }

    void Camera::handleInput(float deltaTime)
    {
        if (!isFocused) return;
        const float speed = 3.0f;

        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_W)) y = -speed * deltaTime;
        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_S)) y = speed * deltaTime;
        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_A)) x = -speed * deltaTime;
        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_D)) x = speed * deltaTime;
		if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_W) == false &&
			Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_S) == false) y = 0.0f;
		if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_A) == false &&
			Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_D) == false) x = 0.0f;

        
        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_U)) {
            zoom *= 0.9f;
        }
        else if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_I)) {
            zoom *= 1.1f;
        }

        if (Application::Get().layerInput->m_Input->IsKeyDown(SDL_SCANCODE_C)) 
        {
            isFocused = false;
            if (camIndex + 1 >= Application::Get().layerGraphics->m_Cameras.size()) { //this if else statement just handles if the camera is trying to access an index that is outside the range of the vector
                Application::Get().layerGraphics->m_Cameras[0]->isFocused = true;
				MoveCamera(-xOffset, -yOffset); //moving camera back to the position of camera 0
            }
            else {
                Application::Get().layerGraphics->m_Cameras[camIndex + 1]->isFocused = true;
                MoveCamera(Application::Get().layerGraphics->m_Cameras[camIndex + 1]->xOffset, Application::Get().layerGraphics->m_Cameras[camIndex + 1]->yOffset);
            }
            Application::Get().layerInput->m_Input->SetKeyReleased(SDL_SCANCODE_C); //this just stops the function from triggering on all cameras at the same time, returning the focus to camera 0
        }

        if (zoom < 0.2f) zoom = 0.2f;
        if (zoom > 5.0f) zoom = 5.0f;

		MoveCamera(x, y);
    }

	void Camera::MoveCamera(float deltaX, float deltaY)
	{
        std::vector<Entity>& entities = Application::Get().layerScene->GetEntities();

        for (auto it = entities.begin(); it != entities.end(); ++it) {
            it->position.y += deltaX;
            it->position.x += deltaY;
        }
	}
}