#include "MusicComponent.h"
#include "soloud.h"
#include "soloud_wav.h"
#include <SDL.h>

namespace IonixEngine {

    MusicComponent::MusicComponent(Entity* entity, const std::string& musicTrack, bool shouldPlayOnAwake)
        : Component(entity, false, false, false)  // No physics, no render, not a tag
    {
        if (!musicTrack.empty()) {
            musicName = musicTrack;
        }
        playOnAwake = shouldPlayOnAwake;
    }

    void MusicComponent::Start() {
        if (playOnAwake && !musicName.empty()) {
            Play(looping, fadeInSeconds);
        }
    }

    void MusicComponent::Update(float deltaTime) {
        // Optional: Add any per-frame logic here (e.g., custom fade curves)
    }

    void MusicComponent::Destroy() {
        Stop(fadeOutSeconds);
    }

    int MusicComponent::Play(bool loop, float fadeInDuration) {
        if (musicName.empty()) {
            SDL_Log("[MusicComponent] Cannot play: no music assigned");
            return -1;
        }

        // Get the music from MusicManager
        SoLoud::Wav* music = MusicManager::GetInstance().GetMusic(musicName);
        if (music == nullptr) {
            SDL_Log("[MusicComponent] Failed to play music: '%s' not found", musicName.c_str());
            return -1;
        }

        // Set looping on the audio source
        music->setLooping(loop);
        looping = loop;

        // Get the SoLoud engine
        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine == nullptr) {
            SDL_Log("[MusicComponent] SoLoud engine not initialized");
            return -1;
        }

        // Calculate actual volume (respecting mute)
        float actualVolume = mute ? 0.0f : volume;

        // Play the music
        if (fadeInDuration > 0.0f) {
            m_Handle = engine->play(*music, 0.0f);  // Start at 0 volume
            if (m_Handle > 0) {
                engine->fadeVolume(m_Handle, actualVolume, fadeInDuration);
            }
        } else {
            m_Handle = engine->play(*music, actualVolume);
        }

        if (m_Handle < 0) {
            SDL_Log("[MusicComponent] Failed to play music '%s'", musicName.c_str());
        }

        return m_Handle;
    }

    void MusicComponent::Stop(float fadeOutDuration) {
        if (m_Handle <= 0) {
            return;
        }

        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine == nullptr) {
            return;
        }

        if (fadeOutDuration > 0.0f) {
            engine->fadeVolume(m_Handle, 0.0f, fadeOutDuration);
            engine->scheduleStop(m_Handle, fadeOutDuration);
        } else {
            engine->stop(m_Handle);
        }

        m_Handle = -1;
    }

    void MusicComponent::Pause() {
        if (m_Handle <= 0) {
            return;
        }

        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine != nullptr) {
            engine->setPause(m_Handle, true);
        }
    }

    void MusicComponent::Resume() {
        if (m_Handle <= 0) {
            return;
        }

        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine != nullptr) {
            engine->setPause(m_Handle, false);
        }
    }

    void MusicComponent::SetPlaybackSpeed(float speed) {
        if (m_Handle <= 0) {
            return;
        }

        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine != nullptr) {
            // THIS IS THE KEY FEATURE - negative speed = reverse playback!
            engine->setRelativePlaySpeed(m_Handle, speed);
            m_CurrentSpeed = speed;
        }
    }

    float MusicComponent::GetPlaybackSpeed() const {
        return m_CurrentSpeed;
    }

    void MusicComponent::Seek(float seconds) {
        if (m_Handle <= 0) {
            return;
        }

        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine != nullptr) {
            engine->seek(m_Handle, seconds);
        }
    }

    float MusicComponent::GetPosition() const {
        if (m_Handle <= 0) {
            return 0.0f;
        }

        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine != nullptr) {
            return static_cast<float>(engine->getStreamPosition(m_Handle));
        }
        return 0.0f;
    }

    float MusicComponent::GetDuration() const {
        if (musicName.empty()) {
            return -1.0f;
        }

        return MusicManager::GetInstance().GetDuration(musicName);
    }

    void MusicComponent::SetVolume(float vol) {
        volume = SDL_clamp(vol, 0.0f, 1.0f);

        if (m_Handle > 0 && !mute) {
            SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
            if (engine != nullptr) {
                engine->setVolume(m_Handle, volume);
            }
        }
    }

    void MusicComponent::FadeVolume(float targetVolume, float duration) {
        if (m_Handle <= 0) {
            return;
        }

        targetVolume = SDL_clamp(targetVolume, 0.0f, 1.0f);
        
        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine != nullptr) {
            float actualTarget = mute ? 0.0f : targetVolume;
            engine->fadeVolume(m_Handle, actualTarget, duration);
            volume = targetVolume;
        }
    }

    void MusicComponent::SetPan(float pan) {
        if (m_Handle <= 0) {
            return;
        }

        pan = SDL_clamp(pan, -1.0f, 1.0f);

        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine != nullptr) {
            engine->setPan(m_Handle, pan);
        }
    }

    void MusicComponent::ToggleMute() {
        mute = !mute;

        if (m_Handle > 0) {
            SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
            if (engine != nullptr) {
                float actualVolume = mute ? 0.0f : volume;
                engine->setVolume(m_Handle, actualVolume);
            }
        }
    }

    bool MusicComponent::IsPlaying() const {
        if (m_Handle <= 0) {
            return false;
        }

        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine != nullptr) {
            return engine->isValidVoiceHandle(m_Handle) && !engine->getPause(m_Handle);
        }
        return false;
    }

    bool MusicComponent::IsPaused() const {
        if (m_Handle <= 0) {
            return false;
        }

        SoLoud::Soloud* engine = MusicManager::GetInstance().GetEngine();
        if (engine != nullptr) {
            return engine->isValidVoiceHandle(m_Handle) && engine->getPause(m_Handle);
        }
        return false;
    }

}
