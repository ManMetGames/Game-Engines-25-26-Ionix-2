#pragma once
#include <imgui.h>
#include <string>
#include <unordered_map>
#include <utility>

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
            fontMap.insert(std::move(newFont));
        }

        void LoadFonts()
        {
            ImGuiIO& io = ImGui::GetIO();

            // IMPORTANT: set atlas size BEFORE adding fonts (esp. for JP/CJK glyph ranges)
            io.Fonts->TexDesiredWidth = 4096;

            ImFontConfig cfg;
            cfg.OversampleH = 3;
            cfg.OversampleV = 2;
            cfg.PixelSnapH  = true;

            // -------------------------
            // ImGui default family (baked at multiple sizes to avoid blurry scaling)
            // -------------------------
            ImFontConfig def18 = cfg;
            def18.SizePixels = 18.0f;
            ImFont* imguiDefault = io.Fonts->AddFontDefault(&def18);
            IM_ASSERT(imguiDefault != nullptr);
            io.FontDefault = imguiDefault;
            AddMap({ "ImGuiDefault", imguiDefault });

            // "Bold" variant for default font (fake bold via rasterizer multiply)
            ImFontConfig bold18 = cfg;
            bold18.SizePixels = 18.0f;
            bold18.RasterizerMultiply = 1.65f; // tweak if needed
            ImFont* imguiDefaultBold = io.Fonts->AddFontDefault(&bold18);
            IM_ASSERT(imguiDefaultBold != nullptr);
            AddMap({ "ImGuiDefaultBold", imguiDefaultBold });

            // Sub / header / title sizes (use these instead of scaling)
            ImFontConfig sub24 = cfg;
            sub24.SizePixels = 24.0f;
            ImFont* imguiSub = io.Fonts->AddFontDefault(&sub24);
            IM_ASSERT(imguiSub != nullptr);
            AddMap({ "ImGuiSub", imguiSub });

            ImFontConfig header28 = cfg;
            header28.SizePixels = 28.0f;
            header28.RasterizerMultiply = 1.65f; // keep header weight similar to Bold
            ImFont* imguiHeader = io.Fonts->AddFontDefault(&header28);
            IM_ASSERT(imguiHeader != nullptr);
            AddMap({ "ImGuiHeader", imguiHeader });

            ImFontConfig title42 = cfg;
            title42.SizePixels = 42.0f;
            title42.RasterizerMultiply = 1.65f;
            ImFont* imguiTitle = io.Fonts->AddFontDefault(&title42);
            IM_ASSERT(imguiTitle != nullptr);
            AddMap({ "ImGuiTitle", imguiTitle });

            // -------------------------
            // Japanese UI fonts (Noto Sans JP) - also baked at multiple sizes
            // -------------------------
            const ImWchar* jpRanges = io.Fonts->GetGlyphRangesJapanese();

            ImFont* jp18 = io.Fonts->AddFontFromFileTTF(
                "NotoSansJP-Regular.ttf", 18.0f, &cfg, jpRanges);
            IM_ASSERT(jp18 != nullptr);
            AddMap({ "ImGuiDefaultJP", jp18 });

            ImFont* jp18b = io.Fonts->AddFontFromFileTTF(
                "NotoSansJP-Bold.ttf", 18.0f, &cfg, jpRanges);
            IM_ASSERT(jp18b != nullptr);
            AddMap({ "ImGuiDefaultBoldJP", jp18b });

            ImFontConfig jpSubCfg = cfg;
            jpSubCfg.SizePixels = 24.0f;
            ImFont* jpSub = io.Fonts->AddFontFromFileTTF(
                "NotoSansJP-Regular.ttf", 24.0f, &jpSubCfg, jpRanges);
            IM_ASSERT(jpSub != nullptr);
            AddMap({ "ImGuiSubJP", jpSub });

            ImFontConfig jpHeaderCfg = cfg;
            jpHeaderCfg.SizePixels = 28.0f;
            ImFont* jpHeader = io.Fonts->AddFontFromFileTTF(
                "NotoSansJP-Bold.ttf", 28.0f, &jpHeaderCfg, jpRanges);
            IM_ASSERT(jpHeader != nullptr);
            AddMap({ "ImGuiHeaderJP", jpHeader });

            ImFontConfig jpTitleCfg = cfg;
            jpTitleCfg.SizePixels = 42.0f;
            ImFont* jpTitle = io.Fonts->AddFontFromFileTTF(
                "NotoSansJP-Bold.ttf", 42.0f, &jpTitleCfg, jpRanges);
            IM_ASSERT(jpTitle != nullptr);
            AddMap({ "ImGuiTitleJP", jpTitle });

            // Build font atlas
            io.Fonts->Build();
        }
    };
}
