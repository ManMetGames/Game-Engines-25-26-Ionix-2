#include <filesystem>
#include <map>
#include <vector>

namespace fs = std::filesystem;

void add_entry(const std::map<std::string, fs::path>& map, const fs::path& path);

int main(void) {
    std::map<std::string, fs::path> images;
    std::map<std::string, fs::path> sounds;
    std::map<std::string, fs::path> fonts;


    for (auto& entry : fs::directory_iterator("./Client/Assets/")) {
        std::string extension = entry.path().extension().string();
        if (extension == "png" || extension == "jgp") {
            add_entry(images, entry.path());
        } else if (extension == "wav" || extension == "mp3") {
            add_entry(sounds, entry.path());
        } else if (extension == "otf" || extension == "ttf") {
            add_entry(fonts, entry.path());
        }
    }
}


void add_entry(const std::map<std::string, fs::path>& map, const fs::path& path) {
    map.insert(path.stem().string(), path);
}
