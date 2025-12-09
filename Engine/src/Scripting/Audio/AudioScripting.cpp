#include "Scripting/Audio/AudioScripting.h"
#include "Architecture/ECS/Entity.hpp"

namespace IonixEngine {

    AudioScripting& AudioScripting::Get() {
        static AudioScripting* s_Instance;
        if (!s_Instance) s_Instance = new AudioScripting();
        return *s_Instance;
    }

    void AudioScripting::Init(sol::state& lua)
    {
        auto play = [](Entity* e, sol::optional<int> fadeMs, sol::optional<int> loops) {
            if (!e) return;
            auto* p = e->GetComponent<AudioPlayer>();
            if (!p) return;
            if (!fadeMs) p->Play();
            else if (!loops) p->Play(*fadeMs);
            else p->Play(*fadeMs, *loops);
            };

        auto pause = [](Entity* e) { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) p->Pause(); };
        auto resume = [](Entity* e) { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) p->Resume(); };
        auto terminate = [](Entity* e) { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) p->End(); };
        auto is_playing = [](Entity* e)->bool { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) return p->IsPlaying(); return false; };
        auto one_shot = [](Entity* e, std::string name, float scale) { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) p->PlayOneShot(name, scale); };
        auto change_vol = [](Entity* e, float v) { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) p->ChangeVolume(v); };
        auto toggle_mute = [](Entity* e) { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) p->ToggleMute(); };
        auto get_volume = [](Entity* e)->float { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) return p->volume; return 0.f; };
        auto get_mute = [](Entity* e)->bool { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) return p->mute; return false; };
        auto get_awake = [](Entity* e)->bool { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) return p->playOnAwake; return false; };

        lua["AudioComponent"] = lua.create_table_with(
            "play", play,
            "pause", pause,
            "resume", resume,
            "terminate", terminate,
            "is_playing", is_playing,
            "play_one_shot", one_shot,
            "change_volume", change_vol,
            "toggle_mute", toggle_mute,
            "get_volume", get_volume,
            "get_mute", get_mute,
            "get_play_on_awake", get_awake
        );
    }
}
