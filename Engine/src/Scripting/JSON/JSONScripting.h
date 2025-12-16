#pragma once
#include "sol/sol.hpp"
#include "json.hpp"

namespace IonixEngine {
    class JsonScripting {
    public:
        static JsonScripting& Get();

        void Init(sol::state& lua);

    private:
        static JsonScripting* s_Instance;
    };
}