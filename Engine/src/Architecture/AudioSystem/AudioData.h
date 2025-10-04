#pragma once
#include <SDL.h>

namespace IonixEngine
{    
    struct AudioData 
    {
        Uint8* buffer = nullptr;   // raw audio data from the buffer
        Uint32 length = 0;         // buffer size
        SDL_AudioSpec spec{};      // format info cus it technically matters in this case (it really matters) (i am kinda doing guesswork, this tbh might need changing)

        ~AudioData() 
        {
            if (buffer) 
            {
                SDL_FreeWAV(buffer);
            }
        }
    };
}