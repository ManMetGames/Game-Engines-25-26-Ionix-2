#include <map>
#include <cstdint>
#include <string>

struct Textures {
    uint32_t Apple = 3562895354u;
    uint32_t Background = 2667361274u;
    uint32_t Coin = 2929616156u;
    uint32_t Debug = 2430642766u;
    uint32_t Flappy = 3750051064u;
    uint32_t FlappyBird = 2136167952u;
    uint32_t FlappyPipe = 1290161702u;
    uint32_t FlappyPipe2 = 2484853856u;
    uint32_t PimBall = 3518613816u;
    uint32_t Sand = 2995081222u;
    uint32_t Test = 510180426u;
    uint32_t ball = 369994598u;
    uint32_t ball2 = 1354646532u;
    uint32_t horse_fat = 963803500u;
    uint32_t office = 2084752704u;
    uint32_t player1 = 2740174956u;

    inline std::map<std::string, std::string> GetTextures() {
        std::map<std::string, std::string> map;
        map["Apple"] = "./Assets/Apple.png";
        map["Background"] = "./Assets/Background.png";
        map["Coin"] = "./Assets/Coin.png";
        map["Debug"] = "./Assets/Debug.png";
        map["Flappy"] = "./Assets/Flappy.png";
        map["FlappyBird"] = "./Assets/FlappyBird.png";
        map["FlappyPipe"] = "./Assets/FlappyPipe.png";
        map["FlappyPipe2"] = "./Assets/FlappyPipe2.png";
        map["PimBall"] = "./Assets/PimBall.png";
        map["Sand"] = "./Assets/Sand.png";
        map["Test"] = "./Assets/Test.png";
        map["ball"] = "./Assets/ball.png";
        map["ball2"] = "./Assets/ball2.png";
        map["horse_fat"] = "./Assets/horse-fat.gif";
        map["office"] = "./Assets/office.png";
        map["player1"] = "./Assets/player1.png";
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
    uint32_t OpenSans_Bold = 3125297026u;
    uint32_t OpenSans_BoldItalic = 3887720094u;
    uint32_t OpenSans_Condensed_Bold = 1962755146u;
    uint32_t OpenSans_Condensed_BoldItalic = 124114000u;
    uint32_t OpenSans_Condensed_ExtraBold = 236531092u;
    uint32_t OpenSans_Condensed_ExtraBoldItalic = 2316031258u;
    uint32_t OpenSans_Condensed_Italic = 1923490440u;
    uint32_t OpenSans_Condensed_Light = 3002729520u;
    uint32_t OpenSans_Condensed_LightItalic = 61440594u;
    uint32_t OpenSans_Condensed_Medium = 1797819644u;
    uint32_t OpenSans_Condensed_MediumItalic = 3012013420u;
    uint32_t OpenSans_Condensed_Regular = 790134158u;
    uint32_t OpenSans_Condensed_SemiBold = 736574964u;
    uint32_t OpenSans_Condensed_SemiBoldItalic = 3639818310u;
    uint32_t OpenSans_ExtraBold = 1467255458u;
    uint32_t OpenSans_ExtraBoldItalic = 2017324020u;
    uint32_t OpenSans_Italic = 3932750124u;
    uint32_t OpenSans_Italic_VariableFont_wdth_wght = 2366781264u;
    uint32_t OpenSans_Light = 2697239780u;
    uint32_t OpenSans_LightItalic = 1393674390u;
    uint32_t OpenSans_Medium = 2195151848u;
    uint32_t OpenSans_MediumItalic = 284701716u;
    uint32_t OpenSans_Regular = 2328541490u;
    uint32_t OpenSans_SemiBold = 3725012926u;
    uint32_t OpenSans_SemiBoldItalic = 712649994u;
    uint32_t OpenSans_SemiCondensed_Bold = 1327673022u;
    uint32_t OpenSans_SemiCondensed_BoldItalic = 3604582006u;
    uint32_t OpenSans_SemiCondensed_ExtraBold = 2981573478u;
    uint32_t OpenSans_SemiCondensed_ExtraBoldItalic = 175442784u;
    uint32_t OpenSans_SemiCondensed_Italic = 1498836336u;
    uint32_t OpenSans_SemiCondensed_Light = 1036914262u;
    uint32_t OpenSans_SemiCondensed_LightItalic = 864852258u;
    uint32_t OpenSans_SemiCondensed_Medium = 4178764238u;
    uint32_t OpenSans_SemiCondensed_MediumItalic = 4263220556u;
    uint32_t OpenSans_SemiCondensed_Regular = 727786730u;
    uint32_t OpenSans_SemiCondensed_SemiBold = 2564416698u;
    uint32_t OpenSans_SemiCondensed_SemiBoldItalic = 3060309024u;
    uint32_t OpenSans_VariableFont = 136494272u;

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

