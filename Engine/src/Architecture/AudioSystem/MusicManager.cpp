#include "MusicManager.h"
#include "soloud.h"
#include "soloud_wav.h"
#include "Architecture/SHA256.hpp"
#include "Architecture/Assets.hpp"
#include <SDL.h>

namespace IonixEngine {

    MusicManager& MusicManager::GetInstance() {
        static MusicManager instance;
        return instance;
    }

    bool MusicManager::Init() {
        if (m_Soloud != nullptr) {
            SDL_Log("[MusicManager] Already initialized");
            return true;
        }

        m_Soloud = new SoLoud::Soloud();
        
        // Initialize SoLoud with SDL2 backend to coexist with SDL_mixer
        // Use CLIP_ROUNDOFF to prevent audio clipping
        SoLoud::result result = m_Soloud->init(
            SoLoud::Soloud::CLIP_ROUNDOFF,  // Flags to prevent clipping
            SoLoud::Soloud::SDL2,            // Use SDL2 backend (shares with SDL_mixer)
            44100,                           // Sample rate
            2048,                            // Buffer size
            2                                // Channels (stereo)
        );

        if (result != SoLoud::SO_NO_ERROR) {
            SDL_Log("[MusicManager] Failed to initialize SoLoud: error code %d", result);
            delete m_Soloud;
            m_Soloud = nullptr;
            return false;
        }

        SDL_Log("[MusicManager] Successfully initialized SoLoud");

        // Auto-load music from Assets
        for (const auto& pair : Assets::Get().music.GetMusic()) {
            LoadMusic(pair.first, pair.second);
        }
        
        return true;
    }

    void MusicManager::Shutdown() {
        if (m_Soloud != nullptr) {
            m_Soloud->deinit();
            delete m_Soloud;
            m_Soloud = nullptr;
        }
        
        m_Music.clear();
        SDL_Log("[MusicManager] Shutdown complete");
    }

    bool MusicManager::LoadMusic(const std::string& name, const std::string& filePath) {
        if (m_Soloud == nullptr) {
            SDL_Log("[MusicManager] Cannot load music - not initialized");
            return false;
        }

        uint32_t hash = Get32BitHash(name);
        
        // Check if already loaded
        if (m_Music.find(hash) != m_Music.end()) {
            SDL_Log("[MusicManager] Music '%s' already loaded", name.c_str());
            return true;
        }

        // Create new music data
        m_Music[hash] = MusicData();
        m_Music[hash].music = new SoLoud::Wav();

        // Load the audio file
        SoLoud::result result = m_Music[hash].music->load(filePath.c_str());

        if (result != SoLoud::SO_NO_ERROR) {
            SDL_Log("[MusicManager] Failed to load music '%s' from '%s': error code %d", 
                    name.c_str(), filePath.c_str(), result);
            m_Music.erase(hash);
            return false;
        }

        SDL_Log("[MusicManager] Loaded music '%s' from '%s' (duration: %.2fs)", 
                name.c_str(), filePath.c_str(), m_Music[hash].music->getLength());
        
        return true;
    }

    SoLoud::Wav* MusicManager::GetMusic(const std::string& name) {
        return GetMusic(Get32BitHash(name));
    }

    SoLoud::Wav* MusicManager::GetMusic(uint32_t hash) {
        auto it = m_Music.find(hash);
        if (it != m_Music.end()) {
            return it->second.music;
        }
        
        SDL_Log("[MusicManager] Music with hash %u not found", hash);
        return nullptr;
    }

    void MusicManager::SetGlobalVolume(float volume) {
        if (m_Soloud != nullptr) {
            m_GlobalVolume = SDL_clamp(volume, 0.0f, 1.0f);
            m_Soloud->setGlobalVolume(m_GlobalVolume);
        }
    }

    float MusicManager::GetGlobalVolume() const {
        return m_GlobalVolume;
    }

    float MusicManager::GetDuration(const std::string& name) {
        return GetDuration(Get32BitHash(name));
    }

    float MusicManager::GetDuration(uint32_t hash) {
        auto it = m_Music.find(hash);
        if (it != m_Music.end() && it->second.music != nullptr) {
            return static_cast<float>(it->second.music->getLength());
        }
        return -1.0f;
    }

    MusicManager::~MusicManager() {
        Shutdown();
    }

}
