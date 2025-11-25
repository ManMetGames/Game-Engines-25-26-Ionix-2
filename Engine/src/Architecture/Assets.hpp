#include <map>
#include <cstdint>
#include <string>

struct Textures {
    uint64_t Background = 13067482129777558234u;
    uint64_t Debug = 16105671890965812228u;
    uint64_t FlappyBird = 12156800037729862389u;
    uint64_t FlappyBirdT = 2875425512902588134u;
    uint64_t Sand = 4972914212072997432u;
    uint64_t Test = 2449874712956322390u;
    uint64_t ball = 17707837483333796221u;
    uint64_t ball2 = 14716692011604621444u;
    uint64_t horse_fat = 14957662674859995436u;

    inline std::map<std::string, std::string> GetTextures() {
        std::map<std::string, std::string> map;
        map["Background"] = "./Assets/Background.png";
        map["Debug"] = "./Assets/Debug.png";
        map["FlappyBird"] = "./Assets/FlappyBird.png";
        map["FlappyBirdT"] = "./Assets/FlappyBirdT.png";
        map["Sand"] = "./Assets/Sand.png";
        map["Test"] = "./Assets/Test.png";
        map["ball"] = "./Assets/ball.png";
        map["ball2"] = "./Assets/ball2.png";
        map["horse_fat"] = "./Assets/horse-fat.gif";
        return map;
    };
};

struct Sounds {
    uint64_t test = 7268973944201526851u;
    uint64_t test0 = 17827761153089726170u;

    inline std::map<std::string, std::string> GetSounds() {
        std::map<std::string, std::string> map;
        map["test"] = "./Assets/test.mp3";
        map["test0"] = "./Assets/test.wav";
        return map;
    };
};

struct Fonts {
    uint64_t OpenSans_Bold = 15907694087263520715u;
    uint64_t OpenSans_BoldItalic = 8563675908245164909u;
    uint64_t OpenSans_Condensed_Bold = 359187643146943972u;
    uint64_t OpenSans_Condensed_BoldItalic = 14849289699928080642u;
    uint64_t OpenSans_Condensed_ExtraBold = 6947855732792450946u;
    uint64_t OpenSans_Condensed_ExtraBoldItalic = 3486891722713028513u;
    uint64_t OpenSans_Condensed_Italic = 4342085688838513660u;
    uint64_t OpenSans_Condensed_Light = 2217696825746649946u;
    uint64_t OpenSans_Condensed_LightItalic = 6175883351937481375u;
    uint64_t OpenSans_Condensed_Medium = 7158168093019054927u;
    uint64_t OpenSans_Condensed_MediumItalic = 8626495147143287104u;
    uint64_t OpenSans_Condensed_Regular = 6994305085030512905u;
    uint64_t OpenSans_Condensed_SemiBold = 8803190280521243502u;
    uint64_t OpenSans_Condensed_SemiBoldItalic = 14588341052304457896u;
    uint64_t OpenSans_ExtraBold = 12227810788226587876u;
    uint64_t OpenSans_ExtraBoldItalic = 8869746834823773578u;
    uint64_t OpenSans_Italic = 16632617568536746513u;
    uint64_t OpenSans_Italic_VariableFont_wdth_wght = 6606613461309615998u;
    uint64_t OpenSans_Light = 168946910481453691u;
    uint64_t OpenSans_LightItalic = 14816344761813584664u;
    uint64_t OpenSans_Medium = 3787703417603136849u;
    uint64_t OpenSans_MediumItalic = 772028055242458903u;
    uint64_t OpenSans_Regular = 7933080968301474893u;
    uint64_t OpenSans_SemiBold = 5098837941272300246u;
    uint64_t OpenSans_SemiBoldItalic = 16659307743882634385u;
    uint64_t OpenSans_SemiCondensed_Bold = 16787162754279270096u;
    uint64_t OpenSans_SemiCondensed_BoldItalic = 7342872189256318675u;
    uint64_t OpenSans_SemiCondensed_ExtraBold = 1439675224711620604u;
    uint64_t OpenSans_SemiCondensed_ExtraBoldItalic = 7137085836259179529u;
    uint64_t OpenSans_SemiCondensed_Italic = 8160819281478521801u;
    uint64_t OpenSans_SemiCondensed_Light = 5791485800500642700u;
    uint64_t OpenSans_SemiCondensed_LightItalic = 14233983156265692247u;
    uint64_t OpenSans_SemiCondensed_Medium = 7676232349246516700u;
    uint64_t OpenSans_SemiCondensed_MediumItalic = 16103702957349022117u;
    uint64_t OpenSans_SemiCondensed_Regular = 1448807020571299991u;
    uint64_t OpenSans_SemiCondensed_SemiBold = 6111457202692262930u;
    uint64_t OpenSans_SemiCondensed_SemiBoldItalic = 126995760760299497u;
    uint64_t OpenSans_VariableFont = 1462764909749496224u;

    inline std::map<std::string, std::string> GetFonts() {
        std::map<std::string, std::string> map;
        map["OpenSans_Bold"] = "./Assets/Open_Sans/static/OpenSans-Bold.ttf";
        map["OpenSans_BoldItalic"] = "./Assets/Open_Sans/static/OpenSans-BoldItalic.ttf";
        map["OpenSans_Condensed_Bold"] = "./Assets/Open_Sans/static/OpenSans_Condensed-Bold.ttf";
        map["OpenSans_Condensed_BoldItalic"] = "./Assets/Open_Sans/static/OpenSans_Condensed-BoldItalic.ttf";
        map["OpenSans_Condensed_ExtraBold"] = "./Assets/Open_Sans/static/OpenSans_Condensed-ExtraBold.ttf";
        map["OpenSans_Condensed_ExtraBoldItalic"] = "./Assets/Open_Sans/static/OpenSans_Condensed-ExtraBoldItalic.ttf";
        map["OpenSans_Condensed_Italic"] = "./Assets/Open_Sans/static/OpenSans_Condensed-Italic.ttf";
        map["OpenSans_Condensed_Light"] = "./Assets/Open_Sans/static/OpenSans_Condensed-Light.ttf";
        map["OpenSans_Condensed_LightItalic"] = "./Assets/Open_Sans/static/OpenSans_Condensed-LightItalic.ttf";
        map["OpenSans_Condensed_Medium"] = "./Assets/Open_Sans/static/OpenSans_Condensed-Medium.ttf";
        map["OpenSans_Condensed_MediumItalic"] = "./Assets/Open_Sans/static/OpenSans_Condensed-MediumItalic.ttf";
        map["OpenSans_Condensed_Regular"] = "./Assets/Open_Sans/static/OpenSans_Condensed-Regular.ttf";
        map["OpenSans_Condensed_SemiBold"] = "./Assets/Open_Sans/static/OpenSans_Condensed-SemiBold.ttf";
        map["OpenSans_Condensed_SemiBoldItalic"] = "./Assets/Open_Sans/static/OpenSans_Condensed-SemiBoldItalic.ttf";
        map["OpenSans_ExtraBold"] = "./Assets/Open_Sans/static/OpenSans-ExtraBold.ttf";
        map["OpenSans_ExtraBoldItalic"] = "./Assets/Open_Sans/static/OpenSans-ExtraBoldItalic.ttf";
        map["OpenSans_Italic"] = "./Assets/Open_Sans/static/OpenSans-Italic.ttf";
        map["OpenSans_Italic_VariableFont_wdth_wght"] = "./Assets/Open_Sans/OpenSans-Italic-VariableFont_wdth,wght.ttf";
        map["OpenSans_Light"] = "./Assets/Open_Sans/static/OpenSans-Light.ttf";
        map["OpenSans_LightItalic"] = "./Assets/Open_Sans/static/OpenSans-LightItalic.ttf";
        map["OpenSans_Medium"] = "./Assets/Open_Sans/static/OpenSans-Medium.ttf";
        map["OpenSans_MediumItalic"] = "./Assets/Open_Sans/static/OpenSans-MediumItalic.ttf";
        map["OpenSans_Regular"] = "./Assets/Open_Sans/static/OpenSans-Regular.ttf";
        map["OpenSans_SemiBold"] = "./Assets/Open_Sans/static/OpenSans-SemiBold.ttf";
        map["OpenSans_SemiBoldItalic"] = "./Assets/Open_Sans/static/OpenSans-SemiBoldItalic.ttf";
        map["OpenSans_SemiCondensed_Bold"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-Bold.ttf";
        map["OpenSans_SemiCondensed_BoldItalic"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-BoldItalic.ttf";
        map["OpenSans_SemiCondensed_ExtraBold"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-ExtraBold.ttf";
        map["OpenSans_SemiCondensed_ExtraBoldItalic"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-ExtraBoldItalic.ttf";
        map["OpenSans_SemiCondensed_Italic"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-Italic.ttf";
        map["OpenSans_SemiCondensed_Light"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-Light.ttf";
        map["OpenSans_SemiCondensed_LightItalic"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-LightItalic.ttf";
        map["OpenSans_SemiCondensed_Medium"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-Medium.ttf";
        map["OpenSans_SemiCondensed_MediumItalic"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-MediumItalic.ttf";
        map["OpenSans_SemiCondensed_Regular"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-Regular.ttf";
        map["OpenSans_SemiCondensed_SemiBold"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-SemiBold.ttf";
        map["OpenSans_SemiCondensed_SemiBoldItalic"] = "./Assets/Open_Sans/static/OpenSans_SemiCondensed-SemiBoldItalic.ttf";
        map["OpenSans_VariableFont"] = "./Assets/Open_Sans/OpenSans-VariableFont.ttf";
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

