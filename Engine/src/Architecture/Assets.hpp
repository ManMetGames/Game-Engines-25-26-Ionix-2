#include <map>
#include <cstdint>
#include <string>

struct Textures {
    uint32_t Debug = 2430642766u;

    inline std::map<std::string, std::string> GetTextures() {
        std::map<std::string, std::string> map;
        map["Debug"] = "./Assets/Open_Sans/Debug.png";
        return map;
    };
};

struct Sounds {

    inline std::map<std::string, std::string> GetSounds() {
        std::map<std::string, std::string> map;
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

