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

        // 1. Get a reference to your leaderboard path.
        // For example, "leaderboard/scores" or just "leaderboard".
        // Each entry can be pushed with a unique ID to form a list.
        // If you want to automatically generate a unique key for each entry, use PushChild().
        auto leaderboard_ref = g_db->GetReference("leaderboard").PushChild();

        // 2. Create a map to hold the name and score.
        // firebase::Variant is used to represent different data types in Firebase.
        std::map<std::string, firebase::Variant> entry_data;
        entry_data["name"] = firebase::Variant(name); // Convert string to Variant
        entry_data["score"] = firebase::Variant(score); // Convert int to Variant

        // 3. Set the map as the value for the new entry.
        firebase::Future<void> result = leaderboard_ref.SetValue(entry_data);

        // 4. Wait for the operation to complete and check the result.
        // For games, polling the status in a game loop is a common approach.
        while (result.status() == firebase::kFutureStatusPending) {
            // In a real game, you would yield control or process other game logic here
            // to avoid blocking the main thread.
        }

        if (result.error() == firebase::database::kErrorNone) {
            std::cout << "Leaderboard entry succeeded! Name: " << name << ", Score: " << score << "\n";
        }
        else {
            std::cout << "Leaderboard entry failed: " << result.error_message() << "\n";
        }
    }

    // Example usage (you would call this from your game logic)
    // int main() {
    //     // Assume g_db is initialized here
    //     WriteLeaderboardEntry("PlayerOne", 1500);
    //     WriteLeaderboardEntry("PlayerTwo", 1200);
    //     return 0;
    // }
    /*
    void FirebaseLeaderboard::TestWrite(int score) {
        if (!g_db) {
            std::cout << "Database not initialized!\n";
            return;
        }

        // Write score to path: /test_score
        auto ref = g_db->GetReference("test_score");

        firebase::Future<void> result = ref.SetValue(score);

        // Simple wait loop (rookie-friendly)
        while (result.status() == firebase::kFutureStatusPending) {}

        if (result.error() == 0)
            std::cout << "Write succeeded! Score uploaded: " << score << "\n";
        else
            std::cout << "Write failed: " << result.error_message() << "\n";
    }
    */
}