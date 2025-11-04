#include "Input.h"
#include <cmath> 

namespace IonixEngine
{
    MouseCoords Input::GetMousePosition()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return MouseCoords(x, y);
    }

    //Keyboard
    bool Input::IsKeyDown(SDL_Scancode code) const
    {
        return !previousKeys.count(code) && currentKeys.count(code);
    }

    bool Input::IsKeyUp(SDL_Scancode code) const
    {
        return previousKeys.count(code) && !currentKeys.count(code);
    }

    bool Input::IsKeyHeld(SDL_Scancode code) const
    {
        return currentKeys.count(code);
    }

    void Input::SetKeyPressed(SDL_Scancode code)
    {
        currentKeys.insert(code);
    }

    void Input::SetKeyReleased(SDL_Scancode code)
    {
        currentKeys.erase(code);
    }

    //Mouse
    bool Input::IsMouseButtonDown(Uint8 mousecode) const
    {
        return currentMouse.count(mousecode);
    }

    bool Input::IsMouseButtonUp(Uint8 mousecode) const
    {
        return previousMouse.count(mousecode);
    }

    void Input::SetMousePressed(Uint8 code)
    {
        currentMouse.insert(code);
    }

    void Input::SetMouseReleased(Uint8 code)
    {
        currentMouse.erase(code);
    }

    //Controller buttons
    bool Input::IsButtonDown(Uint8 btn) const
    {
        return !previousButton.count(btn) && currentButton.count(btn);
    }

    bool Input::IsButtonUp(Uint8 btn) const
    {
        return previousButton.count(btn) && !currentButton.count(btn);
    }

    bool Input::IsButtonHeld(Uint8 btn) const
    {
        return currentButton.count(btn);
    }

    void Input::SetButtonPressed(Uint8 btn)
    {
        currentButton.insert(btn);
    }

    void Input::SetButtonReleased(Uint8 btn)
    {
        currentButton.erase(btn);
    }

    //Controller axes
    void Input::NormaliseLeftXAxis(float axis)
    {
        leftStickHorizontal = static_cast<float>(axis) / 32768.0f;
        leftStickHorizontal = std::round(leftStickHorizontal * 100.0f) / 100.0f;
    }

    void Input::NormaliseLeftYAxis(float axis)
    {
        leftStickVertical = static_cast<float>(axis) / 32768.0f;
        leftStickVertical = std::round(leftStickVertical * 100.0f) / 100.0f;
    }

    void Input::NormaliseRightXAxis(float axis)
    {
        rightStickHorizontal = static_cast<float>(axis) / 32768.0f;
        rightStickHorizontal = std::round(rightStickHorizontal * 100.0f) / 100.0f;
    }

    void Input::NormaliseRightYAxis(float axis)
    {
        rightStickVertical = static_cast<float>(axis) / 32768.0f;
        rightStickVertical = std::round(rightStickVertical * 100.0f) / 100.0f;
    }

    void Input::NormaliseLeftTrigger(float axis)
    {
        leftTriggerPressure = static_cast<float>(axis) / 3276.0f;
        leftTriggerPressure = std::round(leftTriggerPressure * 100.0f) / 100.0f;
    }

    void Input::NormaliseRightTrigger(float axis)
    {
        rightTriggerPressure = static_cast<float>(axis) / 32767.0f;
        rightTriggerPressure = std::round(rightTriggerPressure * 100.0f) / 100.0f;
    }

    //Frame management
    void Input::CopyCodesEndFrame()
    {
        previousKeys = currentKeys;
        previousMouse = currentMouse;
        previousButton = currentButton;
    }
}
