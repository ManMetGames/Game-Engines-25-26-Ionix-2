#pragma once
#include <unordered_set>
#include <SDL.h>
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
        bool IsKeyDown(SDL_Scancode code) const;
        bool IsKeyUp(SDL_Scancode code) const;
        bool IsKeyHeld(SDL_Scancode code) const;
        
        MouseCoords GetMousePosition();

       

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

