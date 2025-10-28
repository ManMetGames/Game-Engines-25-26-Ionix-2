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

        // controller axis variables:
        /*
            // RT
        float GetRightTrigger()
        {
            return rightTrigger;
        }
        float SetRightTrigger()
        {
        }
            // LT
        float GetLeftTrigger()
        {
            return leftTrigger;
        }
        float SetLeftTrigger()
        {
        }
            // Left Stick Horizontal
        float GetLeftStickHorizontal()
        {
            return leftStickHorizontal;
        }
        float SetLeftStickHorizontal()
        {
        }
            // Left Stick Vetical
        float GetLeftStickVertical()
        {
            return leftStickVertical;
        }
        float SetLeftStickVertical()
        {
        }
            // Right Stick Horizontal
        float GetRightStickHoriontal()
        {
            return rightStickHorizontal;
        }
        float SetRightStickHorizontal()
        {
        }
            // Right Stick Vertical
        float GetRightStickVertical()
        {
            return rightStickVertical;
        }
        float SetRightStickVertical()
        {
        }
        */
        

        // normalised controller axis:
        void NormaliseLeftXAxis(float axis)
        {
            leftStickHorizontal = (float) axis / 32768;
            leftStickHorizontal = std::round(leftStickHorizontal * 100) / 100;
        }

        // Previous Frame Keys/Buttons
        void CopyCodesEndFrame()
        {
            previousKeys = currentKeys;
            previousButton = currentButton;
        }
    
        float GetXStickHor() { return leftStickHorizontal; }

    private:
        // Keyboard:
        std::unordered_set<SDL_Scancode> currentKeys;
        std::unordered_set<SDL_Scancode> previousKeys;

        // Controller:
            // Buttons
        std::unordered_set<Uint8> currentButton;
        std::unordered_set<Uint8> previousButton;

        // Axis
        float leftStickVertical;
        float leftStickHorizontal;
        float rightStickVertical;
        float rightStickHorizontal;
        float leftTrigger;
        float rightTrigger;
    };
}

