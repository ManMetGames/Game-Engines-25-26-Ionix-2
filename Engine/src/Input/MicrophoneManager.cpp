#include "MicrophoneManager.h"
#include <iostream>

namespace IonixEngine
{
	bool MicrophoneManager::openDevice(const char* deviceName)
	{
	    //Initialize SDL audio subsystem
	    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
	        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
	        return false;
	    }
	    
	    //Clears the struct to start fresh
        SDL_zero(desired);
        desired.freq = 48000;			 /**< DSP frequency -- samples per second */
        desired.format = AUDIO_F32;		/**< Audio data format */
        desired.channels = 1;		   /**< Number of channels: 1 mono, 2 stereo */
        desired.samples = 4096;		  /**< Audio buffer size in sample FRAMES (total samples divided by channel count) */
        desired.callback = nullptr;  /**< Callback that feeds the audio device (NULL to use SDL_QueueAudio()). */
		
        //Uint8 silence;              /**< Audio buffer silence value (calculated) */
        //Uint16 padding;             /**< Necessary for some compile environments */
        //Uint32 size;                /**< Audio buffer size in bytes (calculated) */
        //void* userdata;             /**< Userdata passed to callback (ignored for NULL callbacks). */
		
	    //Open the actual audio device for recording (capture = 1)
	    deviceID = SDL_OpenAudioDevice(deviceName, 1, &desired, &obtained, 0);
	    if (deviceID == 0) {
	        std::cerr << "Failed to open recording device: " << SDL_GetError() << "\n"; //Error message
	        
	    	//failed to open
	        return false;
	    }
		//opened and ready for recording
	    return true;
	}
    
	//Closes the currently open microphone device if any are alive
    void MicrophoneManager::closeDevice() 
    {
        if (deviceID != 0)
        {
            SDL_CloseAudioDevice(deviceID);			//Only close if a device is actually open
            deviceID = 0;							//reset device ID
            SDL_QuitSubSystem(SDL_INIT_AUDIO); //quit SDL audio subsystem
            
            std::cout << "Microphone closed." << "\n";
        }
    }

    void MicrophoneManager::SetState(recordingState state)
    {

        if (deviceID == 0)
        {
            currentState = ERROR;
            return;
        }

       
        switch (state)
        {
            case RECORDING:
                recordedSamples.clear();
                SDL_PauseAudioDevice(deviceID, 0); //Start capturing audio 
                break;
            case STOPPED:
                SDL_PauseAudioDevice(deviceID, 1); //Stop capturing audio
                break;

    

        }
    }
}