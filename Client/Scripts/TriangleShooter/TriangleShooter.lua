local TriangleShooter = {}

 --=====================================================================
 --  [MODULE] Imports / Dependencies
 --=====================================================================
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local TriangleShooterLevels = require("Scripts.TriangleShooter.TriangleShooterLevels")
local TriangleShooterEnemy = require("Scripts.TriangleShooter.TriangleShooterEnemy")
local TriangleShooterAbilities = require("Scripts.TriangleShooter.TriangleShooterAbilities")
local TriangleShooterPlayerProgress = require("Scripts.TriangleShooter.TriangleShooterPlayerProgress")
local ParticleSystem = require("Scripts.TriangleShooter.ParticleSystem")
local TriangleShooterPickups = require("Scripts.TriangleShooter.TriangleShooterPickups")
local TriangleShooterRewind = require("Scripts.TriangleShooter.TriangleShooterRewind")
local Localisation = require("Scripts.TriangleShooter.Localisation")


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
local TriangleShooterUI = require("Scripts.TriangleShooter.TriangleShooterUI")
local UI_FONT_BOLD = "ImGuiDefaultBold"
local UI_FONT_REG  = "ImGuiDefault"

local function ApplyLanguageFonts()
    if language == "ja" then
      UI_FONT_REG   = "ImGuiDefaultJP"
      UI_FONT_BOLD  = "ImGuiDefaultBoldJP"
      UI_FONT_SUB = "ImGuiSubJP"
      UI_FONT_HEADER = "ImGuiHeaderJP"
      UI_FONT_TITLE = "ImGuiTitleJP"
    else
      UI_FONT_REG   = "ImGuiDefault"
      UI_FONT_BOLD  = "ImGuiDefaultBold"
      UI_FONT_SUB = "ImGuiSub"
      UI_FONT_HEADER = "ImGuiHeader"
      UI_FONT_TITLE = "ImGuiTitle"
    end
end

ApplyLanguageFonts()

-- For other modules to access localisation and fonts
TS_UI = TS_UI or {}
TS_UI.T = function(key) return Localisation.t(key) end
TS_UI.FONT_REG = UI_FONT_REG
TS_UI.FONT_BOLD = UI_FONT_BOLD
TS_UI.FONT_SUB = UI_FONT_SUB
TS_UI.FONT_HEADER = UI_FONT_HEADER
TS_UI.FONT_TITLE = UI_FONT_TITLE

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
local beatStartDelaySeconds = (8 * 4) * secondsPerBeat
local beatStartDelayCounter = 0

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

local isPaused = false
local pauseScreen = "pause" -- "pause" | "settings" | "leaderboard"

local pauseTopLeaderboard = nil
local pauseLeaderboardFetched = false

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

 --=====================================================================
 --  [HELPERS] Time
 --=====================================================================
local function GetDt()
    return Mafs.delta_time()
end

 --=====================================================================
 --  [STATE] Screen / Window / Walls
 --=====================================================================
-- SCREEN BOUNDS (UPDATED EACH FRAME FROM WINDOW SIZE)
local screenW = 1920
local screenH = 1080

-- WALL SETTINGS
local wallPingPongEnabled = true
local wallMaxShrinkX = 600        -- Max pixels each horizontal wall can shrink (1920 - 400 = 1520, /2 = 760)
local wallMaxShrinkY = 200        -- Max pixels each vertical wall can shrink (1080 - 400 = 680, /2 = 340)
local wallShrinkSpeedPerSecond = 70      -- Pixels per second each wall shrinks
local wallExpandDurationSeconds = 0.75   -- how long walls spend expanding after a trigger
local wallMinWindowWidth = 600
local wallMinWindowHeight = 600
local wallMaxWindowWidth = 1920
local wallMaxWindowHeight = 1080
local wallExpandSpeedMultiplier = 3.0

-- Each wall has: offset (current shrink amount), expandTimer (>0 means expanding)
local leftWallOffset = 0
local leftWallExpandTimer = 0
local rightWallOffset = 0
local rightWallExpandTimer = 0
local topWallOffset = 0
local topWallExpandTimer = 0
local bottomWallOffset = 0
local bottomWallExpandTimer = 0

-- Base window size and initial position (captured on first frame)
local windowBaseWidth = 1920
local windowBaseHeight = 1080
local windowInitialX = nil
local windowInitialY = nil

-- Original window size (never changes)
local originalWindowWidth = 1920
local originalWindowHeight = 1080
local windowTransitionActive = false
local windowTransitionTimer = 0
local windowTransitionDurationSeconds = 0.75
local windowTransitionStartW = 0
local windowTransitionStartH = 0
local windowTransitionTargetW = 0
local windowTransitionTargetH = 0
local pendingLevelIndex = nil
local pendingResetPlayerState = false

 --=====================================================================
 --  [STATE] Player (Triangle)
 --=====================================================================
-- PLAYER (TRIANGLE)
local player
local playerSprite
local playerSize = 48
local playerX = 400
local playerY = 300
local playerSpeed = 0.5  -- Mouse sensitivity multiplier
local playerHealth = 100

-- PLAYER FLASH EFFECT
local playerFlashTimer = 0
local playerFlashDuration = 0.2  -- seconds

-- DAMAGE COOLDOWN
local damageCooldown = 0
local damageCooldownDuration = 0.5  -- seconds

-- PROJECTILE SETTINGS
local projectiles = {}      -- Active projectiles
local projectilePool = {}   -- Inactive projectiles (reusable)
local projectileSize = 24
local projectileSpeed = 960 --PIXELS PER SECOND
local projectileLifetimeSeconds = 2  -- seconds projectile can live before auto-despawn
local fireCooldownTimer = 0
local isFiring = false

local currentFireInterval = 0.5

local recoilOffset = 0
local recoilMaxOffset = 16
local recoilLerpSpeed = 12

-- Current aim direction (updated each frame)
local aimDirX = 0
local aimDirY = -1  -- Default: pointing up

-- ENEMY (CUBE)
local enemySize = 48

local enemies = {}
local levelEnemyHealth = 50
local StartLevel    
local LoadLevel

local function CreateEnemy(x, y, config)
    config = config or {}
    return TriangleShooterEnemy.createEnemy(x, y, config, playerX, playerY, playerSize)
end

 --=====================================================================
 --  [LEVEL FLOW] Window Transition
 --=====================================================================
function UpdateWindowTransition()
    if not windowTransitionActive then
        return
    end

    local dt = GetDt()

    if windowTransitionTimer <= 0 then
        windowTransitionActive = false
        local index = pendingLevelIndex
        local reset = pendingResetPlayerState
        pendingLevelIndex = nil
        pendingResetPlayerState = false

        if index ~= nil then
            screenW = Window.get_width()
            screenH = Window.get_height()
            LoadLevel(index, reset)
        end
        return
    end

    windowTransitionTimer = windowTransitionTimer - dt
    local t = 1.0 - (windowTransitionTimer / windowTransitionDurationSeconds)
    if t < 0 then t = 0 end
    if t > 1 then t = 1 end

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
end

local function ClearEnemies()
    TriangleShooterEnemy.clearEnemies(enemies)
    enemies = {}
end

local function ClearAllPlayerProjectiles()
    for i = #projectiles, 1, -1 do
        local proj = projectiles[i]
        Entity.set_global_pos(proj.entity, -1000, -1000)
        table.insert(projectilePool, table.remove(projectiles, i))
    end
end

local enemyProjectiles = {}
local enemyProjectilePool = {}

local function ClearAllEnemyProjectiles()
    for i = #enemyProjectiles, 1, -1 do
        local proj = enemyProjectiles[i]
        Entity.set_global_pos(proj.entity, -1000, -1000)
        table.insert(enemyProjectilePool, table.remove(enemyProjectiles, i))
    end
end

local function ResetRunStateForMenu()
    -- cancel any pending window level loads just in case
    windowTransitionActive = false
    pendingLevelIndex = nil
    pendingResetPlayerState = false

    ClearEnemies()
    ClearAllPlayerProjectiles()
    ClearAllEnemyProjectiles()

    fireCooldownTimer = 0
    damageCooldown = 0
    peaceTimerSeconds = 0
    isStartLevelPeace = false
    isLevelupPeace = false

    -- Reset rewind state
    TriangleShooterRewind.reset()

    -- reset stage/run basics
    currentLevel = 1
    levelTimerSeconds = 0

    isPaused = false
    pauseScreen = "pause"
end

 --=====================================================================
 --  [STATE] Enemy Projectiles
 --=====================================================================
-- ENEMY PROJECTILE SETTINGS
enemyProjectiles = {}
enemyProjectilePool = {}
local enemyProjectileSize = 24
local enemyProjectileSpeed = 420 -- PIXELS PER SECOND 
local enemyShootIntervalSeconds = 0.5

-- COLLISION SETTINGS
local collisionRadius = 24  -- Half of enemy size for circle collision

-- FLASH EFFECT
local flashTimer = 0
local flashDuration = 0.2  -- seconds

local knockbackTimer = 0
local knockbackDuration = 0.2
local knockbackBaseSpeed = 1200
local knockbackDirX = 0
local knockbackDirY = 0

local playerBaseImageWidth = playerSize
local playerBaseImageHeight = playerSize
local enemyBaseImageSize = enemySize

local globalFrame = 0
local peaceTimerSeconds = 0
local startLevelPeaceDuration = 3
local endLevelPeaceDuration = 2.5
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
    local firepower = TriangleShooterPlayerProgress.getFirepower()
    local pierce  = TriangleShooterPlayerProgress.getPierceCount()
    local bounce  = TriangleShooterPlayerProgress.getBounceCount()
    local fireI   = TriangleShooterPlayerProgress.getCurrentFireInterval()
    local maxHP   = TriangleShooterPlayerProgress.getMaxHealth()

    local pLevel, pXp, pXpToNext = TriangleShooterPlayerProgress.getProgress()

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
    isFiring = false
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

    -- MUSIC (base 0..128)
    local musicOut = math.floor(128 * masterVol * musicVol + 0.5)
    if musicEntity then
        AudioComponent.change_volume(musicEntity, musicMuted and 0 or musicOut)
    end

    -- SFX (scale your existing base volumes)
    local sfxMul = masterVol * sfxVol
    if playerDamageSfxEntity then AudioComponent.change_volume(playerDamageSfxEntity, math.floor(48 * sfxMul + 0.5)) end
    if gunshot3SfxEntity     then AudioComponent.change_volume(gunshot3SfxEntity,     math.floor( 4 * sfxMul + 0.5)) end
    if impact3SfxEntity      then AudioComponent.change_volume(impact3SfxEntity,      math.floor(16 * sfxMul + 0.5)) end
end


 --=====================================================================
 --  [LEVEL FLOW] Load / Start Level
 --=====================================================================
local function SpawnEnemiesForLevel(spawnDisabled)
    local cfg = TriangleShooterLevels.getLevelConfig(currentLevel)
    if not cfg then
        return
    end

    -- Clear spawn positions for rewind system
    TriangleShooterRewind.clearSpawnPositions()

    local centerX = screenW / 2 - enemySize / 2
    local centerY = screenH / 2 - enemySize / 2

    local enemyTemplates = TriangleShooterLevels.getEnemyTemplates()
    
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
                TriangleShooterEnemy.setEnemyDisabled(e, true)
            end
            table.insert(enemies, e)
            -- Store spawn position for rewind system
            TriangleShooterRewind.setSpawnPosition(#enemies, spawnX, spawnY)
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
                TriangleShooterEnemy.setEnemyDisabled(e, true)
            end
            table.insert(enemies, e)
            -- Store spawn position for rewind system
            TriangleShooterRewind.setSpawnPosition(#enemies, centerX, centerY)
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
                    TriangleShooterEnemy.setEnemyDisabled(e, true)
                end
                table.insert(enemies, e)
                -- Store spawn position for rewind system
                TriangleShooterRewind.setSpawnPosition(#enemies, ex, ey)
            end
        end
    end
end

LoadLevel = function(index, resetPlayerState)
    local cfg = TriangleShooterLevels.getLevelConfig(index)
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

    wallPingPongEnabled = cfg.wallPingPong and true or false

     --=====================================================================
     --  [LOAD LEVEL] Reset Walls / Clear Enemies
     --=====================================================================
    if wallPingPongEnabled then
        leftWallOffset = 0
        leftWallExpandTimer = 0
        rightWallOffset = 0
        rightWallExpandTimer = 0
        topWallOffset = 0
        topWallExpandTimer = 0
        bottomWallOffset = 0
        bottomWallExpandTimer = 0
        windowInitialX = nil
        windowInitialY = nil
    end

    ClearEnemies()

    levelEnemyHealth = cfg.enemyHealth or levelEnemyHealth
    enemyShootIntervalSeconds = cfg.enemyShootIntervalSeconds or enemyShootIntervalSeconds

    -- Spawn enemies as disabled (preview state) during start-level peace
    SpawnEnemiesForLevel(true)
    
    -- Start the start-level peace timer (enemies will be enabled when it expires)
    peaceTimerSeconds = startLevelPeaceDuration
    isStartLevelPeace = true

     --=====================================================================
     --  [LOAD LEVEL] Reset Player State
     --=====================================================================
    if resetPlayerState then
        playerHealth = TriangleShooterPlayerProgress.getMaxHealth()
        playerX = screenW / 2 - playerSize / 2
        playerY = screenH / 2 - playerSize / 2
        Entity.set_global_pos(player, playerX, playerY)
        Sprite.set_color(playerSprite, 255, 255, 255)
        playerFlashTimer = 0
        damageCooldown = 0
    end

    if playerHealth == nil or playerHealth <= 0 then
        playerHealth = TriangleShooterPlayerProgress.getMaxHealth()
    end
end

StartLevel = function(index, resetPlayerState)
    local cfg = TriangleShooterLevels.getLevelConfig(index)
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

    windowTransitionActive = true
    windowTransitionTimer = windowTransitionDurationSeconds
    windowTransitionStartW = currentWidth
    windowTransitionStartH = currentHeight
    windowTransitionTargetW = targetWidth
    windowTransitionTargetH = targetHeight
    pendingLevelIndex = index
    pendingResetPlayerState = resetPlayerState
end

local function OnEnemyKilled()
    local nextIndex = currentLevel + 1
    TryUpdateBestStage(nextIndex)
    TriangleShooterLevels.regenerateLevel(nextIndex)
    if TriangleShooterLevels.getLevelConfig(nextIndex) then
        StartLevel(nextIndex, false)
    else
        StartLevel(currentLevel, false)
    end
end

local function StartRewindSequence()
    -- Get level config for time limit and enemy target health
    local cfg = TriangleShooterLevels.getLevelConfig(currentLevel)
    local maxHealth = TriangleShooterPlayerProgress.getMaxHealth()
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
    TriangleShooterRewind.start(enemies, playerHealth, maxHealth, levelTimeLimit, enemyTargetHealthList)
    
    -- Pause music
    if musicEntity then
        AudioComponent.pause(musicEntity)
    end
end

local function OnLevelTimeout()
    -- Reduce level health by 20% on timeout (applied to config for next spawn)
    local cfg = TriangleShooterLevels.getLevelConfig(currentLevel)
    if cfg and cfg.enemies then
        for _, enemy in ipairs(cfg.enemies) do
            if enemy.health then
                enemy.health = math.floor(enemy.health * 0.8)
                if enemy.health < 1 then enemy.health = 1 end
            end
        end
    end
    
    -- Start rewind sequence instead of instant restart
    StartRewindSequence()
end

 --=====================================================================
 --  [ENGINE CALLBACKS] OnStart
 --=====================================================================
function TriangleShooter:OnStart()
    -- Enable relative mouse mode (hides cursor, gives delta movement)
    Input.set_relative_mouse_mode(false)
    
    -- Create player triangle
    player = Entity.create_entity()
    
    -- Start at center of screen
    playerX = screenW / 2 - playerSize / 2
    playerY = screenH / 2 - playerSize / 2
    Entity.set_global_pos(player, playerX, playerY)
    
    -- Add sprite component 
    playerSprite = Entity.add_sprite_component(player, assets.textures.Triangle, playerSize, playerSize, 10)
    Sprite.set_columns(playerSprite, 1)

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
    Entity.add_audio_component(musicEntity, "technoSong", false)
    -- Music will start when player presses START GAME

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
        UI.add_centered_label(cx, 82, T("play.error"), UI_FONT_BOLD, 1.0)
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
            namePromptError = "Please enter a name."
        else
            namePromptError = ""
            playerName = name
            needsPlayerName = false
            showNamePrompt = false

            Json.save_player_name(playerName)
            UI.clear_input("ts_player_name")
            if pendingStartAfterName then
                pendingStartAfterName = false
                menuStarting = true
                menuStartTimer = menuStartDelay
            end
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
        local barY = panelY + by - barH - 14

        UI.draw_progress_bar(barX, barY, barW, barH, menuStartDelay, elapsed, 4)

        if menuStartTimer <= 0 then
            menuStarting = false
            inMainMenu = false
            menuScreen = "main"
            Input.set_relative_mouse_mode(true)
            ResetRunStats()
            TriangleShooterPickups.clearAll()
            TriangleShooterPlayerProgress.reset()
            isGameOver = false
            runLeaderboardSubmitted = false
            StartLevel(1, true)
            -- Start music from beginning (synced with gameplay)
            if musicEntity and not musicStartedThisLaunch then
                AudioComponent.play(musicEntity, 0, -1)
                musicStartedThisLaunch = true
            end
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
            menuStarting = true
            menuStartTimer = menuStartDelay
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

end

local function DrawLeaderboardMenu(screenW, screenH, dt)
    UI.add_panel(0, 0, screenW, screenH, 0.65, 0, 0, 0, 0)

    local panelW = math.floor(math.max(520, math.min(screenW * 0.70, 860)))
    local panelH = math.floor(math.max(420, math.min(screenH * 0.70, 600)))
    local panelX = math.floor((screenW - panelW) / 2)
    local panelY = math.floor((screenH - panelH) / 2)

    UI.begin_child(panelX, panelY, panelW, panelH, "TS_Leaderboard",
        true, 0,
        true, 0.92, 12, 25, 25, 25,
        0.01, false, 0.85,
        UI__COLOUR_THEME[1], UI__COLOUR_THEME[2], UI__COLOUR_THEME[3], 1.0 -- RGBA border
    )

    local cx = panelW / 2
    local footerH = 100 -- space reserved for the Back button area
    local contentX = 26
    local contentY = math.floor(panelH * 0.13)  
    local contentW = panelW - 52
    local contentH = panelH - contentY - footerH

    UI.add_centered_label(cx, math.floor(panelH * 0.05), T("menu.leaderboard"), UI_FONT_TITLE, 1)
    UI.add_centered_label(cx, math.floor(panelH * 0.13), T("leaderboard.description"), UI_FONT_REG, 1)

    local NO_BACKGROUND = 128
    UI.begin_child(contentX, contentY, contentW, contentH, "TS_LeaderboardContent",
    false, NO_BACKGROUND, false)

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

                -- Left column: "1. Name"
                UI.add_label(listX, y, 0, 0, string.format("%2d. %s", i, name), UI_FONT_REG, 1.1)

                -- Right column: "Stage X"
                UI.add_label(stageX, y, 0, 0, string.format("Stage %d", stage), UI_FONT_REG, 1.1)
            else
                UI.add_label(listX, y, 0, 0, string.format("%2d. --", i), UI_FONT_REG, 1.1)
            end
        end
    else
        UI.add_centered_label(cx, listY + 10, "(No scores yet)", UI_FONT_REG, 1.1)
    end

    UI.end_child()
    -- Back button
    local bw, bh = math.min(320, math.floor(panelW * 0.55)), 50
    local bx = math.floor((panelW - bw) / 2)
    local by = panelH - bh - 32

    UI.add_button(bx, by, bw, bh, "BACK", "menu_back",
        "ImGuiDefaultBold", 1.0, 12, true, 74, 12, 255, 0.95)

    if UI.was_button_pressed("menu_back") then
        menuScreen = "main"
    end

    UI.end_child()
end

local function DrawSettingsMenu(screenW, screenH, dt)
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
    local footerH = 100 -- space reserved for the Back button area
    local contentX = 26
    local contentY = math.floor(panelH * 0.13)  
    local contentW = panelW - 52
    local contentH = panelH - contentY - footerH

    UI.add_centered_label(cx, math.floor(panelH * 0.05), T("settings.title"), UI_FONT_TITLE, 1)

    -- Child for scrollable content 
    local NO_BACKGROUND = 128
    UI.begin_child(contentX, contentY, contentW, contentH, "TS_SettingsContent",
    false, NO_BACKGROUND, false)

    local innerCX = contentW / 2
    local sliderW = math.floor(contentW * 0.5)
    local sliderX = math.floor((contentW - sliderW) / 2)

    UI.add_centered_label(innerCX, 12, T("settings.audio"), UI_FONT_HEADER, 1.2)

    local sliderStyle = {
    height = 21,        -- thickness
    rounding = 10,      -- track rounding
    grab_size = 16,     -- handle size (easier to grab)
    track = { 30, 30, 30, 220 },     -- RGBA (0-255)
    grab  = { 74, 12, 255, 255 },    -- RGBA (0-255) purple accent)
    }

    local function DrawVolRow(title, id, value, y)
        UI.add_centered_label(innerCX, y - 16 , title, UI_FONT_REG, 1.05)

        UI.add_slider_styled(sliderX, y + 8, sliderW, "", id, 0.0, 1.0, value, nil, nil, " ", sliderStyle)

        local percent = math.floor(((UI.get_slider(id) or value) * 100) + 0.5)
        UI.add_label(sliderX + sliderW + 18, y + 2, 0, 0, tostring(percent) .. "%", "ImGuiDefaultBold", 1.0)
    end

    local y0 = 70        -- start near top of inner child
    local gapY = 66      -- spacing between rows

    -- MASTER
    DrawVolRow(T("audio.master"), "ts_master", masterVol, y0)
    if UI.was_slider_changed("ts_master") then
        masterVol = UI.get_slider("ts_master") or masterVol
        Json.save_setting(GAME_ID, "audio.master", masterVol)
        ApplyAudioVolumes()
    end

    -- MUSIC
    DrawVolRow(T("audio.music"), "ts_music", musicVol, y0 + gapY)
    if UI.was_slider_changed("ts_music") then
        musicVol = UI.get_slider("ts_music") or musicVol
        Json.save_setting(GAME_ID, "audio.music", musicVol)
        ApplyAudioVolumes()
    end

    -- SFX
    DrawVolRow(T("audio.sfx"), "ts_sfx", sfxVol, y0 + gapY * 2)
    if UI.was_slider_changed("ts_sfx") then
        sfxVol = UI.get_slider("ts_sfx") or sfxVol
        Json.save_setting(GAME_ID, "audio.sfx", sfxVol)
        ApplyAudioVolumes()
    end

    local controlsHeaderY = y0 + gapY * 3 + 10
    UI.add_centered_label(innerCX, controlsHeaderY, T("settings.controls"), UI_FONT_HEADER, 1.2)

    local sensY = controlsHeaderY + 60
    -- draw sensitivity slider at sensY

    UI.add_centered_label(innerCX, sensY - 16, T("controls.sensitivity"), UI_FONT_REG, 1.05)
    UI.add_slider_styled(sliderX, sensY + 8, sliderW, "", "ts_sensitivity", 0.25, 2.50, sensitivitySetting, nil, nil, " ", sliderStyle)

    if UI.was_slider_changed("ts_sensitivity") then
        sensitivitySetting = UI.get_slider("ts_sensitivity") or sensitivitySetting
        sensitivitySetting = Clamp(sensitivitySetting, 0.25, 2.50)
        Json.save_setting(GAME_ID, "controls.sensitivity", sensitivitySetting)
    end

    -- show “x” value on the right (2 decimals)
    UI.add_label(sliderX + sliderW + 18, sensY + 2, 0, 0,
        string.format("%.2fx", sensitivitySetting),
        "ImGuiDefaultBold", 1.0
    )

    local langY = sensY + 100

    UI.add_centered_label(innerCX, langY - 42, T("settings.language"), UI_FONT_HEADER, 1.2)

    local opts = { "English", "日本語" }
    local defaultIndex = (language == "ja") and 1 or 0  

    local dropdownW = sliderW
    local dropdownX = sliderX

    local langDropdownFont = "ImGuiDefaultJP" 
    UI.add_dropdown(dropdownX, langY, dropdownW, 32, "", "ts_lang", opts, defaultIndex, langDropdownFont, 1.0)

    if UI.was_dropdown_changed("ts_lang") then
        local idx = UI.get_dropdown_index("ts_lang") or 0
        language = (idx == 1) and "ja" or "en"
        Json.save_setting(GAME_ID, "ui.language", language)
        Localisation.set_language(language)
        ApplyLanguageFonts()
        if TS_UI then
          TS_UI.T = function(key) return Localisation.t(key) end
        end
    end
    
    UI.end_child()
    -- Back button
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
        menuScreen = "main"
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
    UI.add_centered_label(cx, math.floor(panelH * 0.13), "Press ESC to resume", UI_FONT_REG, 1)

    local bw, bh = math.min(340, math.floor(panelW * 0.60)), 50
    local bx = math.floor((panelW - bw) / 2)
    local y0 = math.floor(panelH * 0.34)
    local gap = 14

    UI.add_button(bx, y0 + (bh + gap) * 0, bw, bh, T("pause.resume"), "pause_resume",
        UI_FONT_SUB, 1.0, 12, true, 74, 12, 255, 0.95)

    UI.add_button(bx, y0 + (bh + gap) * 1, bw, bh, T("menu.settings"), "pause_settings",
        UI_FONT_SUB, 1.0, 12, true, 74, 12, 255, 0.90)

    UI.add_button(bx, y0 + (bh + gap) * 2, bw, bh, T("menu.leaderboard"), "pause_leaderboard",
        UI_FONT_SUB, 1.0, 12, true, 74, 12, 255, 0.90)

    UI.add_button(bx, y0 + (bh + gap) * 3, bw, bh, T("pause.quit"), "pause_mainmenu",
        UI_FONT_SUB, 1.0, 12, true, 120, 30, 30, 0.90)

    if UI.was_button_pressed("pause_resume") then
        SetPaused(false)
    elseif UI.was_button_pressed("pause_settings") then
        pauseScreen = "settings"
    elseif UI.was_button_pressed("pause_leaderboard") then
        pauseScreen = "leaderboard"
        pauseLeaderboardFetched = false
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
    local newX = math.floor((displayWidth - MAIN_MENU_W) * 0.5)
    local newY = math.floor((displayHeight - MAIN_MENU_H) * 0.5)
    Window.set_pos(newX, newY)
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
    UI.add_centered_label(cx, math.floor(panelH * 0.05), T("gameover.title"), UI_FONT_TITLE, 1)

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
    local sumH = math.floor(panelH * 0.54)
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
        TriangleShooterPickups.clearAll()
        TriangleShooterPlayerProgress.reset()
        ClearEnemies()
        ClearAllPlayerProjectiles()
        ClearAllEnemyProjectiles()
        Input.set_relative_mouse_mode(true)
        runLeaderboardSubmitted = false
        StartLevel(1, true)
        -- Restart music from beginning to sync with gameplay
        if musicEntity then
            AudioComponent.terminate(musicEntity)
            AudioComponent.play(musicEntity, 0, -1)
        end

    elseif UI.was_button_pressed("gameover_mainmenu") then
        isGameOver = false
        ResetRunStats()
        TriangleShooterPickups.clearAll()
        TriangleShooterPlayerProgress.reset()
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

    -- show cursor in pause menus, lock cursor in gameplay
    Input.set_relative_mouse_mode(not isPaused)

    -- Stop/resume music to prevent desync
    if musicEntity then
        if isPaused then
            AudioComponent.pause(musicEntity)
        else
            AudioComponent.resume(musicEntity)
        end
    end
end

local function DrawPauseLeaderboard(screenW, screenH, dt)
    UI.add_panel(0, 0, screenW, screenH, 0.65, 0, 0, 0, 0)

    local panelW = math.floor(math.max(520, math.min(screenW * 0.70, 860)))
    local panelH = math.floor(math.max(420, math.min(screenH * 0.70, 600)))
    local panelX = math.floor((screenW - panelW) / 2)
    local panelY = math.floor((screenH - panelH) / 2)

    UI.begin_child(panelX, panelY, panelW, panelH, "TS_Leaderboard",
        true, 0,
        true, 0.92, 12, 25, 25, 25,
        0.01, false, 0.85,
        UI__COLOUR_THEME[1], UI__COLOUR_THEME[2], UI__COLOUR_THEME[3], 1.0 -- RGBA border
    )

    local cx = panelW / 2
    local footerH = 100 -- space reserved for the Back button area
    local contentX = 26
    local contentY = math.floor(panelH * 0.13)  
    local contentW = panelW - 52
    local contentH = panelH - contentY - footerH

    UI.add_centered_label(cx, math.floor(panelH * 0.05), T("menu.leaderboard"), UI_FONT_TITLE, 1)
    UI.add_centered_label(cx, math.floor(panelH * 0.13), T("leaderboard.description"), UI_FONT_REG, 1)

    local NO_BACKGROUND = 128
    UI.begin_child(contentX, contentY, contentW, contentH, "TS_LeaderboardContent",
    false, NO_BACKGROUND, false)

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

                -- Left column: "1. Name"
                UI.add_label(listX, y, 0, 0, string.format("%2d. %s", i, name), UI_FONT_REG, 1.1)

                -- Right column: "Stage X"
                UI.add_label(stageX, y, 0, 0, string.format("Stage %d", stage), UI_FONT_REG, 1.1)
            else
                UI.add_label(listX, y, 0, 0, string.format("%2d. --", i), UI_FONT_REG, 1.1)
            end
        end
    else
        UI.add_centered_label(cx, listY + 10, "(No scores yet)", UI_FONT_REG, 1.1)
    end

    UI.end_child()

    local bw, bh = math.min(320, math.floor(panelW * 0.55)), 50
    local bx = math.floor((panelW - bw) / 2)
    local by = panelH - bh - 32

    UI.add_button(bx, by, bw, bh, T("menu.back"), "pause_back_lb",
        UI_FONT_SUB, 1.0, 12, true, 74, 12, 255, 0.95)

    if UI.was_button_pressed("pause_back_lb") then
        pauseScreen = "pause"
    end

    UI.end_child()
end

local function DrawPauseSettingsMenu(screenW, screenH, dt)
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
    local footerH = 100 -- space reserved for the Back button area
    local contentX = 26
    local contentY = math.floor(panelH * 0.13)  
    local contentW = panelW - 52
    local contentH = panelH - contentY - footerH

   UI.add_centered_label(cx, math.floor(panelH * 0.05), T("settings.title"), UI_FONT_TITLE, 1)

     -- Child for scrollable content 
    local NO_BACKGROUND = 128
    UI.begin_child(contentX, contentY, contentW, contentH, "TS_SettingsContent",
    false, NO_BACKGROUND, false)

    local innerCX = contentW / 2
    local sliderW = math.floor(contentW * 0.5)
    local sliderX = math.floor((contentW - sliderW) / 2)

    UI.add_centered_label(innerCX, 12, T("settings.audio"), UI_FONT_HEADER, 1.2)

    local sliderStyle = {
    height = 21,        -- thickness
    rounding = 10,      -- track rounding
    grab_size = 16,     -- handle size (easier to grab)
    track = { 30, 30, 30, 220 },     -- RGBA (0-255)
    grab  = { 74, 12, 255, 255 },    -- RGBA (0-255) purple accent)
    }

    local function DrawVolRow(title, id, value, y)
        UI.add_centered_label(innerCX, y - 16 , title, UI_FONT_REG, 1.05)

        UI.add_slider_styled(sliderX, y + 8, sliderW, "", id, 0.0, 1.0, value, nil, nil, " ", sliderStyle)

        local percent = math.floor(((UI.get_slider(id) or value) * 100) + 0.5)
        UI.add_label(sliderX + sliderW + 18, y + 2, 0, 0, tostring(percent) .. "%", "ImGuiDefaultBold", 1.0)
    end

    local y0 = 70        -- start near top of inner child
    local gapY = 66      -- spacing between rows

    -- MASTER
    DrawVolRow(T("audio.master"), "ts_master", masterVol, y0)
    if UI.was_slider_changed("ts_master") then
        masterVol = UI.get_slider("ts_master") or masterVol
        Json.save_setting(GAME_ID, "audio.master", masterVol)
        ApplyAudioVolumes()
    end

    -- MUSIC
    DrawVolRow(T("audio.music"), "ts_music", musicVol, y0 + gapY)
    if UI.was_slider_changed("ts_music") then
        musicVol = UI.get_slider("ts_music") or musicVol
        Json.save_setting(GAME_ID, "audio.music", musicVol)
        ApplyAudioVolumes()
    end

    -- SFX
    DrawVolRow(T("audio.sfx"), "ts_sfx", sfxVol, y0 + gapY * 2)
    if UI.was_slider_changed("ts_sfx") then
        sfxVol = UI.get_slider("ts_sfx") or sfxVol
        Json.save_setting(GAME_ID, "audio.sfx", sfxVol)
        ApplyAudioVolumes()
    end

    local controlsHeaderY = y0 + gapY * 3 + 10
    UI.add_centered_label(innerCX, controlsHeaderY, T("settings.controls"), UI_FONT_HEADER, 1.2)

    local sensY = controlsHeaderY + 60
    -- draw sensitivity slider at sensY

    UI.add_centered_label(innerCX, sensY - 16, T("controls.sensitivity"), UI_FONT_REG, 1.05)
    UI.add_slider_styled(sliderX, sensY + 8, sliderW, "", "ts_sensitivity", 0.25, 2.50, sensitivitySetting, nil, nil, " ", sliderStyle)

    if UI.was_slider_changed("ts_sensitivity") then
        sensitivitySetting = UI.get_slider("ts_sensitivity") or sensitivitySetting
        sensitivitySetting = Clamp(sensitivitySetting, 0.25, 2.50)
        Json.save_setting(GAME_ID, "controls.sensitivity", sensitivitySetting)
    end

    -- show “x” value on the right (2 decimals)
    UI.add_label(sliderX + sliderW + 18, sensY + 2, 0, 0,
        string.format("%.2fx", sensitivitySetting),
        "ImGuiDefaultBold", 1.0
    )

    local langY = sensY + 100

    UI.add_centered_label(innerCX, langY - 42, T("settings.language"), UI_FONT_HEADER, 1.2)

    local opts = { "English", "日本語" }
    local defaultIndex = (language == "ja") and 1 or 0  

    local dropdownW = sliderW
    local dropdownX = sliderX

    local langDropdownFont = "ImGuiDefaultJP" 
    UI.add_dropdown(dropdownX, langY, dropdownW, 32, "", "ts_lang", opts, defaultIndex, langDropdownFont, 1.0)

    if UI.was_dropdown_changed("ts_lang") then
        local idx = UI.get_dropdown_index("ts_lang") or 0
        language = (idx == 1) and "ja" or "en"
        Json.save_setting(GAME_ID, "ui.language", language)
        Localisation.set_language(language)
        ApplyLanguageFonts()
        if TS_UI then
          TS_UI.T = function(key) return Localisation.t(key) end
        end

    end
    
    UI.end_child()
    -- Back button
    local bw, bh = math.min(320, math.floor(panelW * 0.55)), 50
    local bx = math.floor((panelW - bw) / 2)
    local by = panelH - bh - 32

    UI.add_button(bx, by, bw, bh,
        T("menu.back"), "pause_back_settings",
        UI_FONT_SUB, 1.0,
        12, true,
        74, 12, 255, 0.95
    )

    if UI.was_button_pressed("pause_back_settings") then
        pauseScreen = "pause"
    end

    UI.end_child()
end

local function ResetWallWindowCapture()
    -- makes wall/window system re-capture cleanly 
    windowInitialX = nil
    windowInitialY = nil
    leftWallOffset, rightWallOffset, topWallOffset, bottomWallOffset = 0, 0, 0, 0
    leftWallExpandTimer, rightWallExpandTimer, topWallExpandTimer, bottomWallExpandTimer = 0, 0, 0, 0
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

    ResetWallWindowCapture()

    -- Stop music when returning to main menu
    if musicEntity then
        AudioComponent.terminate(musicEntity)
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
function TriangleShooter:OnUpdate()
     --=====================================================================
     --  [ONUPDATE] Frame Setup
     --=====================================================================
    globalFrame = globalFrame + 1
    local dt = GetDt()

    if inMainMenu then
        screenW = Window.get_width()
        screenH = Window.get_height()
    if menuScreen == "leaderboard" then
        DrawLeaderboardMenu(screenW, screenH, dt)
    elseif menuScreen == "settings" then
        DrawSettingsMenu(screenW, screenH, dt)
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

    if fireCooldownTimer > 0 then
        fireCooldownTimer = fireCooldownTimer - dt
        if fireCooldownTimer < 0 then
            fireCooldownTimer = 0
        end
    end

     --=====================================================================
     --  [ONUPDATE] Transitions / Early Outs
     --=====================================================================
    if windowTransitionActive then
        UpdateWindowTransition()
        return
    end

     --=====================================================================
     --  [ONUPDATE] Upgrade Menu (Freeze Game)
     --=====================================================================
    if TriangleShooterUI.isMenuOpen() then
        screenW = Window.get_width()
        screenH = Window.get_height()
        TriangleShooterUI.draw(screenW, screenH)
        local selectedUpgrade = TriangleShooterUI.handleInput()
        if selectedUpgrade then
            TriangleShooterPlayerProgress.applyUpgrade(selectedUpgrade.type)
            if selectedUpgrade.type == "max_health" then
                local maxH = TriangleShooterPlayerProgress.getMaxHealth()
                playerHealth = math.min(maxH, playerHealth + 20)
            end
            -- Start levelup peace timer and disable enemies
            peaceTimerSeconds = levelupPeaceDuration
            isLevelupPeace = true
            isStartLevelPeace = false
            TriangleShooterEnemy.disableAllEnemies(enemies, screenW, screenH)
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
            if (not TriangleShooterUI.isMenuOpen()) and (not TriangleShooterPlayerProgress.hasPendingLevelUp()) and (not windowTransitionActive) then
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
            DrawPauseSettingsMenu(screenW, screenH, dt)
        elseif pauseScreen == "leaderboard" then
            DrawPauseLeaderboard(screenW, screenH, dt)
        else
            DrawPauseMenu(screenW, screenH, dt)
        end

        return -- <-- THIS is what freezes gameplay
    end


    if TriangleShooterPlayerProgress.hasPendingLevelUp() then
        TriangleShooterPlayerProgress.consumePendingLevelUp()
        local level = TriangleShooterPlayerProgress.getProgress()
        TriangleShooterUI.showUpgradeMenu(nil, level)
        return
    end

    

    -- Track run time (excludes menus, pause, transitions, and upgrade screens)
    runTimeSeconds = runTimeSeconds + dt
 --=====================================================================
     --  [ONUPDATE] World Bounds (Walls / Window)
     --=====================================================================
    -- Update wall lerps
    UpdateWallLerps()
    
    -- Update screen bounds from window
    screenW = Window.get_width()
    screenH = Window.get_height()
    
    if levelTimerSeconds > 0 and #enemies > 0 and not isStartLevelPeace and not isLevelupPeace then
        levelTimerSeconds = levelTimerSeconds - dt
    end

     --=====================================================================
     --  [ONUPDATE] Debug / Music Toggles
     --=====================================================================
    if Input.get_key_down(Keys.ionix_space) then
        print(string.format("[BeatMarker] globalFrame=%d", globalFrame))
    end

    if Input.get_key_down(Keys.ionix_m) then
        musicMuted = not musicMuted
        if musicEntity then
            local targetVolume = musicMuted and 0 or musicVolume
            AudioComponent.change_volume(musicEntity, targetVolume)
        end
    end
    
     --=====================================================================
     --  [ONUPDATE] Player Movement (Mouse + Knockback)
     --=====================================================================
    -- Player can still move during rewind
    -- Get mouse delta (relative movement)
    local delta = Input.get_mouse_delta()
    local deltaX = 0
    local deltaY = 0
    
    if knockbackTimer <= 0 then
        deltaX = delta.x
        deltaY = delta.y
    end
    
    -- Move player by delta (allows knockback since not snapping to cursor)
    local sens = sensitivitySetting or 1.0
    playerX = playerX + deltaX * playerSpeed * sens
    playerY = playerY + deltaY * playerSpeed * sens
    
    if knockbackTimer > 0 then
        local tNorm = 1.0 - (knockbackTimer / knockbackDuration)
        if tNorm < 0 then tNorm = 0 end
        if tNorm > 1 then tNorm = 1 end
        local factor = 1.0 - (tNorm * tNorm)
        local speed = knockbackBaseSpeed * factor * dt
        playerX = playerX + knockbackDirX * speed
        playerY = playerY + knockbackDirY * speed
        
        knockbackTimer = knockbackTimer - dt
    end
    
    -- Clamp to screen bounds
    playerX = math.max(0, math.min(screenW - playerSize, playerX))
    playerY = math.max(0, math.min(screenH - playerSize, playerY))
    
     --=====================================================================
     --  [ONUPDATE] Aim (Nearest Enemy)
     --=====================================================================
    local closestEnemy = nil
    local closestDistSq = nil
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    for i = 1, #enemies do
        local e = enemies[i]
        local enemyCenterX = e.x + enemySize/2
        local enemyCenterY = e.y + enemySize/2
        local dx = enemyCenterX - playerCenterX
        local dy = enemyCenterY - playerCenterY
        local distSq = dx * dx + dy * dy
        if closestDistSq == nil or distSq < closestDistSq then
            closestDistSq = distSq
            closestEnemy = e
        end
    end

    if closestEnemy ~= nil then
        local enemyCenterX = closestEnemy.x + enemySize/2
        local enemyCenterY = closestEnemy.y + enemySize/2
        local dx = enemyCenterX - playerCenterX
        local dy = enemyCenterY - playerCenterY
        local angleRadians = math.atan(dy, dx)
        local angleDegrees = math.deg(angleRadians) + 90  -- +90 because triangle points up by default
        Entity.set_global_rot(player, angleDegrees)

        local dist = math.sqrt(dx * dx + dy * dy)
        if dist > 0 then
            aimDirX = dx / dist
            aimDirY = dy / dist
        end
    end

    -- Apply visual recoil offset to player sprite based on aim direction
    UpdatePlayerRecoil()
    
     --=====================================================================
     --  [ONUPDATE] Shooting
     --=====================================================================
    -- Disable shooting during rewind
    if not TriangleShooterRewind.isActive() then
        if Input.get_mouse_button_down(1) then
            isFiring = true
            if fireCooldownTimer <= 0 then
                SpawnProjectile()
                local interval = TriangleShooterPlayerProgress.getCurrentFireInterval()
                if not interval or interval <= 0 then
                    interval = 0.5
                end
                currentFireInterval = interval
                fireCooldownTimer = interval
            end
        end

        if Input.get_mouse_button_up(1) then
            isFiring = false
        end

        if isFiring and fireCooldownTimer <= 0 then
            SpawnProjectile()
            if gunshot3SfxEntity then
                AudioComponent.change_volume(gunshot3SfxEntity, 4)
                AudioComponent.play(gunshot3SfxEntity)
            end
            local interval = TriangleShooterPlayerProgress.getCurrentFireInterval()
            if not interval or interval <= 0 then
                interval = 0.5
            end
            currentFireInterval = interval
            fireCooldownTimer = interval
        end
    else
        -- Force stop firing during rewind
        isFiring = false
    end
    
     --=====================================================================
     --  [ONUPDATE] Systems Update
     --=====================================================================
    -- Skip normal updates during rewind (handled in rewind logic)
    if not TriangleShooterRewind.isActive() then
        -- Update all projectiles
        UpdateProjectiles()
        UpdateEnemyProjectiles()

        -- Check enemy-player collision and apply damage
        UpdateEnemyCollision()
        
        -- Update enemy movement
        UpdateEnemyMovement()
    end

    ParticleSystem.update(dt)
    TriangleShooterPickups.update(dt)
    
    -- Check pickup collision and heal player (skip during rewind)
    if not TriangleShooterRewind.isActive() then
        local maxHealth = TriangleShooterPlayerProgress.getMaxHealth()
        local healAmount = TriangleShooterPickups.checkPlayerCollision(playerX, playerY, playerSize, maxHealth)
        if healAmount then
            local before = playerHealth
            playerHealth = math.min(maxHealth, playerHealth + healAmount)
            local actual = playerHealth - before
            if actual > 0 then
                runHealingCollected = runHealingCollected + actual
            end
        end
    end
    
    -- Update flash effect
    UpdateFlash()

    -- Skip beat bop during rewind
    if not TriangleShooterRewind.isActive() then
        UpdateBeatBop()
    end

     --=====================================================================
     --  [ONUPDATE] UI
     --=====================================================================
    local levelCfg = TriangleShooterLevels.getLevelConfig(currentLevel)
    if levelCfg ~= nil then
        local maxEnemyHealthTotal = 0
        if levelCfg.enemies then
            for _, enemyCfg in ipairs(levelCfg.enemies) do
                maxEnemyHealthTotal = maxEnemyHealthTotal + (enemyCfg.health or levelEnemyHealth)
            end
        else
            local enemyCount = levelCfg.enemyCount or 1
            maxEnemyHealthTotal = (levelCfg.enemyHealth or levelEnemyHealth) * enemyCount
        end
        local currentEnemyHealthTotal = 0
        for i = 1, #enemies do
            currentEnemyHealthTotal = currentEnemyHealthTotal + (enemies[i].health or 0)
        end
        if maxEnemyHealthTotal < 1 then
            maxEnemyHealthTotal = 1
        end
        UI.draw_progress_bar(20, 20, 200, 20, maxEnemyHealthTotal, currentEnemyHealthTotal, 1)
        if levelCfg.timeLimitSeconds ~= nil and levelCfg.timeLimitSeconds > 0 then
            UI.draw_progress_bar(20, 50, 200, 10, levelCfg.timeLimitSeconds, levelTimerSeconds, 3)
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

    UI.add_centered_label(screenW / 2, 10, "Stage: " .. tostring(currentLevel), "")

    local playerHpBarX = screenW - 220
    local playerHpBarY = 20
    local playerHpBarW = 200
    local playerHpBarH = 20

    local playerMaxHealth = TriangleShooterPlayerProgress.getMaxHealth()
    UI.draw_progress_bar(playerHpBarX, playerHpBarY, playerHpBarW, playerHpBarH, playerMaxHealth, playerHealth, 2)

    local level, xp, xpToNextLevel = TriangleShooterPlayerProgress.getProgress()
    local playerInfoText = "Lv " .. tostring(level) .. "  XP " .. tostring(xp) .. "/" .. tostring(xpToNextLevel)
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
        UI.draw_progress_bar(
            screenW / 2 - 100, 80, 200, 10,
            phaseDuration, elapsed, 4
        )
    end

    -- Count only enabled (non-disabled) enemies as "alive"
    local enemiesAlive = false
    for i = 1, #enemies do
        if not enemies[i].disabled then
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
    if TriangleShooterRewind.isActive() then
        -- During rewind, check for game over
        if playerHealth <= 0 then
            TriangleShooterRewind.reset()
            TriangleShooterRewind.restoreEnemyColors(enemies)
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
        local result = TriangleShooterRewind.update(
            dt, enemies, projectiles, enemyProjectiles,
            UpdateEnemyMovement, clearProjectiles
        )
        
        if result then
            -- Apply returned values
            if result.levelTimer then
                levelTimerSeconds = result.levelTimer
            end
            if result.playerHealth then
                playerHealth = result.playerHealth
            end
            
            -- Clear enemy projectiles when signaled (after they've been rewound)
            if result.clearEnemyProjectiles then
                ClearAllEnemyProjectiles()
            end
            
            -- Handle rewind completion
            if result.done then
                -- Set final values
                local cfg = TriangleShooterLevels.getLevelConfig(currentLevel)
                levelTimerSeconds = cfg and cfg.timeLimitSeconds or 0
                
                -- Clear and respawn enemies with reduced health
                ClearEnemies()
                SpawnEnemiesForLevel(true)  -- Spawn disabled (peace state)
                
                -- Start peace timer and resume music
                peaceTimerSeconds = startLevelPeaceDuration
                isStartLevelPeace = true
                
                if musicEntity then
                    AudioComponent.resume(musicEntity)
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
            TriangleShooterEnemy.enableAllEnemies(enemies)
        end
        -- Don't process normal level flow during levelup peace
        if playerHealth <= 0 then
            TriggerGameOver()
        end
        return
    end

     --=====================================================================
     --  [ONUPDATE] Level Flow (Win / Lose / Timeout)
     --=====================================================================
    if playerHealth <= 0 then
        TriggerGameOver()
        return
    elseif not enemiesAlive then
        if peaceTimerSeconds <= 0 then
            if isStartLevelPeace then
                -- Start-level peace ended, enable the preview enemies
                isStartLevelPeace = false
                TriangleShooterEnemy.enableAllEnemies(enemies)
            else
                -- End-level peace finished, trigger level transition
                OnEnemyKilled()
            end
        else
            peaceTimerSeconds = peaceTimerSeconds - dt
        end
    elseif levelTimerSeconds <= 0 and enemiesAlive and not TriangleShooterRewind.isActive() then
        OnLevelTimeout()
    end
end

 --=====================================================================
 --  [PLAYER PROJECTILES] Spawn / Update
 --=====================================================================
local function SpawnPlayerSingleProjectile(spawnX, spawnY, dirX, dirY, pierceCount, bounceCount, shotData)
    local projData
    shotData = shotData or {}
    
    local isGolden = shotData.isGolden or false
    local damage = shotData.damage or 1
    local sizeMultiplier = shotData.sizeMultiplier or 1
    local actualSize = projectileSize * sizeMultiplier

    -- Try to reuse a pooled projectile
    if #projectilePool > 0 then
        projData = table.remove(projectilePool)
        -- Update sprite size for reused projectile
        local sprite = Entity.get_sprite_component(projData.entity)
        if sprite then
            Sprite.set_image_width(sprite, math.floor(actualSize))
            Sprite.set_image_height(sprite, math.floor(actualSize))
            if isGolden then
                Sprite.set_color(sprite, 255, 215, 0)  -- Gold color
            else
                Sprite.set_color(sprite, 255, 255, 255)  -- Default white
            end
        end
    else
        -- Create new entity only if pool is empty
        local proj = Entity.create_entity()
        -- Layer 4: render under enemies (layer 5)
        local sprite = Entity.add_sprite_component(proj, assets.textures.Ghast_Tear, math.floor(actualSize), math.floor(actualSize), 4)
        if isGolden and sprite then
            Sprite.set_color(sprite, 255, 215, 0)  -- Gold color
        end
        projData = { entity = proj }
    end

    -- Adjust spawn position for larger projectiles
    local adjustedSpawnX = spawnX - (actualSize - projectileSize) / 2
    local adjustedSpawnY = spawnY - (actualSize - projectileSize) / 2

    -- Set position and rotation
    Entity.set_global_pos(projData.entity, adjustedSpawnX, adjustedSpawnY)
    local projAngle = math.deg(math.atan(dirY, dirX)) + 90
    Entity.set_global_rot(projData.entity, projAngle)

    -- Initialize projectile data
    projData.x = adjustedSpawnX
    projData.y = adjustedSpawnY
    projData.vx = dirX * projectileSpeed
    projData.vy = dirY * projectileSpeed
    projData.age = 0
    projData.pierceRemaining = pierceCount or 0
    projData.bounceRemaining = bounceCount or 0
    projData.maxLifetime = projectileLifetimeSeconds + (bounceCount * 2.5)
    projData.hitEnemies = {}
    projData.hasHit = false
    projData.damage = damage
    projData.isGolden = isGolden
    projData.size = actualSize

    table.insert(projectiles, projData)
end

function SpawnProjectile()
    -- Spawn at tip of triangle (offset in aim direction)
    local centerX = playerX + playerSize/2
    local centerY = playerY + playerSize/2
    local tipX = centerX + aimDirX * (playerSize/2)
    local tipY = centerY + aimDirY * (playerSize/2)

    local firepower = TriangleShooterPlayerProgress.getFirepower()
    local pierceCount = TriangleShooterPlayerProgress.getPierceCount()
    local bounceCount = TriangleShooterPlayerProgress.getBounceCount()

    local shots = TriangleShooterAbilities.getShots(firepower, tipX, tipY, aimDirX, aimDirY, projectileSize)
    if not shots then
        return
    end
    runShotsFired = runShotsFired + #shots

    for i = 1, #shots do
        local s = shots[i]
        local offsetX = s.offsetX or 0
        local offsetY = s.offsetY or 0
        local dirX = s.dirX or aimDirX
        local dirY = s.dirY or aimDirY

        local spawnX = tipX + offsetX - projectileSize/2
        local spawnY = tipY + offsetY - projectileSize/2
        SpawnPlayerSingleProjectile(spawnX, spawnY, dirX, dirY, pierceCount, bounceCount, s)
    end
end

----------------------------------------------------------
-- Update all active projectiles
----------------------------------------------------------
local function FindClosestEnemy(fromX, fromY)
    local closestEnemy = nil
    local closestDistSq = math.huge
    for j = 1, #enemies do
        local enemy = enemies[j]
        if enemy.teleportVisible == nil or enemy.teleportVisible then
            local eDisplaySize = enemy.displaySize or enemy.size or enemySize
            local enemyCenterX = enemy.x + eDisplaySize/2
            local enemyCenterY = enemy.y + eDisplaySize/2
            local dx = enemyCenterX - fromX
            local dy = enemyCenterY - fromY
            local distSq = dx * dx + dy * dy
            if distSq < closestDistSq then
                closestDistSq = distSq
                closestEnemy = enemy
            end
        end
    end
    return closestEnemy
end

local function ReturnProjectileToPool(proj, index)
    Entity.set_global_pos(proj.entity, -1000, -1000)
    table.insert(projectilePool, table.remove(projectiles, index))
end

function UpdateProjectiles()
    local dt = GetDt()
    for i = #projectiles, 1, -1 do
        local proj = projectiles[i]
        
        -- Move projectile
        proj.x = proj.x + proj.vx * dt
        proj.y = proj.y + proj.vy * dt
        
        Entity.set_global_pos(proj.entity, proj.x, proj.y)
        
        -- Check collisionRadius with enemies
        local projSize = proj.size or projectileSize
        local projCenterX = proj.x + projSize/2
        local projCenterY = proj.y + projSize/2
        local hitEnemyIndex = nil

        for j = #enemies, 1, -1 do
            local enemy = enemies[j]
            -- Skip disabled enemies (preview state)
            if enemy.disabled then
                goto continue_proj_collision
            end
            local isVisible = enemy.teleportVisible == nil or enemy.teleportVisible
            if isVisible and not proj.hitEnemies[enemy] then
                local eDisplaySize = enemy.displaySize or enemy.size or enemySize
                local enemyCenterX = enemy.x + eDisplaySize/2
                local enemyCenterY = enemy.y + eDisplaySize/2
                local enemyHitRadius = eDisplaySize/2 + projSize/2
                local dx = projCenterX - enemyCenterX
                local dy = projCenterY - enemyCenterY
                local distSq = dx * dx + dy * dy
                if distSq < enemyHitRadius * enemyHitRadius then
                    hitEnemyIndex = j
                    break
                end
            end
            ::continue_proj_collision::
        end

        local shouldRemove = false

        if hitEnemyIndex ~= nil then
            local enemy = enemies[hitEnemyIndex]
            local damage = proj.damage or 1
            if not proj.hasHit then
                proj.hasHit = true
                runShotsHit = runShotsHit + 1
            end
            runDamageDealt = runDamageDealt + damage
            enemy.health = (enemy.health or 0) - damage
            TriangleShooterPlayerProgress.addXp(damage)
            FlashEnemy(enemy)
            TriangleShooterEnemy.updateDisplaySize(enemy)

            local eSize = enemy.size or enemySize
            local enemyCenterX = enemy.x + eSize / 2
            local enemyCenterY = enemy.y + eSize / 2
            local color = enemy.color or {255, 255, 255}
            ParticleSystem.emitHitBurst(enemyCenterX, enemyCenterY, color[1], color[2], color[3])

            if impact3SfxEntity then
                local v = math.random(12, 20)
                AudioComponent.change_volume(impact3SfxEntity, v)
                AudioComponent.play(impact3SfxEntity)
            end

            if enemy.health <= 0 then
                local eSize = enemy.size or enemySize
                local deathX = enemy.x + eSize / 2
                local deathY = enemy.y + eSize / 2
                TriangleShooterPickups.trySpawnHealingOrb(deathX, deathY)
                runEnemiesKilled = runEnemiesKilled + 1
                
                Entity.set_global_pos(enemy.entity, -1000, -1000)
                table.remove(enemies, hitEnemyIndex)
                
                -- Start end-level peace timer when last enemy is killed
                if #enemies == 0 then
                    peaceTimerSeconds = endLevelPeaceDuration
                    isStartLevelPeace = false
                end
            end

            if proj.pierceRemaining > 0 then
                proj.pierceRemaining = proj.pierceRemaining - 1
                proj.hitEnemies[enemy] = true
            else
                shouldRemove = true
            end
        end

        if not shouldRemove then
            proj.age = proj.age + dt
            local maxLife = proj.maxLifetime or projectileLifetimeSeconds

            if proj.age > maxLife then
                shouldRemove = true
            else
                local hitEdge = false
                local edgeX, edgeY = nil, nil

                local projSize = proj.size or projectileSize

                if proj.x < 0 then
                    hitEdge = true
                    edgeX = "left"
                    proj.x = 0
                elseif proj.x + projSize > screenW then
                    hitEdge = true
                    edgeX = "right"
                    proj.x = screenW - projSize
                end

                if proj.y < 0 then
                    hitEdge = true
                    edgeY = "top"
                    proj.y = 0
                elseif proj.y + projSize > screenH then
                    hitEdge = true
                    edgeY = "bottom"
                    proj.y = screenH - projSize
                end

                if hitEdge then
                    if proj.bounceRemaining > 0 then
                        proj.bounceRemaining = proj.bounceRemaining - 1
                        proj.hitEnemies = {}

                        local newProjCenterX = proj.x + projSize/2
                        local newProjCenterY = proj.y + projSize/2
                        local closestEnemy = FindClosestEnemy(newProjCenterX, newProjCenterY)

                        if closestEnemy then
                            local ceDisplaySize = closestEnemy.displaySize or closestEnemy.size or enemySize
                            local targetX = closestEnemy.x + ceDisplaySize/2
                            local targetY = closestEnemy.y + ceDisplaySize/2
                            local dx = targetX - newProjCenterX
                            local dy = targetY - newProjCenterY
                            local len = math.sqrt(dx*dx + dy*dy)
                            if len > 0 then
                                dx = dx / len
                                dy = dy / len
                            end
                            proj.vx = dx * projectileSpeed
                            proj.vy = dy * projectileSpeed
                        else
                            if edgeX then proj.vx = -proj.vx end
                            if edgeY then proj.vy = -proj.vy end
                        end

                        local projAngle = math.deg(math.atan(proj.vy, proj.vx)) + 90
                        Entity.set_global_rot(proj.entity, projAngle)
                        Entity.set_global_pos(proj.entity, proj.x, proj.y)
                    else
                        shouldRemove = true
                    end
                end
            end
        end

        if shouldRemove then
            ReturnProjectileToPool(proj, i)
        end
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
    
    -- Player flash
    if playerFlashDuration > 0 and playerFlashTimer > 0 then
        playerFlashTimer = playerFlashTimer - dt
        if playerFlashTimer < 0 then playerFlashTimer = 0 end

        local t = playerFlashTimer / playerFlashDuration
        if t < 0 then t = 0 end
        if t > 1 then t = 1 end

        local r = 255
        local g = math.floor(255 * (1.0 - t) + 0.5)
        local b = math.floor(255 * (1.0 - t) + 0.5)
        Sprite.set_color(playerSprite, r, g, b)
    end
    
    -- Damage cooldown
    if damageCooldown > 0 then
        damageCooldown = damageCooldown - dt
    end
end

----------------------------------------------------------
-- Enemy-Player collision with damage cooldown
----------------------------------------------------------
function UpdateEnemyCollision()
    if damageCooldown > 0 then
        return
    end
    
    -- Skip collision damage during rewind
    if TriangleShooterRewind.isActive() then
        return
    end
    
    -- Check collision between enemies and player
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    for i = 1, #enemies do
        local enemy = enemies[i]
        -- Skip collision for disabled enemies (preview state)
        if enemy.disabled then
            goto continue_collision
        end
        local enemyCenterX = enemy.x + enemySize/2
        local enemyCenterY = enemy.y + enemySize/2
        local dx = playerCenterX - enemyCenterX
        local dy = playerCenterY - enemyCenterY
        local distSq = dx * dx + dy * dy
        local hitRadius = collisionRadius + playerSize/2
        
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
            playerX = newPlayerCenterX - playerSize/2
            playerY = newPlayerCenterY - playerSize/2
            Entity.set_global_pos(player, playerX, playerY)

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
            knockbackDirX = pushX
            knockbackDirY = pushY
            knockbackTimer = knockbackDuration

            playerHealth = playerHealth - 10
            runDamageTaken = runDamageTaken + 10
            FlashPlayer()
            damageCooldown = damageCooldownDuration
            if playerDamageSfxEntity then
                AudioComponent.play(playerDamageSfxEntity)
            end
            break
        end
        ::continue_collision::
    end
end

function FlashPlayer()
    Sprite.set_color(playerSprite, 255, 0, 0)
    playerFlashTimer = playerFlashDuration
    if playerHealth <= 0 then
        Entity.set_global_pos(player, -1000, -1000)
    end
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
    
    if damageCooldown <= 0 then
        local playerCenterX = playerX + playerSize / 2
        local playerCenterY = playerY + playerSize / 2
        local playerRadius = playerSize / 2
        
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
            playerHealth = playerHealth - BEAM_DAMAGE
            runDamageTaken = runDamageTaken + (BEAM_DAMAGE or 0)
            FlashPlayer()
            damageCooldown = damageCooldownDuration
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
    TriangleShooterEnemy.updateEnemyMovement(
        enemies,
        playerX, playerY, playerSize,
        screenW, screenH,
        enemyShootIntervalSeconds,
        SpawnEnemyProjectile,
        TriggerWallLerp,
        SpawnBeam,
        EmitTeleportBurst,
        EmitBeamCharge
    )
end

 --=====================================================================
 --  [PLAYER FEEL] Recoil
 --=====================================================================
function UpdatePlayerRecoil()
    local dt = GetDt()

    local target = 0

    local interval = currentFireInterval or 0.5
    if interval < 0.001 then
        interval = 0.5
    end

    if fireCooldownTimer >= 0 then
        local remaining = fireCooldownTimer
        if remaining < 0 then remaining = 0 end
        if remaining > interval then remaining = interval end

        local elapsed = interval - remaining
        if elapsed < 0 then elapsed = 0 end
        if elapsed > interval then elapsed = interval end

        local tNorm = 0
        if interval > 0 then
            tNorm = elapsed / interval
            if tNorm < 0 then tNorm = 0 end
            if tNorm > 1 then tNorm = 1 end
        end

        local tri = 0
        if tNorm <= 0.5 then
            tri = tNorm / 0.5
        else
            tri = (1.0 - tNorm) / 0.5
        end

        target = recoilMaxOffset * tri
    end

    recoilOffset = target

    local rx = 0
    local ry = 0
    if recoilOffset > 0 then
        rx = -aimDirX * recoilOffset
        ry = -aimDirY * recoilOffset
    end

    Entity.set_global_pos(player, playerX + rx, playerY + ry)
end

 --=====================================================================
 --  [MUSIC / RHYTHM] Beat Bop
 --=====================================================================
function UpdateBeatBop()
    local dt = GetDt()
    if beatStartDelayCounter < beatStartDelaySeconds then
        beatStartDelayCounter = beatStartDelayCounter + dt
        return
    end

    beatTimer = beatTimer + dt
    if beatTimer >= secondsPerBeat then
        beatTimer = beatTimer - secondsPerBeat
        bopTimer = bopDurationSeconds
    end

    if bopTimer > 0 then
        bopTimer = bopTimer - dt
        local t = bopTimer / bopDurationSeconds
        local scale = 1.0 + bopScale * t

        for i = 1, #enemies do
            local enemy = enemies[i]
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
        end
        
        TriangleShooterPickups.applyBeatBop(t)
    else
        for i = 1, #enemies do
            local enemy = enemies[i]
            local state = enemy.teleportState
            local isTeleporting = state == "shrinking" or state == "growing" or state == "teleporting"
            if enemy and enemy.sprite and not isTeleporting then
                local currentSize = enemy.displaySize or enemy.baseSize or enemy.size or enemyBaseImageSize
                Sprite.set_image_width(enemy.sprite, math.floor(currentSize))
                Sprite.set_image_height(enemy.sprite, math.floor(currentSize))
                Entity.set_global_pos(enemy.entity, enemy.x, enemy.y)
            end
        end
        
        TriangleShooterPickups.resetBop()
    end
end

 --=====================================================================
 --  [ENEMY PROJECTILES] Spawn / Update
 --=====================================================================
local function SpawnEnemySingleProjectile(enemy, dirX, dirY)
    local projData
    
    if #enemyProjectilePool > 0 then
        projData = table.remove(enemyProjectilePool)
        Sprite.set_color(projData.sprite, 128, 0, 255)
    else
        local proj = Entity.create_entity()
        -- Layer 4: render under enemies (layer 5)
        local sprite = Entity.add_sprite_component(proj, assets.textures.Ghast_Tear, enemyProjectileSize, enemyProjectileSize, 4)
        Sprite.set_color(sprite, 128, 0, 255) -- Purple
        projData = { entity = proj, sprite = sprite }
    end
    
    local eSize = enemy.displaySize or enemy.size or enemySize
    local enemyCenterX = enemy.x + eSize/2
    local enemyCenterY = enemy.y + eSize/2
    local spawnX = enemyCenterX - enemyProjectileSize/2
    local spawnY = enemyCenterY - enemyProjectileSize/2
    
    Entity.set_global_pos(projData.entity, spawnX, spawnY)
    local projAngle = math.deg(math.atan(dirY, dirX)) + 90
    Entity.set_global_rot(projData.entity, projAngle)
    
    projData.x = spawnX
    projData.y = spawnY
    projData.vx = dirX * enemyProjectileSpeed
    projData.vy = dirY * enemyProjectileSpeed
    projData.age = 0
    projData.sourceEnemy = enemy  -- Track which enemy spawned this projectile
    
    table.insert(enemyProjectiles, projData)
end

function SpawnEnemyProjectile(enemy)
    local eSize = enemy.displaySize or enemy.size or enemySize
    local enemyCenterX = enemy.x + eSize/2
    local enemyCenterY = enemy.y + eSize/2
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    local dx = playerCenterX - enemyCenterX
    local dy = playerCenterY - enemyCenterY
    local dist = math.sqrt(dx * dx + dy * dy)
    
    local baseDirX, baseDirY = 0, -1
    if dist > 0 then
        baseDirX = dx / dist
        baseDirY = dy / dist
    end
    
    local shootPattern = enemy.shootPattern or "single"
    local projectileCount = enemy.projectileCount or 1
    
    if shootPattern == "single" or projectileCount <= 1 then
        SpawnEnemySingleProjectile(enemy, baseDirX, baseDirY)
        
    elseif shootPattern == "cone" and projectileCount <= 4 then
        local spreadAngle = math.rad(15)
        local baseAngle = math.atan(baseDirY, baseDirX)
        local startAngle = baseAngle - spreadAngle * (projectileCount - 1) / 2
        
        for i = 1, projectileCount do
            local angle = startAngle + spreadAngle * (i - 1)
            local dirX = math.cos(angle)
            local dirY = math.sin(angle)
            SpawnEnemySingleProjectile(enemy, dirX, dirY)
        end
        
    else
        local angleOffset = enemy.shootAngleOffset or 0
        for i = 1, projectileCount do
            local angle = angleOffset + (2 * math.pi * (i - 1)) / projectileCount
            local dirX = math.cos(angle)
            local dirY = math.sin(angle)
            SpawnEnemySingleProjectile(enemy, dirX, dirY)
        end
    end
end

-- Update enemy projectiles
----------------------------------------------------------
function UpdateEnemyProjectiles()
    local dt = GetDt()
    for i = #enemyProjectiles, 1, -1 do
        local proj = enemyProjectiles[i]
        
        -- Move projectile
        proj.x = proj.x + proj.vx * dt
        proj.y = proj.y + proj.vy * dt
        Entity.set_global_pos(proj.entity, proj.x, proj.y)
        
        -- Check collision with player
        local projCenterX = proj.x + enemyProjectileSize/2
        local projCenterY = proj.y + enemyProjectileSize/2
        local playerCenterX = playerX + playerSize/2
        local playerCenterY = playerY + playerSize/2
        
        local dx = projCenterX - playerCenterX
        local dy = projCenterY - playerCenterY
        local distSq = dx * dx + dy * dy
        local hitRadius = playerSize/2 + enemyProjectileSize/2
        
        if distSq < hitRadius * hitRadius and damageCooldown <= 0 then
            -- Hit player
            playerHealth = playerHealth - 5
            runDamageTaken = runDamageTaken + 5
            FlashPlayer()
            damageCooldown = damageCooldownDuration
            Entity.set_global_pos(proj.entity, -1000, -1000)
            table.insert(enemyProjectilePool, table.remove(enemyProjectiles, i))
        else
            -- Age and remove if expired or off screen
            proj.age = proj.age + dt
            if proj.age > projectileLifetimeSeconds or proj.y < -50 or proj.y > screenH + 50 or proj.x < -50 or proj.x > screenW + 50 then
                Entity.set_global_pos(proj.entity, -1000, -1000)
                table.insert(enemyProjectilePool, table.remove(enemyProjectiles, i))
            end
        end
    end
end

 --=====================================================================
 --  [WALLS / WINDOW] Ping-Pong Shrink / Expand
 --=====================================================================
function TriggerWallLerp(wall)
    if not wallPingPongEnabled then return end
    
    if wall == "left" then
        leftWallExpandTimer = wallExpandDurationSeconds
    elseif wall == "right" then
        rightWallExpandTimer = wallExpandDurationSeconds
    elseif wall == "top" then
        topWallExpandTimer = wallExpandDurationSeconds
    elseif wall == "bottom" then
        bottomWallExpandTimer = wallExpandDurationSeconds
    end
end

----------------------------------------------------------
-- Update wall offsets: always shrinking unless expand timer is active
----------------------------------------------------------
function UpdateWallLerps()
    if not wallPingPongEnabled then return end
    if peaceTimerSeconds > 0 and #enemies == 0 then return end
    
    local dt = GetDt()

    local minWindowWidth = wallMinWindowWidth
    local minWindowHeight = wallMinWindowHeight
    local maxShrinkX = math.min(wallMaxShrinkX, math.max(0, (windowBaseWidth - minWindowWidth) * 0.5))
    local maxShrinkY = math.min(wallMaxShrinkY, math.max(0, (windowBaseHeight - minWindowHeight) * 0.5))

    -- Update left wall: always shrinking, expand when hit
    if leftWallExpandTimer > 0 then
        leftWallExpandTimer = leftWallExpandTimer - dt
        leftWallOffset = leftWallOffset - wallShrinkSpeedPerSecond * wallExpandSpeedMultiplier * dt
        if leftWallExpandTimer < 0 then leftWallExpandTimer = 0 end
    else
        leftWallOffset = leftWallOffset + wallShrinkSpeedPerSecond * dt
        if leftWallOffset > maxShrinkX then leftWallOffset = maxShrinkX end
    end
    
    -- Update right wall
    if rightWallExpandTimer > 0 then
        rightWallExpandTimer = rightWallExpandTimer - dt
        rightWallOffset = rightWallOffset - wallShrinkSpeedPerSecond * wallExpandSpeedMultiplier * dt
        if rightWallExpandTimer < 0 then rightWallExpandTimer = 0 end
    else
        rightWallOffset = rightWallOffset + wallShrinkSpeedPerSecond * dt
        if rightWallOffset > maxShrinkX then rightWallOffset = maxShrinkX end
    end
    
    -- Update top wall
    if topWallExpandTimer > 0 then
        topWallExpandTimer = topWallExpandTimer - dt
        topWallOffset = topWallOffset - wallShrinkSpeedPerSecond * wallExpandSpeedMultiplier * dt
        if topWallExpandTimer < 0 then topWallExpandTimer = 0 end
    else
        topWallOffset = topWallOffset + wallShrinkSpeedPerSecond * dt
        if topWallOffset > maxShrinkY then topWallOffset = maxShrinkY end
    end
    
    -- Update bottom wall
    if bottomWallExpandTimer > 0 then
        bottomWallExpandTimer = bottomWallExpandTimer - dt
        bottomWallOffset = bottomWallOffset - wallShrinkSpeedPerSecond * wallExpandSpeedMultiplier * dt
        if bottomWallExpandTimer < 0 then bottomWallExpandTimer = 0 end
    else
        bottomWallOffset = bottomWallOffset + wallShrinkSpeedPerSecond * dt
        if bottomWallOffset > maxShrinkY then bottomWallOffset = maxShrinkY end
    end
    
    -- Capture initial position on first frame
    if windowInitialX == nil then
        windowBaseWidth = Window.get_width()
        windowBaseHeight = Window.get_height()
        originalWindowWidth = windowBaseWidth
        originalWindowHeight = windowBaseHeight

        local displayWidth = Window.get_display_width()
        local displayHeight = Window.get_display_height()
        windowInitialX = math.floor((displayWidth - windowBaseWidth) * 0.5)
        windowInitialY = math.floor((displayHeight - windowBaseHeight) * 0.5)

        Window.set_pos(windowInitialX, windowInitialY)
        Window.set_size(windowBaseWidth, windowBaseHeight)
    end
    
    -- Calculate new window bounds
    local newX = math.floor(windowInitialX + leftWallOffset)
    local newY = math.floor(windowInitialY + topWallOffset)
    local newWidth = math.floor(windowBaseWidth - leftWallOffset - rightWallOffset)
    local newHeight = math.floor(windowBaseHeight - topWallOffset - bottomWallOffset)

    if newWidth < minWindowWidth then newWidth = minWindowWidth end
    if newHeight < minWindowHeight then newHeight = minWindowHeight end

    local realScreenWidth = Window.get_display_width()
    local realScreenHeight = Window.get_display_height()
    local maxWindowWidth = math.min(wallMaxWindowWidth, realScreenWidth)
    local maxWindowHeight = math.min(wallMaxWindowHeight, realScreenHeight)

    if newWidth > maxWindowWidth then newWidth = maxWindowWidth end
    if newHeight > maxWindowHeight then newHeight = maxWindowHeight end
    
    -- Clamp to real screen bounds (top and bottom edges)
    if newY < 0 then
        newY = 0
    end
    if newY + newHeight > realScreenHeight then
        newY = realScreenHeight - newHeight
    end
    
    -- Apply window position and size
    Window.set_pos(newX, newY)
    Window.set_size(newWidth, newHeight)
end

return TriangleShooter
