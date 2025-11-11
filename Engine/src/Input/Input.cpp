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


MouseCoords IonixEngine::Input::GetMousePosition()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    MouseCoords mc(x, y);
    return mc; 
}

// Keyboard
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

// Mouse
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

// Scroll Wheel (your feature)
void Input::SetScrollDiff(float diff)
{
    scrollDiff = diff;
}

float Input::GetScrollDiff() const
{
    return scrollDiff;
}

// Controller buttons (from main)
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

// Controller axis (from main)
float Input::NormaliseStickAxis(float axis)
{
    controllerAxis = static_cast<float>(axis) / 32768.0f;
    controllerAxis = std::round(controllerAxis * 100.0f) / 100.0f;
    return controllerAxis;
}

float Input::NormaliseTrigger(float axis)
{
    triggerPressure = static_cast<float>(axis) / 32767.0f;
    triggerPressure = std::round(triggerPressure * 100.0f) / 100.0f;
    return triggerPressure;
}

// Frame management (merged both versions)
void Input::CopyCodesEndFrame()
{
    previousKeys = currentKeys;
    previousMouse = currentMouse;
    previousButton = currentButton;
    previousScroll = currentScroll;

    scrollDiff = 0.0f;
}