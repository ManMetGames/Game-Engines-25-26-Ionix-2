#pragma once
#include "LayerSystem/Layer.h"
#include "Architecture/AudioSystem/MusicManager.h"
#include "Architecture/ECS/Component.hpp"
#include <string>

namespace IonixEngine
{
    /**
     * @brief Component for controlling music playback on an entity
     * 
     * MusicComponent provides per-entity music control with advanced features like
     * reverse playback, seeking, crossfading, and looping. Unlike AudioPlayer (for sound effects),
     * MusicComponent is designed for longer audio tracks and uses the SoLoud engine.
     * 
     * Key Features:
     * - Reverse playback (rewind effect) via SetPlaybackSpeed(-1.0f)
     * - Seeking to specific positions
     * - Crossfading between tracks
     * - Volume control and fading
     * - Looping with customizable behavior
     * 
     * Example usage:
     *   MusicComponent* bgMusic = entity->AddComponent<MusicComponent>("boss_theme");
     *   bgMusic->Play(true); // Play with looping
     *   bgMusic->SetPlaybackSpeed(-1.0f); // Rewind effect
     */
    class MusicComponent : public Component
    {
    public:
        // Per-instance properties
        float volume = 1.0f;              // Volume (0.0 to 1.0)
        bool mute = false;                 // Mute state
        std::string musicName = "";        // Name of the music track
        bool playOnAwake = false;          // Auto-play when component starts
        bool looping = false;              // Loop the music
        float fadeInSeconds = 0.0f;        // Fade in duration
        float fadeOutSeconds = 0.0f;       // Fade out duration

        /**
         * @brief Constructor
         * @param entity The entity this component is attached to
         * @param musicTrack Name of the music track (must be loaded in MusicManager)
         * @param shouldPlayOnAwake Whether to auto-play on Start()
         */
        MusicComponent(Entity* entity, const std::string& musicTrack = "", bool shouldPlayOnAwake = false);

        // Component lifecycle
        void Start() override;
        void Update(float deltaTime) override;
        void Destroy() override;

        /**
         * @brief Play the music track
         * @param loop Whether to loop the track
         * @param fadeInDuration Optional fade-in duration in seconds
         * @return Handle to the playing sound instance
         */
        int Play(bool loop = false, float fadeInDuration = 0.0f);

        /**
         * @brief Stop the music
         * @param fadeOutDuration Optional fade-out duration in seconds
         */
        void Stop(float fadeOutDuration = 0.0f);

        /**
         * @brief Pause the music
         */
        void Pause();

        /**
         * @brief Resume the music from pause
         */
        void Resume();

        /**
         * @brief Set playback speed (1.0 = normal, -1.0 = reverse/rewind, 2.0 = double speed)
         * @param speed Speed multiplier (negative values play in reverse)
         */
        void SetPlaybackSpeed(float speed);

        /**
         * @brief Get current playback speed
         * @return Current speed multiplier
         */
        float GetPlaybackSpeed() const;

        /**
         * @brief Seek to a specific position in the track
         * @param seconds Position in seconds
         */
        void Seek(float seconds);

        /**
         * @brief Get current playback position
         * @return Current position in seconds
         */
        float GetPosition() const;

        /**
         * @brief Get the duration of the music track
         * @return Duration in seconds, or -1.0f if not loaded
         */
        float GetDuration() const;

        /**
         * @brief Set volume
         * @param vol Volume (0.0 to 1.0)
         */
        void SetVolume(float vol);

        /**
         * @brief Fade volume to a target value
         * @param targetVolume Target volume (0.0 to 1.0)
         * @param duration Fade duration in seconds
         */
        void FadeVolume(float targetVolume, float duration);

        /**
         * @brief Set stereo panning
         * @param pan Pan position (-1.0 = left, 0.0 = center, 1.0 = right)
         */
        void SetPan(float pan);

        /**
         * @brief Toggle mute
         */
        void ToggleMute();

        /**
         * @brief Check if music is currently playing
         * @return true if playing, false otherwise
         */
        bool IsPlaying() const;

        /**
         * @brief Check if music is paused
         * @return true if paused, false otherwise
         */
        bool IsPaused() const;

        /**
         * @brief Get the current playback handle
         * @return SoLoud voice handle (-1 if not playing)
         */
        int GetHandle() const { return m_Handle; }

    private:
        int m_Handle = -1;  // SoLoud voice handle for this instance
        float m_CurrentSpeed = 1.0f;  // Track current speed for queries
    };
}
