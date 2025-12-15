#include "JsonScripting.h"
#include "Architecture/Application.h"
#include <fstream>

namespace IonixEngine
{
	JsonScripting* JsonScripting::s_Instance = nullptr;
	JsonScripting& JsonScripting::Get() {
		if (!s_Instance)
			s_Instance = new JsonScripting();
		return *s_Instance;
	}
	void JsonScripting::Init(sol::state& lua)
	{
		using json = nlohmann::json;

		auto saveHighScore = [](int score) {
			json data;
			data["highscore"] = score;
			std::ofstream file("flappyBirdHighScore.json");
			file << data.dump(4); // pretty print with indent 4

			};
		auto loadHighScore = []() -> int {
			std::ifstream file("flappyBirdHighScore.json");
			if (!file.is_open())
				return 0; // default if no file exists
			json data;
			file >> data;
			return data.value("highscore", 0);
			};

		lua["Json"] = lua.create_table_with(
			"save_high_score", saveHighScore,
			"load_high_score", loadHighScore
		);
	}
}