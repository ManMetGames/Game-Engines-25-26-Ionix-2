#include "MusicData.h"
#include "soloud_wav.h"

namespace IonixEngine {
    
    MusicData::MusicData() : music(nullptr) {}
    
    MusicData::~MusicData() {
        if (music != nullptr) {
            delete music;
            music = nullptr;
        }
    }
    
}
