#include "Input.h"
namespace IonixEngine
{
    MouseCooards IonixEngine::Input::GetMousePosition()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        MouseCooards mc(x, y);
        return mc;
    }
}
