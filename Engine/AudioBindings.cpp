#include "Scripting/Bindings/AudioBindings.h"
#include "Scripting/Audio/AudioScripting.h"

namespace IonixEngine {
    void RegisterAudioBindings(sol::state& lua) {
        AudioScripting::Get().Init(lua);
    }
}