#pragma once
#include <SDL.h>

struct Mix_Chunk;

namespace IonixEngine {
    struct AudioData {
        Mix_Chunk* audio;   // raw audio data from the buffer

        ~AudioData();
    };
};
