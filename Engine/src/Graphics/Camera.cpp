#include "Camera.h"
#include <iostream>
#include "Architecture/Application.h"
#include <SDL.h>

#include "Fysics/FysicsBody.h"

namespace IonixEngine
{
    Camera::Camera(float startX, float startY, float startZoom,
        int height, int width, bool isFocused, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
        : x(startX), y(startY), zoom(startZoom), h(height), w(width),
        bg_r(r), bg_g(g), bg_b(b), bg_a(a), isFocused(isFocused)
    {
    }

    void Camera::SetZoom(SDL_Renderer* renderer)
    {
        SDL_RenderSetScale(renderer, zoom, zoom);
    }

    void Camera::ClearBackground(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, bg_a);
        SDL_RenderClear(renderer);
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

    void Camera::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        SetBackgroundColor(r, g, b, a);
    }

    void Camera::handleInput(float deltaTime)
    {
        if (!isFocused) return;
        const float speed = 3.0f;
        
        if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_O)) 
        {
            zoom *= 0.9f;
            SetZoom(Application::Get().GetWindow().m_Renderer);
        }
        else if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_I)) 
        {
            zoom *= 1.1f;
            SetZoom(Application::Get().GetWindow().m_Renderer);
        }

        if (Application::Get().layerInput->m_Input->IsKeyDown(SDL_SCANCODE_C)) 
        {
            SwitchCamera();
        }

        if (zoom < 0.2f) zoom = 0.2f;
        if (zoom > 5.0f) zoom = 5.0f;
    }
  
  void Camera::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        bg_r = r;
        bg_g = g;
        bg_b = b;
        bg_a = a;
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
        if (!renderTexture) return;

        SDL_SetRenderTarget(renderer, renderTexture);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

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
            SDL_FRect destRect = { posX, posY, sizeX, sizeY }; // position + size on screen
            SDL_RenderCopyF(renderer, camTex, NULL, &destRect);
        }
    }
    
    void Camera::Rotate(float angle)
    {
        auto& entities = Application::Get().layerScene->GetEntities();

        for (auto& e : entities)
        {
            FysicsBody* fb = nullptr;
            SpriteComponent* spr = nullptr;
            
            float angleInRads = angle * (3.14159265f / 180.0f);
            
            if (!e.TryGetComponent<FysicsBody>(&fb)) {
                e.AddComponent<FysicsBody>(new FysicsBody(&e, 2, false));
            }
            
            if (e.TryGetComponent<SpriteComponent>(&spr) && e.TryGetComponent<FysicsBody>(&fb)) {
                spr->setAngle(spr->getAngle() + angleInRads);
                if (spr->getAngle() >= 360.0f) spr->setAngle(spr->getAngle() - 360); //these 2 lines normalize the rotation
                if (spr->getAngle() < 0.0f)   spr->setAngle(spr->getAngle() + 360);
                fb->SetAngle(&e, fb->GetAngle(&e) + angleInRads);
                if (fb->GetAngle(&e) >= 360.0f) fb->SetAngle(&e, fb->GetAngle(&e) - 360); //these 2 lines normalize the rotation 
                if (fb->GetAngle(&e) < 0.0f)   fb->SetAngle(&e, fb->GetAngle(&e) + 360);
            }
        }
    }

}
