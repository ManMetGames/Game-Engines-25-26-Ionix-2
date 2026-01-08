#pragma once
#include "sol/sol.hpp"

namespace IonixEngine {
    class NavScripting {
    public:
        static NavScripting& Get();

        void Init(sol::state& lua);

    private:
        static NavScripting* s_Instance;
    };
}
