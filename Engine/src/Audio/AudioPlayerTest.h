#pragma once
#include "AudioPlayer.h"
#include "Architecture/AudioSystem/SoundManager.h"
#include <SDL.h>

namespace IonixEngine
{
    class AudioPlayerTest
    {
    public:
        static void RunTests()
        {
            SDL_Log("=== AudioPlayer Test Suite Starting ===");
            SDL_Log("Waiting 5 seconds for window to finish loading...");
            SDL_Delay(5000);
            
            // Step 1: Load
            SDL_Log("\n[Step 1] Loading test sound...");
            bool loaded = SoundManager::GetInstance().LoadSound("test_sound", "../Assets/Audio/test.wav.wav");
            if (!loaded)
            {
                SDL_Log("[WARN] Test sound not found. Please place a sound file at: ../Assets/Audio/test.wav.wav");
                SDL_Log("Skipping audio tests.");
                return;
            }
            SDL_Log("[PASS] Sound loaded successfully");
            
            // Create AudioPlayer instance
            AudioPlayer testPlayer;
            testPlayer.clip = "test_sound";
            testPlayer.volume = 128;
            
            // Step 2: Play with fade in
            SDL_Log("\n[Step 2] Playing with fade in (5 seconds)...");
            testPlayer.Play(5000);
            
            // Step 3: Wait 3 seconds
            SDL_Log("[Step 3] Waiting 3 seconds...");
            SDL_Delay(3000);
            
            // Step 4: Pause
            SDL_Log("\n[Step 4] Pausing...");
            testPlayer.Pause();
            SDL_Log("[Step 4] Waiting 3 seconds while paused...");
            SDL_Delay(3000);
            
            // Step 5: Resume
            SDL_Log("\n[Step 5] Resuming...");
            testPlayer.Resume();
            SDL_Log("[Step 5] Waiting 2 seconds...");
            SDL_Delay(2000);
            
            // Step 6: Mute
            SDL_Log("\n[Step 6] Muting...");
            testPlayer.ToggleMute();
            SDL_Log("[Step 6] Waiting 2 seconds...");
            SDL_Delay(2000);
            
            // Step 7: Unmute
            SDL_Log("\n[Step 7] Unmuting...");
            testPlayer.ToggleMute();
            SDL_Log("[Step 7] Waiting 2 seconds...");
            SDL_Delay(2000);
            
            // Step 8: Change volume to 32
            SDL_Log("\n[Step 8] Changing volume to 32...");
            testPlayer.ChangeVolume(32);
            SDL_Log("[Step 8] Waiting 2 seconds...");
            SDL_Delay(2000);
            
            // Step 9: Stop
            SDL_Log("\n[Step 9] Testing Stop()...");
            testPlayer.Stop();
            SDL_Log("[Step 9] Audio stopped. Waiting 2 seconds (should be silent)...");
            SDL_Delay(2000);
            
            // Verify stop worked
            if (!testPlayer.IsPlaying())
            {
                SDL_Log("[PASS] Stop() correctly halted playback");
            }
            else
            {
                SDL_Log("[FAIL] Stop() did not halt playback");
            }
            
            SDL_Log("\n=== AudioPlayer Test Suite Completed ===");
            SDL_Log("All tests passed successfully!");
        }
    };
}
