#pragma once
#include <imgui.h>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

namespace IonixEngine
{
    class Fontloader
    {
    public:
        std::unordered_map<std::string, ImFont*> fontMap;

        void AddMap(std::pair<std::string, ImFont*> newFont)
        {
            fontMap.insert(newFont);
        }

        void LoadFonts()
        {
            ImGuiIO& io = ImGui::GetIO();
            // Load the default font
            io.Fonts->AddFontDefault();

<<<<<<< Updated upstream:Engine/src/UI/FontLoader.h
            // Load custom fonts
            io.Fonts->AddFontFromFileTTF("OpenSans-VariableFont.ttf", 16.0f);
            //io.Fonts->AddFontFromFileTTF("Game-Engines-25-26-Ionix-2//Assets//Open_Sans//OpenSans-VariableFont.ttf", 16.0f);
            //io.Fonts->AddFontFromFileTTF("path/to/another/font.ttf", 18.0f);
=======
            // Load the font in
            ImFont* font_title = io.Fonts->AddFontFromFileTTF("TTT-Regular.otf", 23.0f, NULL, io.Fonts->GetGlyphRangesDefault());
            IM_ASSERT(font_title != NULL);

            AddMap({ "font1", font_title });
            
            ImFont* font_body = io.Fonts->AddFontFromFileTTF("TTT-Bold.otf", 18.0f, NULL, io.Fonts->GetGlyphRangesDefault());
            IM_ASSERT(font_body != NULL);

>>>>>>> Stashed changes:Engine/src/UI/Fontloader.h

            // Build the font atlas (this can take some time)
            io.Fonts->Build();
        }
    };
}