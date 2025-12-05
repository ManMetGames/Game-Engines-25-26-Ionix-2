#pragma once
#include <imgui.h>
#include <string>
#include <unordered_map>
#include <filesystem>

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

        void AddMap(const std::string& name, ImFont* font)
        {
            fontMap[name] = font;
        }

        void LoadFonts()
        {
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontDefault();

            const std::string fontPath = "Assets/Fonts/";

            for (const auto& file : std::filesystem::directory_iterator(fontPath))
            {
                if (!file.is_regular_file())
                    continue;

                std::string path = file.path().string();
                std::string ext = file.path().extension().string();

                //THIS WILL ONLY LOAD .TTF OR .OTF
                if (ext != ".ttf" && ext != ".otf")
                    continue;

                std::string filename = file.path().filename().string();

                ImFont* loadedFont = io.Fonts->AddFontFromFileTTF(
                    path.c_str(),
                    18.0f,                                
                    NULL,
                    io.Fonts->GetGlyphRangesDefault()
                    // all of these are adjustible
                );

                if (loadedFont)
                {
                    //removing them and cleaning the extension
                    std::string cleanName = file.path().stem().string();
                    AddMap(cleanName, loadedFont);
                    printf("[FontLoader] Loaded font: %s\n", cleanName.c_str());
                }
                else
                {
                    printf("[FontLoader] Failed to load: %s\n", path.c_str());
                }
            }

            io.Fonts->Build();
        }
    };
}

