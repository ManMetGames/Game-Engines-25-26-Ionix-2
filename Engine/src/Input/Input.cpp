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

    //Micrphone 
    

    // bool Input::IsAudioRecording() const
    // void Input::SetRecordingAudioStart() const
    // void Input::SetRecordingAudioStop() const

   

    //Frame management
    void Input::CopyCodesEndFrame()
    {
        previousKeys = currentKeys;
        previousMouse = currentMouse;

    }
}
