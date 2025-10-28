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

        ImFont* GetFont(std::string fontName)
        {
            return fontMap[fontName];
        }

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
            AddMap({"Font1", font_title });
            ImFont* font_body = io.Fonts->AddFontFromFileTTF("TTT-Bold.otf", 18.0f, NULL, io.Fonts->GetGlyphRangesDefault());
            IM_ASSERT(font_body != NULL);
            AddMap({ "Font1Bold",font_body });

            ImFont* font_title2 = io.Fonts->AddFontFromFileTTF("CenturyGothic.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesDefault());
            IM_ASSERT(font_title2 != NULL);
            AddMap({ "Font2",font_title2 });
            ImFont* font_title2bold = io.Fonts->AddFontFromFileTTF("CenturyGothicBold.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesDefault());
            IM_ASSERT(font_title2bold != NULL);
            AddMap({ "Font2Bold",font_title2bold });
            ImFont* font_title2italic = io.Fonts->AddFontFromFileTTF("CenturyGothicItalic.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesDefault());
            IM_ASSERT(font_title2italic != NULL);
            AddMap({ "Font2Italic",font_title2italic });



            // Build the font atlas (this can take some time)
            io.Fonts->Build();
        }
    };
}