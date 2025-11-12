#include "Architecture/Application.h"
#include "Graphics/Camera.h"
#include "Scripting/Scripting.h"
#include "LayerSystem/LayerStack.h"
#include "LayerSystem/Layers/LayerEditor.h"
#include "LayerSystem/Layers/LayerInput.h"
#include "LayerSystem/Layers/LayerFysics.h"
#include "LayerSystem/Layers/LayerSound.h"
#include "LayerSystem/Layers/LayerUI.h"
#include "LayerSystem/Layers/LayerGraphics.h"
#include "Window/Window.h"
#include "imgui.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "EventSystem/Event.h"
#include <SDL.h>
#include <SDL_render.h>

// Define global camera pointer
IonixEngine::Camera* cam = new IonixEngine::Camera();

namespace IonixEngine {
    Application* Application::s_Instance = nullptr;
}

namespace IonixEngine
{
    Application& Application::Get() { return *s_Instance; }

    Application::Application()
        : m_Window(new Window())
    {
        s_Instance = this;
        startTick = SDL_GetPerformanceCounter();
        currentTick = SDL_GetPerformanceCounter();

        // Initialise layers
        layerEditor = new LayerEditor();
        AddLayer(layerEditor);

        layerInput = new LayerInput();
        AddLayer(layerInput);

        layerFysics = new LayerFysics();
        AddLayer(layerFysics);

        layerSound = new LayerSound();
        AddLayer(layerSound);

        layerUI = new LayerUI();
        AddLayer(layerUI);

        layerGraphics = new LayerGraphics();
        AddLayer(layerGraphics);

        Scripting::Get().Init();
        Scripting::Get().GetLuaState().script_file("Scripts/Settings.lua");
        Scripting::Get().GetLuaState().script_file("Scripts/TestCamera.lua"); // load test script
    }

    Application::~Application()
    {
        delete m_Window;
        m_Window = nullptr;
    }

    void Application::OnEvent(IonixEvent& e)
    {
        for (auto layer : m_LayerStack.GetLayers()) {
            layer->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        m_Running = true;
        Scripting::Get().CallHook("OnStart");

        SDL_Renderer* renderer = m_Window->GetSdlRenderer();

        while (m_Running) {
            uint64_t lastTick = currentTick;
            currentTick = SDL_GetPerformanceCounter();
            deltaTime = static_cast<double>(currentTick - lastTick) / SDL_GetPerformanceFrequency();
            time += deltaTime;

            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

            for (auto layer : m_LayerStack.GetLayers()) {
                if (layer)
                    layer->OnUpdate();
            }

            cam->handleInput(1.0f);

            // Lua hook updates (renders to texture)
            Scripting::Get().CallHook("OnUpdate");

            // Draw the camera’s texture back onto the window
            SDL_Texture* camTex = cam->GetRenderTexture();
            if (camTex) {
                SDL_Rect destRect = { 50, 50, 256, 256 }; // position + size on screen
                SDL_RenderCopy(renderer, camTex, NULL, &destRect);
            }

            ImGui::Render();
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_Window->GetSdlRenderer());
            SDL_RenderPresent(m_Window->m_Renderer);

            layerInput->m_Input->CopyCodesEndFrame();
            m_Window->OnUpdate();
        }

        std::vector<Layer*> layers = m_LayerStack.GetLayers();
        for (size_t i = layers.size() - 1; i > 0; i--) {
            layers[i]->OnDetach();
        }
    }
}