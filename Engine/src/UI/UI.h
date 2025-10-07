#pragma once

namespace IonixEngine
{
    class UI
    {
    public:
        void DrawLabel(char* text, int xsize, int ysize, int xpos, int ypos);
        bool DrawButton(char* text, int xsize, int ysize, int xpos, int ypos);
        float DrawSlider(char* text, static float i, int xsize, int ysize, int xpos, int ypos, int minval, int maxval);


    };
}

