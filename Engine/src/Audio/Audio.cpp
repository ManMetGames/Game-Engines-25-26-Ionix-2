#include "Audio.h"
#include "SDL_mixer.h"

namespace IonixEngine {
    Audio::Audio(const std::string& name)
    {
        chunk = Mix_LoadWAV(name.c_str());
        // chunk = SoundManager::GetInstance().GetAudio(name);

        //check if audio format can be played. Add to SDL_Log below if it works.
        //if (chunk == NULL)
        //{
        //    SDL_Log("error incorect audio format, use MP3 or Wav or Flac", Mix_GetError());
        //    return;
        //}


    }
    void Audio::PlayAudio()
    {
        Mix_PlayChannel(-1, chunk, 0);
    }

    void Audio::ChangeVolume(float volume)
    {
        Mix_MasterVolume(volume);
    }

    void Audio::Pause()
    {
        Mix_Pause(-1);
    }

    void Audio::Resume()
    {
        Mix_Resume(-1);
    }

    void Audio::LoopAudio(double music)
    {
        // noop
	}
}