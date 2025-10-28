#include "AudioPlayer.h"

namespace IonixEngine {

AudioPlayer::AudioPlayer(Entity* entity, const std::string& audioClip, bool shouldPlayOnAwake) 
    : Component(entity, false, false, false)  // no physics, no render, not a tag
{
    if (!audioClip.empty())
    {
        clip = audioClip;
    }
    playOnAwake = shouldPlayOnAwake;
}

void AudioPlayer::Start()
{
    if (playOnAwake && !clip.empty())
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
