#include "MicrophoneManager.h"
#include <cmath> 

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

    void MicrophoneManager::MicrophoneRecordingStates()
    {
        if (SELECTING_DEVICE){}
        if (STOPPED){}
        if(RECORDING){}
        if(RECORDED){}
        if(PLAYBACK){}
        if(ERROR){}
    }
}