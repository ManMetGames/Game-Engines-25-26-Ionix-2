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

    void FirebaseLeaderboard::TestWrite(int score) {
        if (!g_db) {
            std::cout << "Database not initialized!\n";
            return;
        }

        // Write score to path: /test_score
        auto ref = g_db->GetReference("test_score");

        firebase::Future<void> result = ref.SetValue(score);

        // Simple wait loop
        while (result.status() == firebase::kFutureStatusPending) {}

        if (result.error() == 0)
            std::cout << "Write succeeded! Score uploaded: " << score << "\n";
        else
            std::cout << "Write failed: " << result.error_message() << "\n";
    }
}