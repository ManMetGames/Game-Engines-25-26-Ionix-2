#pragma once
#include <unordered_map>
namespace IonixEngine
{
    class UI
    {
    public:
        void DrawLabel(char* text, int xpos, int ypos, int xsize, int ysize, const char* font);
        bool DrawButton(char* text, int xsize, int ysize, int xpos, int ypos);

        float DrawSlider(char* text, static float i, int xsize, int ysize, int xpos, int ypos, int minval, int maxval);
        void DrawCheckbox(int id, char* text, int xpos, int ypos, int xsize, int ysize);

        void DrawRadioButton(int xpos, int ypos, char* text, int e, int value, bool sameline);

        std::unordered_map<int, bool> checkboxMap;
        bool& getCheckboxState(int index) { return checkboxMap[index]; }

        

    };
}

