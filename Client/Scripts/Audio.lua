local audio = {}
local bgmEntity
local soundEntity
local volume = 64 -- 0–128 range
local assets

function audio:OnStart()
    print("[Lua] Audio system initializing...")

    -- Create an entity
    soundEntity = Entity.create_entity()

    -- Attach AudioPlayer and load directly from disk
    Entity.add_audio_component(soundEntity, "test", false)

    -- Start playback and set volume
    AudioComponent.play(soundEntity)
    AudioComponent.change_volume(soundEntity, volume)

    print(string.format("[Lua] Playing 'Client/Assets/test.wav' at volume %d", volume))
end

function audio:OnUpdate()
    if not soundEntity then return end

    -- Pause / Resume / Mute
    if Input.get_key_down(Keys.ionix_p) then
        AudioComponent.pause(soundEntity)
        print("[Lua] Paused sound.")
    elseif Input.get_key_down(Keys.ionix_r) then
        AudioComponent.resume(soundEntity)
        print("[Lua] Resumed sound.")
    elseif Input.get_key_down(Keys.ionix_m) then
        AudioComponent.toggle_mute(soundEntity)
        print("[Lua] Toggled mute.")
    end

    -- Volume controls
    if Input.get_key_down(Keys.ionix_minus) then
        volume = Mafs.clamp(volume - 8, 0, 128)
        AudioComponent.change_volume(soundEntity, volume)
        print(string.format("[Lua] Volume decreased to %d", volume))
    elseif Input.get_key_down(Keys.ionix_equals) then
        volume = Mafs.clamp(volume + 8, 0, 128)
        AudioComponent.change_volume(soundEntity, volume)
        print(string.format("[Lua] Volume increased to %d", volume))
    end
end

function audio:OnShutdown()
    if soundEntity and AudioComponent.is_playing(soundEntity) then
        AudioComponent.terminate(soundEntity)
        print("[Lua] Sound stopped on shutdown.")
    end
    print("[Lua] Audio system shutting down...")
end

function audio:play_hit()
    local hitEntity = Entity.create_entity()
    Entity.add_audio_component(hitEntity, "hit", false)
    AudioComponent.play(hitEntity)
end

return audio
