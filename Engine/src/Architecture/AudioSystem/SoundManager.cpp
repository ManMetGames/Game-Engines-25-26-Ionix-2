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

  // what do you think it does???
  return true;
}

void SoundManager::Shutdown() {
  m_Sounds.clear();
  m_Volumes.clear();
}

bool SoundManager::LoadSound(const std::string &name, const std::string &filePath) {
    m_Sounds[name] = AudioData();
    AudioData* clip = &m_Sounds[name];
    Mix_Chunk* audio = Mix_LoadWAV(filePath.c_str());


    if (audio) {
        m_Sounds[name].audio = audio;
        printf("Test Audio Length: %f", GetPlayTime(name));
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

/*void SoundManager::PlaySound(const std::string& name, int loops) {
  auto it = SoundManager::GetInstance().m_Sounds.find(name);
  if (it == SoundManager::GetInstance().m_Sounds.end()) {
    std::cerr << "Sound not found: " << name << std::endl;
    return;
  }

  auto &clip = it->second;
  float volume = SoundManager::GetInstance().m_Volumes.count(name) ? SoundManager::GetInstance().m_Volumes[name] : 1.0f;
  //clip.audio->volume = (Uint8)SDL_lroundf(SoundManager::GetInstance().m_Volumes[name] * 128.0f);
  Mix_PlayChannel(-1, clip.audio, loops);
}*/

void SoundManager::SetVolume(const std::string& name, float volume) {
  m_Volumes[name] = SDL_clamp(volume, 0, 1);
  // does not play the sound; volume applies to future 'PlaySound' calls
}

float SoundManager::GetPlayTime(const std::string& alias)
{
    Mix_Chunk* audio = GetAudio(alias);
    
    int freq = 0;
    Uint16 format = 0;
    int channels = 0;

    if (audio) {
        if (!Mix_QuerySpec(&freq, &format, &channels)) { return -1.0f; }
        return (float) audio->alen / (float)(freq * channels * ((format & 0xFF) / 8));
    }
    else {
        return -1;
    }
}

SoundManager::~SoundManager() { Shutdown(); }

} // namespace IonixEngine
