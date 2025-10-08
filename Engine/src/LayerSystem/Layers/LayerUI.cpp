#include "LayerSystem/Layers/LayerUI.h"
#include "EventSystem/Event.h"
#include "Architecture/Application.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <SDL.h>

#include <iostream>

namespace IonixEngine
{
    void LayerUI::OnAttach() 
    {

        //Get window and renderer
        SDL_Window* window = Application::Get().GetWindow().GetSdlWindow();
        SDL_Renderer* renderer = Application::Get().GetWindow().GetSdlRenderer();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer2_Init(renderer);

        std::cout << "ImGui Initialised " << std::endl;
    }

    void LayerUI::OnDetach() {}

    void LayerUI::OnUpdate() 
    {
        // Start the Dear ImGui frame. Immediate mode rendering - UI gets rebuilt each frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        //Shows the big ImGui demo window
        ImGui::ShowDemoWindow();

        // Rendering
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), Application::Get().GetWindow().m_Renderer);

        //TODO - Will be done by graphics unit eventually. Here for testing for the time being.
        SDL_RenderPresent(Application::Get().GetWindow().m_Renderer);
    }

    void LayerUI::OnEvent(IonixEvent& e)
    {
        // Switch statement routes the event and invokes the relevant event handler

        switch (e.Type)
        {
            // Add more cases as needed.... (Note: Most engine features don't require events, they
            //                              can just be callable functions.
        }
    }
}