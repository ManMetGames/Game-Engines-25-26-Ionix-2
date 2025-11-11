#pragma once
#include "sol/sol.hpp"

namespace IonixEngine {
    class UIScripting {
    public:
        static UIScripting& Get();

        void Init(sol::state& lua);

    private:
        static UIScripting* s_Instance;
    };
}