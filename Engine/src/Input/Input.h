#pragma once
#include "SDL.h"

namespace IonixEngine
{
    struct MouseCoords 
    {
        int x;
        int y;

        MouseCoords(int x, int y) : x(x), y(y) {};
    };

    class Input
    {
    public:
        MouseCoords GetMousePosition() 
        {
            int x, y;
            SDL_GetMouseState(&x, &y);

            MouseCoords mc(x, y);
            return mc;
        }
    };
}

