#include "AudioPlayer.h"
#include "Architecture/ECS/Component.hpp"
#include "Architecture/SHA256.hpp"
#include <cstdint>

namespace IonixEngine {

AudioPlayer::AudioPlayer(Entity* entity, const std::string& audioClip, bool shouldPlayOnAwake) 
    : Component(entity, false, false, false)  // no physics, no render, not a tag
{
    if (!audioClip.empty())
    {
        hash = Get32BitHash(audioClip);
    }
    playOnAwake = shouldPlayOnAwake;
}
AudioPlayer::AudioPlayer(Entity* entity, uint32_t hash, bool shouldPlayOnAwake) : Component(entity, false, false, false) {
    this->hash = hash;
    playOnAwake = shouldPlayOnAwake;
}

void AudioPlayer::Start()
{
    if (playOnAwake && hash != uint32_t(-1))
    {
        Play();
    }
}

void AudioPlayer::Update(float deltaTime)
{
}

void AudioPlayer::Destroy()
{
    if (m_Channel != -1)
    {
        Mix_HaltChannel(m_Channel);
        m_Channel = -1;
    }
}

}
