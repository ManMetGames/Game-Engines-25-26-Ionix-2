#pragma once
#include <unordered_map>
#include <string>
#include "imgui.h"

namespace IonixEngine
{
    class UI
    {
    public:
        void DrawLabel(char* text, int xpos, int ypos, int xsize, int ysize, std::string font);

        bool DrawButton(char* text, int xsize, int ysize, int xpos, int ypos);

        float DrawSlider(char* text, static float i, int xsize, int ysize, int xpos, int ypos, int minval, int maxval);

        void DrawCheckbox(int id, char* text, int xpos, int ypos, int xsize, int ysize);

        void DrawRadioButton(int xpos, int ypos, char* text, int& e, int value, bool sameline);
        float DrawColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color);
        float myColor[4] = {};
        std::unordered_map<int, bool> checkboxMap;
        bool& getCheckboxState(int index) { return checkboxMap[index]; }
        void DrawProgressBar(int xpos, int ypos, float xsize, float ysize, float maxvalue, float currentvalue, int colorId = 0);
        float ProgressBar(int xpos, int ypos, float xsize, float ysize, float maxvalue, float& currentvalue, float incrementamount);
        float DrawDropdown(int xPos, int yPos, float xSize, float ySize, const char* text, std::vector<std::string> options, int* currentIndex);
        bool InputText(const char* label, int xPos, int yPos, float width, char* buffer, size_t bufferSize, ImGuiInputTextFlags flags = 0);
    };
}
