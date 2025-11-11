#include "Camera.h"
#include <iostream>
#include "Architecture/Application.h"

namespace IonixEngine
{
    Camera::Camera(float startX, float startY, float startZoom, int height, int width)
        : x(startX), y(startY), zoom(startZoom), h(height), w(width)
    {
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

        std::vector<Entity>& entities = Application::Get().layerScene->GetEntities();

        for (auto it = entities.begin(); it != entities.end(); ++it) {
            it->position.y += y;
            it->position.x += x;
        }

        class Camera {
        public:
            uint32_t cullingMask;

            Camera() : cullingMask(0xFFFFFFFF) {}// Default to render all layers

            void SetCullingMask(uint32_t mask) {
                cullingMask = mask;
            }

            void AddLayerToMask(Layer layer) {
                cullingMask |= layer;
            }

            void RemoveLayerFromMask(Layer layer) {
                cullingMask &= ~layer;
            }

            bool IsLayerVisible(Layer layer) const {
                return (cullingMask & layer) != 0;
            }
        };
    }

