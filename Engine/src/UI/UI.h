#pragma once

namespace IonixEngine
{
    class UI
    {
    public:
        void DrawLabel(char* text, int xsize, int ysize, int xpos, int ypos);
        bool DrawButton(char* text, int xsize, int ysize, int xpos, int ypos);
    };
}

