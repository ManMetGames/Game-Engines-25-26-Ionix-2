#include <iostream>
#include "json.hpp"
#include <fstream>

namespace IonixEngine
{
    using json = nlohmann::json;

    void SaveHighScore(int score) {
        json data;
        data["highscore"] = score;

        std::ofstream file("flappyBirdHighScore.json");
        file << data.dump(4); // pretty print with indent 4
    }

    int LoadHighScore() {
        std::ifstream file("flappyBirdHighScore.json");
        if (!file.is_open())
            return 0; // default if no file exists

        json data;
        file >> data;

        return data.value("highscore", 0);
    }
}
