#pragma once
#include <unordered_set>
#include <SDL.h>

namespace IonixEngine
{

    class MicrophoneManager
    {
    public:
  
        bool openDevice(const char* deviceName);
        void closeDevice();
        
        // various recording states
        enum recordingState
        {
            SELECTING_DEVICE,
            STOPPED,
            RECORDING,
            RECORDED,
            PLAYBACK,
            ERROR
        };

        void MicrophoneRecordingStates(recordingState state);

    private:
        SDL_AudioSpec desired{};
        SDL_AudioSpec obtained{};
        SDL_AudioDeviceID deviceID;
    };
}

