#pragma once
#include <unordered_set>
#include <SDL.H>
#include <iostream>

namespace IonixEngine
{
    class unordered_set;

    struct MouseCoords 
    {
        int x;
        int y;

        MouseCoords(int x, int y) : x(x), y(y) {};
    };

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
        MouseCoords GetMousePosition();

        bool IsKeyDown(SDL_Scancode code) const;


        bool IsKeyUp(SDL_Scancode code) const;
        
       
        bool IsKeyHeld(SDL_Scancode code) const;
       

        //Mouse Button Down 
        bool IsMouseButtonDown(Uint8 mousecode) const;

        bool IsMouseButtonUp(Uint8 mousecode) const;
      

        // for Key Down
        void SetKeyPressed(SDL_Scancode code); 

        
        // for Key Up
        void SetKeyReleased(SDL_Scancode code);

        // for Mouse Down
        void SetMousePressed(Uint8 code);


        // for Mouse Up
        void SetMouseReleased(Uint8 code);
      
        // for Previous Key
        void CopyCodesEndFrame();
  
        private:
            std::unordered_set<SDL_Scancode> currentKeys;
            std::unordered_set<SDL_Scancode> previousKeys;

            std::unordered_set<Uint8> currentMouse;
            std::unordered_set<Uint8> previousMouse;


    };
}

