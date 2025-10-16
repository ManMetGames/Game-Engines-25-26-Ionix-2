#include "Scripting/Audio/AudioScripting.h"

namespace IonixEngine
{
    AudioScripting* AudioScripting::s_Instance = nullptr;
    AudioScripting& AudioScripting::Get() {
        if (!s_Instance) {
            s_Instance = new AudioScripting();
        }
        return *s_Instance;
    }
    void AudioScripting::Init(sol::state& sharedState)
    {
        if (m_LuaState == &sharedState) {
            std::cout << "Lua Audio Scripting already initialized on this state.\n";
            return;
        }

        std::cout << "Lua Audio Scripting is now being initialized." << std::endl;
        m_LuaState = &sharedState;
        RegisterAudioBindings();
        std::cout << "Lua Audio Scripting has been initialised successfully." << std::endl;
    }


    void AudioScripting::RegisterAudioBindings()
    {
        sol::state& lua = *m_LuaState;

        // Instance-based Audio class
        lua.new_usertype<Audio>("Audio",
            sol::constructors<Audio(const std::string&, const std::string&)>(),
            "Play", &Audio::Play,
            "SetVolume", &Audio::SetVolume,
            "Loop", &Audio::Loop,
            "PauseAll", &Audio::PauseAll,
            "ResumeAll", &Audio::ResumeAll
        );

        // Static/global sound manager access
        lua["AudioManager"] = lua.create_table_with(
            "PlaySound", [](const std::string& name, int loops = 0) {
                SoundManager::GetInstance().PlaySound(name, loops);
            },
            "LoadSound", [](const std::string& name, const std::string& path) {
                SoundManager::GetInstance().LoadSound(name, path);
            },
            "SetVolume", [](const std::string& name, float volume) {
                SoundManager::GetInstance().SetVolume(name, volume);
            },
            "ChangeAudio", [](const std::string& name, const std::string& path) {
                // Load the new sound (overwriting if needed)
                if (SoundManager::GetInstance().LoadSound(name, path))
                {
                    std::cout << "[AudioManager] Changed to: " << path << std::endl;
                    SoundManager::GetInstance().PlaySound(name);
                }
                else
                {
                    std::cerr << "[AudioManager] Failed to change audio to: " << path << std::endl;
                }
            }
        );
    }
}

