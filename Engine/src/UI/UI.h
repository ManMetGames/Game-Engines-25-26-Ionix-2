#pragma once
#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "imgui.h"

namespace IonixEngine
{
    class UI
    {
    public:
        void DrawLabel(const char* text, int xpos, int ypos, int xsize, int ysize);
        bool DrawButton(char* text, int xsize, int ysize, int xpos, int ypos);
        bool DrawSlider(const char* label, float* value, float width,
            int xpos, int ypos, float minval, float maxval);
        bool DrawCheckbox(const char* label, bool* value, int xpos, int ypos);

        bool DrawRadioButton(int xpos, int ypos, const char* text, int* v, int value);
        bool DrawColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color4);
        void DrawProgressBar(int xpos, int ypos, float xsize, float ysize, float maxvalue, float currentvalue, int colorId = 0);
        float ProgressBar(int xpos, int ypos, float xsize, float ysize, float maxvalue, float& currentvalue, float incrementamount);
        bool DrawDropdown(int xPos, int yPos, float xSize, float ySize, const char* text,
            const std::vector<std::string>& options, int* currentIndex);


        bool InputText(const char* label, int xPos, int yPos, float width, char* buffer, size_t bufferSize, ImGuiInputTextFlags flags = 0);

        void DrawPanel(int x, int y, float w, float h,
            float alpha = 0.45f, float rounding = 6.0f,
            int r = 0, int g = 0, int b = 0);

        float CalcTextWidth(const char* text);
    };
}
