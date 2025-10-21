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
        MouseCoords GetMousePosition();

        bool IsKeyDown(SDL_Scancode code) const;


        bool IsKeyUp(SDL_Scancode code) const;
        
       
        bool IsKeyHeld(SDL_Scancode code) const;
       

        //Mouse Button Down 
        bool IsMouseButtonDown(Uint8 mousecode) const;


        // for Key Down
        void SetKeyPressed(SDL_Scancode code); 

        
        // for Key Up
        void SetKeyReleased(SDL_Scancode code);
      
        // for Previous Key
        void CopyCodesEndFrame();
  
        private:
            std::unordered_set<SDL_Scancode> currentKeys;
            std::unordered_set<SDL_Scancode> previousKeys;

    };
}

