#pragma once
#include <unordered_set>
#include <SDL.h>
#include <iostream>

namespace IonixEngine
{
    struct MouseCoords
    {
        int x;
        int y;

        MouseCoords(int x, int y) : x(x), y(y) {}
    };

    struct ScrollCoords
    {
        int y;
        
        ScrollCoords(int y) : y(y) {};
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

        //Update state
        void CopyCodesEndFrame();


        // for Mouse Up
        void SetMouseReleased(Uint8 code);

        void SetScrollDiff(float diff);

        float GetScrollDiff() const;
      
        // for Previous Key
        void CopyCodesEndFrame();

        
  
        private:
            std::unordered_set<SDL_Scancode> currentKeys;
            std::unordered_set<SDL_Scancode> previousKeys;

            std::unordered_set<Uint8> currentMouse;
            std::unordered_set<Uint8> previousMouse;
            std::unordered_set<Uint8> currentScroll;
            std::unordered_set<Uint8> previousScroll;


            float scrollDiff = 0.0f; //Resets each frame
    private:
        //Keyboard
        std::unordered_set<SDL_Scancode> currentKeys;
        std::unordered_set<SDL_Scancode> previousKeys;

        //Mouse
        std::unordered_set<Uint8> currentMouse;
        std::unordered_set<Uint8> previousMouse;

    };
}

//Console Logs For Testing

// Keyboard
/*if (layerInput->m_Input->IsKeyDown(SDL_SCANCODE_SPACE))
{
    std::cout << "Spacebar was pressed once \n";
}
else if (layerInput->m_Input->IsKeyUp(SDL_SCANCODE_SPACE))
{
    std::cout << "Spacebar has been lifted \n";
}
else if (layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_SPACE))
{
    std::cout << "Spacebar is being held down \n";
}*/

// Controller
    // Buttons
        // A Button
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_A))
//{
//    std::cout << "Button A on the controller was pressed once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_A))
//{
//    std::cout << "Button A on the controller has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_A))
//{
//    std::cout << "Button A on the controller is being held down \n";
//}
//        // B Button
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_B))
//{
//    std::cout << "Button B on the controller was pressed once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_B))
//{
//    std::cout << "Button B on the controller has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_B))
//{
//    std::cout << "Button B on the controller is being held down \n";
//}
//        // X Button
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_X))
//{
//    std::cout << "Button X on the controller was pressed once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_X))
//{
//    std::cout << "Button X on the controller has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_X))
//{
//    std::cout << "Button X on the controller is being held down \n";
//}
//        // Y Button
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_Y))
//{
//    std::cout << "Button Y on the controller was pressed once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_Y))
//{
//    std::cout << "Button Y on the controller has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_Y))
//{
//    std::cout << "Button Y on the controller is being held down \n";
//}

    // D-Pad
        // Up
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_UP))
//{
//    std::cout << "Up on the controller D-Pad was pressed once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_DPAD_UP))
//{
//    std::cout << "Up on the controller D-Pad has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_UP))
//{
//    std::cout << "Up on the controller D-Pad is being held down \n";
//}
//        // Down
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
//{
//    std::cout << "Down on the controller D-Pad was pressed once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
//{
//    std::cout << "Down on the controller D-Pad has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
//{
//    std::cout << "Down on the controller D-Pad is being held down \n";
//}
//        // Left
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
//{
//    std::cout << "Left on the controller D-Pad was pressed once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
//{
//    std::cout << "Left on the controller D-Pad has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
//{
//    std::cout << "Left on the controller D-Pad is being held down \n";
//}
//        // Right
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
//{
//    std::cout << "Right on the controller D-Pad was pressed once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
//{
//    std::cout << "Right on the controller D-Pad has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
//{
//    std::cout << "Right on the controller D-Pad is being held down \n";
//}

    // Touchpad
/*if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_TOUCHPAD))
{
    std::cout << "Touchpad was pressed once \n";
}
else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_TOUCHPAD))
{
    std::cout << "Touchpad has been lifted \n";
}
else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_TOUCHPAD))
{
    std::cout << "Touchpad is being held down \n";
}*/

// Bumpers
     // Left
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
//{
//    std::cout << "the left bumper was pressed once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
//{
//    std::cout << "the left bumper has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
//{
//    std::cout << "the left bumper is being held down \n";
//}
//        // Right
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
//{
//    std::cout << "the right bumper was pressed once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
//{
//    std::cout << "the right bumper has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
//{
//    std::cout << "the right bumper is being held down \n";
//}

    // Sticks
        // Pressing Left
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_LEFTSTICK))
//{
//    std::cout << "the left stick was pressed down once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_LEFTSTICK)) 
//{
//    std::cout << "the left stick has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_LEFTSTICK))
//{
//    std::cout << "the left stick is being held down \n";
//}
//        // Pressing Right
//if (layerInput->m_Input->IsButtonDown(SDL_CONTROLLER_BUTTON_RIGHTSTICK))
//{
//    std::cout << "the right stick was pressed down once \n";
//}
//else if (layerInput->m_Input->IsButtonUp(SDL_CONTROLLER_BUTTON_RIGHTSTICK))
//{
//    std::cout << "the right stick has been lifted \n";
//}
//else if (layerInput->m_Input->IsButtonHeld(SDL_CONTROLLER_BUTTON_RIGHTSTICK))
//{
//    std::cout << "the right stick is being held down \n";
//}