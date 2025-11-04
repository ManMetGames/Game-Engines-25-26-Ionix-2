#include "Scripting/Audio/AudioScripting.h"
#include "Architecture/ECS/Entity.hpp"

namespace IonixEngine {


    AudioScripting& AudioScripting::Get() {
        static AudioScripting* s_Instance;
        if (!s_Instance)
            s_Instance = new AudioScripting();
        return *s_Instance;
    }

    void AudioScripting::Init(sol::state& lua)
    {

         // --- Bind SoundManager singleton ---
        auto getInstance = []() -> SoundManager& {
            return SoundManager::GetInstance();
            };

        auto loadSound = [](std::string name, std::string filePath) -> bool {
            SoundManager::GetInstance().LoadSound(name, filePath);
            };

        auto setVolume = [](std::string name, float volume) {
            SoundManager::GetInstance().SetVolume(name, volume);
            };

        auto getAudio = [](std::string name) -> Mix_Chunk* {
            SoundManager::GetInstance().GetAudio(name);
            };

        auto getPlaytime = [](std::string name) -> float {
            SoundManager::GetInstance().GetPlayTime(name);
            };
         
        lua["SoundManager"] = lua.create_table_with(
            "get_sound_manager", getInstance,
            "load_sound", loadSound,
            "set_volume", setVolume,
            "get_audio", getAudio,
            "get_playtime", getPlaytime
        );
        
         
         // --- Bind AudioPlayer ---

        auto play = [](Entity* entity) {
            entity->GetComponent<AudioPlayer>()->Play();
            };

        auto play = [](Entity* entity, int fadeMilliseconds = 0) {
            entity->GetComponent<AudioPlayer>()->Play(fadeMilliseconds);
            };

        auto play = [](Entity* entity, int fadeMilliseconds = 0, int numLoops = 0) {
            entity->GetComponent<AudioPlayer>()->Play(fadeMilliseconds, numLoops);
            };

        auto pause = [](Entity* entity) {
            entity->GetComponent<AudioPlayer>()->Pause();
            };

        auto resume = [](Entity* entity){
            entity->GetComponent<AudioPlayer>()->Resume();
            };

        auto end = [](Entity* entity){
            entity->GetComponent<AudioPlayer>()->End();
            };

        auto isPlaying = [](Entity* entity) -> bool {
            return entity->GetComponent<AudioPlayer>()->IsPlaying();
            };

        auto oneShot = [](Entity* entity, std::string name, float volumeScale = 1.0f)
            {
                entity->GetComponent<AudioPlayer>()->PlayOneShot(name, volumeScale);
            };

        auto changeVolume = [](Entity* entity, float volume) {
            entity->GetComponent<AudioPlayer>()->ChangeVolume(volume);
            };

        auto toggleMute = [](Entity* entity) {
            entity->GetComponent<AudioPlayer>()->ToggleMute();
            };

        auto getVolume = [](Entity* entity) -> float {
            return entity->GetComponent<AudioPlayer>()->volume;
            };

        auto mute = [](Entity* entity) -> bool {
            return entity->GetComponent<AudioPlayer>()->mute;
            };

        auto getClip = [](Entity* entity) -> std::string {
            return entity->GetComponent<AudioPlayer>()->clip;
            };

        auto getAwake = [](Entity* entity) -> bool {
            return entity->GetComponent<AudioPlayer>()->playOnAwake;
            };
       
        lua["AudioComponent"] = lua.create_table_with(
            "play", play,
            "pause", pause,
            "resume", resume,
            "end", end,
            "is_playing", isPlaying,
            "play_one_shot", oneShot,
            "change_volume", changeVolume,
            "toggle_mute", toggleMute,
            "get_volume", getVolume,
            "get_mute", mute,
            "get_clip", getClip,
            "get_play_on_awake", getAwake
        );
    }

}
