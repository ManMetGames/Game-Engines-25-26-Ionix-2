#include "Scripting/Audio/AudioScripting.h"
#include <iostream>

namespace IonixEngine {

    AudioScripting* AudioScripting::s_Instance = nullptr;

    AudioScripting& AudioScripting::Get() {
        if (!s_Instance)
            s_Instance = new AudioScripting();
        return *s_Instance;
    }


} // namespace IonixEngine
