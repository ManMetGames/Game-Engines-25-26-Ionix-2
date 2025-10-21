#pragma once
#include <sol/sol.hpp>
#include "Audio/Audio.h"

namespace IonixEngine {

    class AudioScripting {
    public:
        static AudioScripting& Get();

    private:
        static AudioScripting* s_Instance;
    };

} 