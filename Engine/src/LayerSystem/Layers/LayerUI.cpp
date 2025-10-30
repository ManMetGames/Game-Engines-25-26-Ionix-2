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
		uiManager.setUIRenderer(m_UI);
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
         ImGui_ImplSDLRenderer2_NewFrame();

         ImGui_ImplSDL2_NewFrame();
         ImGui::NewFrame();

        
         std::vector<std::string> dropdownOptions = { "Option 1", "Option 2", "Option 3" };
         int dropdownIndex = 0;
         bool checkboxValue = true;
         int radioValue = 0;
         static float sliderValue = 0.5f;
         
         
         uiManager.BeginPanel("Test Panel");

         uiManager.AddLabel(10, 10, 100, 20, "Test Label");
         uiManager.AddButton(10, 40, 100, 25, "Click Me", []() { printf("Button clicked!\n"); });
         uiManager.AddCheckbox(10, 70, 120, 25, "Enable", &checkboxValue);
         uiManager.AddSliderFloat(10, 100, 150, 25, "Slider", &sliderValue, 0.0f, 1.0f);
         uiManager.AddRadioButton(10, 130, 100, 25, "Option 1", &radioValue, 0,true);
         uiManager.AddRadioButton(120, 130, 100, 25, "Option 2", &radioValue, 1, true);
         uiManager.AddDropdown(10, 160, 150, 25, "Dropdown", dropdownOptions, &dropdownIndex);
         uiManager.AddColorPicker(10, 190, 150, 150, "Pick Color", m_UI->myColor);
		 
		



         // Render the UI
         uiManager.RenderUI();
         uiManager.EndPanel();
        
		
         // Rendering
         

         // TODO - Will be done by graphics unit eventually. Here for testing for the time being.

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
