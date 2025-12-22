#include "Architecture/Application.h"
#include "firebaseLeaderboard.h"
#include <fstream>

namespace IonixEngine
{
    firebase::App* g_app = nullptr;
    firebase::database::Database* g_db = nullptr;
   
    void FirebaseLeaderboard::Init() {
        firebase::AppOptions options;

        options.set_project_id("flappybird-leaderboard-2489c");
        options.set_app_id("1:18212669222:android:44ceaaa844e1b92a51a3e7");
        options.set_api_key("AIzaSyCyZO-OrwcbnAqMHL57NxCMyAr46iMXTGM");
        options.set_database_url("https://flappybird-leaderboard-2489c-default-rtdb.firebaseio.com/");

        g_app = firebase::App::Create(options);

        if (!g_app) {
            std::cout << "Firebase App creation failed!\n";
            return;
        }
        else {
            std::cout << "Firebase App created!\n";
        }

        g_db = firebase::database::Database::GetInstance(g_app);

        if (!g_db) {
            std::cout << "Firebase Database Init FAILED!\n";
        }
        else {
            std::cout << "Firebase Database initialized successfully!\n";
        }
    }

    static std::string MakeNameKey(std::string name)
    {
        // lowercase = case-insensitive names (simplest)
        for (char& c : name) c = (char)tolower((unsigned char)c);

        // Firebase RTDB keys cannot contain: . # $ [ ] /
        for (char& c : name)
        {
            if (c == '.' || c == '#' || c == '$' || c == '[' || c == ']' || c == '/')
                c = '_';
            if (c == ' ') c = '_';
        }

        if (name.empty()) name = "anon";
        return name;
    }

    void FirebaseLeaderboard::SubmitScore(const std::string& gameId, const std::string& name, int score)
    {
        if (!g_db) {
            std::cout << "Database not initialized!\n";
            return;
        }

        const std::string key = MakeNameKey(name);
        auto path = std::string("leaderboards/") + gameId;
        firebase::database::DatabaseReference player_ref =
            g_db->GetReference(path.c_str()).Child(key.c_str());

        // Transaction: read existing score, only replace if score is higher.
        auto future = player_ref.RunTransaction([name, score](firebase::database::MutableData* data) {
            int64_t oldScore = -1;

            if (data->HasChild("score")) {
                firebase::Variant v = data->Child("score").value();
                if (v.is_int64()) oldScore = v.int64_value();
                else if (v.is_double()) oldScore = (int64_t)v.double_value();
            }

            // If existing score is already >= new score, do nothing.
            if (oldScore >= (int64_t)score) {
                return firebase::database::kTransactionResultAbort;
            }

            // Write whole entry at /leaderboard/<key>
            std::map<std::string, firebase::Variant> entry;
            entry["name"] = firebase::Variant(name);
            entry["score"] = firebase::Variant((int64_t)score);

            data->set_value(firebase::Variant(entry));
            return firebase::database::kTransactionResultSuccess;
            });

        while (future.status() == firebase::kFutureStatusPending) {}

        if (future.error() == firebase::database::kErrorNone) {
            std::cout << "SubmitScore done for '" << name << "' with " << score << "\n";
        }
        else {
            std::cout << "SubmitScore failed: " << future.error_message() << "\n";
        }
    }


    std::vector<FirebaseLeaderboard::LeaderboardEntry>
        FirebaseLeaderboard::GetTopScores(const std::string& gameId, int count)
    {
        std::vector<LeaderboardEntry> top_scores;

        if (!g_db) return top_scores;

        auto path = std::string("leaderboards/") + gameId;
        auto leaderboard_ref = g_db->GetReference(path.c_str());
        auto query = leaderboard_ref.OrderByChild("score").LimitToLast(count);
        auto result = query.GetValue();

        while (result.status() == firebase::kFutureStatusPending) {}

        if (result.error() != firebase::database::kErrorNone)
            return top_scores;

        const auto& snapshot = *result.result();

        for (const auto& child : snapshot.children())
        {
            if (!child.HasChild("name") || !child.HasChild("score")) continue;

            firebase::Variant nameVar = child.Child("name").value();
            firebase::Variant scoreVar = child.Child("score").value();

            top_scores.push_back({ nameVar.string_value(), (int)scoreVar.int64_value() });
        }

        std::sort(top_scores.begin(), top_scores.end(),
            [](const LeaderboardEntry& a, const LeaderboardEntry& b) { return a.score > b.score; });

        return top_scores;
    }
}