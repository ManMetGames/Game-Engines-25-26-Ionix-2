#include "Scripting/Input/InputScripting.h"
#include "Architecture/Application.h"
#include <SDL.h>
#include <SDL_gamecontroller.h>
#include <cmath>

namespace IonixEngine {

    InputScripting* InputScripting::s_Instance = nullptr;

    InputScripting& InputScripting::Get() {
        if (!s_Instance)
            s_Instance = new InputScripting();
        return *s_Instance;
    }

    void InputScripting::RefreshControllers()
    {
        for (auto* c : m_Controllers)
            if (c) SDL_GameControllerClose(c);
        m_Controllers.clear();

        int num = SDL_NumJoysticks();
        for (int i = 0; i < num; i++)
        {
            if (SDL_IsGameController(i))
            {
                SDL_GameController* ctrl = SDL_GameControllerOpen(i);
                if (ctrl)
                {
                    m_Controllers.push_back(ctrl);
                    std::cout << "[LuaInput] Controller " << i << ": "
                        << SDL_GameControllerName(ctrl) << " connected\n";
                }
            }
        }
    }

    void InputScripting::Init(sol::state& lua)
    {
        RefreshControllers();

        auto getKeyUp = [](int code) -> bool {
            return Application::Get().layerInput->m_Input->IsKeyUp(static_cast<SDL_Scancode>(code));
            };
        auto getKeyDown = [](int code) -> bool {
            return Application::Get().layerInput->m_Input->IsKeyDown(static_cast<SDL_Scancode>(code));
            };
        auto getKeyHeld = [](int code) -> bool {
            return Application::Get().layerInput->m_Input->IsKeyHeld(static_cast<SDL_Scancode>(code));
            };

        auto getMouseX = []() -> int {
            return Application::Get().layerInput->m_Input->GetMousePosition().x;
            };
        auto getMouseY = []() -> int {
            return Application::Get().layerInput->m_Input->GetMousePosition().y;
            };
        auto getMouseButtonDown = [](int mousecode)-> bool {
            return Application::Get().layerInput->m_Input->IsMouseButtonDown(static_cast<uint8>(mousecode));
            };
        auto getMouseButtonUp = [](int mousecode)-> bool {
            return Application::Get().layerInput->m_Input->IsMouseButtonUp(static_cast<uint8>(mousecode));
            };

        auto getButtonDown = [this](int index, int btn) -> bool {
            return Application::Get().layerInput->IsControllerButtonDown(index, static_cast<Uint8>(btn));
            };

        auto getButtonUp = [this](int index, int btn) -> bool {
            return Application::Get().layerInput->IsControllerButtonUp(index, static_cast<Uint8>(btn));
            };

        auto getButtonHeld = [this](int index, int btn) -> bool {
            return Application::Get().layerInput->IsControllerButtonHeld(index, static_cast<Uint8>(btn));
            };

        auto getStickAxis = [this](int index, SDL_GameControllerAxis axis) -> float {
            return Application::Get().layerInput->GetControllerAxis(index, axis);
            };

        auto getTriggerPressure = [this](int index, SDL_GameControllerAxis trigger) -> float {
            return Application::Get().layerInput->GetControllerPressure(index, trigger);
            };

        auto getLeftStickX = [=](int index) { return getStickAxis(index, SDL_CONTROLLER_AXIS_LEFTX); };
        auto getLeftStickY = [=](int index) { return getStickAxis(index, SDL_CONTROLLER_AXIS_LEFTY); };
        auto getRightStickX = [=](int index) { return getStickAxis(index, SDL_CONTROLLER_AXIS_RIGHTX); };
        auto getRightStickY = [=](int index) { return getStickAxis(index, SDL_CONTROLLER_AXIS_RIGHTY); };
        auto getLeftTrigger = [=](int index) { return getTriggerPressure(index, SDL_CONTROLLER_AXIS_TRIGGERLEFT); };
        auto getRightTrigger = [=](int index) { return getTriggerPressure(index, SDL_CONTROLLER_AXIS_TRIGGERRIGHT); };

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
            "ionix_space", SDL_SCANCODE_SPACE,
            "ionix_escape", SDL_SCANCODE_ESCAPE,
            "ionix_return", SDL_SCANCODE_RETURN,
            "ionix_tab", SDL_SCANCODE_TAB,
            "ionix_backspace", SDL_SCANCODE_BACKSPACE,
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

        lua["Buttons"] = lua.create_table_with(
            "ionix_a", SDL_CONTROLLER_BUTTON_A,
            "ionix_b", SDL_CONTROLLER_BUTTON_B,
            "ionix_x", SDL_CONTROLLER_BUTTON_X,
            "ionix_y", SDL_CONTROLLER_BUTTON_Y,
            "ionix_start", SDL_CONTROLLER_BUTTON_START,
            "ionix_back", SDL_CONTROLLER_BUTTON_BACK,
            "ionix_dpad_up", SDL_CONTROLLER_BUTTON_DPAD_UP,
            "ionix_dpad_down", SDL_CONTROLLER_BUTTON_DPAD_DOWN,
            "ionix_dpad_left", SDL_CONTROLLER_BUTTON_DPAD_LEFT,
            "ionix_dpad_right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
            "ionix_left_shoulder", SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
            "ionix_right_shoulder", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
            "ionix_left_stick", SDL_CONTROLLER_BUTTON_LEFTSTICK,
            "ionix_right_stick", SDL_CONTROLLER_BUTTON_RIGHTSTICK,
            "ionix_right_trigger", SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
            "ionix_left_trigger", SDL_CONTROLLER_AXIS_TRIGGERLEFT
        );

        lua["Input"] = lua.create_table_with(
            "get_key_up", getKeyUp,
            "get_key_down", getKeyDown,
            "get_key_held", getKeyHeld,

            "get_mouse_x", getMouseX,
            "get_mouse_y", getMouseY,
            "get_mouse_button_down", getMouseButtonDown,
            "get_mouse_button_up", getMouseButtonUp,

            "get_button_down", getButtonDown,
            "get_button_up", getButtonUp,
            "get_button_held", getButtonHeld,
            "get_left_stick_x", getLeftStickX,
            "get_left_stick_y", getLeftStickY,
            "get_right_stick_x", getRightStickX,
            "get_right_stick_y", getRightStickY,
            "get_left_trigger", getLeftTrigger,
            "get_right_trigger", getRightTrigger
        );
    }
}
