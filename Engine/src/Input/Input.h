#pragma once
#include <unordered_set>
#include <SDL.H>

namespace IonixEngine
{
    class Input
    {
    public:

    // Keyboard

        // keyboard input booleans
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

     // Controller

         // button input booleans
        bool isButtonDown(Uint8 btn) const
        {
            // Not held down previous frame
            

            // Held down current frame
            return !previousButton.count(btn) && currentButton.count(btn);
        }
        bool isButtonUp(Uint8 btn) const
        {
            // Was held down previous frame

            // No longer held down on current frame
            return previousButton.count(btn) && !currentButton.count(btn);
        }
        bool isButtonHeld(Uint8 btn) const
        {
            return currentButton.count(btn);
        }

        // controller button down
        void SetButtonPressed(Uint8 btn)
        {
            currentButton.insert(btn);
        }
        // controller button up
        void SetButtonReleased(Uint8 btn)
        {
            currentButton.erase(btn);
        }
 

    // Previous Frame Keys/Buttons
       void CopyCodesEndFrame()
        {
            previousKeys = currentKeys;
            previousButton = currentButton;
        }


    private:
        // Keyboard
        std::unordered_set<SDL_Scancode> currentKeys;
        std::unordered_set<SDL_Scancode> previousKeys;
        // Controller
        std::unordered_set<Uint8> currentButton;
        std::unordered_set<Uint8> previousButton;
       
    };
}

