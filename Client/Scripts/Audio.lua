local AudioSystem = {}

local testSound = nil

function AudioSystem:OnStart()
    print("[Lua] Audio system initializing...")

    -- Load sound into SoundManager
    local sm = SoundManager.GetInstance()
    sm:LoadSound("test", "./Assets/test.wav")

    -- Create AudioPlayer (assuming 'entity' is passed from engine)
    testSound = AudioPlayer:new(entity, "test", false)
    testSound.volume = 128
    testSound.loop = false

    -- Play sound
    testSound:Play()
    print("[Lua] Playing test.wav...")
end

function AudioSystem:OnShutdown()
    print("[Lua] Audio system shutting down...")
end

return AudioSystem
