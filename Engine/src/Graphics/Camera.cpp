#include "Camera.h"
#include <iostream>
#include "Architecture/Application.h"
#include <SDL.h>

namespace IonixEngine
{
    Camera::Camera(float startX, float startY, float startZoom, int height, int width, bool isFocused, int renderLayer)
		: x(startX - 50), y(startY - 50), zoom(startZoom), h(height), w(width), isFocused(isFocused), renderLayer(renderLayer)
    {
    }

    void Camera::Init() 
    {
		InitRenderTexture(Application::Get().GetWindow().m_Renderer);
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
        Application::Get().currentCam = *cameras[nextIndex];

        MoveCamera(cameras[nextIndex]->camDeltaX - cameras[camIndex]->camDeltaX, cameras[nextIndex]->camDeltaY - cameras[camIndex]->camDeltaY, false);

        Application::Get().layerInput->m_Input->SetKeyReleased(SDL_SCANCODE_C); //this just stops the function from triggering on all cameras at the same time, returning the focus to camera 0
    }

    void Camera::InitRenderTexture(SDL_Renderer* renderer) {
        // Use camera width/height for texture size
        rtWidth = w;
        rtHeight = h;

        // Creates a texture for render taget
        renderTexture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,     // 32 bit texture formating
            SDL_TEXTUREACCESS_TARGET,     // allows for SDL rendering
            rtWidth,
            rtHeight
        );

        if (!renderTexture) {
            std::cerr << "Failed to create render texture: " << SDL_GetError() << std::endl; //error message
        }
    }

    void Camera::RenderToTexture(SDL_Renderer* renderer) {
        if (!isFocused) return;
        if (!renderTexture) return;

        SDL_SetRenderTarget(renderer, renderTexture);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        std::vector<Entity>& entities = Application::Get().layerScene->GetEntities();

		auto& layerGraphics = Application::Get().layerGraphics;
        for (auto it = entities.begin(); it != entities.end(); ++it)
        {
            if (it->layer != renderLayer)
            {
                for (auto it2 = it->components.begin(); it2 != it->components.end(); ++it2)
                {
                    Component* comp = *it2;
                    if (comp->CanRender())
                    {
                        comp->SetCanRender(false);
                    }
                }
            }
            else if (it->layer == renderLayer)
            {
                for (auto it2 = it->components.begin(); it2 != it->components.end(); ++it2)
                {
                    Component* comp = *it2;
                    if (!comp->CanRender())
                    {
                        comp->SetCanRender(true);
                    }
                }
            }
        }
        // Flush the graphics queue into this texture
        Application::Get().layerGraphics->GetQueue()->RenderFromQueue();

        SDL_SetRenderTarget(renderer, NULL);
    }

    SDL_Texture* Camera::GetRenderTexture() const {
        return renderTexture;
    }


    void Camera::RenderToScreen(SDL_Renderer* renderer, float posX, float posY, float sizeX, float sizeY) 
    {
        RenderToTexture(renderer);
        SDL_Texture* camTex = GetRenderTexture();
        if (camTex) {
            SDL_Rect destRect = { posX, posY, sizeX, sizeY }; // position + size on screen
            SDL_RenderCopy(renderer, camTex, NULL, &destRect);
        }
    }

}