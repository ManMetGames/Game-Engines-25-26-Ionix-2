#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
#include <memory>
#include "MusicData.h"

// Forward declarations
namespace SoLoud {
    class Soloud;
    class Wav;
}

namespace IonixEngine
{
    /**
     * @brief Singleton manager for music playback using SoLoud
     * 
     * MusicManager handles loading and managing music files through the SoLoud audio engine.
     * Unlike SoundManager (which handles short sound effects), MusicManager is designed for
     * longer audio tracks like background music, with support for seeking, looping, and
     * advanced playback features like reverse playback (rewind effect).
     */
    class MusicManager
    {
    public:
        static MusicManager& GetInstance();

        /**
         * @brief Initialize the SoLoud audio engine
         * @return true if initialization was successful, false otherwise
         */
        bool Init();
        
        /**
         * @brief Shutdown SoLoud and clean up resources
         */
        void Shutdown();

        /**
         * @brief Load a music file from disk
         * @param name Unique identifier for this music track
         * @param filePath Path to the audio file (supports .wav, .ogg, .mp3, .flac)
         * @return true if loaded successfully, false otherwise
         */
        bool LoadMusic(const std::string& name, const std::string& filePath);
        
        /**
         * @brief Get a loaded music track by name
         * @param name The identifier used when loading
         * @return Pointer to SoLoud::Wav or nullptr if not found
         */
        SoLoud::Wav* GetMusic(const std::string& name);
        
        /**
         * @brief Get a loaded music track by hash
         * @param hash The 32-bit hash of the name
         * @return Pointer to SoLoud::Wav or nullptr if not found
         */
        SoLoud::Wav* GetMusic(uint32_t hash);
        
        /**
         * @brief Set global volume for all music
         * @param volume Volume level (0.0f = silent, 1.0f = full)
         */
        void SetGlobalVolume(float volume);
        
        /**
         * @brief Get global volume
         * @return Current global volume (0.0f to 1.0f)
         */
        float GetGlobalVolume() const;
        
        /**
         * @brief Get the duration of a music track in seconds
         * @param name The identifier of the music track
         * @return Duration in seconds, or -1.0f if not found
         */
        float GetDuration(const std::string& name);
        
        /**
         * @brief Get the duration of a music track in seconds
         * @param hash The hash of the music track identifier
         * @return Duration in seconds, or -1.0f if not found
         */
        float GetDuration(uint32_t hash);

        /**
         * @brief Get the SoLoud engine instance (for advanced use)
         * @return Pointer to the SoLoud engine
         */
        SoLoud::Soloud* GetEngine() { return m_Soloud; }

    private:
        MusicManager() = default;
        ~MusicManager();

        MusicManager(const MusicManager&) = delete;
        MusicManager& operator=(const MusicManager&) = delete;

        SoLoud::Soloud* m_Soloud = nullptr;
        std::unordered_map<uint32_t, MusicData> m_Music;
        float m_GlobalVolume = 1.0f;
    };
}
