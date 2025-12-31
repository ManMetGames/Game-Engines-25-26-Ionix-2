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
		
		auto submitHighScore = [](const std::string& gameId, const std::string& name, int score)
			{
				Application::Get().firebaseLeaderboard->SubmitScore(gameId, name, score);
			};

		auto retrieveHighScore = [](sol::this_state ts, const std::string& gameId, int count)
			{
				sol::state_view lua(ts);

				auto scores = Application::Get().firebaseLeaderboard->GetTopScores(gameId, count);

				sol::table out = lua.create_table((int)scores.size(), 0);
				int i = 1;
				for (auto& e : scores)
					out[i++] = lua.create_table_with("name", e.name, "score", e.score);

				return out;
			};


		lua["Firebase"] = lua.create_table_with(
			"submit_high_score", submitHighScore,
			"retrieve_high_score", retrieveHighScore
		);
		
	}
}