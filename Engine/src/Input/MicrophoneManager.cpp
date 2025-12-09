#include "MicrophoneManager.h"
#include <cmath> 

namespace IonixEngine
{
	bool MicrophoneManager::openDevice(const char* deviceName)
	{
        SDL_zero(desired);
        desired.freq = 48000;       //int freq;  /**< DSP frequency -- samples per second */
        desired.format = AUDIO_F32; //SDL_AudioFormat format;     /**< Audio data format */
        desired.channels = 1;       //Uint8 channels;             /**< Number of channels: 1 mono, 2 stereo */
        desired.samples = 4096;     //Uint16 samples;             /**< Audio buffer size in sample FRAMES (total samples divided by channel count) */
        desired.callback = nullptr; //SDL_AudioCallback callback; /**< Callback that feeds the audio device (NULL to use SDL_QueueAudio()). */

        return deviceID;
	}
    void MicrophoneManager::closeDevice() const 
    {
        if (!deviceID)
        {
            SDL_CloseAudio();

        }
    }
}
