local AudioSystem = {}

local testSound = nil

function AudioSystem:OnStart()
    print("[Lua] Audio system initializing...")

    -- Load sound into SoundManager
    local sm = SoundManager.GetInstance()
    sm:LoadSound("test", "./Assets/test.mp3")

    -- Create AudioPlayer (assuming 'entity' is passed from engine)
    testSound = AudioPlayer:new(entity, "test", false)
    testSound.volume = 128

    -- Play sound
    testSound:Play(0, 2)
    print("[Lua] Test file length is: " + sm.GetPlayTime())
    print("[Lua] Playing test.wav...")
end

function AudioSystem:OnShutdown()
    print("[Lua] Audio system shutting down...")
end

return AudioSystem
