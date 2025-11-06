#pragma once
#include "sol/sol.hpp"
#include "Maf/MafUtils.h"

namespace IonixEngine {
    class MafsScripting {
    public:
        static MafsScripting& Get();

        void Init(sol::state& lua);

    private:
        static MafsScripting* s_Instance;
    };
}
