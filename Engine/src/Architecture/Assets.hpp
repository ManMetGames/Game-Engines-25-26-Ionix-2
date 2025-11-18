#include <map>
#include <cstdint>
#include <string>

struct Textures {
    uint64_t BetterLetters = 2464976856345177249u;
    uint64_t Debug = 13718016529779414311u;
    uint64_t Letters = 16351359864206833473u;
    uint64_t Test = 6128689281041194533u;
    uint64_t ball = 5225819788789863859u;
    uint64_t ball2 = 8540420223266660866u;
    uint64_t horse_fat = 1265999279503785142u;

    inline std::map<std::string, std::string> GetTextures() {
        std::map<std::string, std::string> map;
        map["BetterLetters"] = "./Assets/BetterLetters.png";
        map["Debug"] = "./Assets/Debug.png";
        map["Letters"] = "./Assets/Letters.png";
        map["Test"] = "./Assets/Test.png";
        map["ball"] = "./Assets/ball.png";
        map["ball2"] = "./Assets/ball2.png";
        map["horse_fat"] = "./Assets/horse-fat.gif";
        return map;
    };
};

struct Sounds {
    uint64_t test = 15086333167179663880u;
    uint64_t test0 = 16286206250710619806u;

    inline std::map<std::string, std::string> GetSounds() {
        std::map<std::string, std::string> map;
        map["test"] = "./Assets/test.mp3";
        map["test0"] = "./Assets/test.wav";
        return map;
    };
};

struct Fonts {
    uint64_t OpenSans_Bold = 18218641110960709569u;
    uint64_t OpenSans_BoldItalic = 13024091119898391375u;
    uint64_t OpenSans_Condensed_Bold = 14919764598910263845u;
    uint64_t OpenSans_Condensed_BoldItalic = 33898620003740200u;
    uint64_t OpenSans_Condensed_ExtraBold = 6157712735007971018u;
    uint64_t OpenSans_Condensed_ExtraBoldItalic = 11276515552535112333u;
    uint64_t OpenSans_Condensed_Italic = 10186543856528593220u;
    uint64_t OpenSans_Condensed_Light = 15012900026793656856u;
    uint64_t OpenSans_Condensed_LightItalic = 1421376522066116905u;
    uint64_t OpenSans_Condensed_Medium = 13852931639892788862u;
    uint64_t OpenSans_Condensed_MediumItalic = 2361335609534174902u;
    uint64_t OpenSans_Condensed_Regular = 7494130786263383751u;
    uint64_t OpenSans_Condensed_SemiBold = 2673907469032595194u;
    uint64_t OpenSans_Condensed_SemiBoldItalic = 16055796091777031203u;
    uint64_t OpenSans_ExtraBold = 2099464000956417361u;
    uint64_t OpenSans_ExtraBoldItalic = 10894008372049147386u;
    uint64_t OpenSans_Italic = 14482838294132719766u;
    uint64_t OpenSans_Italic_VariableFont_wdth_wght = 7675336041385499560u;
    uint64_t OpenSans_Light = 192434351041039986u;
    uint64_t OpenSans_LightItalic = 2939390856899650635u;
    uint64_t OpenSans_Medium = 17039498864906448372u;
    uint64_t OpenSans_MediumItalic = 14852958965204785674u;
    uint64_t OpenSans_Regular = 2544399497738673305u;
    uint64_t OpenSans_SemiBold = 6854180939799697375u;
    uint64_t OpenSans_SemiBoldItalic = 1799124700723222149u;
    uint64_t OpenSans_SemiCondensed_Bold = 13858384759917467487u;
    uint64_t OpenSans_SemiCondensed_BoldItalic = 14686690485652473659u;
    uint64_t OpenSans_SemiCondensed_ExtraBold = 10699801352396839347u;
    uint64_t OpenSans_SemiCondensed_ExtraBoldItalic = 2340613249906148784u;
    uint64_t OpenSans_SemiCondensed_Italic = 8879318348008732344u;
    uint64_t OpenSans_SemiCondensed_Light = 14649876522230810411u;
    uint64_t OpenSans_SemiCondensed_LightItalic = 15995267612355742865u;
    uint64_t OpenSans_SemiCondensed_Medium = 13427812767353106663u;
    uint64_t OpenSans_SemiCondensed_MediumItalic = 17634547107124595366u;
    uint64_t OpenSans_SemiCondensed_Regular = 9377779586138411637u;
    uint64_t OpenSans_SemiCondensed_SemiBold = 4163883439414375005u;
    uint64_t OpenSans_SemiCondensed_SemiBoldItalic = 1727944077903941136u;
    uint64_t OpenSans_VariableFont = 12954013778786213472u;

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

