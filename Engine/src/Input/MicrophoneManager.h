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

        void MicrophoneRecordingStates(recordingState state); // <- don't move this!!! stuff in .cpp breaks if so (c++ logic moment)

    private:
        SDL_AudioSpec desired{};
        SDL_AudioSpec obtained{};
        SDL_AudioDeviceID deviceID;
    };
}

