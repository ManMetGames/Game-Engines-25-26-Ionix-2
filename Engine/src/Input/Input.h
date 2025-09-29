#pragma once
#include "SDL.h"

namespace IonixEngine
{
    struct MouseCoords
    {
        int x;
        int y;

        MouseCoords(int x, int y) : x(x), y(y) {} ;
    };

    class Input
    {

    public:
        MouseCoords GetMousePosition();       
    };
}

