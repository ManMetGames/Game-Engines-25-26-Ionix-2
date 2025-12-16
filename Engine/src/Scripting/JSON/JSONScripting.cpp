#include "JsonScripting.h"
#include "Architecture/Application.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

namespace IonixEngine
{
	JsonScripting* JsonScripting::s_Instance = nullptr;
	JsonScripting& JsonScripting::Get() {
		if (!s_Instance)
			s_Instance = new JsonScripting();
		return *s_Instance;
	}

    namespace {
        using json = nlohmann::json;
        static const char* kLocalSettingsPath = "local_settings.json";

        json LoadJsonFile(const char* path)
        {
            json data = json::object();
            std::ifstream in(path);
            if (!in.is_open()) return data;
            try { in >> data; }
            catch (...) { return json::object(); }
            return data;
        }

        void SaveJsonFile(const char* path, const json& data)
        {
            std::ofstream out(path);
            out << data.dump(4);
        }

        std::vector<std::string> SplitDotPath(const std::string& path)
        {
            std::vector<std::string> out;
            std::stringstream ss(path);
            std::string item;
            while (std::getline(ss, item, '.'))
                if (!item.empty()) out.push_back(item);
            return out;
        }

        json SolToJson(const sol::object& v)
        {
            if (!v.valid() || v == sol::nil) return nullptr;
            if (v.is<bool>()) return v.as<bool>();
            if (v.get_type() == sol::type::number)
            {
                double d = v.as<double>();

                // Round to 2 decimal places
                d = std::round(d * 100.0) / 100.0;

                // Keep integers as integers (1.0 -> 1)
                if (std::floor(d) == d) return (int64_t)d;

                return d;
            }
            if (v.is<std::string>()) return v.as<std::string>();

            if (v.is<sol::table>()) {
                sol::table t = v.as<sol::table>();

                // treat {1=...,2=...,...} as array; otherwise object
                bool isArray = true;
                int maxIndex = 0;
                for (auto& kv : t) {
                    sol::object k = kv.first;
                    if (!k.is<int>()) { isArray = false; break; }
                    int idx = k.as<int>();
                    if (idx < 1) { isArray = false; break; }
                    if (idx > maxIndex) maxIndex = idx;
                }

                if (isArray) {
                    json arr = json::array();
                    for (int i = 1; i <= maxIndex; ++i)
                        arr.push_back(SolToJson(t[i]));
                    return arr;
                }
                else {
                    json obj = json::object();
                    for (auto& kv : t) {
                        sol::object k = kv.first;
                        if (k.is<std::string>())
                            obj[k.as<std::string>()] = SolToJson(kv.second);
                    }
                    return obj;
                }
            }

            return nullptr; // unsupported types
        }

        sol::object JsonToSol(sol::state_view lua, const json& j)
        {
            if (j.is_null()) return sol::make_object(lua, sol::nil);
            if (j.is_boolean()) return sol::make_object(lua, j.get<bool>());
            if (j.is_number_integer()) return sol::make_object(lua, j.get<int>());
            if (j.is_number_float()) return sol::make_object(lua, j.get<double>());
            if (j.is_string()) return sol::make_object(lua, j.get<std::string>());

            if (j.is_array()) {
                sol::table t = lua.create_table();
                int i = 1;
                for (const auto& it : j) t[i++] = JsonToSol(lua, it);
                return sol::make_object(lua, t);
            }

            if (j.is_object()) {
                sol::table t = lua.create_table();
                for (auto it = j.begin(); it != j.end(); ++it)
                    t[it.key()] = JsonToSol(lua, it.value());
                return sol::make_object(lua, t);
            }

            return sol::make_object(lua, sol::nil);
        }
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

        auto saveSetting = [](const std::string& gameId, const std::string& keyPath, sol::object value)
            {
                json data = LoadJsonFile(kLocalSettingsPath);

                auto tokens = SplitDotPath(keyPath);
                if (tokens.empty()) return;

                json& root = data["settings"][gameId];
                if (!root.is_object()) root = json::object();

                json* node = &root;
                for (size_t i = 0; i + 1 < tokens.size(); ++i) {
                    json& next = (*node)[tokens[i]];
                    if (!next.is_object()) next = json::object();
                    node = &next;
                }

                (*node)[tokens.back()] = SolToJson(value);
                SaveJsonFile(kLocalSettingsPath, data);
            };

        auto loadSetting = [](sol::this_state s, const std::string& gameId, const std::string& keyPath, sol::object defaultValue) -> sol::object
            {
                sol::state_view luaState(s);

                json data = LoadJsonFile(kLocalSettingsPath);
                if (!data.contains("settings") || !data["settings"].contains(gameId))
                    return defaultValue;

                const json* node = &data["settings"][gameId];

                auto tokens = SplitDotPath(keyPath);
                for (const auto& tok : tokens) {
                    if (!node->is_object() || !node->contains(tok))
                        return defaultValue;
                    node = &(*node)[tok];
                }

                return JsonToSol(luaState, *node);
            };


        lua["Json"] = lua.create_table_with(
            "save_high_score", saveHighScore,
            "load_high_score", loadHighScore,

            "save_player_name", savePlayerName,
            "load_player_name", loadPlayerName,

            "save_setting", saveSetting,
            "load_setting", loadSetting
		);
	}
}