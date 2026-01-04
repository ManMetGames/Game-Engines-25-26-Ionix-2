local SystemShooterAudio = {}

 --=====================================================================
 --  [SOUND EFFECTS] Entity References
 --=====================================================================
local playerDamageSfxEntity
local gunshot3SfxEntity
local impact3SfxEntity
local beamSfxEntity
local lightning1SfxEntity
local lightning2SfxEntity
local powerUp1SfxEntity
local powerUp2SfxEntity
local powerDown1SfxEntity
local powerDown2SfxEntity

-- Lightning alternator (toggles between lightning1 and lightning2)
local lightningToggle = false

 --=====================================================================
 --  [INIT] Create Sound Effect Entities
 --=====================================================================
function SystemShooterAudio.init()
    playerDamageSfxEntity = Entity.create_entity()
    Entity.add_audio_component(playerDamageSfxEntity, "playerDamage", false)

    gunshot3SfxEntity = Entity.create_entity()
    Entity.add_audio_component(gunshot3SfxEntity, "gunshot3", false)

    impact3SfxEntity = Entity.create_entity()
    Entity.add_audio_component(impact3SfxEntity, "impact3", false)

    beamSfxEntity = Entity.create_entity()
    Entity.add_audio_component(beamSfxEntity, "beam", false)

    lightning1SfxEntity = Entity.create_entity()
    Entity.add_audio_component(lightning1SfxEntity, "lightning1", false)

    lightning2SfxEntity = Entity.create_entity()
    Entity.add_audio_component(lightning2SfxEntity, "lightning2", false)

    powerUp1SfxEntity = Entity.create_entity()
    Entity.add_audio_component(powerUp1SfxEntity, "powerUp1", false)

    powerUp2SfxEntity = Entity.create_entity()
    Entity.add_audio_component(powerUp2SfxEntity, "powerUp2", false)

    powerDown1SfxEntity = Entity.create_entity()
    Entity.add_audio_component(powerDown1SfxEntity, "powerDown1", false)

    powerDown2SfxEntity = Entity.create_entity()
    Entity.add_audio_component(powerDown2SfxEntity, "powerDown2", false)
end

 --=====================================================================
 --  [VOLUME] Apply Volume Settings
 --=====================================================================
function SystemShooterAudio.applyVolumes(masterVol, sfxVol)
    local sfxMul = masterVol * sfxVol
    if playerDamageSfxEntity then AudioComponent.change_volume(playerDamageSfxEntity, math.floor(48 * sfxMul + 0.5)) end
    if gunshot3SfxEntity     then AudioComponent.change_volume(gunshot3SfxEntity,     math.floor(32 * sfxMul + 0.5)) end
    if impact3SfxEntity      then AudioComponent.change_volume(impact3SfxEntity,      math.floor(64 * sfxMul + 0.5)) end
    if beamSfxEntity         then AudioComponent.change_volume(beamSfxEntity,         math.floor(64 * sfxMul + 0.5)) end
    if lightning1SfxEntity   then AudioComponent.change_volume(lightning1SfxEntity,   math.floor(64 * sfxMul + 0.5)) end
    if lightning2SfxEntity   then AudioComponent.change_volume(lightning2SfxEntity,   math.floor(64 * sfxMul + 0.5)) end
    if powerUp1SfxEntity     then AudioComponent.change_volume(powerUp1SfxEntity,     math.floor(64 * sfxMul + 0.5)) end
    if powerUp2SfxEntity     then AudioComponent.change_volume(powerUp2SfxEntity,     math.floor(64 * sfxMul + 0.5)) end
    if powerDown1SfxEntity   then AudioComponent.change_volume(powerDown1SfxEntity,   math.floor(64 * sfxMul + 0.5)) end
    if powerDown2SfxEntity   then AudioComponent.change_volume(powerDown2SfxEntity,   math.floor(64 * sfxMul + 0.5)) end
end

 --=====================================================================
 --  [PLAY] Sound Effect Playback Functions
 --=====================================================================
function SystemShooterAudio.playPlayerDamage()
    if playerDamageSfxEntity then
        AudioComponent.play(playerDamageSfxEntity)
    end
end

function SystemShooterAudio.playGunshot()
    if gunshot3SfxEntity then
        AudioComponent.play(gunshot3SfxEntity)
    end
end

function SystemShooterAudio.playImpact()
    if impact3SfxEntity then
        AudioComponent.play(impact3SfxEntity)
    end
end

function SystemShooterAudio.playBeam()
    if beamSfxEntity then
        AudioComponent.play(beamSfxEntity)
    end
end

function SystemShooterAudio.playLightning1()
    if lightning1SfxEntity then
        AudioComponent.play(lightning1SfxEntity)
    end
end

function SystemShooterAudio.playLightning2()
    if lightning2SfxEntity then
        AudioComponent.play(lightning2SfxEntity)
    end
end

function SystemShooterAudio.playLightningAlternate()
    lightningToggle = not lightningToggle
    if lightningToggle then
        SystemShooterAudio.playLightning1()
    else
        SystemShooterAudio.playLightning2()
    end
end

function SystemShooterAudio.playPowerUpRandom()
    if math.random() < 0.5 then
        if powerUp1SfxEntity then
            AudioComponent.play(powerUp1SfxEntity)
        end
    else
        if powerUp2SfxEntity then
            AudioComponent.play(powerUp2SfxEntity)
        end
    end
end

function SystemShooterAudio.playPowerDownRandom()
    if math.random() < 0.5 then
        if powerDown1SfxEntity then
            AudioComponent.play(powerDown1SfxEntity)
        end
    else
        if powerDown2SfxEntity then
            AudioComponent.play(powerDown2SfxEntity)
        end
    end
end

return SystemShooterAudio
