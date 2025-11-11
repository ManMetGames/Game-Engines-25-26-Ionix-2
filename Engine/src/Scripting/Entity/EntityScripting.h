#pragma once
#include "sol/sol.hpp"

namespace IonixEngine {
    class EntityScripting {
    public:
        static EntityScripting& Get();

        void Init(sol::state& lua);

    private:
        static EntityScripting* s_Instance;
    };
}