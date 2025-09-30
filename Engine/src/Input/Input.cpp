#include "Input.h"

namespace IonixEngine
{
    MouseCoords IonixEngine::Input::GetMousePosition()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        MouseCoords mc(x, y);
        return mc;
    }
}
