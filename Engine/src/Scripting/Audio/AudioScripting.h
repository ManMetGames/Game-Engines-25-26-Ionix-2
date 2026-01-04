#pragma once
#include <sol/sol.hpp>
#include "Audio/AudioPlayer.h"
#include "Audio/MusicComponent.h"
#include "Architecture/AudioSystem/SoundManager.h"
#include "Architecture/AudioSystem/MusicManager.h"
#include <lua.h>

namespace IonixEngine {

    class AudioScripting {
    public:
        static AudioScripting& Get();

        void Init(sol::state& lua);

    private:
        static AudioScripting* s_Instance;
    };

} 