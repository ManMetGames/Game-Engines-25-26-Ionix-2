#pragma once
#include "SDL.h"

namespace IonixEngine
{
    struct MouseCooards
    {
        int x;
        int y;
        MouseCooards(int x, int y) : x(x), y(y) {};
    };
    class Input
    {
    public:
        MouseCooards GetMousePosition();
        
     };
}

