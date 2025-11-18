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


        


    private:
        SDL_AudioSpec desired{};
        SDL_AudioDeviceID deviceID;

    };
}

