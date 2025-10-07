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

    // scancodes
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

    // keycodes
    typedef enum SDL_Keycode
    {
        SDLK_UNKNOWN = 0,

        // alphabet
        SDLK_a = 'a',
        SDLK_b = 'b',
        SDLK_c = 'c',
        SDLK_d = 'd',
        SDLK_e = 'e',
        SDLK_f = 'f',
        SDLK_g = 'g',
        SDLK_h = 'h',
        SDLK_i = 'i',
        SDLK_j = 'j',
        SDLK_k = 'k',
        SDLK_l = 'l',
        SDLK_m = 'm',
        SDLK_n = 'n',
        SDLK_o = 'o',
        SDLK_p = 'p',
        SDLK_q = 'q',
        SDLK_r = 'r',
        SDLK_s = 's',
        SDLK_t = 't',
        SDLK_u = 'u',
        SDLK_v = 'v',
        SDLK_w = 'w',
        SDLK_x = 'x',
        SDLK_y = 'y',
        SDLK_z = 'z',

        //numbers
        SDLK_1 = '1',
        SDLK_2 = '2',
        SDLK_3 = '3',
        SDLK_4 = '4',
        SDLK_5 = '5',
        SDLK_6 = '6',
        SDLK_7 = '7',
        SDLK_8 = '8',
        SDLK_9 = '9',
        SDLK_0 = '0',


        //other keys
        //SDLK_TAB = '\t',
        //SDLK_SPACE = ' ',
        //SDLK_RIGHT = "",
        //SDLK_LEFT = 80,
        //SDLK_DOWN = 81,
        //SDLK_UP = 82
        //this will be done at a later time


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
        SDL_Scancode scnCode;
        SDL_Keycode keyCode;
        bool isPressed;

        KeyboardEvent(SDL_Scancode scnCode, SDL_Keycode keycode, bool isPressed) : 
            scnCode(scnCode), keyCode(keyCode), isPressed(isPressed), IonixEvent(IonixEventType::Keyboard){}
    };

    // .. add more as needed...
}