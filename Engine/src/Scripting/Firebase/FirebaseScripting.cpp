#include "FirebaseScripting.h"
#include <fstream>

namespace IonixEngine
{
	FirebaseScripting* FirebaseScripting::s_Instance = nullptr;
	FirebaseScripting& FirebaseScripting::Get() {
		if (!s_Instance)
			s_Instance = new FirebaseScripting();
		return *s_Instance;
	}
	void FirebaseScripting::Init(sol::state& lua)
	{
		
		auto submitHighScore = [](const std::string& name, int score) {Application::Get().firebaseLeaderboard->SubmitScore(name, score);
			};

		auto retrieveHighScore = [](int count) {Application::Get().firebaseLeaderboard->RetrieveTopScores(count);
			};

		lua["Firebase"] = lua.create_table_with(
			"submit_high_score", submitHighScore,
			"retrieve_high_score", retrieveHighScore
		);
		
	}
}