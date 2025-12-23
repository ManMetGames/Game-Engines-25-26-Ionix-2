#pragma once
#include <imgui.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <filesystem>

namespace IonixEngine
{
    class Fontloader
    {
    public:
        std::unordered_map<std::string, ImFont*> fontMap;
        bool fontsLoaded = false;

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

            if (fontsLoaded) return;
            fontsLoaded = true;

            // Clear existing fonts/atlas so we don't keep growing the atlas on re-init
            io.Fonts->Clear();
            fontMap.clear();


            // IMPORTANT: set atlas size BEFORE adding fonts (esp. for JP/CJK glyph ranges)
            io.Fonts->TexDesiredWidth = 8192; // was 4096; helps keep atlas height under GPU limits
            io.Fonts->TexGlyphPadding = 1;

            ImFontConfig cfg;
            cfg.OversampleH = 2;
            cfg.OversampleV = 1;
            cfg.PixelSnapH = true;

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

            auto AddFontFromFileIfExists = [&](
                const char* filename,
                float sizePixels,
                const ImFontConfig* fontCfg,
                const ImWchar* ranges,
                ImFont* fallback,
                const char* mapName
                )
                {
                    ImFont* font = fallback;
                    if (std::filesystem::exists(filename))
                    {
                        font = io.Fonts->AddFontFromFileTTF(filename, sizePixels, fontCfg, ranges);
                        if (!font)
                            font = fallback;
                    }
                    AddMap({ mapName, font });
                    return font;
                };

            AddFontFromFileIfExists(
                "Fonts/NotoSansJP-Regular.ttf", 23.0f, &cfg, jpRanges, imguiDefault, "ImGuiDefaultJP");

            AddFontFromFileIfExists(
                "Fonts/NotoSansJP-Bold.ttf", 23.0f, &cfg, jpRanges, imguiDefaultBold, "ImGuiDefaultBoldJP");

            ImFontConfig jpSubCfg = cfg;
            jpSubCfg.SizePixels = 29.0f;
            AddFontFromFileIfExists(
                "Fonts/NotoSansJP-Regular.ttf", 29.0f, &jpSubCfg, jpRanges, imguiSub, "ImGuiSubJP");

            ImFontConfig jpHeaderCfg = cfg;
            jpHeaderCfg.SizePixels = 33.0f;
            AddFontFromFileIfExists(
                "Fonts/NotoSansJP-Bold.ttf", 33.0f, &jpHeaderCfg, jpRanges, imguiHeader, "ImGuiHeaderJP");

            ImFontConfig jpTitleCfg = cfg;
            jpTitleCfg.SizePixels = 47.0f;
            AddFontFromFileIfExists(
                "Fonts/NotoSansJP-Bold.ttf", 47.0f, &jpTitleCfg, jpRanges, imguiTitle, "ImGuiTitleJP");


            AddFontFromFileIfExists(
                "Fonts/JF-Dot-MPlus12.ttf", 19.0f, &cfg, jpRanges, imguiDefault, "JFDotJP");

            AddFontFromFileIfExists(
                "Fonts/JF-Dot-MPlus12B.ttf", 19.0f, &cfg, jpRanges, imguiDefaultBold, "JFDotBoldJP");

            ImFontConfig jpSubCfg2 = cfg;
            jpSubCfg.SizePixels = 25.0f;
            AddFontFromFileIfExists(
                "Fonts/JF-Dot-MPlus12.ttf", 25.0f, &jpSubCfg2, jpRanges, imguiSub, "JFDotSubJP");

            ImFontConfig jpHeaderCfg2 = cfg;
            jpHeaderCfg.SizePixels = 29.0f;
            AddFontFromFileIfExists(
                "Fonts/JF-Dot-MPlus12B.ttf", 29.0f, &jpHeaderCfg2, jpRanges, imguiHeader, "JFDotHeaderJP");

            ImFontConfig jpTitleCfg2 = cfg;
            jpTitleCfg.SizePixels = 43.0f;
            AddFontFromFileIfExists(
                "Fonts/JF-Dot-MPlus12B.ttf", 43.0f, &jpTitleCfg2, jpRanges, imguiTitle, "JFDotTitleJP");


            // Build font atlas
            io.Fonts->Build();
        }
    };
}