#pragma once
#include "sol/sol.hpp"
#include "saveToJson.h"

namespace IonixEngine {
    class JSONScripting {
    public:
        static JSONScripting& Get();

        void Init(sol::state& lua);

    private:
        static JSONScripting* s_Instance;
    };
}