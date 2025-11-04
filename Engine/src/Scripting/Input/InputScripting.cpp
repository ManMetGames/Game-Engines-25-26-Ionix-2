#include "Scripting/Input/InputScripting.h"
#include "Architecture/Application.h"

namespace IonixEngine {
    
    InputScripting* InputScripting::s_Instance = nullptr;

    InputScripting& InputScripting::Get() {
        if (!s_Instance)
            s_Instance = new InputScripting();
        return *s_Instance;
    }

    void InputScripting::Init(sol::state& lua)
    {
        auto getKeyUp = [](int code) -> bool {
            return Application::Get().layerInput->m_Input->IsKeyUp(static_cast<SDL_Scancode>(code));
            };
        auto getKeyDown = [](int code) -> bool {
            return Application::Get().layerInput->m_Input->IsKeyDown(static_cast<SDL_Scancode>(code));
            };
        auto getKeyHeld = [](int code) -> bool {
            return Application::Get().layerInput->m_Input->IsKeyHeld(static_cast<SDL_Scancode>(code));
            };

        lua["Keys"] = lua.create_table_with(
            "ionix_a", SDL_SCANCODE_A,
            "ionix_b", SDL_SCANCODE_B,
            "ionix_c", SDL_SCANCODE_C,
            "ionix_d", SDL_SCANCODE_D,
            "ionix_e", SDL_SCANCODE_E,
            "ionix_f", SDL_SCANCODE_F,
            "ionix_g", SDL_SCANCODE_G,
            "ionix_h", SDL_SCANCODE_H,
            "ionix_i", SDL_SCANCODE_I,
            "ionix_j", SDL_SCANCODE_J,
            "ionix_k", SDL_SCANCODE_K,
            "ionix_l", SDL_SCANCODE_L,
            "ionix_m", SDL_SCANCODE_M,
            "ionix_n", SDL_SCANCODE_N,
            "ionix_o", SDL_SCANCODE_O,
            "ionix_p", SDL_SCANCODE_P,
            "ionix_q", SDL_SCANCODE_Q,
            "ionix_r", SDL_SCANCODE_R,
            "ionix_s", SDL_SCANCODE_S,
            "ionix_t", SDL_SCANCODE_T,
            "ionix_u", SDL_SCANCODE_U,
            "ionix_v", SDL_SCANCODE_V,
            "ionix_w", SDL_SCANCODE_W,
            "ionix_x", SDL_SCANCODE_X,
            "ionix_y", SDL_SCANCODE_Y,
            "ionix_z", SDL_SCANCODE_Z,
            "ionix_1", SDL_SCANCODE_1,
            "ionix_2", SDL_SCANCODE_2,
            "ionix_3", SDL_SCANCODE_3,
            "ionix_4", SDL_SCANCODE_4,
            "ionix_5", SDL_SCANCODE_5,
            "ionix_6", SDL_SCANCODE_6,
            "ionix_7", SDL_SCANCODE_7,
            "ionix_8", SDL_SCANCODE_8,
            "ionix_9", SDL_SCANCODE_9,
            "ionix_0", SDL_SCANCODE_0,
            "ionix_return", SDL_SCANCODE_RETURN,
            "ionix_escape", SDL_SCANCODE_ESCAPE,
            "ionix_backspace", SDL_SCANCODE_BACKSPACE,
            "ionix_tab", SDL_SCANCODE_TAB,
            "ionix_space", SDL_SCANCODE_SPACE,
            "ionix_minus", SDL_SCANCODE_MINUS,
            "ionix_equals", SDL_SCANCODE_EQUALS,
            "ionix_leftbracket", SDL_SCANCODE_LEFTBRACKET,
            "ionix_rightbracket", SDL_SCANCODE_RIGHTBRACKET,
            "ionix_backslash", SDL_SCANCODE_BACKSLASH,
            "ionix_lctrl", SDL_SCANCODE_LCTRL,
            "ionix_lshift", SDL_SCANCODE_LSHIFT,
            "ionix_lalt", SDL_SCANCODE_LALT,
            "ionix_lgui", SDL_SCANCODE_LGUI,
            "ionix_rctrl", SDL_SCANCODE_RCTRL,
            "ionix_rshift", SDL_SCANCODE_RSHIFT,
            "ionix_ralt", SDL_SCANCODE_RALT,
            "ionix_rgui", SDL_SCANCODE_RGUI
        );

        lua["Input"] = lua.create_table_with(
            "get_key_up", getKeyUp,
            "get_key_down", getKeyDown,
            "get_key_held", getKeyHeld
        );
                
    }
}