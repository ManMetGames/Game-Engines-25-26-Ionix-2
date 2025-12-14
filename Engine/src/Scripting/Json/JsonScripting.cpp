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

        auto saveHighScore = [](const std::string& gameId, int score) {
            using json = nlohmann::json;

            json data;

            // load existing
            std::ifstream in("user_profile.json");
            if (in.is_open()) {
                try { in >> data; }
                catch (...) {}
            }

            data["highScores"][gameId] = score;

            std::ofstream out("user_profile.json");
            out << data.dump(4);
            };

        auto loadHighScore = [](const std::string& gameId) -> int {
            using json = nlohmann::json;

            std::ifstream file("user_profile.json");
            if (!file.is_open())
                return 0;

            json data;
            try { file >> data; }
            catch (...) { return 0; }

            if (!data.contains("highScores"))
                return 0;

            return data["highScores"].value(gameId, 0);
            };

        // persistent player name
        auto savePlayerName = [](const std::string& name) {
            using json = nlohmann::json;

            json data;
            std::ifstream in("user_profile.json");
            if (in.is_open()) {
                try { in >> data; }
                catch (...) {}
            }

            data["playerName"] = name;

            std::ofstream out("user_profile.json");
            out << data.dump(4);
            };

        auto loadPlayerName = []() -> std::string {
            using json = nlohmann::json;

            std::ifstream file("user_profile.json");
            if (!file.is_open())
                return "";

            json data;
            try { file >> data; }
            catch (...) { return ""; }

            return data.value("playerName", "");
            };

        lua["Json"] = lua.create_table_with(
            "save_high_score", saveHighScore,
            "load_high_score", loadHighScore,
            "save_player_name", savePlayerName,
            "load_player_name", loadPlayerName
		);
	}
}