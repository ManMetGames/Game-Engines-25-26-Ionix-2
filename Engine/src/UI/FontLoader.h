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

        ImFont* GetFont(const std::string& fontName)
        {
            auto it = fontMap.find(fontName);
            return (it != fontMap.end()) ? it->second : nullptr;
        }


        void AddMap(std::pair<std::string, ImFont*> newFont)
        {
            fontMap.insert(newFont);
        }

        void LoadFonts()
        {
            ImGuiIO& io = ImGui::GetIO();
            // Load the default font
            ImFont* imguiDefault = io.Fonts->AddFontDefault();
            IM_ASSERT(imguiDefault != NULL);
            io.FontDefault = imguiDefault;
            AddMap({"ImGuiDefault", imguiDefault });
            


            


            ImFontConfig bold_cfg;
            bold_cfg.RasterizerMultiply = 1.65f; // tweak 1.10 - 1.40
            ImFont* imguiDefaultBold = io.Fonts->AddFontDefault(&bold_cfg);
            IM_ASSERT(imguiDefaultBold != NULL);
            AddMap({"ImGuiDefaultBold", imguiDefaultBold });
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
            // Set a larger texture size for font atlas to accommodate CJK characters
            io.Fonts->TexDesiredWidth = 4096;

            // Japanese UI fonts
            ImFont* font_jp = io.Fonts->AddFontFromFileTTF("NotoSansJP-Regular.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
            IM_ASSERT(font_jp != nullptr);
            AddMap({ "ImGuiDefaultJP", font_jp });
            ImFont* font_jp_bold = io.Fonts->AddFontFromFileTTF("NotoSansJP-Bold.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese()
            );
            IM_ASSERT(font_jp_bold != nullptr);
            AddMap({ "ImGuiDefaultBoldJP", font_jp_bold });


            // Build the font atlas (this can take some time)
            io.Fonts->Build();
        }
    };
}
