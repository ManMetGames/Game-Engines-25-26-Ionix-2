#pragma once

// Forward declaration for SoLoud::Wav
namespace SoLoud {
    class Wav;
}

namespace IonixEngine {
    struct MusicData {
        SoLoud::Wav* music;   // SoLoud music data

        MusicData();
        ~MusicData();
    };
}
