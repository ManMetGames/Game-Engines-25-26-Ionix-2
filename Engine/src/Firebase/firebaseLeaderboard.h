#pragma once
#include "sol/sol.hpp"
#include "firebase/app.h"
//#include "firebase/database.h"

namespace IonixEngine 
{
    class FirebaseLeaderboard
    {
    public:
        static void Init();
        static void TestWrite();
    };
}