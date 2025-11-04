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

    -- Play sound
    testSound:Play(0, 2) -- First overload is the fade in time and the second is the number of loops, -1 for infinite
    print("[Lua] Test file length is: " .. sm:GetPlayTime("test"))
    print("[Lua] Playing test.wav...")
end

function AudioSystem:OnShutdown()
    print("[Lua] Audio system shutting down...")
end

return AudioSystem
