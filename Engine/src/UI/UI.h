#pragma once
#include <unordered_map>
namespace IonixEngine
{
    class UI
    {
    public:
        void DrawLabel(char* text, int xpos, int ypos, int xsize, int ysize, const char* font);
    
        bool DrawButton(char* text, int xsize, int ysize, int xpos, int ypos);

        float AddSlider(char* label, float value, int xsize, int ysize, int xpos, int ypos, float minval, float maxval);

        void DrawCheckbox(int id, char* text, int xpos, int ypos, int xsize, int ysize);

        void DrawRadioButton(int xpos, int ypos, char* text, int e, int value, bool sameline);
        float DrawColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color);
        float myColor[4] = {};
        std::unordered_map<int, bool> checkboxMap;
        bool& getCheckboxState(int index) { return checkboxMap[index]; }
        void DrawProgressBar(int xpos, int ypos, float xsize, float ysize, float maxvalue, float currentvalue);
        float ProgressBar(int xpos, int ypos, float xsize, float ysize, float maxvalue, float& currentvalue, float incrementamount);


    };
}
