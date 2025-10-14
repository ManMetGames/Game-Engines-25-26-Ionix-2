#pragma once
#include <unordered_set>
#include <SDL.H>

namespace IonixEngine
{
    class Input
    {
    public:
        void SetKeyPressed(SDL_Scancode code)
        {
            currentKeys.insert(code);
        }
        void SetKeyReleased(SDL_Scancode code)
        {
            currentKeys.erase(code);
        }
        void CopyCodesEndFrame()
        {
            previousKeys = currentKeys;
        }
    private:
        std::unordered_set<SDL_Scancode> currentKeys;
        std::unordered_set<SDL_Scancode> previousKeys;
    };
}

