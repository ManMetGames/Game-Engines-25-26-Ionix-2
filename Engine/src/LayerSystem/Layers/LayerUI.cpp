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
    
    /*enum UIType
    {
        Label,
        Button
    };

    struct UIData
    {
        UIType type;

        char* text;
        int x;
        int y;

        UIData(UIType type, char* text, int x, int y) : type(type), text(text), x(x), y(y) {}
    };

   

    std::vector<UIData> uiDrawData;
    */
    // Factory class needs a method to add a UIData object to the above vector

    void LayerUI::OnAttach() 
    {
        m_UI = new UI();
        //Get window and renderer
        SDL_Window* window = Application::Get().GetWindow().GetSdlWindow();
        SDL_Renderer* renderer = Application::Get().GetWindow().GetSdlRenderer();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        /*
        for (auto ui : uiDrawData)
        {
            if (ui.type == UIType::Label)
            {
                Application::Get().layerUI->m_UI->DrawLabel(ui.text, ui.x, ui.y);
            }
        }
        */

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
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        //Shows the big ImGui demo window
        ImGui::ShowDemoWindow();
        ImGui::Begin("null", nullptr, window_flags);

        // access the vector of struct details (UI)
        // loop 
        ImGui::SetCursorPos(ImVec2(100, 100));
        if (ImGui::Button("Button 1", ImVec2(80, 80)))
        {
            std::cout << "Button pressed\n";
        }
        static int slider_i =5;
        ImGui::SliderInt("SliderInt (0 -> 100)", &slider_i, 0, 100, "%d" );
        ImGui::End();
        // Rendering
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), Application::Get().GetWindow().m_Renderer); //ask if we can leave this here

        //TODO - Will be done by graphics unit eventually. Here for testing for the time being.
        SDL_RenderPresent(Application::Get().GetWindow().m_Renderer);
        SDL_RenderClear(Application::Get().GetWindow().m_Renderer);
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