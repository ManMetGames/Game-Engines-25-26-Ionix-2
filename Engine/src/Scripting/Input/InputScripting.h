#pragma once
#include "sol/sol.hpp"
#include <SDL_gamecontroller.h>
#include <vector>

namespace IonixEngine {
    class InputScripting {
    public:
        static InputScripting& Get();

        void Init(sol::state& lua);

        void RefreshControllers();

    private:
        static InputScripting* s_Instance;
        std::vector<SDL_GameController*> m_Controllers;
    };
}
