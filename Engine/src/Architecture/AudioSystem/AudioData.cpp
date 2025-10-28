#include "AudioData.h"
#include "SDL_mixer.h"

namespace IonixEngine {
  AudioData::~AudioData() {
    if (audio) {
        Mix_FreeChunk(audio);
    }
  }
}
