#pragma once
#include "sol/sol.hpp"

namespace IonixEngine {
    class VFXScripting {
    public:
        static VFXScripting& Get();

        void Init(sol::state& lua);

    private:
        static VFXScripting* s_Instance;
    };
}
