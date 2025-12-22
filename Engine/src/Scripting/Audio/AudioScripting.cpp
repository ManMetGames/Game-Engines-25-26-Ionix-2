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
        auto get_clip = [](Entity* e)->std::string { if (auto* p = e ? e->GetComponent<AudioPlayer>() : nullptr) return p->clip; return {}; };
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
            "get_clip", get_clip,
            "get_play_on_awake", get_awake
        );




        // ================================================
        // MUSIC COMPONENT BINDINGS (USES SOLOUD LIBRARY)
        //=================================================
        auto music_play = [](Entity* e, sol::optional<bool> loop, sol::optional<float> fadeIn) {
            if (!e) return;
            auto* m = e->GetComponent<MusicComponent>();
            if (!m) return;
            m->Play(loop.value_or(false), fadeIn.value_or(0.0f));
        };
        
        auto music_stop = [](Entity* e, sol::optional<float> fadeOut) {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) 
                m->Stop(fadeOut.value_or(0.0f));
        };
        
        auto music_pause = [](Entity* e) {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) m->Pause();
        };
        
        auto music_resume = [](Entity* e) {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) m->Resume();
        };
        
        auto music_set_speed = [](Entity* e, float speed) {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) m->SetPlaybackSpeed(speed);
        };
        
        auto music_get_speed = [](Entity* e)->float {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) return m->GetPlaybackSpeed();
            return 1.0f;
        };
        
        auto music_seek = [](Entity* e, float seconds) {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) m->Seek(seconds);
        };
        
        auto music_get_position = [](Entity* e)->float {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) return m->GetPosition();
            return 0.0f;
        };
        
        auto music_get_duration = [](Entity* e)->float {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) return m->GetDuration();
            return -1.0f;
        };
        
        auto music_set_volume = [](Entity* e, float vol) {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) m->SetVolume(vol);
        };
        
        auto music_fade_volume = [](Entity* e, float target, float duration) {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) m->FadeVolume(target, duration);
        };
        
        auto music_set_pan = [](Entity* e, float pan) {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) m->SetPan(pan);
        };
        
        auto music_toggle_mute = [](Entity* e) {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) m->ToggleMute();
        };
        
        auto music_is_playing = [](Entity* e)->bool {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) return m->IsPlaying();
            return false;
        };
        
        auto music_is_paused = [](Entity* e)->bool {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) return m->IsPaused();
            return false;
        };
        
        auto music_get_volume = [](Entity* e)->float {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) return m->volume;
            return 0.0f;
        };
        
        auto music_get_mute = [](Entity* e)->bool {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) return m->mute;
            return false;
        };
        
        auto music_get_name = [](Entity* e)->std::string {
            if (auto* m = e ? e->GetComponent<MusicComponent>() : nullptr) return m->musicName;
            return "";
        };

        lua["MusicComponent"] = lua.create_table_with(
            "play", music_play,
            "stop", music_stop,
            "pause", music_pause,
            "resume", music_resume,
            "set_speed", music_set_speed,
            "get_speed", music_get_speed,
            "seek", music_seek,
            "get_position", music_get_position,
            "get_duration", music_get_duration,
            "set_volume", music_set_volume,
            "fade_volume", music_fade_volume,
            "set_pan", music_set_pan,
            "toggle_mute", music_toggle_mute,
            "is_playing", music_is_playing,
            "is_paused", music_is_paused,
            "get_volume", music_get_volume,
            "get_mute", music_get_mute,
            "get_name", music_get_name
        );

        // MusicManager global functions
        auto music_manager_set_volume = [](float vol) {
            MusicManager::GetInstance().SetGlobalVolume(vol);
        };
        
        auto music_manager_get_volume = []()->float {
            return MusicManager::GetInstance().GetGlobalVolume();
        };
        
        auto music_manager_get_duration = [](const std::string& name)->float {
            return MusicManager::GetInstance().GetDuration(name);
        };

        lua["MusicManager"] = lua.create_table_with(
            "set_global_volume", music_manager_set_volume,
            "get_global_volume", music_manager_get_volume,
            "get_duration", music_manager_get_duration
        );
    }
}