#include "MicrophoneManager.h"
#include <cmath> 
#include <iostream>

namespace IonixEngine
{
	bool MicrophoneManager::openDevice(const char* deviceName)
	{
        SDL_zero(desired);
        desired.freq = 48000;
        desired.format = AUDIO_F32;
        desired.channels = 1;
        desired.samples = 4096;
        desired.padding;
        desired.size;
        desired.callback = nullptr; 

        //int freq;                   /**< DSP frequency -- samples per second */
        //SDL_AudioFormat format;     /**< Audio data format */
        //Uint8 channels;             /**< Number of channels: 1 mono, 2 stereo */
        //Uint8 silence;              /**< Audio buffer silence value (calculated) */
        //Uint16 samples;             /**< Audio buffer size in sample FRAMES (total samples divided by channel count) */
        //Uint16 padding;             /**< Necessary for some compile environments */
        //Uint32 size;                /**< Audio buffer size in bytes (calculated) */
        //SDL_AudioCallback callback; /**< Callback that feeds the audio device (NULL to use SDL_QueueAudio()). */
        //void* userdata;             /**< Userdata passed to callback (ignored for NULL callbacks). */
        return true;
	}
    void MicrophoneManager::closeDevice() 
    {
        if (!deviceID)
        {
            SDL_CloseAudio();
        }
    }

    void MicrophoneManager::MicrophoneRecordingStates(recordingState state)
    {
        // turned it into a switch for easier control elsewhere
        switch (state)
        {
            case SELECTING_DEVICE:
                std::cout << "Device is being selected";
                // something something - once device is selected, ID needs to be specified or the name of device
                // for a different part but just putting it down before i forget
                break;

            case STOPPED:
                // SDL_PauseAudioDevice( [something related to the current device in here - like "device, ID number"] );   <- i think that's the SDL function we have to use??
                // there is no function for StartAudioDevice only pause sadly :(
                break;

            case RECORDING:
                // something to clear what was last recorded + the buffer version 
                // then just what's on the tin
                break;

            case RECORDED:
                // saves whatever has been recorded /w buffer encase all audio isn't captured
                break;

            case PLAYBACK:
                // plays the saved audio /w buffer 
                break;

            case ERROR:
                std::cout << "Microphone ERROR"; // you can make this more specific or fancy if you want
                break;
        }
    }
}