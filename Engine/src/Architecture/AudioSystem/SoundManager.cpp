#include "SoundManager.h"
#include "SDL_stdinc.h"
#include <cstdint>
#include <string>
#include "Assets.hpp"
#include "SHA256.hpp"
#include <SDL_mixer.h> // i think this is right I cant tell.

namespace IonixEngine {

// you got ur singleton!
SoundManager &SoundManager::GetInstance() {
  static SoundManager instance;
  return instance;
}

bool SoundManager::Init(int freq, SDL_AudioFormat format, int channels, int chunksize) {
  for (const std::pair<std::string, std::string>& pair : Assets::Get().sounds.GetSounds()) {
      LoadSound(pair.first, pair.second);
  }
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
    uint64_t hash = Get64BitHash(name);
    m_Sounds[hash] = AudioData();
    AudioData* clip = &m_Sounds[hash];
    Mix_Chunk* audio = Mix_LoadWAV(filePath.c_str());

    if (audio) {
        m_Sounds[hash].audio = audio;
        SDL_Log("[Sound Manager] Loaded sound at %s with alias %s", filePath.c_str(), name.c_str());
        return true;
    }
    SDL_Log("[Sound Manager] Could not load audio file: %s due to: %s", filePath.c_str(), Mix_GetError());
    return false;
}

Mix_Chunk* SoundManager::GetAudio(const std::string& name) {
    uint64_t hash = Get64BitHash(name);
    auto it = m_Sounds.find(hash);
    if (it != m_Sounds.end()) {
        return m_Sounds[hash].audio;
    }
    else {
        SDL_Log("[Sound Manager] Sound %s was not present, returned nullptr", name.c_str());
        return nullptr;
    }
}

Mix_Chunk* SoundManager::GetAudio(uint64_t hash) {
    auto it = m_Sounds.find(hash);
    if (it != m_Sounds.end()) {
        return m_Sounds[hash].audio;
    }
    else {
        SDL_Log("[Sound Manager] Sound %ull was not present, returned nullptr", hash);
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

void SoundManager::SetVolume(const std::string &name, float volume) {
  uint64_t hash = Get64BitHash(name);
  m_Volumes[hash] = SDL_clamp(volume, 0, 1);
  // does not play the sound; volume applies to future 'PlaySound' calls
}

void SoundManager::SetVolume(uint64_t hash, float volume) {
  m_Volumes[hash] = SDL_clamp(volume, 0, 1);
  // does not play the sound; volume applies to future 'PlaySound' calls
}

SoundManager::~SoundManager() { Shutdown(); }

} // namespace IonixEngine
