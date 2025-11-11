#pragma once
#include "sol/sol.hpp"

namespace IonixEngine {
    class InputScripting {
    public:
        static InputScripting& Get();

        void Init(sol::state& lua);

    private:
        static InputScripting* s_Instance;
    };
}