#pragma once
#include <imgui.h>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <iostream>

namespace IonixEngine
{
    class Fontloader
    {
    public:
        std::unordered_map<std::string, ImFont*> fontMap;

        ImFont* GetFont(const std::string& name)
        {
            auto it = fontMap.find(name);
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

            const std::string rootFolder = "Assets";   

            if (!std::filesystem::exists(rootFolder))
            {
                std::cout << "[FontLoader] ERROR: Assets folder does not exist.\n";
                return;
            }

            
            for (auto& file : std::filesystem::recursive_directory_iterator(rootFolder))
            {
                if (!file.is_regular_file())
                    continue;

                std::string ext = file.path().extension().string();
                for (auto& c : ext) c = tolower(c);

				// ONLY USE THESE FONT FORMATS!!!!
                if (ext != ".ttf" && ext != ".otf" && ext != ".woff" && ext != ".woff2")
                    continue;

                std::string fullpath = file.path().string();
                std::string cleanName = file.path().stem().string();

                ImFont* f = io.Fonts->AddFontFromFileTTF(
                    fullpath.c_str(),
                    20.0f,                   
                    nullptr,
                    io.Fonts->GetGlyphRangesDefault()
                    // all of these are adjustible
                );

                if (f)
                {
                    AddMap(cleanName, f);
                    std::cout << "[FontLoader] Loaded: " << fullpath << "\n";
                }
                else
                {
                    std::cout << "[FontLoader] FAILED: " << fullpath << "\n";
                }
            }

            io.Fonts->Build();
        }
    };
}
