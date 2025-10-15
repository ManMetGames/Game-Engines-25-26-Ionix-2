#pragma once
#include "sol/sol.hpp"
#include "Scripting/Scripting.h"
#include "LayerSystem/Layers/LayerSound.h"
#include "Architecture/AudioSystem/SoundManager.h"
#include "Audio/Audio.h"

namespace IonixEngine
{
    class AudioScripting
    {
    public:
        static AudioScripting & Get();
        void Init(sol::state& sharedState);
        void RegisterAudioBindings();

    private:
        static AudioScripting* s_Instance;
        sol::state* m_LuaState = nullptr; // changed from sol::state to pointer
    };
}