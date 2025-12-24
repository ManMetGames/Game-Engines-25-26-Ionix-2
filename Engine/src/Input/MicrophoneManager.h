#pragma once
#include <unordered_set>
#include <SDL.h>
#include <vector>

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



        void SetState(recordingState state);
        void Update();
        void OpenPlaybackDevice();


        const std::vector<float>& GetRecordedData() const { return recordedSamples;  }

    private:
        SDL_AudioSpec desired{};
        SDL_AudioSpec obtained{};
        SDL_AudioDeviceID deviceID = 0;

        SDL_AudioDeviceID playbackDeviceID = 0;

        recordingState currentState = STOPPED;

        std::vector<float> recordedSamples;
        std::vector<float> tempBuffer;


    };
}

