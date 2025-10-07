#pragma once
#include "Architecture/Macros.h"

namespace IonixEngine
{
    enum class IonixEventType
    {
        None = 0,
        WindowClosed,
        Keyboard
        // ...
    };

    // keycodes. yay
    typedef enum SDL_Scancode
    {
        SDL_SCANCODE_UNKNOWN = 0,

        // alphabet
        SDL_SCANCODE_A = 1,
        SDL_SCANCODE_B = 2,
        SDL_SCANCODE_C = 3,
        SDL_SCANCODE_D = 4,
        SDL_SCANCODE_E = 5, 
        SDL_SCANCODE_F = 6,
        SDL_SCANCODE_G = 7,
        SDL_SCANCODE_H = 8,
        SDL_SCANCODE_I = 9,
        SDL_SCANCODE_J = 10,
        SDL_SCANCODE_K = 11,
        SDL_SCANCODE_L = 12,
        SDL_SCANCODE_M = 13,
        SDL_SCANCODE_N = 14,
        SDL_SCANCODE_O = 15,
        SDL_SCANCODE_P = 16,
        SDL_SCANCODE_Q = 17,
        SDL_SCANCODE_R = 18,
        SDL_SCANCODE_S = 19,
        SDL_SCANCODE_T = 20,
        SDL_SCANCODE_U = 21,
        SDL_SCANCODE_V = 22, 
        SDL_SCANCODE_W = 23,
        SDL_SCANCODE_X = 24,
        SDL_SCANCODE_Y = 25,
        SDL_SCANCODE_Z = 26,

        //numbers
        SDL_SCANCODE_1 = 27,
        SDL_SCANCODE_2 = 28,
        SDL_SCANCODE_3 = 29,
        SDL_SCANCODE_4 = 30,
        SDL_SCANCODE_5 = 31,
        SDL_SCANCODE_6 = 32,
        SDL_SCANCODE_7 = 33,
        SDL_SCANCODE_8 = 34,
        SDL_SCANCODE_9 = 35,
        SDL_SCANCODE_0 = 36,

        //other keys
        SDL_SCANCODE_TAB = 43,
        SDL_SCANCODE_SPACE = 44,
        SDL_SCANCODE_RIGHT = 79,
        SDL_SCANCODE_LEFT = 80,
        SDL_SCANCODE_DOWN = 81,
        SDL_SCANCODE_UP = 82
    };

    // Event class
    class IonixEvent
    {
    public:
        IonixEventType Type = IonixEventType::None;
        bool Handled = false;

        IonixEvent(IonixEventType type) : Type(type) {}
        virtual ~IonixEvent() = default;
    };

    // Window Events
    class WindowClosedEvent : public IonixEvent
    {
    public:
        WindowClosedEvent() : IonixEvent(IonixEventType::WindowClosed) {}
    };

    // Keyboard Events
    class KeyboardEvent : public IonixEvent
    {
    public:
        KeyboardEvent(SDL_Scancode scnCode) : 
            scnCode(scnCode), IonixEvent(IonixEventType::Keyboard){} // Keyboard

        SDL_Scancode scnCode;
        bool isPressed;
    };


    // .. add more as needed...
}