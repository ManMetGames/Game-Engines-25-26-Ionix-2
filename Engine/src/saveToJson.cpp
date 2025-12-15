#include <iostream>
#include <fstream>
#include "saveToJson.h"

namespace IonixEngine
{

    void SaveFlappyBirdHighScore(int score) {
        json data;
        data["highscore"] = score;

        std::ofstream file("flappyBirdHighScore.json");
        file << data.dump(4); // pretty print with indent 4
    }

    int LoadFlappyBirdHighScore() {
        std::ifstream file("flappyBirdHighScore.json");
        if (!file.is_open())
            return 0; // default if no file exists

        json data;
        file >> data;

        return data.value("highscore", 0);
    }
}
