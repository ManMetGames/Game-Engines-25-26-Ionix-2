#pragma once
#include "sol/sol.hpp"

namespace IonixEngine {
    class FysicsScripting {
    public:
        static FysicsScripting& Get();

        void Init(sol::state& lua);

    private:
        static FysicsScripting* s_Instance;
    };
}
