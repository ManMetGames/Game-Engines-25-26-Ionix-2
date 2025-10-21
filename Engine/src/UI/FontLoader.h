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


            
           

            // Load the font in
            ImFont* font_title = io.Fonts->AddFontFromFileTTF("TTT-Regular.otf", 23.0f, NULL, io.Fonts->GetGlyphRangesDefault());
            IM_ASSERT(font_title != NULL);

            AddMap({ "font1", font_title });
            
            ImFont* font_body = io.Fonts->AddFontFromFileTTF("TTT-Bold.otf", 18.0f, NULL, io.Fonts->GetGlyphRangesDefault());
            IM_ASSERT(font_body != NULL);


            // Build the font atlas (this can take some time)
            io.Fonts->Build();
        }
    };
}