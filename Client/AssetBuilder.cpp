#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>

// Header only sha256 implementation: https://github.com/okdshin/PicoSHA2
#include "picosha2.hpp"

using std::ofstream;

void add_entry(std::map<std::string, std::string> &map, const std::filesystem::path &path);

void replace_all(std::string& str, const std::string& pattern, const std::string& to);

std::string get_struct(const std::string& name, std::map<std::string, std::string> map);

std::string get_asset_struct(const std::string& textures, const std::string& sounds, const std::string& fonts);

std::string get_lua_table(std::map<std::string, std::string>& textures, std::map<std::string, std::string>& sounds, std::map<std::string, std::string>& fonts);

uint32_t get_32_bit_hash(const std::string& str);

int main(int argc, char* argv[]) {
    std::map<std::string, std::string> textures;
    std::map<std::string, std::string> sounds;
    std::map<std::string, std::string> fonts;

    for (auto& entry : std::filesystem::recursive_directory_iterator("./Assets")) {
        std::string extension = entry.path().extension().string();
        if (extension == ".png" || extension == ".jpg" || extension == ".gif") {
            add_entry(textures, entry.path());
        } else if (extension == ".wav" || extension == ".mp3") {
            add_entry(sounds, entry.path());
        } else if (extension == ".otf" || extension == ".ttf") {
            add_entry(fonts, entry.path());
        }
    }

    std::string textures_definition = get_struct("Textures", textures);
    std::string sounds_definition = get_struct("Sounds", sounds);
    std::string fonts_definition = get_struct("Fonts", fonts);
    std::string assets_definition = get_asset_struct("Textures", "Sounds", "Fonts");

    std::stringstream assets_hpp;
    assets_hpp << "#include <map>\n";
    assets_hpp << "#include <cstdint>\n";
    assets_hpp << "#include <string>\n";
    assets_hpp << "\n";
    assets_hpp << textures_definition.c_str();
    assets_hpp << "\n";
    assets_hpp << sounds_definition.c_str();
    assets_hpp << "\n";
    assets_hpp << fonts_definition.c_str();
    assets_hpp << "\n";
    assets_hpp << assets_definition.c_str();
    assets_hpp << "\n";

    ofstream assets_header_file = std::ofstream("../Engine/src/Architecture/Assets.hpp", std::ios::trunc);
    assets_header_file << assets_hpp.str();
    assets_header_file.close();
    std::cout << "Successfully wrote new assets header\n";

    ofstream assets_lua_file = std::ofstream("./Scripts/Assets.lua", std::ios::trunc);
    assets_lua_file << get_lua_table(textures, sounds, fonts);
    assets_lua_file.close();
    std::cout << "Successfully wrote new lua file\n";
}

std::string get_asset_struct(const std::string& textures, const std::string& sounds, const std::string& fonts) {
    std::stringstream struct_definition;

    struct_definition << "struct Assets {\n";

        std::string textures_lower = std::string(textures);
        std::string sounds_lower   = std::string(sounds);
        std::string fonts_lower    = std::string(fonts);

        std::transform(textures_lower.begin(), textures_lower.end(), textures_lower.begin(), [](unsigned char c){ return std::tolower(c); });
        std::transform(sounds_lower.begin(), sounds_lower.end(), sounds_lower.begin(), [](unsigned char c){ return std::tolower(c); });
        std::transform(fonts_lower.begin(), fonts_lower.end(), fonts_lower.begin(), [](unsigned char c){ return std::tolower(c); });

        struct_definition << "    " << textures << " " << textures_lower << ";\n";
        struct_definition << "    " << sounds << " " << sounds_lower << ";\n";
        struct_definition << "    " << fonts << " " << fonts_lower << ";\n\n";
        struct_definition << "    static Assets& Get() {\n        static Assets assets;\n        return assets;         \n    };\n";

    struct_definition << "};\n";

    return struct_definition.str();
}

std::string get_struct(const std::string& name, std::map<std::string, std::string> map) {
    std::stringstream struct_definition;

    int indent = 0;
    struct_definition << "struct " << name << " {\n";
    indent++;
    for (const std::pair<std::string, std::string> pair : map) {
        for (int i = 0; i < indent; i++) { struct_definition << "    "; }
        struct_definition << "uint32_t " << pair.first.c_str() << " = " << get_32_bit_hash(pair.first) << "u;\n";
    }

    struct_definition << "\n";

    for (int i = 0; i < indent; i++) { struct_definition << "    "; }
    struct_definition << "inline std::map<std::string, std::string> Get" << name << "() {\n";
    indent++;
    for (int i = 0; i < indent; i++) { struct_definition << "    "; }
    struct_definition << "std::map<std::string, std::string> map;\n";
    for (const std::pair<std::string, std::filesystem::path>& pair : map) {
        for (int i = 0; i < indent; i++) { struct_definition << "    "; }
        struct_definition << "map[\"" << pair.first << "\"] = " << pair.second << ";\n";
    }
    for (int i = 0; i < indent; i++) { struct_definition << "    "; }
    struct_definition << "return map;\n";
    indent--;
    for (int i = 0; i < indent; i++) { struct_definition << "    "; }
    struct_definition << "};\n";
    struct_definition << "};\n";
    return struct_definition.str();
}

std::string get_lua_table(std::map<std::string, std::string>& textures, std::map<std::string, std::string>& sounds, std::map<std::string, std::string>& fonts) {
    std::stringstream table;

    table << "return {\n";
        table << "    " << "textures = {\n";
        for (const std::pair<std::string, std::string>& pair : textures) {
            table << "        " << pair.first.c_str() << " = " << get_32_bit_hash(pair.first) << ",\n";
        }
        table << "    " << "},\n";
        table << "    " << "sounds = {\n";
        for (const std::pair<std::string, std::string>& pair : sounds) {
            table << "        " << pair.first.c_str() << " = " << get_32_bit_hash(pair.first) << ",\n";
        }
        table << "    " << "},\n";
        table << "    " << "fonts = {\n";
        for (const std::pair<std::string, std::string>& pair : fonts) {
            table << "        " << pair.first.c_str() << " = " << get_32_bit_hash(pair.first) << ",\n";
        }
        table << "    " << "},\n";
    table << "}\n";

    return table.str();
}

void add_entry(std::map<std::string, std::string>& map, const std::filesystem::path& path) {
    std::string entry = path.stem().string();
    replace_all(entry, "-", "_");
    replace_all(entry, " ", "_");
    replace_all(entry, ",", "_");

    std::string filepath = path.string();
    replace_all(filepath, "\\", "/");

    if (map.find(entry) != map.end()) {
        int i = 0;
        std::string new_entry = entry + std::to_string(i);
        while (map.find(new_entry) != map.end()) {
            i++;
            new_entry.pop_back();
            new_entry.push_back(i);
        }
        map[new_entry] = filepath;
    } else {
        map[entry] = filepath;
    }
}

void replace_all(std::string& str, const std::string& pattern, const std::string& to) {
    if(pattern.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(pattern, start_pos)) != std::string::npos) {
        str.replace(start_pos, pattern.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

uint32_t get_32_bit_hash(const std::string& str) {
    std::string hash_str = picosha2::hash256_hex_string(str);
    char* success;
    uint32_t hash = strtoull(hash_str.substr(hash_str.length() - 16).c_str(), &success, 16) << 1;
    if (success) {
        return hash;
    } else {
        return static_cast<uint32_t>(-1);
    }
}