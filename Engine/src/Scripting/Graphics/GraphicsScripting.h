#pragma once
#include "sol/sol.hpp"

namespace IonixEngine {
    class GraphicsScripting {
    public:
        static GraphicsScripting& Get();

        void Init(sol::state& lua);

    private:
        static GraphicsScripting* s_Instance;
    };
}