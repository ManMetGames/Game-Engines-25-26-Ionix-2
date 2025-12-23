local SystemShooter = {}

 --=====================================================================
 --  [MODULE] Imports / Dependencies
 --=====================================================================
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local SystemShooterLevels = require("Scripts.SystemShooter.SystemShooterLevels")
local SystemShooterEnemy = require("Scripts.SystemShooter.SystemShooterEnemy")
local SystemShooterPlayerProgress = require("Scripts.SystemShooter.SystemShooterPlayerProgress")
local ParticleSystem = require("Scripts.SystemShooter.ParticleSystem")
local SystemShooterPickups = require("Scripts.SystemShooter.SystemShooterPickups")
local SystemShooterRewind = require("Scripts.SystemShooter.SystemShooterRewind")
local SystemShooterProjectiles = require("Scripts.SystemShooter.SystemShooterProjectiles")
local SystemShooterPlayer = require("Scripts.SystemShooter.SystemShooterPlayer")
local Localisation = require("Scripts.SystemShooter.Localisation")


 --=====================================================================
 --  [LEADERBOARD / SAVE DATA / LANGUAGE]
 --=====================================================================
local GAME_ID = "SYSTEM_SHOOTER"

-- Highest stage reached (persisted locally per-game)
local bestStage = Json.load_high_score(GAME_ID) or 0

-- Persistent player name (shared across games)
local playerName = Json.load_player_name()
if playerName == nil then playerName = "" end
local needsPlayerName = (playerName == "")

local showNamePrompt = false
local pendingStartAfterName = false
local namePromptError = ""
local function T(key) return Localisation.t(key) end
local language = Json.load_setting(GAME_ID, "ui.language", "en") or "en"
Localisation.set_language(language)
local SystemShooterUI = require("Scripts.SystemShooter.SystemShooterUI")
local UI_FONT_BOLD = "ImGuiDefaultBold"
local UI_FONT_REG  = "ImGuiDefault"

TS_UI = TS_UI or {}
local function SyncTSUI()
    TS_UI.T = function(key) return Localisation.t(key) end
    TS_UI.FONT_REG = UI_FONT_REG
    TS_UI.FONT_BOLD = UI_FONT_BOLD
    TS_UI.FONT_SUB = UI_FONT_SUB
    TS_UI.FONT_HEADER = UI_FONT_HEADER
    TS_UI.FONT_TITLE = UI_FONT_TITLE
end

local function ApplyLanguageFonts()
    if language == "ja" then
      UI_FONT_REG   = "JFDotJP"
      UI_FONT_BOLD  = "JFDotBoldJP"
      UI_FONT_SUB = "JFDotSubJP"
      UI_FONT_HEADER = "JFDotHeaderJP"
      UI_FONT_TITLE = "JFDotTitleJP"
    else
      UI_FONT_REG   = "ImGuiDefault"
      UI_FONT_BOLD  = "ImGuiDefaultBold"
      UI_FONT_SUB = "ImGuiSub"
      UI_FONT_HEADER = "ImGuiHeader"
      UI_FONT_TITLE = "ImGuiTitle"
    end
end

ApplyLanguageFonts()
SyncTSUI()
-- For other modules to access localisation and fonts
 --=====================================================================
 --  [Settings] per-game saved settings
 --=====================================================================
 -- Audio
local masterVol = Json.load_setting(GAME_ID, "audio.master", 0.80) or 0.80
local musicVol  = Json.load_setting(GAME_ID, "audio.music",  0.80) or 0.80
local sfxVol    = Json.load_setting(GAME_ID, "audio.sfx",    0.80) or 0.80

-- WIP
local musicEntity
local musicVolume = 128
local musicMuted = false
local bpm = 133 
local secondsPerBeat = 60.0 / bpm
local beatTimer = 0
local bopDurationSeconds = 8 / 60.0
local bopTimer = 0
local bopScale = 0.25
local beatStartDelaySeconds = 14.4  -- Drop happens at 14.4 seconds (start of 9th bar)
local beatBopHasStarted = false

-- Music rewind sync
local rememberedMusicPosition = 0    -- Position to restore after rewind
local musicRewindStartPosition = 0   -- Position when rewind starts
local musicBaseVolume = 1.0          -- Base volume before rewind effects
local lastRewindPhase = nil          -- Track phase changes to avoid re-setting speed every frame

-- Controls
local sensitivitySetting = Json.load_setting(GAME_ID, "controls.sensitivity", 1.0) or 1.0
local function Clamp(v, a, b)
    if v < a then return a end
    if v > b then return b end
    return v
end
sensitivitySetting = Clamp(sensitivitySetting, 0.25, 2.50)

 --=====================================================================
 --  [STATE] Main/Pause Menus
 --=====================================================================
-- Menu screens: "main" | "leaderboard" | "pause"
local UI__COLOUR_THEME = { 44, 8, 160 }
local menuScreen = "main"
local topLeaderboard = nil
local leaderboardFetched = false
local showDifficultyMenu = false
local diffChosen = false
local isPaused = false
local pauseScreen = "pause" -- "pause" | "settings" | "leaderboard"

-- Window resize for pause menu
local prePauseWindowW = nil
local prePauseWindowH = nil
local MIN_PAUSE_W = 1000
local MIN_PAUSE_H = 700

local NO_BACKGROUND = 128

local function GetPlayerNameForLeaderboard()
    if playerName and playerName ~= "" then
        return playerName
    end
    return "Anon"
end

local function TryUpdateBestStage(stage)
    if stage == nil then return end
    stage = math.floor(stage)
    if stage > (bestStage or 0) then
        bestStage = stage
        Json.save_high_score(GAME_ID, bestStage)
    end
end

local pendingQuit = false

-- Orange timer customisation
local startTimerStyle = {
    rounding = 10,
    border_size = 0,

    bg     = { 255, 153, 0, 35 },    
    fill   = { 255, 153, 0, 255 },
}
 --=====================================================================
 --  [HELPERS] Time
 --=====================================================================
local function GetDt()
    return Mafs.delta_time()
end

-- Mutated level config tracking (enemy HP cap / rewind)
local mutatedLevels = {}
local hpCap_waitingForFull = false
local hpCap_hasBeenHit = false


 --=====================================================================
 --  [STATE] Screen / Window
 --=====================================================================
-- SCREEN BOUNDS (UPDATED EACH FRAME FROM WINDOW SIZE)
local screenW = 1920
local screenH = 1080

-- Window transition state (for level changes)
local windowTransitionActive = false
local windowTransitionProgress = 0  -- 0 to 1
local windowTransitionSpeed = 300   -- pixels per second
local windowTransitionStartW = 0
local windowTransitionStartH = 0
local windowTransitionTargetW = 0
local windowTransitionTargetH = 0
local windowTransitionTotalPixels = 0  -- total distance to travel
local pendingLevelIndex = nil
local pendingResetPlayerState = false
local playerInitialized = false  -- Track if player has been initialized (delayed until first transition completes)

-- Transition phase state machine (to prevent lag spikes)
local transitionPhase = "none"  -- "none" | "pre" | "window" | "post"
local transitionTimer = 0
local PRE_TRANSITION_DELAY = 0.15   -- seconds before window resize starts
local POST_TRANSITION_DELAY = 0.3  -- seconds after window resize ends


-- ENEMY (CUBE)
local enemySize = 48

local enemies = {}
local levelEnemyHealth = 50
local StartLevel

-- Overhealth ring VFX
local overhealthRingId = nil  -- VFX ring ID for overhealth effect
local wasOverhealthActive = false  -- Track previous state to detect transitions    

local function GetActiveEnemyCount()
    local count = 0
    for i = 1, #enemies do
        if not enemies[i].disabled and not enemies[i].isDead then
            count = count + 1
        end
    end
    return count
end

local function IsNoWitnessesActive()
    local stacks = SystemShooterPlayerProgress.getLowEnemyDamageStacks()
    if stacks <= 0 then return false end
    local activeCount = GetActiveEnemyCount()
    -- NoWitnesses only active when enemies are alive
    if activeCount <= 0 then return false end
    
    if stacks == 1 then
        return activeCount == 1
    else -- stacks >= 2
        return activeCount <= 3
    end
end

local LoadLevel

local function CreateEnemy(x, y, config)
    config = config or {}
    return SystemShooterEnemy.createEnemy(x, y, config)
end

 --=====================================================================
 --  [LEVEL FLOW] Window Transition
 --=====================================================================
function UpdateWindowTransition(dt)
    if not windowTransitionActive then
        return false
    end

    -- Check if transition is complete
    if windowTransitionProgress >= 1.0 then
        windowTransitionActive = false
        windowTransitionProgress = 0
        screenW = Window.get_width()
        screenH = Window.get_height()
        
        -- Initialize player after first window transition (MainMenu -> Level 1) completes
        if not playerInitialized then
            SystemShooterPlayer.init({
                assets = assets,
                screenW = screenW,
                screenH = screenH,
            })
            -- Skip first mouse delta to prevent spawn position snap
            SystemShooterPlayer.skipNextDelta()
            playerInitialized = true
            
            -- Start music after first window transition completes (synced with gameplay)
            if musicEntity and not musicStartedThisLaunch then
                MusicComponent.play(musicEntity, true, 2.0)  -- loop=true, fadeIn=2.0 seconds
                musicStartedThisLaunch = true
                -- Reset beat bop flag to sync with music start
                beatBopHasStarted = false
            end
        end
        
        -- Transition to post-phase (delay before loading level)
        transitionPhase = "post"
        transitionTimer = POST_TRANSITION_DELAY
        return true  -- Still transitioning (in post-phase)
    end

    -- Calculate progress increment based on speed (pixels per second)
    local progressIncrement = 0
    if windowTransitionTotalPixels > 0 then
        progressIncrement = (windowTransitionSpeed * dt) / windowTransitionTotalPixels
    else
        progressIncrement = 1.0  -- Instant if no distance
    end
    
    windowTransitionProgress = windowTransitionProgress + progressIncrement
    if windowTransitionProgress > 1.0 then
        windowTransitionProgress = 1.0
    end

    local t = windowTransitionProgress
    local startW = windowTransitionStartW
    local startH = windowTransitionStartH
    local targetW = windowTransitionTargetW
    local targetH = windowTransitionTargetH

    local newW = startW + (targetW - startW) * t
    local newH = startH + (targetH - startH) * t

    local newWidth = math.floor(newW + 0.5)
    local newHeight = math.floor(newH + 0.5)

    local displayWidth = Window.get_display_width()
    local displayHeight = Window.get_display_height()
    local newX = math.floor((displayWidth - newWidth) * 0.5)
    local newY = math.floor((displayHeight - newHeight) * 0.5)

    Window.set_pos(newX, newY)
    Window.set_size(newWidth, newHeight)
    
    -- Scale player position proportionally to maintain relative position
    local oldScreenW = screenW
    local oldScreenH = screenH
    screenW = newWidth
    screenH = newHeight
    
    if oldScreenW > 0 and oldScreenH > 0 then
        local pX, pY = SystemShooterPlayer.getPosition()
        local pSize = SystemShooterPlayer.getSize()
        -- Calculate relative position (0-1) in old screen
        local relX = (pX + pSize/2) / oldScreenW
        local relY = (pY + pSize/2) / oldScreenH
        -- Apply to new screen size
        local newPX = relX * screenW - pSize/2
        local newPY = relY * screenH - pSize/2
        SystemShooterPlayer.setScreenBounds(screenW, screenH)
        SystemShooterPlayer.setPosition(newPX, newPY)
        
        -- Update overhealth ring position if active
        if overhealthRingId and overhealthRingId >= 0 then
            local playerCenterX = newPX + pSize / 2
            local playerCenterY = newPY + pSize / 2
            VFX.set_ring_position(overhealthRingId, playerCenterX, playerCenterY)
        end
    end
    
    return true  -- Transition still active
end

local function ClearEnemies()
    SystemShooterEnemy.clearEnemies(enemies)
    enemies = {}
end

local function ClearAllPlayerProjectiles()
    SystemShooterProjectiles.clearAllPlayerProjectiles()
end

local function ClearAllEnemyProjectiles()
    SystemShooterProjectiles.clearAllEnemyProjectiles()
end


local function RestoreMutatedLevelConfigs()
    for levelIdx, _ in pairs(mutatedLevels) do
        local cfg = SystemShooterLevels.getLevelConfig(levelIdx)
        if cfg then
            if cfg.enemies then
                for _, enemy in ipairs(cfg.enemies) do
                    if enemy.base_health ~= nil then
                        enemy.health = enemy.base_health
                        enemy.base_health = nil
                    end
                end
            end
            if cfg.base_enemyHealth ~= nil then
                cfg.enemyHealth = cfg.base_enemyHealth
                cfg.base_enemyHealth = nil
            end
        end
        mutatedLevels[levelIdx] = nil
    end
end

local function ResetRunStateForMenu()
    RestoreMutatedLevelConfigs()
    hpCap_waitingForFull = false
    hpCap_hasBeenHit = false
    -- cancel any pending window level loads just in case
    windowTransitionActive = false
    pendingLevelIndex = nil
    pendingResetPlayerState = false

    ClearEnemies()
    ClearAllPlayerProjectiles()
    ClearAllEnemyProjectiles()
    
    -- Clean up overhealth ring VFX
    if overhealthRingId and overhealthRingId >= 0 then
        VFX.destroy_ring(overhealthRingId)
        overhealthRingId = nil
    end
    wasOverhealthActive = false

    -- Reset player state via module
    SystemShooterPlayer.resetForRun()
    
    peaceTimerSeconds = 0
    isStartLevelPeace = false
    isLevelupPeace = false

    -- Reset rewind state
    SystemShooterRewind.reset()

    -- reset stage/run basics
    currentLevel = 1
    levelTimerSeconds = 0

    isPaused = false
    pauseScreen = "pause"
end

 --=====================================================================
 --  [STATE] Enemy Projectiles (now managed by SystemShooterProjectiles module)
 --=====================================================================
local enemyShootIntervalSeconds = 0.5

-- COLLISION SETTINGS
local collisionRadius = 24  -- Half of enemy size for circle collision

-- FLASH EFFECT
local flashTimer = 0
local flashDuration = 0.2  -- seconds

local enemyBaseImageSize = enemySize

local globalFrame = 0
local peaceTimerSeconds = 0
local startLevelPeaceDuration = 3
local endLevelPeaceDuration = 4.0
local levelupPeaceDuration = 2.0
local isStartLevelPeace = false  -- true = waiting before enemies spawn, false = end-of-level peace
local isLevelupPeace = false     -- true = peace period after selecting a levelup upgrade

-- SFX
local playerDamageSfxEntity
local gunshot3SfxEntity
local impact3SfxEntity

-- LEVEL SETTINGS
local currentLevel = 1
local levelTimerSeconds = 0
local levelXpGained = 0
local notificationMessage = ""
local notificationTimer = 0

-- Run stats (per attempt; used for Game Over summary)
local runTimeSeconds = 0
local runEnemiesKilled = 0
local runShotsFired = 0
local runShotsHit = 0 -- fired projectiles that hit at least once
local runDamageDealt = 0
local runDamageTaken = 0
local runHealingCollected = 0


-- Leaderboard sync (submit once per run, on Game Over)
local runLeaderboardSubmitted = false
-- Game Over state + snapshot
local isGameOver = false
local endRunSummary = nil
-- Main Menu
local inMainMenu = true
local menuStarting = false
local menuStartDelay = 2
local menuStartTimer = 0
local musicStartedThisLaunch = false
local MAIN_MENU_W, MAIN_MENU_H = 1280, 720

--=====================================================================
--  [RUN STATS] Helpers
--=====================================================================
local function ResetRunStats()
    runTimeSeconds = 0
    runEnemiesKilled = 0
    runShotsFired = 0
    runShotsHit = 0
    runDamageDealt = 0
    runDamageTaken = 0
    runHealingCollected = 0
    endRunSummary = nil
end

local function FormatTimeMMSS(seconds)
    seconds = seconds or 0
    if seconds < 0 then seconds = 0 end
    local total = math.floor(seconds + 0.5)
    local mm = math.floor(total / 60)
    local ss = total % 60
    return string.format("%02d:%02d", mm, ss)
end

local function CaptureEndRunSummary()
    local firepower = SystemShooterPlayerProgress.getFirepower()
    local pierce  = SystemShooterPlayerProgress.getPierceCount()
    local bounce  = SystemShooterPlayerProgress.getBounceCount()
    local fireI   = SystemShooterPlayerProgress.getCurrentFireInterval()
    local maxHP   = SystemShooterPlayerProgress.getMaxHealth()

    local pLevel, pXp, pXpToNext = SystemShooterPlayerProgress.getProgress()

    return {
        stageReached = currentLevel or 1,
        playerLevel = pLevel or 1,
        xp = pXp or 0,
        timeSurvived = runTimeSeconds or 0,
        enemiesKilled = runEnemiesKilled or 0,
        shotsFired = runShotsFired or 0,
        shotsHit = runShotsHit or 0,
        damageDealt = runDamageDealt or 0,
        damageTaken = runDamageTaken or 0,
        healingCollected = runHealingCollected or 0,
        build = {
            firepower = firepower or 1,
            pierce = pierce or 0,
            bounce = bounce or 0,
            fireInterval = fireI or 0.5,
            maxHealth = maxHP or 100,
        }
    }
end

local function TriggerGameOver()
    if isGameOver then return end
    isGameOver = true
    endRunSummary = CaptureEndRunSummary()
    -- show cursor for menu
    Input.set_relative_mouse_mode(false)
    -- stop firing immediately
    SystemShooterPlayer.stopFiring()
    -- Stop music when run ends
    if musicEntity then
        MusicComponent.stop(musicEntity, 0)  -- fadeOut=0
        musicStartedThisLaunch = false
    end
end

 --=====================================================================
 --  [SETTINGS] Audio Volume
 --=====================================================================

local function Clamp01(v)
    if v < 0 then return 0 end
    if v > 1 then return 1 end
    return v
end

local function ApplyAudioVolumes()
    masterVol = Clamp01(masterVol or 0.8)
    musicVol  = Clamp01(musicVol  or 0.8)
    sfxVol    = Clamp01(sfxVol    or 0.8)

    -- MUSIC (SoLoud uses 0.0..1.0 range)
    local musicOut = masterVol * musicVol
    if musicEntity then
        MusicComponent.set_volume(musicEntity, musicMuted and 0 or musicOut)
    end

    -- SFX (scale your existing base volumes)
    local sfxMul = masterVol * sfxVol
    if playerDamageSfxEntity then AudioComponent.change_volume(playerDamageSfxEntity, math.floor(48 * sfxMul + 0.5)) end
    if gunshot3SfxEntity     then AudioComponent.change_volume(gunshot3SfxEntity,     math.floor(32 * sfxMul + 0.5)) end
    if impact3SfxEntity      then AudioComponent.change_volume(impact3SfxEntity,      math.floor(64 * sfxMul + 0.5)) end
end


 --=====================================================================
 --  [LEVEL FLOW] Load / Start Level
 --=====================================================================
local function SpawnEnemiesForLevel(spawnDisabled)
    local cfg = SystemShooterLevels.getLevelConfig(currentLevel)
    if not cfg then
        return
    end

    -- Clear spawn positions for rewind system
    SystemShooterRewind.clearSpawnPositions()

    local centerX = screenW / 2 - enemySize / 2
    local centerY = screenH / 2 - enemySize / 2

    local enemyTemplates = SystemShooterLevels.getEnemyTemplates()
    
    if cfg.enemies then
        for i, enemyCfg in ipairs(cfg.enemies) do
            local movementType = enemyCfg.movementType or "bounce"
            local template = enemyTemplates[movementType]
            local templateSize = template and template.baseSize or 32
            
            local spawnX = enemyCfg.x or centerX
            local spawnY = enemyCfg.y or centerY
            
            if not enemyCfg.x and not enemyCfg.y and #cfg.enemies > 1 then
                local radius = 120
                local angle = (2 * math.pi * (i - 1)) / #cfg.enemies
                spawnX = screenW / 2 + math.cos(angle) * radius - templateSize / 2
                spawnY = screenH / 2 + math.sin(angle) * radius - templateSize / 2
            end
            
            local config = {
                health = enemyCfg.health or levelEnemyHealth,
                healthScaling = enemyCfg.healthScaling,
                size = templateSize,
                color = enemyCfg.color,
                speed = enemyCfg.speed,
                baseSpeed = enemyCfg.baseSpeed,
                movementType = enemyCfg.movementType,
                shootPattern = enemyCfg.shootPattern,
                projectileCount = enemyCfg.projectileCount,
                shootInterval = enemyCfg.shootInterval or enemyShootIntervalSeconds,
                spinWhileShooting = enemyCfg.spinWhileShooting,
                bounceSteer = enemyCfg.bounceSteer,
                steerStrength = enemyCfg.steerStrength,
                orbitCenter = enemyCfg.orbitCenter,
                orbitRadius = enemyCfg.orbitRadius,
                orbitSpeed = enemyCfg.orbitSpeed,
            }
            
            local e = CreateEnemy(spawnX, spawnY, config)
            if spawnDisabled then
                SystemShooterEnemy.setEnemyDisabled(e, true)
            end
            table.insert(enemies, e)
            -- Store spawn position for rewind system
            SystemShooterRewind.setSpawnPosition(#enemies, spawnX, spawnY)
        end
    else
        local enemyCount = cfg.enemyCount or 1
        local defaultConfig = {
            health = levelEnemyHealth,
            shootInterval = enemyShootIntervalSeconds,
            size = 32,
        }
        
        if enemyCount == 1 then
            local e = CreateEnemy(centerX, centerY, defaultConfig)
            if spawnDisabled then
                SystemShooterEnemy.setEnemyDisabled(e, true)
            end
            table.insert(enemies, e)
            -- Store spawn position for rewind system
            SystemShooterRewind.setSpawnPosition(#enemies, centerX, centerY)
        else
            local radius = 120
            local playerCenterX = screenW / 2
            local playerCenterY = screenH / 2
            for i = 1, enemyCount do
                local angle = (2 * math.pi * (i - 1)) / enemyCount
                local ex = playerCenterX + math.cos(angle) * radius - enemySize / 2
                local ey = playerCenterY + math.sin(angle) * radius - enemySize / 2
                local e = CreateEnemy(ex, ey, defaultConfig)
                if spawnDisabled then
                    SystemShooterEnemy.setEnemyDisabled(e, true)
                end
                table.insert(enemies, e)
                -- Store spawn position for rewind system
                SystemShooterRewind.setSpawnPosition(#enemies, ex, ey)
            end
        end
    end
end

LoadLevel = function(index, resetPlayerState)
    local cfg = SystemShooterLevels.getLevelConfig(index)
    if not cfg then
        return
    end

     --=====================================================================
     --  [LOAD LEVEL] Apply Config / Global Toggles
     --=====================================================================

    currentLevel = index

    -- Persist best stage + submit to leaderboard (only if higher stage than last time)
    TryUpdateBestStage(currentLevel)
    levelTimerSeconds = cfg.timeLimitSeconds or 0
    levelXpGained = 0
    notificationMessage = ""
    notificationTimer = 0

    ClearEnemies()

    levelEnemyHealth = cfg.enemyHealth or levelEnemyHealth
    enemyShootIntervalSeconds = cfg.enemyShootIntervalSeconds or enemyShootIntervalSeconds

    -- Spawn enemies as disabled (preview state) during start-level peace
    SpawnEnemiesForLevel(true)
    
    -- Remember music position for rewind restoration
    if musicEntity and MusicComponent.is_playing(musicEntity) then
        rememberedMusicPosition = MusicComponent.get_position(musicEntity)
    end
    
    -- Start the start-level peace timer (enemies will be enabled when it expires)
    peaceTimerSeconds = startLevelPeaceDuration
    isStartLevelPeace = true

     --=====================================================================
     --  [LOAD LEVEL] Reset Player State
     --=====================================================================
    if resetPlayerState then
        SystemShooterPlayer.setScreenBounds(screenW, screenH)
        SystemShooterPlayer.resetForLevel(true)
    end

    local playerHealth = SystemShooterPlayer.getHealth()
    if playerHealth == nil or playerHealth <= 0 then
        SystemShooterPlayer.setHealth(SystemShooterPlayerProgress.getMaxHealth())
    end
end

StartLevel = function(index, resetPlayerState)
    local cfg = SystemShooterLevels.getLevelConfig(index)
    if not cfg then
        return
    end

    local currentWidth = Window.get_width()
    local currentHeight = Window.get_height()
    local targetWidth = cfg.windowWidth or currentWidth
    local targetHeight = cfg.windowHeight or currentHeight

    if currentWidth == targetWidth and currentHeight == targetHeight then
        screenW = currentWidth
        screenH = currentHeight
        LoadLevel(index, resetPlayerState)
        return
    end

    -- Calculate total pixel distance for speed-based transition
    local dw = targetWidth - currentWidth
    local dh = targetHeight - currentHeight
    local totalPixels = math.sqrt(dw * dw + dh * dh)

    -- Store window transition parameters
    windowTransitionStartW = currentWidth
    windowTransitionStartH = currentHeight
    windowTransitionTargetW = targetWidth
    windowTransitionTargetH = targetHeight
    windowTransitionTotalPixels = totalPixels
    pendingLevelIndex = index
    pendingResetPlayerState = resetPlayerState
    
    -- Start pre-transition phase (lightweight cleanup)
    transitionPhase = "pre"
    transitionTimer = PRE_TRANSITION_DELAY
    
    -- Do lightweight cleanup immediately
    ClearAllPlayerProjectiles()
    ClearAllEnemyProjectiles()
    if playerInitialized then
        SystemShooterPlayer.stopFiring()
    end
end

local function OnEnemyKilled()
    local nextIndex = currentLevel + 1
    TryUpdateBestStage(nextIndex)
    SystemShooterLevels.regenerateLevel(nextIndex)
    if SystemShooterLevels.getLevelConfig(nextIndex) then
        StartLevel(nextIndex, false)
    else
        StartLevel(currentLevel, false)
    end
end

local function StartRewindSequence()
    -- Get level config for time limit and enemy target health
    local cfg = SystemShooterLevels.getLevelConfig(currentLevel)
    local levelTimeLimit = cfg and cfg.timeLimitSeconds or 0
    
    -- Build target health list from level config (these are the reduced values after timeout)
    local enemyTargetHealthList = {}
    if cfg and cfg.enemies then
        for i, enemyCfg in ipairs(cfg.enemies) do
            enemyTargetHealthList[i] = enemyCfg.health or levelEnemyHealth
        end
    else
        -- Fallback: use current enemy health as target
        for i, enemy in ipairs(enemies) do
            enemyTargetHealthList[i] = enemy.health or 0
        end
    end
    
    -- Start the rewind sequence via the module
    SystemShooterRewind.start(enemies, levelTimeLimit, enemyTargetHealthList)
    
    -- Start music slowdown effect
    if musicEntity and MusicComponent.is_playing(musicEntity) then
        musicRewindStartPosition = MusicComponent.get_position(musicEntity)
        musicBaseVolume = masterVol * musicVol
        -- Seek back 10 seconds before starting rewind for smoother effect
        local newPosition = math.max(0, musicRewindStartPosition - 10.0)
        MusicComponent.seek(musicEntity, newPosition)
        print("[Rewind Music] Seeking back 10 seconds: " .. tostring(musicRewindStartPosition) .. " -> " .. tostring(newPosition))
        -- Music will slow down and fade out during the "slowing" phase (handled in update)
    end
end


local function OnLevelTimeout()
    -- Increment timeout counter
    local currentTimeouts = SystemShooterPlayerProgress.incrementTimeoutCount()
    local maxTimeouts = SystemShooterPlayerProgress.getMaxTimeouts()
    
    -- Check if we've reached the timeout limit
    if currentTimeouts >= maxTimeouts then
        -- Set player HP to 0 (this will trigger game over)
        SystemShooterPlayer.setHealth(0)
        return
    end
    
    -- Otherwise, proceed with normal timeout behavior (reduce enemy health and rewind)
    local cfg = SystemShooterLevels.getLevelConfig(currentLevel)
    if cfg then
        if cfg.enemies then
            for _, enemy in ipairs(cfg.enemies) do
                if enemy.health then
                    if enemy.base_health == nil then
                        enemy.base_health = enemy.health
                    end
                    enemy.health = math.floor(enemy.health * 0.8)
                    if enemy.health < 1 then enemy.health = 1 end
                end
            end
        else
            if cfg.enemyHealth ~= nil then
                if cfg.base_enemyHealth == nil then
                    cfg.base_enemyHealth = cfg.enemyHealth
                end
                cfg.enemyHealth = math.floor(cfg.enemyHealth * 0.8)
                if cfg.enemyHealth < 1 then cfg.enemyHealth = 1 end
            end
        end
    end

    mutatedLevels[currentLevel] = true

    hpCap_waitingForFull = true
    hpCap_hasBeenHit = false

    StartRewindSequence()
end


 --=====================================================================
 --  [ENGINE CALLBACKS] OnStart
 --=====================================================================
function SystemShooter:OnStart()
    -- Enable relative mouse mode (hides cursor, gives delta movement)
    Input.set_relative_mouse_mode(false)
    
    
    -- Initialize projectile module with callbacks
    SystemShooterProjectiles.init({
        callbacks = {
            getEnemies = function() return enemies end,
            getPlayerPos = function() 
                local pX, pY = SystemShooterPlayer.getPosition()
                local pSize = SystemShooterPlayer.getSize()
                return pX, pY, pSize
            end,
            isNoWitnessesActive = IsNoWitnessesActive,
            getNoWitnessesDamageMultiplier = function() return SystemShooterPlayerProgress.getNoWitnessesDamageMultiplier() end,
            getLowEnemyDamageStacks = function() return SystemShooterPlayerProgress.getLowEnemyDamageStacks() end,
            getActiveEnemyCount = GetActiveEnemyCount,
            onEnemyHit = function(enemy, damage, proj)
                FlashEnemy(enemy)
                SystemShooterEnemy.updateDisplaySize(enemy)
                local eSize = enemy.size or enemySize
                local enemyCenterX = enemy.x + eSize / 2
                local enemyCenterY = enemy.y + eSize / 2
                local color = enemy.color or {255, 255, 255}
                ParticleSystem.emitHitBurst(enemyCenterX, enemyCenterY, color[1], color[2], color[3])
                if impact3SfxEntity then
                    AudioComponent.play(impact3SfxEntity)
                end
            end,
            onEnemyKilled = function(enemy, allEnemies)
                local eSize = enemy.size or enemySize
                local deathX = enemy.x + eSize / 2
                local deathY = enemy.y + eSize / 2
                SystemShooterPickups.trySpawnHealingOrb(deathX, deathY)
                runEnemiesKilled = runEnemiesKilled + 1
                enemy.isDead = true
                Entity.set_global_pos(enemy.entity, -1000, -1000)
                local allDead = true
                for _, e in ipairs(allEnemies) do
                    if not e.isDead then
                        allDead = false
                        break
                    end
                end
                if allDead then
                    -- Calculate bonus XP based on remaining time
                    local cfg = SystemShooterLevels.getLevelConfig(currentLevel)
                    local totalTime = cfg and cfg.timeLimitSeconds or 0
                    
                    -- Only award bonus XP if level hasn't been rewound (mutated)
                    if not mutatedLevels[currentLevel] and totalTime > 0 and levelTimerSeconds > 0 then
                        local bonusRatio = levelTimerSeconds / totalTime
                        local bonusXp = math.floor(levelXpGained * 0.35 * bonusRatio)
                        if bonusXp > 0 then
                            SystemShooterPlayerProgress.addXp(bonusXp)
                            notificationMessage = "Bonus XP: " .. bonusXp .. " (Time Left: " .. string.format("%.1f", levelTimerSeconds) .. "s)"
                            notificationTimer = endLevelPeaceDuration
                        end
                    end

                    peaceTimerSeconds = endLevelPeaceDuration
                    isStartLevelPeace = false
                end
            end,
            onPlayerHit = function(damage)
                local currentHealth = SystemShooterPlayer.getHealth()
                local currentOverhealth = SystemShooterPlayerProgress.getOverhealth()
                
                -- Damage overhealth first if present
                if currentOverhealth > 0 then
                    if currentOverhealth >= damage then
                        -- Overhealth absorbs all damage
                        SystemShooterPlayerProgress.setOverhealth(currentOverhealth - damage)
                    else
                        -- Overhealth partially absorbs, remaining damage goes to health
                        local remainingDamage = damage - currentOverhealth
                        SystemShooterPlayerProgress.setOverhealth(0)
                        SystemShooterPlayer.setHealth(currentHealth - remainingDamage)
                    end
                else
                    -- No overhealth, damage health directly
                    SystemShooterPlayer.setHealth(currentHealth - damage)
                end
                
                SystemShooterPlayer.flash()
                SystemShooterPlayer.setDamageCooldown(SystemShooterPlayer.getDamageCooldownDuration())
            end,
            isAntivirusActive = function() return SystemShooterPlayer.isAntivirusActive() end,
            addXp = function(amount) 
                levelXpGained = levelXpGained + amount
                SystemShooterPlayerProgress.addXp(amount) 
            end,
        }
    })

    -- Main menu window size (target)
    local displayWidth = Window.get_display_width()
    local displayHeight = Window.get_display_height()
    local newX = math.floor((displayWidth - MAIN_MENU_W) * 0.5)
    local newY = math.floor((displayHeight - MAIN_MENU_H) * 0.5)
    Window.set_pos(newX, newY)
    Window.set_size(MAIN_MENU_W, MAIN_MENU_H)
    screenW = MAIN_MENU_W
    screenH = MAIN_MENU_H

    StartLevel(1, true)

    musicEntity = Entity.create_entity()
    Entity.add_music_component(musicEntity, "technoSong", false)
    -- Music will start when player presses START GAME (now using SoLoud)

    playerDamageSfxEntity = Entity.create_entity()
    Entity.add_audio_component(playerDamageSfxEntity, "playerDamage", false)
    -- Set player damage SFX volume to half (range is 0-128)

    gunshot3SfxEntity = Entity.create_entity()
    Entity.add_audio_component(gunshot3SfxEntity, "gunshot3", false)

    impact3SfxEntity = Entity.create_entity()
    Entity.add_audio_component(impact3SfxEntity, "impact3", false)

    ApplyAudioVolumes()

end

 --=====================================================================
 --=====================================================================
 --  [DRAW] Name Prompt / Main Menu / Leaderboard / Settings / Pause Menu
 --=====================================================================

local function DrawNamePrompt(screenW, screenH)
    -- darken background
    UI.add_panel(0, 0, screenW, screenH, 0.60, 0, 0, 0, 0)

    local w, h = 520, 220
    local x = math.floor((screenW - w) / 2)
    local y = math.floor((screenH - h) / 2)

    UI.begin_child(x, y, w, h, "TS_NamePrompt",
        true, 0,
        true, 0.96, 12, 28, 28, 28,
        0.1, true, 2
    )

    local cx = w / 2
    UI.add_centered_label(cx, 18, T("play.prompttxt"), UI_FONT_HEADER, 1)
    UI.add_centered_label(cx, 48, T("play.promptdesc"), UI_FONT_REG, 1.2)
    if namePromptError ~= "" then
        UI.add_centered_label_colored(
            cx, 82,
            namePromptError,
            {255, 80, 80, 255},   -- RGBA (0–255)
            UI_FONT_BOLD, 1.0
        )
    end


    local inputW = 300
    local inputX = math.floor((w - inputW) / 2)
    UI.add_input_text(inputX, 98, inputW, "", "ts_player_name", 16)

    local bw, bh = 160, 40
    local bx = math.floor((w - bw) / 2)
    UI.add_button(bx, 150, bw, bh,
        "CONTINUE", "ts_name_ok",
        UI_FONT_BOLD, 1.0,
        10, true,
        74, 12, 255, 0.95
    )

    local committed =
        UI.was_input_committed("ts_player_name") or
        UI.was_button_pressed("ts_name_ok")

    if committed then
        local getName = UI.get_input_text_live or UI.get_input_text
        local name = getName("ts_player_name") or ""
        name = name:gsub("^%s+", ""):gsub("%s+$", "")

        if name == "" then
            namePromptError = T("play.error")
        else
            namePromptError = ""
            playerName = name
            needsPlayerName = false
            showNamePrompt = false

            Json.save_player_name(playerName)
            UI.clear_input("ts_player_name")
            if pendingStartAfterName then
                pendingStartAfterName = false
                showDifficultyMenu = true
            end
        end
    end


    UI.end_child()
end

local function DrawDifficultyPrompt(screenW, screenH)
    -- Dim background
    UI.add_panel(0, 0, screenW, screenH, 0.65, 0, 0, 0, 0)

    local panelW = 610
    local panelH = 420
    local panelX = math.floor((screenW - panelW) / 2)
    local panelY = math.floor((screenH - panelH) / 2+10)

    UI.begin_child(panelX, panelY, panelW, panelH, "TS_Difficulty",
        true, 0,
        true, 1, 12, 25, 25, 25,
        5, true, 1.35
    )

    local cx = panelW / 2
    UI.add_centered_label(cx, 34, T("play.difficulty"), UI_FONT_HEADER, 1.0)

    local bw, bh = math.floor(panelW * 0.62), 56
    local bx = math.floor((panelW - bw) / 2)
    local gap = 16

    -- Reserve space for Back button (and a little padding)
    local backH = 52
    local backY = panelH - 76
    local bottomPadding = 18

    -- Child area for the difficulty buttons
    local listX = bx
    local listY = 90
    local listW = bw
    local listH = (backY - bottomPadding) - listY

    UI.begin_child(listX, listY, listW, listH, "TS_Difficulty_List",
        false, 0,
        true, 0.0, 0, 0, 0, 0,
        0, false, 1.0
    )

    local by = 20 -- inside the child
    UI.add_button(0, by + 0*(bh + gap), listW, bh, T("difficulty.easy"),   "ts_diff_easy", UI_FONT_HEADER, 1, 12, true, 0,   170, 110, 0.95)
    UI.add_button(0, by + 1*(bh + gap), listW, bh, T("difficulty.medium"), "ts_diff_med",  UI_FONT_HEADER, 1, 12, true, 220, 140, 0,   0.95)
    UI.add_button(0, by + 2*(bh + gap), listW, bh, T("difficulty.hard"),   "ts_diff_hard", UI_FONT_HEADER, 1, 12, true, 200, 60,  60,  0.95)

    UI.end_child()

    if not diffChosen then
        if UI.was_button_pressed("ts_diff_easy") then
            diffChosen = true
            menuStarting = true
            menuStartTimer = menuStartDelay

        elseif UI.was_button_pressed("ts_diff_med") then
            diffChosen = true
            menuStarting = true
            menuStartTimer = menuStartDelay

        elseif UI.was_button_pressed("ts_diff_hard") then
            diffChosen = true
            menuStarting = true
            menuStartTimer = menuStartDelay
        end

        UI.add_button(bx, backY, bw, backH, T("menu.back"), "ts_diff_back",
            UI_FONT_HEADER, 1, 12, true, 120, 120, 120, 0.95
        )

        if UI.was_button_pressed("ts_diff_back") then
            showDifficultyMenu = false
        end
    end

    UI.end_child()
end



local function DrawMainMenu(screenW, screenH, dt)
    UI.add_panel(0, 0, screenW, screenH, 0.65, 0, 0, 0, 0)

    -- Bigger, responsive menu panel
    local panelW = math.floor(math.min(screenW - 120, 1000))
    local panelH = math.floor(math.min(screenH - 120, 600))
    panelW = math.max(panelW, 820)
    panelH = math.max(panelH, 520)

    local panelX = math.floor((screenW - panelW) / 2)
    local panelY = math.floor((screenH - panelH) / 2)

    UI.begin_child(panelX, panelY, panelW, panelH, "TS_MainMenu",
        true, 0,
        true, 0.92, 12, 25, 25, 25,
        5, true, 1.35
    )

    local cx = panelW / 2

    -- Title / subtitle anchors (relative to panel height)
    local titleY = math.floor(panelH * 0.08)
    local subY   = titleY + math.floor(panelH * 0.10)

    UI.add_centered_label(cx, titleY, "SYSTEM SHOOTER", "ImGuiTitle", 1.2)
    UI.add_centered_label(cx, subY, T("menu.basiccontrols"), UI_FONT_REG, 1)
    UI.add_centered_label(panelW*0.9, titleY + math.floor(panelH * 0.0), playerName, "ImGuiDefault", 1.2)

    -- Best stage
    UI.add_centered_label(cx, subY + math.floor(panelH * 0.08), T("menu.beststage") .. tostring(bestStage or 0), UI_FONT_SUB, 1)

    -- Buttons
    local bw, bh = math.floor(math.min(panelW * 0.62, 560)), 60
    local bx = math.floor((panelW - bw) / 2)
    local by = math.floor(panelH * 0.38)

    -- Progress bar should sit just above the start button
    if menuStarting then
        menuStartTimer = menuStartTimer - dt
        local elapsed = menuStartDelay - math.max(menuStartTimer, 0)

        -- UI.draw_progress_bar is screen-space, so convert child-space to absolute screen coords
        local barW = math.floor(math.max(200, math.min(bw * 0.75, 320)))
        local barH = 12
        local barX = panelX + bx + math.floor((bw - barW) / 2)
        local barY = panelY + by - barH - 24

        UI.draw_progress_bar_styled(barX, barY, barW, barH, menuStartDelay, elapsed, 4, startTimerStyle, "")

        if menuStartTimer <= 0 then
            menuStarting = false
            showDifficultyMenu = false
            diffChosen = false
            inMainMenu = false
            menuScreen = "main"
            Input.set_relative_mouse_mode(true)
            ResetRunStats()
            SystemShooterPickups.clearAll()
            SystemShooterPlayerProgress.reset()
            isGameOver = false
            runLeaderboardSubmitted = false
            StartLevel(1, true)
            -- Music will start after first window transition completes
        end
    end


    local startLabel = menuStarting and T("menu.starting") or T("menu.play")
    UI.add_button(bx, by, bw, bh,
        startLabel, "menu_start",
        UI_FONT_HEADER, 1,
        12, true,
        74, 12, 255, 0.95
    )

    if (not menuStarting) and UI.was_button_pressed("menu_start") then
        if needsPlayerName then
            showNamePrompt = true
            namePromptError = ""
            pendingStartAfterName = true
            UI.clear_input("ts_player_name") -- ensures a clean box
        else
            showDifficultyMenu = true
        end
    end


    -- Leaderboard button
    local gap = 16
    UI.add_button(bx, by + bh + gap, bw, bh,
        T("menu.leaderboard"), "menu_leaderboard",
        UI_FONT_HEADER, 1,
        12, true,
        0, 170, 110, 0.95
    )

    -- Settings button
    UI.add_button(bx, by + (bh + gap) * 2, bw, bh,
        T("menu.settings"), "menu_settings",
        UI_FONT_HEADER, 1,
        12, true,
        120, 120, 120, 0.95
    )

    UI.add_button(bx, by + (bh + gap) * 3, bw, bh,
    T("menu.quit"), "menu_exit",
    UI_FONT_HEADER, 1,
    12, true,
    120, 30, 30, 0.95
    )

    if (not menuStarting) and (not showNamePrompt) and UI.was_button_pressed("menu_exit") then
        pendingQuit = true
    end
    if (not menuStarting) and (not showNamePrompt) and UI.was_button_pressed("menu_settings") then
        menuScreen = "settings"
    end


    if (not menuStarting) and (not showNamePrompt) and UI.was_button_pressed("menu_leaderboard") then
        menuScreen = "leaderboard"
        leaderboardFetched = false
    end

    UI.end_child()

    if pendingQuit then
        pendingQuit = false
        Window.quit()
    end
    if showNamePrompt then
        DrawNamePrompt(screenW, screenH)
    end
    if showDifficultyMenu and (not showNamePrompt) then
        DrawDifficultyPrompt(screenW, screenH)
    end
end

local function DrawLeaderboardMenu(screenW, screenH, dt, context)
    context = context or "main"

    UI.add_panel(0, 0, screenW, screenH, 0.65, 0, 0, 0, 0)

    local panelW = math.floor(math.max(520, math.min(screenW * 0.70, 860)))
    local panelH = math.floor(math.max(420, math.min(screenH * 0.70, 600)))
    local panelX = math.floor((screenW - panelW) / 2)
    local panelY = math.floor((screenH - panelH) / 2)

    UI.begin_child(panelX, panelY, panelW, panelH, "TS_Leaderboard",
        true, 0,
        true, 0.92, 12, 25, 25, 25,
        0.01, false, 0.85,
        UI__COLOUR_THEME[1], UI__COLOUR_THEME[2], UI__COLOUR_THEME[3], 1.0
    )

    local cx = panelW / 2
    local footerH = 100
    local contentX = 26
    local contentY = math.floor(panelH * 0.13)
    local contentW = panelW - 52
    local contentH = panelH - contentY - footerH

    UI.add_centered_label(cx, math.floor(panelH * 0.05), T("menu.leaderboard"), UI_FONT_TITLE, 1)
    UI.add_centered_label(cx, math.floor(panelH * 0.15), T("leaderboard.description"), UI_FONT_SUB, 1)

    local NO_BACKGROUND = 128
    UI.begin_child(contentX, contentY, contentW, contentH, "TS_LeaderboardContent",
        false, NO_BACKGROUND, false)

    -- Use shared topLeaderboard / leaderboardFetched to avoid duplicated network calls and data
    if not leaderboardFetched then
        topLeaderboard = Firebase.retrieve_high_score(GAME_ID, 10)
        leaderboardFetched = true
    end

    local listX = math.floor(contentW * 0.20)
    local listY = math.floor(contentH * 0.15)
    local lineH = 26

    if topLeaderboard then
        local stageX = math.floor(contentW * 0.68)

        for i = 1, 10 do
            local e = topLeaderboard[i]
            local y = listY + (i - 1) * lineH

            if e then
                local name = tostring(e.name)
                local stage = tonumber(e.score) or 0
                UI.add_label(listX, y, 0, 0, string.format("%2d. %s", i, name), UI_FONT_REG, 1.1)
                UI.add_label(stageX, y, 0, 0, string.format(T("leaderboard.stage"), stage), UI_FONT_REG, 1.1)
            else
                UI.add_label(listX, y, 0, 0, string.format("%2d. --", i), "ImGuiDefault", 1.1)
            end
        end
    else
        UI.add_centered_label(cx, listY + 10, "(No scores yet)", UI_FONT_REG, 1.1)
    end

    UI.end_child()

    -- Back button (same visual but behaviour differs by context)
    local bw, bh = math.min(320, math.floor(panelW * 0.55)), 50
    local bx = math.floor((panelW - bw) / 2)
    local by = panelH - bh - 32

    UI.add_button(bx, by, bw, bh, T("menu.back"), "menu_back",
        UI_FONT_SUB, 1.0, 12, true, 74, 12, 255, 0.95)

    if UI.was_button_pressed("menu_back") then
        if context == "pause" then
            pauseScreen = "pause"
        else
            menuScreen = "main"
        end
    end

    UI.end_child()
end

local function DrawSettingsMenu(screenW, screenH, dt, context)
    context = context or "main"

    UI.add_panel(0, 0, screenW, screenH, 0.65, 0, 0, 0, 0)

    local panelW = math.floor(math.max(520, math.min(screenW * 0.70, 860)))
    local panelH = math.floor(math.max(420, math.min(screenH * 0.70, 600)))
    local panelX = math.floor((screenW - panelW) / 2)
    local panelY = math.floor((screenH - panelH) / 2)

    UI.begin_child(panelX, panelY, panelW, panelH, "TS_Settings",
        true, 0,
        true, 0.92, 12, 25, 25, 25,
        2.5, true, 1.35
    )

    local cx = panelW / 2
    local footerH = 100
    local contentX = 26
    local contentY = math.floor(panelH * 0.13)
    local contentW = panelW - 52
    local contentH = panelH - contentY - footerH

    UI.add_centered_label(cx, math.floor(panelH * 0.05), T("settings.title"), UI_FONT_TITLE, 1)

    UI.begin_child(contentX, contentY, contentW, contentH, "TS_SettingsContent",
        false, NO_BACKGROUND, false)

    local innerCX = contentW / 2
    local sliderW = math.floor(contentW * 0.5)
    local sliderX = math.floor((contentW - sliderW) / 2)

    UI.add_centered_label(innerCX, 12, T("settings.audio"), UI_FONT_HEADER, 1.2)

    local sliderStyle = {
        height = 21,
        rounding = 10,
        grab_size = 16,
        track = { 30, 30, 30, 220 },
        grab  = { 74, 12, 255, 255 },
    }

    local function DrawVolRow(title, id, value, y)
        UI.add_centered_label(innerCX, y - 16 , title, UI_FONT_REG, 1.05)
        UI.add_slider_styled(sliderX, y + 8, sliderW, "", id, 0.0, 1.0, value, nil, nil, " ", sliderStyle)
        local percent = math.floor(((UI.get_slider(id) or value) * 100) + 0.5)
        UI.add_label(sliderX + sliderW + 18, y + 2, 0, 0, tostring(percent) .. "%", "ImGuiDefaultBold", 1.0)
    end

    local y0 = 70
    local gapY = 66

    DrawVolRow(T("audio.master"), "ts_master", masterVol, y0)
    if UI.was_slider_changed("ts_master") then
        masterVol = UI.get_slider("ts_master") or masterVol
        Json.save_setting(GAME_ID, "audio.master", masterVol)
        ApplyAudioVolumes()
    end

    DrawVolRow(T("audio.music"), "ts_music", musicVol, y0 + gapY)
    if UI.was_slider_changed("ts_music") then
        musicVol = UI.get_slider("ts_music") or musicVol
        Json.save_setting(GAME_ID, "audio.music", musicVol)
        ApplyAudioVolumes()
    end

    DrawVolRow(T("audio.sfx"), "ts_sfx", sfxVol, y0 + gapY * 2)
    if UI.was_slider_changed("ts_sfx") then
        sfxVol = UI.get_slider("ts_sfx") or sfxVol
        Json.save_setting(GAME_ID, "audio.sfx", sfxVol)
        ApplyAudioVolumes()
    end

    local controlsHeaderY = y0 + gapY * 3 + 10
    UI.add_centered_label(innerCX, controlsHeaderY, T("settings.controls"), UI_FONT_HEADER, 1.2)

    local sensY = controlsHeaderY + 76
    UI.add_centered_label(innerCX, sensY - 16, T("controls.sensitivity"), UI_FONT_REG, 1.05)
    UI.add_slider_styled(sliderX, sensY + 8, sliderW, "", "ts_sensitivity", 0.25, 2.50, sensitivitySetting, nil, nil, " ", sliderStyle)

    if UI.was_slider_changed("ts_sensitivity") then
        sensitivitySetting = UI.get_slider("ts_sensitivity") or sensitivitySetting
        sensitivitySetting = Clamp(sensitivitySetting, 0.25, 2.50)
        Json.save_setting(GAME_ID, "controls.sensitivity", sensitivitySetting)
    end

    UI.add_label(sliderX + sliderW + 18, sensY + 2, 0, 0,
        string.format("%.2fx", sensitivitySetting),
        "ImGuiDefaultBold", 1.0
    )

    local langY = sensY + 84

    UI.add_centered_label(innerCX, langY - 16, T("settings.language"), UI_FONT_HEADER, 1.2)

    local opts = { "English", "日本語" }
    local defaultIndex = (language == "ja") and 1 or 0

    local dropdownW = sliderW
    local dropdownX = sliderX

    local ddStyle = {
      height = 32,
      rounding = 10,
      popup_rounding = 10,
      border_size = 1,
      frame        = { 30, 30, 30, 220 },
      frame_hover  = { 40, 40, 40, 230 },
      frame_active = { 50, 50, 50, 240 },
      popup_bg = { 20, 20, 20, 240 },
      border   = { 0, 0, 0,  80 },
      item        = { 74, 12, 255, 120 },
      item_hover  = { 74, 12, 255, 180 },
      item_active = { 74, 12, 255, 220 },
      text = { 255, 255, 255, 255 },
    }
    local langDropdownFont = "ImGuiDefaultJP"
    UI.add_dropdown_styled(dropdownX, langY + 24, dropdownW, 32, "", "ts_lang", opts, defaultIndex, langDropdownFont, 1.0, ddStyle)

    if UI.was_dropdown_changed("ts_lang") then
        local idx = UI.get_dropdown_index("ts_lang") or 0
        language = (idx == 1) and "ja" or "en"
        Json.save_setting(GAME_ID, "ui.language", language)
        Localisation.set_language(language)
        ApplyLanguageFonts()
        SyncTSUI()
    end

    UI.end_child()

    local bw, bh = math.min(320, math.floor(panelW * 0.55)), 50
    local bx = math.floor((panelW - bw) / 2)
    local by = panelH - bh - 32

    UI.add_button(bx, by, bw, bh,
        T("menu.back"), "menu_back_settings",
        UI_FONT_SUB, 1.0,
        12, true,
        74, 12, 255, 0.95
    )

    if UI.was_button_pressed("menu_back_settings") then
        if context == "pause" then
            pauseScreen = "pause"
        else
            menuScreen = "main"
        end
    end

    UI.end_child()
end

local SetPaused
local GoToMainMenuFromPause

local function DrawPauseMenu(screenW, screenH, dt)
    UI.add_panel(0, 0, screenW, screenH, 0.55, 0, 0, 0, 0)

    local panelW = math.floor(math.max(520, math.min(screenW * 0.55, 760)))
    local panelH = math.floor(math.max(420, math.min(screenH * 0.60, 560)))
    local panelX = math.floor((screenW - panelW) / 2)
    local panelY = math.floor((screenH - panelH) / 2)

    UI.begin_child(panelX, panelY, panelW, panelH, "TS_PauseMenu",
        true, 0,
        true, 0.92, 12, 25, 25, 25,
        2.5, true, 1.35
    )

    local cx = panelW / 2
    UI.add_centered_label(cx, math.floor(panelH * 0.05), T("pause.title"), UI_FONT_TITLE, 1)
    UI.add_centered_label(cx, math.floor(panelH * 0.15), "Press ESC to resume", UI_FONT_REG, 1)

    local bw, bh = math.min(340, math.floor(panelW * 0.60)), 50
    local gap = 14

    -- Button panel sizing (fits 4 buttons + padding)
    local btnPadY = 18
    local btnPanelH = (bh * 4) + (gap * 3) + (btnPadY * 2)

    local btnPanelW = math.min(panelW - 90, bw + 120)
    local btnPanelX = math.floor((panelW - btnPanelW) / 2)
    local btnPanelY = math.floor(panelH * 0.27)  -- tweak if you want it higher/lower

    UI.begin_child(
        btnPanelX, btnPanelY, btnPanelW, btnPanelH, "TS_PauseButtons",
        false, NO_BACKGROUND,
        false, 0
    )

    local bx = math.floor((btnPanelW - bw) / 2)
    local y0 = btnPadY

    UI.add_button(bx, y0 + (bh + gap) * 0, bw, bh, T("pause.resume"), "pause_resume",
        UI_FONT_SUB, 1.0, 12, true, 74, 12, 255, 0.95)

    UI.add_button(bx, y0 + (bh + gap) * 1, bw, bh, T("menu.settings"), "pause_settings",
        UI_FONT_SUB, 1.0, 12, true, 74, 12, 255, 0.90)

    UI.add_button(bx, y0 + (bh + gap) * 2, bw, bh, T("menu.leaderboard"), "pause_leaderboard",
        UI_FONT_SUB, 1.0, 12, true, 74, 12, 255, 0.90)

    UI.add_button(bx, y0 + (bh + gap) * 3, bw, bh, T("pause.quit"), "pause_mainmenu",
        UI_FONT_SUB, 1.0, 12, true, 120, 30, 30, 0.90)

    UI.end_child()

    -- Button handling stays outside (same IDs)
    if UI.was_button_pressed("pause_resume") then
        SetPaused(false)
    elseif UI.was_button_pressed("pause_settings") then
        pauseScreen = "settings"
    elseif UI.was_button_pressed("pause_leaderboard") then
        pauseScreen = "leaderboard"
        leaderboardFetched = false
    elseif UI.was_button_pressed("pause_mainmenu") then
        GoToMainMenuFromPause()
    end

    UI.end_child()
end

local function SubmitLeaderboardScoreOnce(score)
    if runLeaderboardSubmitted then return end
    if needsPlayerName then return end
    if playerName == nil or playerName == "" then return end

    score = math.floor(tonumber(score) or 0)
    if score <= 0 then return end

    Firebase.submit_high_score(GAME_ID, playerName, score)
    runLeaderboardSubmitted = true
end

local function DrawGameOverMenu(screenW, screenH, dt)
    Input.set_relative_mouse_mode(false)

    -- Main menu window size (target)
    local displayWidth = Window.get_display_width()
    local displayHeight = Window.get_display_height()
    Window.set_size(MAIN_MENU_W, MAIN_MENU_H)
    screenW = MAIN_MENU_W
    screenH = MAIN_MENU_H

    UI.add_panel(0, 0, screenW, screenH, 0.65, 0, 0, 0, 0)

    local panelW = math.floor(math.max(720, math.min(screenW * 0.72, 980)))
    local panelH = math.floor(math.max(560, math.min(screenH * 0.78, 760)))
    local panelX = math.floor((screenW - panelW) / 2)
    local panelY = math.floor((screenH - panelH) / 2)

    UI.begin_child(panelX, panelY, panelW, panelH, "TS_GameOver",
        true, 0,
        true, 0.92, 12, 25, 25, 25,
        2.5, true, 1.35
    )

    local cx = panelW / 2
    UI.add_centered_label_colored(cx, math.floor(panelH * 0.03),
    T("gameover.title"),
    {255, 40, 40, 255},
    UI_FONT_TITLE, 1)

    local summary = endRunSummary or CaptureEndRunSummary()
    SubmitLeaderboardScoreOnce(summary.stageReached or 1)
    local shotsFired = tonumber(summary.shotsFired or 0) or 0
    local shotsHit = tonumber(summary.shotsHit or 0) or 0

    local accuracyText = "—"
    if shotsFired > 0 then
        local pct = (shotsHit / shotsFired) * 100.0
        accuracyText = string.format("%.0f%%", pct)
    end

    -- Summary panel
    local sumW = math.floor(panelW * 0.90)
    local sumH = math.floor(panelH * 0.64)
    local sumX = math.floor((panelW - sumW) / 2)
    local sumY = math.floor(panelH * 0.13)

    UI.begin_child(sumX, sumY, sumW, sumH, "TS_EndRunSummary",
        true, 0,
        true, 0.55, 10, 15, 15, 15,
        1.0, true, 1.25
    )

    local scx = sumW / 2
    UI.add_centered_label(scx, 10, T("gameover.summary"), UI_FONT_HEADER, 1)

    local leftX = 20
    local rightX = math.floor(sumW * 0.75)
    local lineH = 26

    local y = 46
    local function AddKV(x, yy, k, v, isBold)
        UI.add_label(x, yy, 0, 0, tostring(k), UI_FONT_REG, 1)
        UI.add_label(x + math.floor(sumW * 0.28), yy, 0, 0, tostring(v), UI_FONT_REG, 1)
    end

    -- Left column: results + combat
    UI.add_label(leftX, 10, 0, 0, T("summary.overall"), UI_FONT_HEADER, 0.8)
    AddKV(leftX, y, T("summary.stage"), summary.stageReached or 1, true); y = y + lineH
    AddKV(leftX, y, T("summary.level"), summary.playerLevel or 1, true); y = y + lineH
    AddKV(leftX, y, T("summary.duration"), FormatTimeMMSS(summary.timeSurvived or 0), true); y = y + lineH
    AddKV(leftX, y, T("summary.totalkilled"), summary.enemiesKilled or 0, true); y = y + (lineH*2)

    y = y + 10
    UI.add_label(leftX, y - 5, 0, 0, T("summary.combat"), UI_FONT_HEADER, 0.8); y = y + lineH
    AddKV(leftX, y, T("summary.shotsfired"), shotsFired, false); y = y + lineH
    AddKV(leftX, y, T("summary.accuracy"), accuracyText, false); y = y + lineH
    AddKV(leftX, y, T("summary.dmgdealt"), summary.damageDealt or 0, false); y = y + lineH
    AddKV(leftX, y, T("summary.dmgtaken"), summary.damageTaken or 0, false); y = y + lineH
    AddKV(leftX, y, T("summary.hphealed"), summary.healingCollected or 0, false); y = y + lineH

    -- Right column: build recap
    local b = summary.build or {}
    local ry = 46
    UI.add_label(rightX, 10, 0, 0, T("summary.recap"), UI_FONT_HEADER, 0.8);
    UI.add_label(rightX, ry, 0, 0, T("summary.firepower") .. tostring(b.firepower or 1), UI_FONT_REG, 1); ry = ry + lineH
    UI.add_label(rightX, ry, 0, 0, T("summary.pierce") .. tostring(b.pierce or 0), UI_FONT_REG, 1); ry = ry + lineH
    UI.add_label(rightX, ry, 0, 0, T("summary.bounce") .. tostring(b.bounce or 0), UI_FONT_REG, 1); ry = ry + lineH
    UI.add_label(rightX, ry, 0, 0, string.format(T("summary.fireinterval"), tonumber(b.fireInterval or 0.5) or 0.5), UI_FONT_REG, 1); ry = ry + lineH
    UI.add_label(rightX, ry, 0, 0, T("summary.maxhp") .. tostring(b.maxHealth or 100), UI_FONT_REG, 1)

    UI.end_child()

    -- Buttons
    local totalW = math.floor(panelW * 0.90)
    local gap = 16
    local bw = math.floor((totalW - gap) / 2)
    local bh = 56
    local bx = math.floor((panelW - totalW) / 2)
    local by = math.floor(panelH * 0.80)

    UI.add_button(bx, by, bw, bh,
        T("gameover.retry"), "gameover_retry",
        UI_FONT_SUB, 1,
        12, true,
        74, 12, 255, 0.95
    )

    UI.add_button(bx + bw + gap, by, bw, bh,
        T("gameover.back"), "gameover_mainmenu",
        UI_FONT_SUB, 1,
        12, true,
        120, 30, 30, 0.95
    )

    if UI.was_button_pressed("gameover_retry") then
        isGameOver = false
        ResetRunStats()
        SystemShooterPickups.clearAll()
        SystemShooterPlayerProgress.reset()
        ClearEnemies()
        ClearAllPlayerProjectiles()
        ClearAllEnemyProjectiles()
        Input.set_relative_mouse_mode(true)
        runLeaderboardSubmitted = false
        StartLevel(1, true)
        -- Restart music from beginning to sync with gameplay
        if musicEntity then
            MusicComponent.stop(musicEntity, 0)  -- fadeOut=0
            MusicComponent.play(musicEntity, true, 0)  -- loop=true, fadeIn=0
        end

    elseif UI.was_button_pressed("gameover_mainmenu") then
        isGameOver = false
        ResetRunStats()
        SystemShooterPickups.clearAll()
        SystemShooterPlayerProgress.reset()
        ClearEnemies()
        ClearAllPlayerProjectiles()
        ClearAllEnemyProjectiles()
        GoToMainMenuFromPause()
    end

    UI.end_child()
end


local function GetEscapeKey()
    if Keys then
        return Keys.ionix_escape or Keys.ionix_esc or Keys.escape or Keys.Escape
    end
    return nil
end

SetPaused = function(p)
    if isPaused == p then return end
    isPaused = p
    pauseScreen = "pause"

    if isPaused then
        -- Check if window is too small for pause menu
        local currentW = Window.get_width()
        local currentH = Window.get_height()
        
        if currentW < MIN_PAUSE_W or currentH < MIN_PAUSE_H then
            -- Store current size
            prePauseWindowW = currentW
            prePauseWindowH = currentH
            
            -- Resize to comfortable size
            local targetW = math.max(currentW, MIN_PAUSE_W)
            local targetH = math.max(currentH, MIN_PAUSE_H)
            local displayW = Window.get_display_width()
            local displayH = Window.get_display_height()
            local x = math.floor((displayW - targetW) * 0.5)
            local y = math.floor((displayH - targetH) * 0.5)
            Window.set_pos(x, y)
            Window.set_size(targetW, targetH)
        end
    else
        -- Restore original window size if it was resized for the pause menu
        if prePauseWindowW and prePauseWindowH then
            local displayW = Window.get_display_width()
            local displayH = Window.get_display_height()
            local x = math.floor((displayW - prePauseWindowW) * 0.5)
            local y = math.floor((displayH - prePauseWindowH) * 0.5)
            Window.set_pos(x, y)
            Window.set_size(prePauseWindowW, prePauseWindowH)
            prePauseWindowW = nil
            prePauseWindowH = nil
        end
    end

    -- show cursor in pause menus, lock cursor in gameplay
    Input.set_relative_mouse_mode(not isPaused)

    -- Skip first mouse delta to prevent teleport on unpause
    if not isPaused then
        SystemShooterPlayer.skipNextDelta()
    end

    -- Stop/resume music to prevent desync
    if musicEntity then
        if isPaused then
            MusicComponent.pause(musicEntity)
        else
            MusicComponent.resume(musicEntity)
        end
    end
end


local function SetCenteredWindowSize(w, h)
    local displayW = Window.get_display_width()
    local displayH = Window.get_display_height()
    local x = math.floor((displayW - w) * 0.5)
    local y = math.floor((displayH - h) * 0.5)
    Window.set_pos(x, y)
    Window.set_size(w, h)
end

GoToMainMenuFromPause = function()
    SetPaused(false)

    SubmitLeaderboardScoreOnce(currentLevel or 1)

    ResetRunStateForMenu()

    -- Stop music when returning to main menu
    if musicEntity then
        MusicComponent.stop(musicEntity, 0)  -- fadeOut=0
        musicStartedThisLaunch = false
    end

    -- go to main menu
    inMainMenu = true
    menuScreen = "main"
    menuStarting = false
    menuStartTimer = 0

    -- main menu size you wanted
    SetCenteredWindowSize(1280, 720)

    -- keep cursor available in menus
    Input.set_relative_mouse_mode(false)
end

 --=====================================================================
 --  [ENGINE CALLBACKS] OnUpdate (Main Loop)
 --=====================================================================
function SystemShooter:OnUpdate()
     --=====================================================================
     --  [ONUPDATE] Frame Setup
     --=====================================================================
    globalFrame = globalFrame + 1
    local dt = GetDt()

    if inMainMenu then
        screenW = Window.get_width()
        screenH = Window.get_height()

        if menuScreen == "leaderboard" then
            DrawLeaderboardMenu(screenW, screenH, dt, "main")
        elseif menuScreen == "settings" then
            DrawSettingsMenu(screenW, screenH, dt, "main")
        else
            DrawMainMenu(screenW, screenH, dt)
        end
        return
    end

    if isGameOver then
        screenW = Window.get_width()
        screenH = Window.get_height()
        DrawGameOverMenu(screenW, screenH, dt)
        return
    end


     --=====================================================================
     --  [ONUPDATE] Transition State Machine (prevents lag spikes)
     --=====================================================================
    -- Handle pre-transition phase (delay before window resize starts)
    if transitionPhase == "pre" then
        transitionTimer = transitionTimer - dt
        if transitionTimer <= 0 then
            -- Pre-transition complete, start window transition
            transitionPhase = "window"
            windowTransitionActive = true
            windowTransitionProgress = 0
        end
        -- Allow player movement during pre-phase
        if playerInitialized then
            SystemShooterPlayer.setScreenBounds(screenW, screenH)
            SystemShooterPlayer.updateMovement(dt, sensitivitySetting)
        end
        return
    end
    
    -- Handle post-transition phase (delay after window resize ends)
    if transitionPhase == "post" then
        transitionTimer = transitionTimer - dt
        if transitionTimer <= 0 then
            -- Post-transition complete, load the level (heavy work)
            transitionPhase = "none"
            local index = pendingLevelIndex
            local reset = pendingResetPlayerState
            pendingLevelIndex = nil
            pendingResetPlayerState = false
            if index ~= nil then
                LoadLevel(index, reset)
            end
        end
        -- Allow player movement during post-phase
        if playerInitialized then
            SystemShooterPlayer.setScreenBounds(screenW, screenH)
            SystemShooterPlayer.updateMovement(dt, sensitivitySetting)
        end
        return
    end
    
     --=====================================================================
     --  [ONUPDATE] Window Transition (player continues to update)
     --=====================================================================
    local isTransitioning = UpdateWindowTransition(dt)
    if isTransitioning then
        -- During transition: only update player movement (if player exists)
        -- No aiming, recoil, flash, shooting, projectiles, UI, or enemy updates
        if playerInitialized then
            SystemShooterPlayer.setScreenBounds(screenW, screenH)
            SystemShooterPlayer.updateMovement(dt, sensitivitySetting)
            SystemShooterPlayer.stopFiring()
        end
        
        -- Update healing orbs during transition (so they don't freeze)
        SystemShooterPickups.update(dt)
        SystemShooterPickups.constrainToScreen(screenW, screenH)
        
        return
    end

     --=====================================================================
     --  [ONUPDATE] Upgrade Menu (Freeze Game)
     --=====================================================================
    if SystemShooterUI.isMenuOpen() then
        screenW = Window.get_width()
        screenH = Window.get_height()
        SystemShooterUI.draw(screenW, screenH)
        local selectedUpgrade = SystemShooterUI.handleInput()
        if selectedUpgrade then
            SystemShooterPlayerProgress.applyUpgrade(selectedUpgrade.type)
            if selectedUpgrade.type == "max_health" then
                local maxH = SystemShooterPlayerProgress.getMaxHealth()
                local currentH = SystemShooterPlayer.getHealth()
                SystemShooterPlayer.setHealth(math.min(maxH, currentH + 20))
            elseif selectedUpgrade.type == "overhealth" then
                local maxH = SystemShooterPlayerProgress.getMaxHealth()
                local currentH = SystemShooterPlayer.getHealth()
                SystemShooterPlayer.setHealth(math.min(maxH, currentH + 20))
            end
            -- Start levelup peace timer and disable enemies
            peaceTimerSeconds = levelupPeaceDuration
            isLevelupPeace = true
            isStartLevelPeace = false
            SystemShooterEnemy.disableAllEnemies(enemies, screenW, screenH)
            ClearAllEnemyProjectiles()
        end
        return
    end

    -- =====================================================================
    --  [PAUSE] ESC toggles pause (freezes gameplay)
    -- =====================================================================
    local esc = GetEscapeKey()
    if esc and Input.get_key_down(esc) then
        if not isPaused then
            -- don’t pause over upgrade menus; they already freeze the game
            if (not SystemShooterUI.isMenuOpen()) and (not SystemShooterPlayerProgress.hasPendingLevelUp()) and (not windowTransitionActive) then
                SetPaused(true)
            end
        else
            -- if you’re inside pause settings/leaderboard, ESC goes back one screen first
            if pauseScreen ~= "pause" then
                pauseScreen = "pause"
            else
                SetPaused(false)
            end
        end
    end

    if isPaused then
        screenW = Window.get_width()
        screenH = Window.get_height()

        if pauseScreen == "settings" then
            DrawSettingsMenu(screenW, screenH, dt, "pause")
        elseif pauseScreen == "leaderboard" then
            DrawLeaderboardMenu(screenW, screenH, dt, "pause")
        else
            DrawPauseMenu(screenW, screenH, dt)
        end

        return -- <-- THIS is what freezes gameplay
    end


    if SystemShooterPlayerProgress.hasPendingLevelUp() then
        SystemShooterPlayerProgress.consumePendingLevelUp()
        local level = SystemShooterPlayerProgress.getProgress()
        SystemShooterUI.showUpgradeMenu(nil, level)
        return
    end

    

    -- Track run time (excludes menus, pause, transitions, and upgrade screens)
    runTimeSeconds = runTimeSeconds + dt
 --=====================================================================
     --  [ONUPDATE] World Bounds (Window)
     --=====================================================================
    -- Update screen bounds from window
    screenW = Window.get_width()
    screenH = Window.get_height()
    SystemShooterProjectiles.setScreenBounds(screenW, screenH)
    SystemShooterPickups.constrainToScreen(screenW, screenH)
    
    -- Check if in peace phase (end-level peace when no enemies alive)
    local inPeacePhase = isStartLevelPeace or isLevelupPeace or (not enemiesAlive and peaceTimerSeconds > 0)
    
    if levelTimerSeconds > 0 and #enemies > 0 and not inPeacePhase then
        levelTimerSeconds = levelTimerSeconds - dt
    end

     --=====================================================================
     --  [ONUPDATE] Debug / Music Toggles
     --=====================================================================
    if Input.get_key_down(Keys.ionix_space) then
        print(string.format("[BeatMarker] globalFrame=%d", globalFrame))
    end

    
     --=====================================================================
     --  [ONUPDATE] Player Movement, Aiming, Shooting (via SystemShooterPlayer)
     --=====================================================================
    -- Guard all player updates - player may not be initialized yet
    if not playerInitialized then
        return
    end
    
    -- Player can still move during rewind
    SystemShooterPlayer.setScreenBounds(screenW, screenH)
    SystemShooterPlayer.updateMovement(dt, sensitivitySetting)
    SystemShooterPlayer.updateAiming(enemies, enemySize)
    SystemShooterPlayer.updateRecoil(dt)
    
    -- Shooting (disabled during rewind)
    if not SystemShooterRewind.isActive() then
        local playGunshotSfx = function()
            if gunshot3SfxEntity then
                AudioComponent.play(gunshot3SfxEntity)
            end
        end
        local onShotFired = function(count)
            runShotsFired = runShotsFired + count
        end
        SystemShooterPlayer.updateShooting(dt, IsNoWitnessesActive, playGunshotSfx, onShotFired)
    else
        -- Force stop firing during rewind
        SystemShooterPlayer.stopFiring()
    end
    
     --=====================================================================
     --  [ONUPDATE] Systems Update
     --=====================================================================
    -- Skip normal updates during rewind (handled in rewind logic)
    if not SystemShooterRewind.isActive() then
        -- Update all projectiles via module
        local runStats = { shotsHit = runShotsHit, damageDealt = runDamageDealt, damageTaken = runDamageTaken }
        SystemShooterProjectiles.updatePlayerProjectiles(dt, runStats)
        SystemShooterProjectiles.updateEnemyProjectiles(dt, SystemShooterPlayer.getDamageCooldown(), runStats)
        runShotsHit = runStats.shotsHit
        runDamageDealt = runStats.damageDealt
        runDamageTaken = runStats.damageTaken

        -- Check enemy-player collision and apply damage
        UpdateEnemyCollision()
        
        -- Update overhealth system and apply lightning zap damage to enemies
        local overhealthActive, overhealthRadius, shouldZap, zapDamage, maxZapTargets = SystemShooterPlayerProgress.updateOverhealth(dt)
        
        -- Manage overhealth ring VFX
        if overhealthActive and not wasOverhealthActive then
            -- Overhealth just became active - create the ring
            local pX, pY = SystemShooterPlayer.getPosition()
            local pSize = SystemShooterPlayer.getSize()
            local playerCenterX = pX + pSize / 2
            local playerCenterY = pY + pSize / 2
            overhealthRingId = VFX.create_ring(playerCenterX, playerCenterY, overhealthRadius, 1.5)
            if overhealthRingId and overhealthRingId >= 0 then
                -- Electric blue/cyan color with pulsing effect
                VFX.set_ring_color(overhealthRingId, 0, 200, 255, 200)
                VFX.set_ring_pulsing(overhealthRingId, true, 3.0, 100, 255)
                VFX.set_ring_render_layer(overhealthRingId, 0, 15)  -- High z-order to render on top
                VFX.set_ring_segments(overhealthRingId, 48)
                VFX.set_ring_distortion(overhealthRingId, true, 2, 4.0, 3.5)
            end
        elseif not overhealthActive and wasOverhealthActive then
            -- Overhealth just became inactive - destroy the ring
            if overhealthRingId and overhealthRingId >= 0 then
                VFX.destroy_ring(overhealthRingId)
                overhealthRingId = nil
            end
        end
        wasOverhealthActive = overhealthActive
        
        -- Update ring position to follow player if active
        if overhealthActive and overhealthRingId and overhealthRingId >= 0 then
            local pX, pY = SystemShooterPlayer.getPosition()
            local pSize = SystemShooterPlayer.getSize()
            local playerCenterX = pX + pSize / 2
            local playerCenterY = pY + pSize / 2
            VFX.set_ring_position(overhealthRingId, playerCenterX, playerCenterY)
            VFX.set_ring_radius(overhealthRingId, overhealthRadius)
            
            -- Apply lightning zap damage when timer triggers
            if shouldZap then
                -- Gather all enemies within radius
                local enemiesInRange = {}
                for i = 1, #enemies do
                    local enemy = enemies[i]
                    if not enemy.isDead and not enemy.disabled then
                        local enemyCenterX = enemy.x + enemySize / 2
                        local enemyCenterY = enemy.y + enemySize / 2
                        local dx = enemyCenterX - playerCenterX
                        local dy = enemyCenterY - playerCenterY
                        local dist = math.sqrt(dx * dx + dy * dy)
                        
                        if dist <= overhealthRadius then
                            table.insert(enemiesInRange, { enemy = enemy, dist = dist, cx = enemyCenterX, cy = enemyCenterY })
                        end
                    end
                end
                
                -- Randomly select up to maxZapTargets enemies to zap
                local targetsToZap = {}
                local availableCount = #enemiesInRange
                local numToZap = math.min(maxZapTargets, availableCount)
                
                -- Fisher-Yates shuffle to randomly select targets
                for i = 1, numToZap do
                    local randomIndex = math.random(i, availableCount)
                    enemiesInRange[i], enemiesInRange[randomIndex] = enemiesInRange[randomIndex], enemiesInRange[i]
                    table.insert(targetsToZap, enemiesInRange[i])
                end
                
                -- Get overhealth config for lightning settings
                local overhealthCfg = SystemShooterPlayerProgress.getOverhealthConfig()
                local lightningLifetime = overhealthCfg.lightningLifetime or 0.25
                local lightningColor = overhealthCfg.lightningColor or { r = 100, g = 200, b = 255, a = 255 }
                
                -- Zap each selected target
                for _, target in ipairs(targetsToZap) do
                    local enemy = target.enemy
                    local enemyCenterX = target.cx
                    local enemyCenterY = target.cy
                    
                    -- Create lightning bolt VFX from player to enemy
                    local lightningId = VFX.create_lightning(playerCenterX, playerCenterY, enemyCenterX, enemyCenterY, lightningLifetime)
                    if lightningId and lightningId >= 0 then
                        VFX.set_lightning_color(lightningId, lightningColor.r, lightningColor.g, lightningColor.b, lightningColor.a)
                        VFX.set_lightning_flicker(lightningId, true, 0.03)  -- Fast flicker for electric effect
                        VFX.set_lightning_render_layer(lightningId, 0, 20)  -- High z-order, above ring
                        VFX.set_lightning_fade_out(lightningId, true)
                    end
                    
                    -- Calculate damage as 5% of enemy max health, with a minimum of 10
                    local enemyMaxHealth = enemy.maxHealth or 100
                    local percentDamage = enemyMaxHealth * 0.10
                    local actualDamage = math.max(percentDamage, 15)
                    
                    -- Apply burst damage
                    enemy.health = enemy.health - actualDamage
                    runDamageDealt = runDamageDealt + actualDamage
                    
                    if enemy.health <= 0 then
                        enemy.isDead = true
                        Entity.set_global_pos(enemy.entity, -1000, -1000)
                        
                        -- Try to spawn healing orb at enemy death location
                        local eSize = enemy.size or enemySize
                        local deathX = enemy.x + eSize / 2
                        local deathY = enemy.y + eSize / 2
                        SystemShooterPickups.trySpawnHealingOrb(deathX, deathY)
                        
                        runEnemiesKilled = runEnemiesKilled + 1
                        local xpAmount = enemy.xpReward or 5
                        levelXpGained = levelXpGained + xpAmount
                        SystemShooterPlayerProgress.addXp(xpAmount)
                    end
                end
            end
        end
        
        -- Update enemy movement
        UpdateEnemyMovement()
    end

    ParticleSystem.update(dt)
    SystemShooterPickups.update(dt)
    
    -- Check pickup collision and heal player (skip during rewind)
    if not SystemShooterRewind.isActive() then
        local maxHealth = SystemShooterPlayerProgress.getMaxHealth()
        local pX, pY = SystemShooterPlayer.getPosition()
        local pSize = SystemShooterPlayer.getSize()
        local healAmount = SystemShooterPickups.checkPlayerCollision(pX, pY, pSize, maxHealth)
        if healAmount then
            local before = SystemShooterPlayer.getHealth()
            local newHealth = before + healAmount
            
            -- Check if overhealth is enabled
            if SystemShooterPlayerProgress.isOverhealthEnabled() then
                -- If healing would go over max, put excess into overhealth
                if newHealth > maxHealth then
                    local excess = newHealth - maxHealth
                    SystemShooterPlayer.setHealth(maxHealth)
                    SystemShooterPlayerProgress.addOverhealth(excess)
                    runHealingCollected = runHealingCollected + healAmount
                else
                    SystemShooterPlayer.setHealth(newHealth)
                    runHealingCollected = runHealingCollected + healAmount
                end
            else
                -- No overhealth upgrade, just cap at max
                newHealth = math.min(maxHealth, newHealth)
                SystemShooterPlayer.setHealth(newHealth)
                local actual = newHealth - before
                if actual > 0 then
                    runHealingCollected = runHealingCollected + actual
                end
            end
        end
    end
    
    -- Update flash effect
    UpdateFlash()

    -- Skip beat bop during rewind
    if not SystemShooterRewind.isActive() then
        UpdateBeatBop()
    end

     --=====================================================================
     --  [ONUPDATE] UI
     --=====================================================================
    local levelCfg = SystemShooterLevels.getLevelConfig(currentLevel)
    if levelCfg ~= nil then
        local baseEnemyHealthTotal = 0
        local capEnemyHealthTotal = 0

        if levelCfg.enemies then
            for _, enemyCfg in ipairs(levelCfg.enemies) do
                local cap = enemyCfg.health or levelEnemyHealth
                local base = enemyCfg.base_health or cap
                capEnemyHealthTotal = capEnemyHealthTotal + cap
                baseEnemyHealthTotal = baseEnemyHealthTotal + base
            end
        else
            local enemyCount = levelCfg.enemyCount or 1
            local capSingle = (levelCfg.enemyHealth or levelEnemyHealth)
            local baseSingle = (levelCfg.base_enemyHealth or capSingle)
            capEnemyHealthTotal = capSingle * enemyCount
            baseEnemyHealthTotal = baseSingle * enemyCount
        end

        if baseEnemyHealthTotal < 1 then baseEnemyHealthTotal = 1 end
        if capEnemyHealthTotal < 1 then capEnemyHealthTotal = 1 end

        local currentEnemyHealthTotal = 0
        for i = 1, #enemies do
            currentEnemyHealthTotal = currentEnemyHealthTotal + (enemies[i].health or 0)
        end
        if currentEnemyHealthTotal < 0 then currentEnemyHealthTotal = 0 end
        if currentEnemyHealthTotal > capEnemyHealthTotal then
            currentEnemyHealthTotal = capEnemyHealthTotal
        end

-- Enemy HP bar with "lost max" segment that:
-- - appears only after rewind refills to the capped max
-- - pulses yellow until the first hit, then turns grey
local eps = 0.001

if capEnemyHealthTotal >= baseEnemyHealthTotal - eps then
    -- no cap active
    hpCap_waitingForFull = false
    hpCap_hasBeenHit = false
else
    if hpCap_waitingForFull then
        if currentEnemyHealthTotal >= capEnemyHealthTotal - eps then
            hpCap_waitingForFull = false
            hpCap_hasBeenHit = false
        end
    else
        if (not hpCap_hasBeenHit) and (currentEnemyHealthTotal < capEnemyHealthTotal - eps) then
            hpCap_hasBeenHit = true
        end
    end
end

local capMaxForUI = capEnemyHealthTotal
local capFill = {255, 210, 60, 220}

if hpCap_waitingForFull or capEnemyHealthTotal >= baseEnemyHealthTotal - eps then
    capMaxForUI = baseEnemyHealthTotal
else
    if hpCap_hasBeenHit then
        capFill = {30, 30, 30, 200} -- grey
    else
        -- pulse while visible before first hit
        local pulse = 0.5 + 0.5 * math.sin(Mafs.time() * 5.0)
        local a = math.floor(50 + 70 * pulse)
        capFill = {255, 210, 60, a} -- yellow (pulsing alpha)
    end
end

local enemyHpStyle = {
    rounding = 10,
    border_size = 0,
    bg = {30, 30, 30, 220},
    fill = {255, 0, 0, 255},
    cap_max = capMaxForUI,
    cap_fill = capFill,
}

-- Note: we intentionally do NOT normalise to "full" here.
-- The bar starts at the capped max (e.g. 80%) and drains from there.
UI.draw_progress_bar_styled(20, 20, 200, 20,
    baseEnemyHealthTotal, currentEnemyHealthTotal, 1,
    enemyHpStyle, "")
if levelCfg.timeLimitSeconds ~= nil and levelCfg.timeLimitSeconds > 0 then
        local barW, barH = 200, 10
        local x, y = 20, 50

        local maxT = levelCfg.timeLimitSeconds
        local tLeft = math.max(0, math.min(maxT, levelTimerSeconds)) -- assumes levelTimerSeconds = remaining

        -- Pulse only near the end (last 15%)
        local frac = (maxT > 0) and (tLeft / maxT) or 0
        local pulse = 1.0
        if frac < 0.15 then
            pulse = 0.6 + 0.4 * (0.5 + 0.5 * math.sin(Mafs.time() * 10.0))
        end

        local timerStyle = {
            rounding = barH / 2,
            border_size = 1,
            bg     = {255, 255, 255, 35},
            fill   = {255, 255, 255, math.floor(220 * pulse)},
            border = {0, 0, 0, 120},
        }

        UI.draw_progress_bar_styled(x, y, barW, barH, maxT, tLeft, 3, timerStyle, "")
    end
    else
        local currentEnemyHealthTotal = 0
        for i = 1, #enemies do
            currentEnemyHealthTotal = currentEnemyHealthTotal + (enemies[i].health or 0)
        end
        if currentEnemyHealthTotal < 1 then
            currentEnemyHealthTotal = 1
        end
        UI.draw_progress_bar(20, 20, 200, 20, currentEnemyHealthTotal, currentEnemyHealthTotal, 1)
    end

    UI.add_centered_label(screenW / 2, 10, T("gameplay.stage") .. tostring(currentLevel), UI_FONT_REG)
    
    -- Display timeout counter
    local timeoutCount = SystemShooterPlayerProgress.getTimeoutCount()
    local maxTimeouts = SystemShooterPlayerProgress.getMaxTimeouts()
    UI.add_centered_label(screenW / 2, 30, "Timeouts: " .. tostring(timeoutCount) .. " / " .. tostring(maxTimeouts), UI_FONT_REG)

    -- Display notification
    if notificationTimer > 0 then
        notificationTimer = notificationTimer - dt
        UI.add_centered_label(screenW / 2, 100, notificationMessage, UI_FONT_BOLD, 0.75)
    end

    local playerHpBarX = screenW - 220
    local playerHpBarY = 20
    local playerHpBarW = 200
    local playerHpBarH = 20

    local playerMaxHealth = SystemShooterPlayerProgress.getMaxHealth()
    local playerHealth = SystemShooterPlayer.getHealth()
    local playerOverhealth = SystemShooterPlayerProgress.getOverhealth()

    local hpStyle = {
      rounding = 10,
      border_size = 0,
      bg   = {30,30,30,220},
      fill = {0,255,0,255},
      overfill = playerOverhealth,
      overfill_color = {50, 150, 255, 200},  -- Blue for overhealth
    }

    UI.draw_progress_bar_layered(playerHpBarX, playerHpBarY, playerHpBarW, playerHpBarH, playerMaxHealth, playerHealth, 2, hpStyle, "")

    local level, xp, xpToNextLevel = SystemShooterPlayerProgress.getProgress()
    local playerInfoText = T("gameplay.level") .. tostring(level) .. T("gameplay.exp") .. tostring(math.floor(xp)) .. "/" .. tostring(math.floor(xpToNextLevel))
    UI.add_centered_label(playerHpBarX + playerHpBarW / 2, playerHpBarY + playerHpBarH + 8, playerInfoText, UI_FONT_REG, 0.85)

    -- Peace progress bar (during start-level or end-level peace)
    if peaceTimerSeconds > 0 then
        local phaseDuration = startLevelPeaceDuration
        if isLevelupPeace then
            phaseDuration = levelupPeaceDuration
        elseif not isStartLevelPeace then
            phaseDuration = endLevelPeaceDuration
        end
        local elapsed = phaseDuration - peaceTimerSeconds
        if elapsed < 0 then
            elapsed = 0
        end
        

        UI.draw_progress_bar_styled(
            screenW / 2 - 100, 80, 200, 10,
            phaseDuration, elapsed, 4, startTimerStyle, ""
        )
    end

    -- Count only enabled (non-disabled) and non-dead enemies as "alive"
    local enemiesAlive = false
    for i = 1, #enemies do
        if not enemies[i].disabled and not enemies[i].isDead then
            enemiesAlive = true
            break
        end
    end
    
    -- Reset peace timer if active enemies exist (safety check)
    -- But not during start-level peace or levelup peace (enemies are intentionally disabled)
    if enemiesAlive and peaceTimerSeconds > 0 and not isStartLevelPeace and not isLevelupPeace then
        peaceTimerSeconds = 0
    end

    --=====================================================================
    --  [ONUPDATE] Rewind System (delegated to module)
    --=====================================================================
    if SystemShooterRewind.isActive() then
        -- During rewind, check for game over
        if SystemShooterPlayer.getHealth() <= 0 then
            SystemShooterRewind.reset()
            SystemShooterRewind.restoreEnemyColors(enemies)
            TriggerGameOver()
            return
        end
        
        -- Helper functions for the rewind module
        local function clearProjectiles(playerOnly)
            ClearAllPlayerProjectiles()
            if not playerOnly then
                ClearAllEnemyProjectiles()
            end
        end
        
        -- Update rewind system
        local result = SystemShooterRewind.update(
            dt, enemies, SystemShooterProjectiles.getPlayerProjectiles(), SystemShooterProjectiles.getEnemyProjectiles(),
            UpdateEnemyMovement, clearProjectiles
        )
        
        -- Sync music with rewind phases
        if result and musicEntity then
            local timeScale = SystemShooterRewind.getTimeScale()
            local phase = result.phase
            local isPlaying = MusicComponent.is_playing(musicEntity)
            
            -- Only log and set speed when phase changes
            if phase ~= lastRewindPhase then
                print("[Rewind Music] Phase changed: " .. tostring(lastRewindPhase) .. " -> " .. tostring(phase) .. ", IsPlaying: " .. tostring(isPlaying))
                lastRewindPhase = phase
                
                if phase == "slowing" then
                    -- Start slowing down - speed will be updated every frame for smooth slowdown
                    print("[Rewind Music] Starting slowdown phase")
                    
                elseif phase == "stopped" then
                    -- Brief pause - keep music playing at very slow speed, just mute it
                    if isPlaying then
                        print("[Rewind Music] Stopped phase - keeping stream alive, muting")
                        -- Don't change speed, just mute
                        MusicComponent.set_volume(musicEntity, 0)
                    end
                    
                elseif phase == "rewinding" then
                    -- Log when entering rewind phase, but don't set speed yet (will be smoothed every frame)
                    if isPlaying then
                        local currentSpeed = MusicComponent.get_speed(musicEntity)
                        local currentPos = MusicComponent.get_position(musicEntity)
                        print("[Rewind Music] ENTERING REWIND PHASE - Current speed: " .. tostring(currentSpeed) .. ", Position: " .. tostring(currentPos))
                        print("[Rewind Music] Will smoothly transition to reverse speed -1.0")
                    else
                        print("[Rewind Music] WARNING: Music not playing when entering rewind phase!")
                    end
                end
            end
            
            -- Update speed and volume every frame during slowing and rewinding
            if phase == "slowing" then
                if isPlaying then
                    MusicComponent.set_speed(musicEntity, timeScale)  -- Update speed for smooth slowdown
                    -- Use cubic ease-in for more dramatic slowdown (faster at start, smoother at end)
                    local volumeProgress = 1.0 - timeScale
                    local easedVolumeProgress = volumeProgress * volumeProgress * volumeProgress
                    local volumeFade = 1.0 - easedVolumeProgress
                    MusicComponent.set_volume(musicEntity, musicMuted and 0 or (musicBaseVolume * volumeFade))
                end
                
            elseif phase == "rewinding" then
                if isPlaying then
                    -- Calculate rewind progress (0 at start, 1 at end)
                    local rewindTimer = SystemShooterRewind.getTimer()
                    local rewindDuration = SystemShooterRewind.getDuration()
                    local rewindProgress = 1.0 - (rewindTimer / rewindDuration)
                    if rewindProgress < 0 then rewindProgress = 0 end
                    if rewindProgress > 1 then rewindProgress = 1 end
                    
                    -- Calculate required average playback speed to reach rememberedMusicPosition exactly
                    local currentMusicPos = MusicComponent.get_position(musicEntity)
                    local targetMusicPos = rememberedMusicPosition
                    local positionDifference = currentMusicPos - targetMusicPos
                    
                    -- Required average speed = distance / time remaining
                    -- Negative because we're going backwards
                    local requiredAvgSpeed = -positionDifference / rewindTimer
                    if rewindTimer < 0.01 then requiredAvgSpeed = 0 end  -- Avoid division by zero
                    
                    -- Apply sine wave curve to match visual rewind (peaks in middle, slows at ends)
                    -- sin(progress * π) gives us: 0 at start → 1.0 at middle → 0 at end
                    local speedCurve = math.sin(rewindProgress * math.pi)
                    
                    -- Scale the curve so that the average equals requiredAvgSpeed
                    -- Integral of sin(x*π) from 0 to 1 is 2/π ≈ 0.6366
                    -- So multiply by π/2 ≈ 1.571 to normalize the average to 1.0
                    local normalizedSpeed = requiredAvgSpeed * speedCurve * (math.pi / 2)
                    MusicComponent.set_speed(musicEntity, normalizedSpeed)
                    
                    -- Smoothly increase volume from 0% to 50% (capped) as rewind progresses
                    -- Use ease-in-out for smoother volume transition
                    local volumeFade = rewindProgress < 0.5 
                        and 2 * rewindProgress * rewindProgress 
                        or 1 - math.pow(-2 * rewindProgress + 2, 2) / 2
                    -- Cap at 50% volume during rewind
                    volumeFade = volumeFade * 0.5
                    MusicComponent.set_volume(musicEntity, musicMuted and 0 or (musicBaseVolume * volumeFade))
                end
            end
        end
        
        if result then
            -- Apply returned values
            if result.levelTimer then
                levelTimerSeconds = result.levelTimer
            end
            
            -- Clear enemy projectiles when signaled (after they've been rewound)
            if result.clearEnemyProjectiles then
                ClearAllEnemyProjectiles()
            end
            
            -- Handle rewind completion
            if result.done then
                -- Set final values
                local cfg = SystemShooterLevels.getLevelConfig(currentLevel)
                levelTimerSeconds = cfg and cfg.timeLimitSeconds or 0
                
                -- Enemies are already at their spawnpoints with correct HP from rewind
                -- Just disable them for peace state (they'll be enabled when peace ends)
                for _, enemy in ipairs(enemies) do
                    SystemShooterEnemy.setEnemyDisabled(enemy, true)
                    -- Reset shoot timer to resync with beat after rewind
                    enemy.shootTimer = 0
                end
                
                -- Start peace timer and restore music
                peaceTimerSeconds = startLevelPeaceDuration
                isStartLevelPeace = true
                
                -- Restore music to normal playback (position should already be at rememberedMusicPosition from smooth rewind)
                if musicEntity then
                    local finalPos = MusicComponent.get_position(musicEntity)
                    print("[Rewind Music] Rewind complete - Final position: " .. tostring(finalPos) .. ", Target was: " .. tostring(rememberedMusicPosition))
                    MusicComponent.set_speed(musicEntity, 1.0)  -- Normal speed
                    -- Position should already be correct from smooth rewind, but ensure it's exact
                    if math.abs(finalPos - rememberedMusicPosition) > 0.1 then
                        print("[Rewind Music] Position drift detected, correcting...")
                        MusicComponent.seek(musicEntity, rememberedMusicPosition)
                    end
                    -- Fade in volume from current (50%) to full over 1 second
                    MusicComponent.fade_volume(musicEntity, musicMuted and 0 or musicBaseVolume, 1.0)
                    
                    -- Resync beatTimer with current music position to maintain beat timing
                    local musicPos = MusicComponent.get_position(musicEntity)
                    if beatBopHasStarted and musicPos >= beatStartDelaySeconds then
                        -- Calculate how many beats have elapsed since drop
                        local timeSinceDrop = musicPos - beatStartDelaySeconds
                        local beatCount = math.floor(timeSinceDrop / secondsPerBeat)
                        -- Set beatTimer to the remainder (current position within the beat)
                        beatTimer = timeSinceDrop - (beatCount * secondsPerBeat)
                    end
                end
            end
        end
        return
    end

    -- Handle levelup peace timer separately (enemies exist but are disabled)
    if isLevelupPeace and peaceTimerSeconds > 0 then
        peaceTimerSeconds = peaceTimerSeconds - dt
        if peaceTimerSeconds <= 0 then
            isLevelupPeace = false
            SystemShooterEnemy.enableAllEnemies(enemies)
        end
        -- Don't process normal level flow during levelup peace
        if SystemShooterPlayer.getHealth() <= 0 then
            TriggerGameOver()
        end
        return
    end

     --=====================================================================
     --  [ONUPDATE] Level Flow (Win / Lose / Timeout)
     --=====================================================================
    if SystemShooterPlayer.getHealth() <= 0 then
        TriggerGameOver()
        return
    elseif not enemiesAlive then
        if peaceTimerSeconds <= 0 then
            if isStartLevelPeace then
                -- Start-level peace ended, enable the preview enemies
                isStartLevelPeace = false
                SystemShooterEnemy.enableAllEnemies(enemies)
            else
                -- End-level peace finished, trigger level transition
                OnEnemyKilled()
            end
        else
            peaceTimerSeconds = peaceTimerSeconds - dt
        end
    elseif levelTimerSeconds <= 0 and enemiesAlive and not SystemShooterRewind.isActive() then
        OnLevelTimeout()
    end
end

 --=====================================================================
 --  [DAMAGE / FEEDBACK] Flash + Damage Cooldowns
 --=====================================================================
function FlashEnemy(enemy)
    if not enemy or not enemy.sprite then return end
    enemy.flashTimer = flashDuration
end

function UpdateFlash()
    local dt = GetDt()

    -- Enemy flash
    if flashDuration > 0 then
        for i = 1, #enemies do
            local enemy = enemies[i]
            if enemy.flashTimer and enemy.flashTimer > 0 then
                enemy.flashTimer = enemy.flashTimer - dt
                if enemy.flashTimer < 0 then enemy.flashTimer = 0 end

                local t = enemy.flashTimer / flashDuration
                if t < 0 then t = 0 end
                if t > 1 then t = 1 end

                local baseColor = enemy.color or {255, 255, 255}
                local r = math.floor(255 * t + baseColor[1] * (1.0 - t) + 0.5)
                local g = math.floor(0 * t + baseColor[2] * (1.0 - t) + 0.5)
                local b = math.floor(0 * t + baseColor[3] * (1.0 - t) + 0.5)
                
                -- Respect disabled state (darken colors)
                if enemy.disabled then
                    r = math.floor(r * 0.5)
                    g = math.floor(g * 0.5)
                    b = math.floor(b * 0.5)
                end
                
                Sprite.set_color(enemy.sprite, r, g, b)
            end
        end
    end

    SystemShooterPlayer.updateFlash(dt)
end

----------------------------------------------------------
-- Enemy-Player collision with damage cooldown
----------------------------------------------------------
function UpdateEnemyCollision()
    -- Skip collision if antivirus is active
    if SystemShooterPlayer.isAntivirusActive() then
        return
    end
    
    if SystemShooterPlayer.getDamageCooldown() > 0 then
        return
    end
    
    -- Skip collision damage during rewind
    if SystemShooterRewind.isActive() then
        return
    end
    
    -- Check collision between enemies and player
    local pX, pY = SystemShooterPlayer.getPosition()
    local pSize = SystemShooterPlayer.getSize()
    local playerCenterX = pX + pSize/2
    local playerCenterY = pY + pSize/2
    for i = 1, #enemies do
        local enemy = enemies[i]
        -- Skip collision for disabled or dead enemies
        if enemy.disabled or enemy.isDead then
            goto continue_collision
        end
        local enemyCenterX = enemy.x + enemySize/2
        local enemyCenterY = enemy.y + enemySize/2
        local dx = playerCenterX - enemyCenterX
        local dy = playerCenterY - enemyCenterY
        local distSq = dx * dx + dy * dy
        local hitRadius = collisionRadius + pSize/2
        
        if distSq < hitRadius * hitRadius then
             -- Collision! Damage player
            local dist = math.sqrt(distSq)
            if dist == 0 then
                dist = 1
                dx = 0
                dy = -1
            end

            local nx = dx / dist
            local ny = dy / dist
            local padding = 2
            local targetDistance = hitRadius + padding

            local newPlayerCenterX = enemyCenterX + nx * targetDistance
            local newPlayerCenterY = enemyCenterY + ny * targetDistance
            local newPX = newPlayerCenterX - pSize/2
            local newPY = newPlayerCenterY - pSize/2
            SystemShooterPlayer.setPosition(newPX, newPY)

            local pushX = nx
            local pushY = ny
            local dirX = enemy.dashDirX or 0
            local dirY = enemy.dashDirY or 0
            if dirX ~= 0 or dirY ~= 0 then
                local sideLX = -dirY
                local sideLY = dirX
                local sideRX = dirY
                local sideRY = -dirX
                local dotL = nx * sideLX + ny * sideLY
                local dotR = nx * sideRX + ny * sideRY
                if dotL > dotR then
                    pushX = sideLX
                    pushY = sideLY
                else
                    pushX = sideRX
                    pushY = sideRY
                end
            end
            SystemShooterPlayer.applyKnockback(pushX, pushY)

            local currentHealth = SystemShooterPlayer.getHealth()
            local currentOverhealth = SystemShooterPlayerProgress.getOverhealth()
            local damage = 10
            
            -- Damage overhealth first if present
            if currentOverhealth > 0 then
                if currentOverhealth >= damage then
                    -- Overhealth absorbs all damage
                    SystemShooterPlayerProgress.setOverhealth(currentOverhealth - damage)
                else
                    -- Overhealth partially absorbs, remaining damage goes to health
                    local remainingDamage = damage - currentOverhealth
                    SystemShooterPlayerProgress.setOverhealth(0)
                    SystemShooterPlayer.setHealth(currentHealth - remainingDamage)
                end
            else
                -- No overhealth, damage health directly
                SystemShooterPlayer.setHealth(currentHealth - damage)
            end
            
            runDamageTaken = runDamageTaken + damage
            SystemShooterPlayer.flash()
            SystemShooterPlayer.setDamageCooldown(SystemShooterPlayer.getDamageCooldownDuration())
            if playerDamageSfxEntity then
                AudioComponent.play(playerDamageSfxEntity)
            end
            break
        end
        ::continue_collision::
    end
end

function FlashPlayer()
    SystemShooterPlayer.flash()
end

 --=====================================================================
 --  [ENEMY SPECIAL] Beam + Teleport Particles
 --=====================================================================
local BEAM_RADIUS = 16
local BEAM_DAMAGE = 15

function SpawnBeam(enemy, fromX, fromY, toX, toY)
    local dx = toX - fromX
    local dy = toY - fromY
    local dist = math.sqrt(dx * dx + dy * dy)
    if dist < 1 then return end
    
    local dirX = dx / dist
    local dirY = dy / dist
    
    local maxDist = math.sqrt(screenW * screenW + screenH * screenH)
    local spacing = 8
    local particleCount = math.floor(maxDist / spacing)
    
    for i = 0, particleCount - 1 do
        local offset = i * spacing
        local px = fromX + dirX * offset
        local py = fromY + dirY * offset
        if px >= -20 and px <= screenW + 20 and py >= -20 and py <= screenH + 20 then
            ParticleSystem.emitBeamParticle(px, py, 255, 255, 50, dirX, dirY)
        end
    end
    
    if SystemShooterPlayer.getDamageCooldown() <= 0 then
        local pX, pY = SystemShooterPlayer.getPosition()
        local pSize = SystemShooterPlayer.getSize()
        local playerCenterX = pX + pSize / 2
        local playerCenterY = pY + pSize / 2
        local playerRadius = pSize / 2
        
        local apx = playerCenterX - fromX
        local apy = playerCenterY - fromY
        local abx = dirX * maxDist
        local aby = dirY * maxDist
        
        local abLenSq = abx * abx + aby * aby
        local t = (apx * abx + apy * aby) / abLenSq
        if t < 0 then t = 0 end
        if t > 1 then t = 1 end
        
        local closestX = fromX + abx * t
        local closestY = fromY + aby * t
        
        local distToPlayerX = playerCenterX - closestX
        local distToPlayerY = playerCenterY - closestY
        local distToPlayerSq = distToPlayerX * distToPlayerX + distToPlayerY * distToPlayerY
        
        local hitRadius = playerRadius + BEAM_RADIUS
        if distToPlayerSq < hitRadius * hitRadius then
            local currentHealth = SystemShooterPlayer.getHealth()
            local currentOverhealth = SystemShooterPlayerProgress.getOverhealth()
            
            -- Damage overhealth first if present
            if currentOverhealth > 0 then
                if currentOverhealth >= BEAM_DAMAGE then
                    -- Overhealth absorbs all damage
                    SystemShooterPlayerProgress.setOverhealth(currentOverhealth - BEAM_DAMAGE)
                else
                    -- Overhealth partially absorbs, remaining damage goes to health
                    local remainingDamage = BEAM_DAMAGE - currentOverhealth
                    SystemShooterPlayerProgress.setOverhealth(0)
                    SystemShooterPlayer.setHealth(currentHealth - remainingDamage)
                end
            else
                -- No overhealth, damage health directly
                SystemShooterPlayer.setHealth(currentHealth - BEAM_DAMAGE)
            end
            
            runDamageTaken = runDamageTaken + (BEAM_DAMAGE or 0)
            SystemShooterPlayer.flash()
            SystemShooterPlayer.setDamageCooldown(SystemShooterPlayer.getDamageCooldownDuration())
        end
    end
end

function EmitTeleportBurst(x, y, r, g, b, inward)
    ParticleSystem.emitTeleportBurst(x, y, r, g, b, inward)
end

function EmitBeamCharge(fromX, fromY, toX, toY, r, g, b)
    ParticleSystem.emitBeamCharge(fromX, fromY, toX, toY, r, g, b)
end

 --=====================================================================
 --  [ENEMIES] Movement / Behavior (Wrapper)
 --=====================================================================
function UpdateEnemyMovement()
    local pX, pY = SystemShooterPlayer.getPosition()
    local pSize = SystemShooterPlayer.getSize()
    SystemShooterEnemy.updateEnemyMovement(
        enemies,
        pX, pY, pSize,
        screenW, screenH,
        enemyShootIntervalSeconds,
        SpawnEnemyProjectile,
        nil, -- TriggerWallLerp removed
        SpawnBeam,
        EmitTeleportBurst,
        EmitBeamCharge
    )
end

 --=====================================================================
 --  [MUSIC / RHYTHM] Beat Bop
 --=====================================================================
function UpdateBeatBop()
    local dt = GetDt()
    
    -- Check if music has reached the drop point (using actual music position)
    local beatBopActive = false
    if not beatBopHasStarted and musicEntity and MusicComponent.is_playing(musicEntity) then
        local musicPos = MusicComponent.get_position(musicEntity)
        if musicPos >= beatStartDelaySeconds then
            beatBopHasStarted = true
            beatBopActive = true
            -- Trigger immediate bop on the drop
            bopTimer = bopDurationSeconds
        end
    elseif beatBopHasStarted then
        beatBopActive = true
    end

    -- Update beat timer (only when beatBopActive)
    if beatBopActive then
        beatTimer = beatTimer + dt
        if beatTimer >= secondsPerBeat then
            beatTimer = beatTimer - secondsPerBeat
            bopTimer = bopDurationSeconds
        end
    end

    if beatBopActive and bopTimer > 0 then
        bopTimer = bopTimer - dt
        local t = bopTimer / bopDurationSeconds
        local scale = 1.0 + bopScale * t

        for i = 1, #enemies do
            local enemy = enemies[i]
            -- Skip dead enemies
            if enemy.isDead then
                goto continue_bop1
            end
            local state = enemy.teleportState
            local isTeleporting = state == "shrinking" or state == "growing" or state == "teleporting"
            if enemy and enemy.sprite and not isTeleporting then
                local currentSize = enemy.displaySize or enemy.baseSize or enemy.size or enemyBaseImageSize
                local scaledSize = math.floor(currentSize * scale)
                Sprite.set_image_width(enemy.sprite, scaledSize)
                Sprite.set_image_height(enemy.sprite, scaledSize)
                local offset = (scaledSize - currentSize) / 2
                Entity.set_global_pos(enemy.entity, enemy.x - offset, enemy.y - offset)
            end
            ::continue_bop1::
        end
        
        SystemShooterPickups.applyBeatBop(t)
    else
        -- Always update enemy display sizes to match current health (not just during bop)
        for i = 1, #enemies do
            local enemy = enemies[i]
            -- Skip dead enemies
            if enemy.isDead then
                goto continue_bop2
            end
            local state = enemy.teleportState
            local isTeleporting = state == "shrinking" or state == "growing" or state == "teleporting"
            if enemy and enemy.sprite and not isTeleporting then
                local currentSize = enemy.displaySize or enemy.baseSize or enemy.size or enemyBaseImageSize
                Sprite.set_image_width(enemy.sprite, math.floor(currentSize))
                Sprite.set_image_height(enemy.sprite, math.floor(currentSize))
                Entity.set_global_pos(enemy.entity, enemy.x, enemy.y)
            end
            ::continue_bop2::
        end
        
        SystemShooterPickups.resetBop()
    end
end

 --=====================================================================
 --  [ENEMY PROJECTILES] Spawn (now uses SystemShooterProjectiles module)
 --=====================================================================
function SpawnEnemyProjectile(enemy)
    SystemShooterProjectiles.spawnEnemyProjectile(enemy)
end

return SystemShooter