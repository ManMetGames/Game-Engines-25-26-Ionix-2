#include <map>
#include <cstdint>
#include <string>

struct Textures {
    uint32_t Attack_3 = 1629298326u;
    uint32_t Background = 2667361274u;
    uint32_t COUPON = 2070168214u;
    uint32_t Coin = 2929616156u;
    uint32_t Debug = 2430642766u;
    uint32_t FlappyBird = 2136167952u;
    uint32_t FlappyPipe = 1290161702u;
    uint32_t FlappyPipe2 = 2484853856u;
    uint32_t Hurt = 2152628710u;
    uint32_t Idle = 572691386u;
    uint32_t PimBall = 3518613816u;
    uint32_t Player = 3740353548u;
    uint32_t Sand = 2995081222u;
    uint32_t Test = 510180426u;
    uint32_t Walk = 1497561796u;
    uint32_t ball = 369994598u;
    uint32_t ball2 = 1354646532u;
    uint32_t hermer = 242799782u;
    uint32_t horse_fat = 963803500u;
    uint32_t office = 2084752704u;

    inline std::map<std::string, std::string> GetTextures() {
        std::map<std::string, std::string> map;
        map["Attack_3"] = "./Assets/Attack 3.png";
        map["Background"] = "./Assets/Background.png";
        map["COUPON"] = "./Assets/COUPON.png";
        map["Coin"] = "./Assets/Coin.png";
        map["Debug"] = "./Assets/Debug.png";
        map["FlappyBird"] = "./Assets/FlappyBird.png";
        map["FlappyPipe"] = "./Assets/FlappyPipe.png";
        map["FlappyPipe2"] = "./Assets/FlappyPipe2.png";
        map["Hurt"] = "./Assets/Hurt.png";
        map["Idle"] = "./Assets/Idle.png";
        map["PimBall"] = "./Assets/PimBall.png";
        map["Player"] = "./Assets/Player.png";
        map["Sand"] = "./Assets/Sand.png";
        map["Test"] = "./Assets/Test.png";
        map["Walk"] = "./Assets/Walk.png";
        map["ball"] = "./Assets/ball.png";
        map["ball2"] = "./Assets/ball2.png";
        map["hermer"] = "./Assets/hermer.png";
        map["horse_fat"] = "./Assets/horse-fat.gif";
        map["office"] = "./Assets/office.png";
        return map;
    };
};

struct Sounds {
    uint32_t test = 1642075152u;
    uint32_t test0 = 280276284u;

    inline std::map<std::string, std::string> GetSounds() {
        std::map<std::string, std::string> map;
        map["test"] = "./Assets/test.mp3";
        map["test0"] = "./Assets/test.wav";
        return map;
    };
};

struct Fonts {

    inline std::map<std::string, std::string> GetFonts() {
        std::map<std::string, std::string> map;
        return map;
    };
};

struct Assets {
    Textures textures;
    Sounds sounds;
    Fonts fonts;

    static Assets& Get() {
        static Assets assets;
        return assets;         
    };
};

