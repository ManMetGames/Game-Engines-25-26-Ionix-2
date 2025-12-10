#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX // Prevents conflicts with min/max macros
#include <windows.h>

#include <iostream>
#include "sol/sol.hpp"
#include "firebase/app.h"
#include "firebase/database.h"


namespace IonixEngine 
{
    class FirebaseLeaderboard
    {
    public:
        static void Init();
        static void TestWrite(int score);
    };
}