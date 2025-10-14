#pragma once
#include <imgui.h>
#include <string>
#include <functional>
#include <vector>

namespace IonixEngine
{
    class Fontloader
    {
    public:
        static void LoadFonts()
        {
            ImGuiIO& io = ImGui::GetIO();
            // Load the default font
            io.Fonts->AddFontDefault();

            // Load custom fonts
            io.Fonts->AddFontFromFileTTF("OpenSans-VariableFont.ttf", 16.0f);
            //io.Fonts->AddFontFromFileTTF("Game-Engines-25-26-Ionix-2//Assets//Open_Sans//OpenSans-VariableFont.ttf", 16.0f);
            //io.Fonts->AddFontFromFileTTF("path/to/another/font.ttf", 18.0f);

            // Build the font atlas (this can take some time)
            io.Fonts->Build();
        }
    };
}