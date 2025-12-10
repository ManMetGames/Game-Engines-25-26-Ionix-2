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

    void FirebaseLeaderboard::SubmitScore(const std::string& name, int score) {
        if (!g_db) {
            std::cout << "Database not initialized!\n";
            return;
        }

        // Gets a reference to your leaderboard path.
        auto leaderboard_ref = g_db->GetReference("leaderboard").PushChild();

        // Creates a map to holds the name and score.
        std::map<std::string, firebase::Variant> entry_data;
        entry_data["name"] = firebase::Variant(name); 
        entry_data["score"] = firebase::Variant(score); 

        firebase::Future<void> result = leaderboard_ref.SetValue(entry_data);

        while (result.status() == firebase::kFutureStatusPending) {}

        if (result.error() == firebase::database::kErrorNone) {
            std::cout << "Leaderboard entry succeeded! Name: " << name << ", Score: " << score << "\n";
        }
        else {
            std::cout << "Leaderboard entry failed: " << result.error_message() << "\n";
        }
    }

    void FirebaseLeaderboard::RetrieveTopScores(int count) {
        if (!g_db) {
            std::cout << "Database not initialized!\n";
            return;
        }

        firebase::database::DatabaseReference leaderboard_ref = g_db->GetReference("leaderboard");
        firebase::database::Query query = leaderboard_ref.OrderByChild("score").LimitToLast(count);
        firebase::Future<firebase::database::DataSnapshot> result = query.GetValue();

        std::cout << "Retrieving top " << count << " scores...\n";

        while (result.status() == firebase::kFutureStatusPending) {}

        if (result.error() == firebase::database::kErrorNone) {
            std::cout << "Successfully retrieved scores!\n";
            const firebase::database::DataSnapshot& snapshot = *result.result();

            std::vector<LeaderboardEntry> top_scores;

            if (snapshot.children_count() > 0) {
                std::cout << "--- Leaderboard Top " << count << " ---\n";
                for (const auto& child_snapshot : snapshot.children()) {
                    if (child_snapshot.HasChild("name") && child_snapshot.HasChild("score")) {
                        firebase::Variant nameVar = child_snapshot.Child("name").value();
                        firebase::Variant scoreVar = child_snapshot.Child("score").value();
                        std::string name = nameVar.string_value();
                        int score = (int)scoreVar.int64_value();
                        top_scores.push_back({ name, score });
                    }
                }

                std::sort(top_scores.begin(), top_scores.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
                    return a.score > b.score;
                    });

                for (size_t i = 0; i < top_scores.size(); ++i) {
                    std::cout << (i + 1) << ". Name: " << top_scores[i].name << ", Score: " << top_scores[i].score << "\n";
                }
                std::cout << "-----------------------\n";
            }
            else {
                std::cout << "No scores found in the leaderboard.\n";
            }

        }
        else {
            std::cout << "Failed to retrieve scores: " << result.error_message() << "\n";
        }
    }
}