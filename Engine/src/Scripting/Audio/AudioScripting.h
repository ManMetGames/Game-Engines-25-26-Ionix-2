#pragma once
#include <sol/sol.hpp>
#include "Audio/AudioPlayer.h"
#include "Architecture/AudioSystem/SoundManager.h"
#include <lua.h>

namespace IonixEngine {

    class AudioScripting {
    public:
        static AudioScripting& Get();

        void Init(sol::state& lua) {}
        float volume = 128.0f;
        bool mute = false;
        std::string clip = "";
        bool loop = false;
        bool playOnAwake = false;
    private:
        static AudioScripting* s_Instance;
        int m_Channel = -1;
    };

} 