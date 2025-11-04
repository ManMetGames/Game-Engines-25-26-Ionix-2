#pragma once
#include <unordered_set>
#include <SDL.h>

namespace IonixEngine
{
    struct MouseCoords
    {
        int x;
        int y;

        MouseCoords(int x, int y) : x(x), y(y) {}
    };

    class Input
    {
    public:
        //Mouse
        MouseCoords GetMousePosition();

        //Keyboard
        bool IsKeyDown(SDL_Scancode code) const;
        bool IsKeyUp(SDL_Scancode code) const;
        bool IsKeyHeld(SDL_Scancode code) const;
        void SetKeyPressed(SDL_Scancode code);
        void SetKeyReleased(SDL_Scancode code);

        //Mouse buttons
        bool IsMouseButtonDown(Uint8 mousecode) const;
        bool IsMouseButtonUp(Uint8 mousecode) const;
        void SetMousePressed(Uint8 code);
        void SetMouseReleased(Uint8 code);

        //Controller buttons
        bool IsButtonDown(Uint8 btn) const;
        bool IsButtonUp(Uint8 btn) const;
        bool IsButtonHeld(Uint8 btn) const;
        void SetButtonPressed(Uint8 btn);
        void SetButtonReleased(Uint8 btn);

        //Controller axis normalization
        void NormaliseLeftXAxis(float axis);

        //Update state
        void CopyCodesEndFrame();

        //Getters
        float GetXStickHor() { return leftStickHorizontal; }

    private:
        //Keyboard
        std::unordered_set<SDL_Scancode> currentKeys;
        std::unordered_set<SDL_Scancode> previousKeys;

        //Mouse
        std::unordered_set<Uint8> currentMouse;
        std::unordered_set<Uint8> previousMouse;

        //Controller buttons
        std::unordered_set<Uint8> currentButton;
        std::unordered_set<Uint8> previousButton;

        //Controller axes
        float leftStickVertical = 0.0f;
        float leftStickHorizontal = 0.0f;
        float rightStickVertical = 0.0f;
        float rightStickHorizontal = 0.0f;
        float leftTrigger = 0.0f;
        float rightTrigger = 0.0f;
    };
}
