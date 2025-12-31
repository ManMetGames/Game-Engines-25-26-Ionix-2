#pragma once
#include "sol/sol.hpp"
#include "Architecture/Application.h"
#include "Firebase/firebaseLeaderboard.h"

namespace IonixEngine {
    class FirebaseScripting {
    public:
        static FirebaseScripting& Get();

        void Init(sol::state& lua);

    private:
        static FirebaseScripting* s_Instance;
    };
}