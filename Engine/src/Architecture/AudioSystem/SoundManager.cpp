#include "SoundManager.h"
#include "SDL_stdinc.h"
#include <SDL_mixer.h> // i think this is right I cant tell.
#include <iostream>

namespace IonixEngine {

// you got ur singleton!
SoundManager &SoundManager::GetInstance() {
  static SoundManager instance;
  return instance;
}

bool SoundManager::Init(int freq, SDL_AudioFormat format, int channels, int chunksize) {
  SDL_AudioSpec desiredSpec{};
  desiredSpec.freq = freq;
  desiredSpec.format = format;
  desiredSpec.channels = channels;
  desiredSpec.samples = chunksize;
  desiredSpec.callback = nullptr; // this is the callback, it might need changing one day.

  m_Device = SDL_OpenAudioDevice(nullptr, 0, &desiredSpec, &m_DeviceSpec, 0);
  if (!m_Device) {
    SDL_Log("Failed to open audio device: %s", SDL_GetError());
    return false;
  }

  SDL_PauseAudioDevice(m_Device, 0); // what do you think it does???
  return true;
}

void SoundManager::Shutdown() {
  m_Sounds.clear();
  m_Volumes.clear();

  if (m_Device != 0) {
    SDL_CloseAudioDevice(m_Device);
    m_Device = 0;
  }
}

bool SoundManager::LoadSound(const std::string &name, const std::string &filePath) {
    m_Sounds[name] = AudioData();
    AudioData* clip = &m_Sounds[name];
    Mix_Chunk* audio = Mix_LoadWAV(filePath.c_str());

    printf("%4f%", SoundManager::GetPlayTime(filePath.c_str()));

    if (audio) {
        m_Sounds[name].audio = audio;
        return true;
    }
    SDL_Log("[Sound Manager] Could not load audio file: %s due to: %s", filePath.c_str(), Mix_GetError());
    return false;
}

Mix_Chunk* SoundManager::GetAudio(const std::string& name) {
    auto it = m_Sounds.find(name);
    if (it != m_Sounds.end()) {
        return m_Sounds[name].audio;
    }
    else {
        SDL_Log("[Sound Manager] Sound %s was not present, returned nullptr", name.c_str());
        return nullptr;
    }
}

//void SoundManager::PlaySound(const std::string &name, int loops) {
//  auto it = m_Sounds.find(name);
//  if (it == m_Sounds.end()) {
//    std::cerr << "Sound not found: " << name << std::endl;
//    return;
//  }
//
//  auto &clip = it->second;
//  float volume = m_Volumes.count(name) ? m_Volumes[name] : 1.0f;
//  clip.audio->volume = (Uint8)SDL_lroundf(m_Volumes[name] * 128.0f);
//  Mix_PlayChannel(-1, clip.audio, loops);
//}

void SoundManager::SetVolume(const std::string& name, float volume) {
  m_Volumes[name] = SDL_clamp(volume, 0, 1);
  // does not play the sound; volume applies to future 'PlaySound' calls
}

float SoundManager::GetPlayTime(const char* filename)
{
    SDL_AudioSpec spec;
    uint32_t audioLen;
    uint8_t* audioBuf;
    double seconds = 0.0;

    if (SDL_LoadWAV(filename, &spec, &audioBuf, &audioLen) != NULL) {
        // we aren't using the actual audio in this example
        SDL_FreeWAV(audioBuf);
        uint32_t sampleSize = SDL_AUDIO_BITSIZE(spec.format) / 8;
        uint32_t sampleCount = audioLen / sampleSize;
        // could do a sanity check and make sure (audioLen % sampleSize) is 0
        uint32_t sampleLen = 0;
        if (spec.channels) {
            sampleLen = sampleCount / spec.channels;
        }
        else {
            // spec.channels *should* be 1 or higher, but just in case
            sampleLen = sampleCount;
        }
        seconds = (double)sampleLen / (double)spec.freq;
    }
    else {
        // uh-oh!
        fprintf(stderr, "ERROR: can't load: %s: %s\n", filename, SDL_GetError());
    }

    return seconds;
}

SoundManager::~SoundManager() { Shutdown(); }

} // namespace IonixEngine
