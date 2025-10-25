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
        // for Previous Key
        void CopyCodesEndFrame()
        {
            previousKeys = currentKeys;
        }

     // Controller

        // connect/remove controller
        void FindController()
        {
            SDL_GameController* controller = findController(); // finds the controller 
            
            // notes on how i want to do this in theory: logic is probably wrong. oh well
            // findController() gets the ID of the current controller connected (i assume an int)
            // if [SOMETHING] = the ID, then print controller is found
            // else say it's not there
            // also check to see if a controller was already previously connected or something
        }

        // controller button down
        /*
        * void SetButtonPressed(SDL_GameControllerButton btn)
        * {
        *       currentButton.insert(btn);
        * }
        */
        // controller button up
        /*
        * void SetButtonReleased(SDL_GameControllerButton btn)
        * {
        *       currentButton.erase(btn);
        * }
        */
        // previous button
        /*
        * void CopyButtonsEndFrame()
        * {
        *       previousButton = currentButton;
        * }
        */

        // button input booleans
        bool isButtonDown(SDL_GameControllerButton btn) const
        {
            // Not held down previous frame
            // Held down current frame
                // return !previousButton.count(btn) && currentButton.count(btn);
        }
        bool isButtonUp(SDL_GameControllerButton btn) const
        {
            // Was held down previous frame
            // No longer held down on current frame
                // return previousButton.count(btn) && !currentButton.count(btn);
        }
        bool isButtonHeld(SDL_GameControllerButton btn) const
        {
            // return currentButton.count(btn);
        }

    private:
        std::unordered_set<SDL_Scancode> currentKeys;
        std::unordered_set<SDL_Scancode> previousKeys;
        std::unordered_set<SDL_GameControllerButton> currentButton;
        std::unordered_set<SDL_GameControllerButton> previousButton;
        
        // Pointer to find controller
        // supposed to grab its ID to say how many there are 
        SDL_GameController* findController()
        {
            for (int i = 0; i < SDL_NumJoysticks(); i++)
            {
                if (SDL_IsGameController(i))
                {
                    return SDL_GameControllerOpen(i);
                }
            }
            return nullptr;
        }
    };
}

