#pragma once
#include "sol/sol.hpp"
#include "json.hpp"

namespace IonixEngine 
{
	using json = nlohmann::json;
	void SaveFlappyBirdHighScore(int score);
	int LoadFlappyBirdHighScore();
}