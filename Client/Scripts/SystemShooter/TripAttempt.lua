local SystemShooterTrip = {}

local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local ParticleSystem = require("Scripts.SystemShooter.ParticleSystem")
local PsychedelicTripDemo = require("Scripts.SystemShooter.PsychedelicTripDemo")

local screenW = 1920
local screenH = 1080

local musicEntity
local musicVolume = 32
local musicMuted = false

local function GetDt()
    return Mafs.delta_time()
end

function SystemShooterTrip:OnStart()
    local targetW = 1920
    local targetH = 1080

    local displayWidth = Window.get_display_width()
    local displayHeight = Window.get_display_height()
    local newX = math.floor((displayWidth - targetW) * 0.5)
    local newY = math.floor((displayHeight - targetH) * 0.5)

    Window.set_pos(newX, newY)
    Window.set_size(targetW, targetH)

    screenW = targetW
    screenH = targetH

    musicEntity = Entity.create_entity()
    Entity.add_audio_component(musicEntity, "technoSong", false)
    AudioComponent.play(musicEntity, 0, -1)
    AudioComponent.change_volume(musicEntity, musicVolume)
end

function SystemShooterTrip:OnUpdate()
    local dt = GetDt()

    screenW = Window.get_width()
    screenH = Window.get_height()

    if Input.get_key_down(Keys.ionix_m) then
        musicMuted = not musicMuted
        if musicEntity then
            local targetVolume = musicMuted and 0 or musicVolume
            AudioComponent.change_volume(musicEntity, targetVolume)
        end
    end

    PsychedelicTripDemo.update(dt, screenW, screenH)

    ParticleSystem.update(dt)
end

return SystemShooterTrip
