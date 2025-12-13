#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX // Prevents conflicts with min/max macros
#include <windows.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdint> 
#include "sol/sol.hpp"
#include "firebase/app.h"
#include "firebase/database.h"
#include "firebase/variant.h"


namespace IonixEngine
{
    class FirebaseLeaderboard
    {
    public:
        static void Init();

        static void SubmitScore(const std::string& name, int score);

        struct LeaderboardEntry {
            std::string name;
            int score;
        };

        static void RetrieveTopScores(int count);

    };
};