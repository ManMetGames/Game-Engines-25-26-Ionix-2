#pragma once
#include "sol/sol.hpp"

namespace IonixEngine {
    class WindowScripting {
    public:
        static WindowScripting& Get();

        void Init(sol::state& lua);

    private:
        static WindowScripting* s_Instance;
    };
}