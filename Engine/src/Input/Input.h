#pragma once
#include <unordered_set>
#include <SDL.h>

namespace IonixEngine
{
    class Input
    {
    public:
        bool IsKeyDown(SDL_Scancode code) const
        {
            // Not held down previous frame

            // Held down current frame
            return !previousKeys.count(code) && currentKeys.count(code);
        }

        bool IsKeyUp(SDL_Scancode code) const
        {
            // Was held down previous frame

            // No longer held down on current frame
            return previousKeys.count(code) && !currentKeys.count(code);
        }
        bool IsKeyHeld(SDL_Scancode code) const
        {
            return currentKeys.count(code);
        }

        // for Key Down
        void SetKeyPressed(SDL_Scancode code)
        {
            currentKeys.insert(code);
        }
        // for Key Up
        void SetKeyReleased(SDL_Scancode code)
        {
            currentKeys.erase(code);
        }
        // for Previous Key
        void CopyCodesEndFrame()
        {
            previousKeys = currentKeys;
        }
    private:
        std::unordered_set<SDL_Scancode> currentKeys;
        std::unordered_set<SDL_Scancode> previousKeys;
    };
}

