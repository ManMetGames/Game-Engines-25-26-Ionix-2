#include "Camera.h"
#include <iostream>
#include "Architecture/Application.h"

namespace IonixEngine
{
    Camera::Camera(float startX, float startY, float startZoom, int height, int width, bool isFocused)
		: x(startX-50), y(startY-50), zoom(startZoom), h(height), w(width), isFocused(isFocused)
    {
    }

    void Camera::Init() 
    {
        MoveCamera(x, y);
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

        /*float moveX = 0.0f;
        float moveY = 0.0f;

        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_W)) moveY = -speed;
        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_S)) moveY = speed;
        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_A)) moveX = -speed;
        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_D)) moveX = speed;
		if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_W) == false &&
			Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_S) == false) y = 0.0f;
		if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_A) == false &&
			Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_D) == false) x = 0.0f;*/

        
        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_U)) 
        {
            zoom *= 0.9f;
        }
        else if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_I)) 
        {
            zoom *= 1.1f;
        }

        if (Application::Get().layerInput->m_Input->IsKeyDown(SDL_SCANCODE_C)) 
        {
            SwitchCamera();
        }

        if (zoom < 0.2f) zoom = 0.2f;
        if (zoom > 5.0f) zoom = 5.0f;

        /*if (moveX != 0.0f || moveY != 0.0f)
        {
            MoveCamera(moveX, moveY);
        }*/
    }

	void Camera::MoveCamera(float deltaX, float deltaY, bool moveCamDelta)
	{
        if (moveCamDelta) {
            camDeltaX += deltaX;
			camDeltaY += deltaY;
        }

        std::vector<Entity>& entities = Application::Get().layerScene->GetEntities();

        for (auto it = entities.begin(); it != entities.end(); ++it) {
            it->position.x += deltaX;
            it->position.y += deltaY;
        }
	}
    
    void Camera::SwitchCamera() 
    {
        isFocused = false;
        auto& cameras = Application::Get().layerGraphics->m_Cameras;
        int nextIndex = (camIndex + 1) % cameras.size();
        cameras[nextIndex]->isFocused = true;

        MoveCamera(cameras[nextIndex]->camDeltaX - cameras[camIndex]->camDeltaX, cameras[nextIndex]->camDeltaY - cameras[camIndex]->camDeltaY, false);

        Application::Get().layerInput->m_Input->SetKeyReleased(SDL_SCANCODE_C); //this just stops the function from triggering on all cameras at the same time, returning the focus to camera 0
    }

}