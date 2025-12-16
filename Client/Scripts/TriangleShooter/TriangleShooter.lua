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
local TriangleShooterUI = require("Scripts.TriangleShooter.TriangleShooterUI")

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

 --=====================================================================
 --  [STATE] Enemy Projectiles
 --=====================================================================
-- ENEMY PROJECTILE SETTINGS
local enemyProjectiles = {}
local enemyProjectilePool = {}
local enemyProjectileSize = 24
local enemyProjectileSpeed = 720 -- PIXELS PER SECOND 
local enemyShootIntervalSeconds = 0.5
local enemyProjectilesEnabled = true

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

-- MUSIC CONTROL
local musicEntity
local musicVolume = 32
local musicMuted = false
local bpm = 133 
local secondsPerBeat = 60.0 / bpm
local beatTimer = 0
local bopDurationSeconds = 8 / 60.0
local bopTimer = 0
local bopScale = 0.25
local beatStartDelaySeconds = (8 * 4) * secondsPerBeat
local beatStartDelayCounter = 0

local playerBaseImageWidth = playerSize
local playerBaseImageHeight = playerSize
local enemyBaseImageSize = enemySize

local globalFrame = 0
local peaceTimerSeconds = 0
local peaceDurationSeconds = 4

-- SFX
local playerDamageSfxEntity
local gunshot3SfxEntity
local impact3SfxEntity

-- LEVEL SETTINGS
local currentLevel = 1
local levelTimerSeconds = 0

 --=====================================================================
 --  [LEVEL FLOW] Load / Start Level
 --=====================================================================
LoadLevel = function(index, resetPlayerState)
    local cfg = TriangleShooterLevels.getLevelConfig(index)
    if not cfg then
        return
    end

     --=====================================================================
     --  [LOAD LEVEL] Apply Config / Global Toggles
     --=====================================================================

    currentLevel = index
    levelTimerSeconds = cfg.timeLimitSeconds or 0

    wallPingPongEnabled = cfg.wallPingPong and true or false

    enemyProjectilesEnabled = cfg.enemyProjectiles and true or false

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

    local centerX = screenW / 2 - enemySize / 2
    local centerY = screenH / 2 - enemySize / 2

     --=====================================================================
     --  [LOAD LEVEL] Spawn Enemies
     --=====================================================================
    if cfg.enemies then
        for i, enemyCfg in ipairs(cfg.enemies) do
            local spawnX = enemyCfg.x or centerX
            local spawnY = enemyCfg.y or centerY
            
            if not enemyCfg.x and not enemyCfg.y and #cfg.enemies > 1 then
                local radius = 120
                local angle = (2 * math.pi * (i - 1)) / #cfg.enemies
                spawnX = screenW / 2 + math.cos(angle) * radius - (enemyCfg.size or enemySize) / 2
                spawnY = screenH / 2 + math.sin(angle) * radius - (enemyCfg.size or enemySize) / 2
            end
            
            local config = {
                health = enemyCfg.health or levelEnemyHealth,
                healthScaling = enemyCfg.healthScaling,
                size = enemyCfg.size or enemySize,
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
            table.insert(enemies, e)
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
            table.insert(enemies, e)
        else
            local radius = 120
            local playerCenterX = screenW / 2
            local playerCenterY = screenH / 2
            for i = 1, enemyCount do
                local angle = (2 * math.pi * (i - 1)) / enemyCount
                local ex = playerCenterX + math.cos(angle) * radius - enemySize / 2
                local ey = playerCenterY + math.sin(angle) * radius - enemySize / 2
                local e = CreateEnemy(ex, ey, defaultConfig)
                table.insert(enemies, e)
            end
        end
    end

    playerHealth = TriangleShooterPlayerProgress.getMaxHealth()

     --=====================================================================
     --  [LOAD LEVEL] Reset Player State
     --=====================================================================
    if resetPlayerState then
        playerX = screenW / 2 - playerSize / 2
        playerY = screenH / 2 - playerSize / 2
        Entity.set_global_pos(player, playerX, playerY)
        Sprite.set_color(playerSprite, 255, 255, 255)
        playerFlashTimer = 0
        damageCooldown = 0
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
    TriangleShooterLevels.regenerateLevel(nextIndex)
    if TriangleShooterLevels.getLevelConfig(nextIndex) then
        StartLevel(nextIndex, false)
    else
        StartLevel(currentLevel, false)
    end
end

local function OnLevelTimeout()
    StartLevel(currentLevel, true)
end

 --=====================================================================
 --  [ENGINE CALLBACKS] OnStart
 --=====================================================================
function TriangleShooter:OnStart()
    -- Enable relative mouse mode (hides cursor, gives delta movement)
    Input.set_relative_mouse_mode(true)
    
    -- Create player triangle
    player = Entity.create_entity()
    
    -- Start at center of screen
    playerX = screenW / 2 - playerSize / 2
    playerY = screenH / 2 - playerSize / 2
    Entity.set_global_pos(player, playerX, playerY)
    
    -- Add sprite component 
    playerSprite = Entity.add_sprite_component(player, assets.textures.Triangle, playerSize, playerSize, 10)
    Sprite.set_columns(playerSprite, 1)

    local cfg = TriangleShooterLevels.getLevelConfig(1)
    if cfg and cfg.windowWidth and cfg.windowHeight then
        local targetW = cfg.windowWidth
        local targetH = cfg.windowHeight
        local displayWidth = Window.get_display_width()
        local displayHeight = Window.get_display_height()
        local newX = math.floor((displayWidth - targetW) * 0.5)
        local newY = math.floor((displayHeight - targetH) * 0.5)
        Window.set_pos(newX, newY)
        Window.set_size(targetW, targetH)
        screenW = targetW
        screenH = targetH
    end

    LoadLevel(1, true)

    musicEntity = Entity.create_entity()
    Entity.add_audio_component(musicEntity, "technoSong", false)
    AudioComponent.play(musicEntity, 0, -1)
    AudioComponent.change_volume(musicEntity, musicVolume)

    playerDamageSfxEntity = Entity.create_entity()
    Entity.add_audio_component(playerDamageSfxEntity, "playerDamage", false)
    -- Set player damage SFX volume to half (range is 0-128)
    AudioComponent.change_volume(playerDamageSfxEntity, 48)

    gunshot3SfxEntity = Entity.create_entity()
    Entity.add_audio_component(gunshot3SfxEntity, "gunshot3", false)
    AudioComponent.change_volume(gunshot3SfxEntity, 4)

    impact3SfxEntity = Entity.create_entity()
    Entity.add_audio_component(impact3SfxEntity, "impact3", false)
    AudioComponent.change_volume(impact3SfxEntity, 16)
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
        end
        return
    end

    if TriangleShooterPlayerProgress.hasPendingLevelUp() then
        TriangleShooterPlayerProgress.consumePendingLevelUp()
        local level = TriangleShooterPlayerProgress.getProgress()
        TriangleShooterUI.showUpgradeMenu(nil, level)
        return
    end

     --=====================================================================
     --  [ONUPDATE] World Bounds (Walls / Window)
     --=====================================================================
    -- Update wall lerps
    UpdateWallLerps()
    
    -- Update screen bounds from window
    screenW = Window.get_width()
    screenH = Window.get_height()
    
    if levelTimerSeconds > 0 and #enemies > 0 then
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
    -- Get mouse delta (relative movement)
    local delta = Input.get_mouse_delta()
    local deltaX = 0
    local deltaY = 0
    
    if knockbackTimer <= 0 then
        deltaX = delta.x
        deltaY = delta.y
    end
    
    -- Move player by delta (allows knockback since not snapping to cursor)
    playerX = playerX + deltaX * playerSpeed
    playerY = playerY + deltaY * playerSpeed
    
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
    
     --=====================================================================
     --  [ONUPDATE] Systems Update
     --=====================================================================
    -- Update all projectiles
    UpdateProjectiles()
    UpdateEnemyProjectiles()

    ParticleSystem.update(dt)
    
    -- Update flash effect
    UpdateFlash()
    
    -- Check enemy-player collision and apply damage
    UpdateEnemyCollision()
    
    -- Update enemy movement
    UpdateEnemyMovement()

    UpdateBeatBop()

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
    local playerInfoText = "Player Lv: " .. tostring(level) .. "  XP: " .. tostring(xp) .. " / " .. tostring(xpToNextLevel)
    UI.add_centered_label(playerHpBarX + playerHpBarW / 2, playerHpBarY + playerHpBarH + 8, playerInfoText, "")

    -- Peace progress bar (only during inter-level peace)
    if #enemies == 0 and peaceTimerSeconds > 0 then
        local elapsed = peaceDurationSeconds - peaceTimerSeconds
        if elapsed < 0 then
            elapsed = 0
        end
        UI.draw_progress_bar(
            screenW / 2 - 100, 80, 200, 10,
            peaceDurationSeconds, elapsed, 4
        )
    end

    local enemiesAlive = #enemies > 0
    if enemiesAlive and peaceTimerSeconds > 0 then
        peaceTimerSeconds = 0
    end

     --=====================================================================
     --  [ONUPDATE] Level Flow (Win / Lose / Timeout)
     --=====================================================================
    if playerHealth <= 0 then
        StartLevel(currentLevel, true)
    elseif not enemiesAlive then
        if peaceTimerSeconds <= 0 then
            peaceTimerSeconds = peaceDurationSeconds
        else
            peaceTimerSeconds = peaceTimerSeconds - dt
            if peaceTimerSeconds <= 0 then
                OnEnemyKilled()
            end
        end
    elseif levelTimerSeconds <= 0 and enemiesAlive then
        OnLevelTimeout()
    end
end

 --=====================================================================
 --  [PLAYER PROJECTILES] Spawn / Update
 --=====================================================================
local function SpawnPlayerSingleProjectile(spawnX, spawnY, dirX, dirY, pierceCount, bounceCount)
    local projData

    -- Try to reuse a pooled projectile
    if #projectilePool > 0 then
        projData = table.remove(projectilePool)
    else
        -- Create new entity only if pool is empty
        local proj = Entity.create_entity()
        Entity.add_sprite_component(proj, assets.textures.Ghast_Tear, projectileSize, projectileSize, 5)
        projData = { entity = proj }
    end

    -- Set position and rotation
    Entity.set_global_pos(projData.entity, spawnX, spawnY)
    local projAngle = math.deg(math.atan(dirY, dirX)) + 90
    Entity.set_global_rot(projData.entity, projAngle)

    -- Initialize projectile data
    projData.x = spawnX
    projData.y = spawnY
    projData.vx = dirX * projectileSpeed
    projData.vy = dirY * projectileSpeed
    projData.age = 0
    projData.pierceRemaining = pierceCount or 0
    projData.bounceRemaining = bounceCount or 0
    projData.maxLifetime = projectileLifetimeSeconds + (bounceCount * 2.5)
    projData.hitEnemies = {}

    table.insert(projectiles, projData)
end

function SpawnProjectile()
    -- Spawn at tip of triangle (offset in aim direction)
    local centerX = playerX + playerSize/2
    local centerY = playerY + playerSize/2
    local tipX = centerX + aimDirX * (playerSize/2)
    local tipY = centerY + aimDirY * (playerSize/2)

    local bulletCount = TriangleShooterPlayerProgress.getBulletCount()
    local pierceCount = TriangleShooterPlayerProgress.getPierceCount()
    local bounceCount = TriangleShooterPlayerProgress.getBounceCount()

    local shots = TriangleShooterAbilities.getShots(bulletCount, tipX, tipY, aimDirX, aimDirY, projectileSize)
    if not shots then
        return
    end

    for i = 1, #shots do
        local s = shots[i]
        local offsetX = s.offsetX or 0
        local offsetY = s.offsetY or 0
        local dirX = s.dirX or aimDirX
        local dirY = s.dirY or aimDirY

        local spawnX = tipX + offsetX - projectileSize/2
        local spawnY = tipY + offsetY - projectileSize/2
        SpawnPlayerSingleProjectile(spawnX, spawnY, dirX, dirY, pierceCount, bounceCount)
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
        local projCenterX = proj.x + projectileSize/2
        local projCenterY = proj.y + projectileSize/2
        local hitRadius = collisionRadius + projectileSize/2
        local hitEnemyIndex = nil

        for j = #enemies, 1, -1 do
            local enemy = enemies[j]
            local isVisible = enemy.teleportVisible == nil or enemy.teleportVisible
            if isVisible and not proj.hitEnemies[enemy] then
                local eDisplaySize = enemy.displaySize or enemy.size or enemySize
                local enemyCenterX = enemy.x + eDisplaySize/2
                local enemyCenterY = enemy.y + eDisplaySize/2
                local enemyHitRadius = eDisplaySize/2 + projectileSize/2
                local dx = projCenterX - enemyCenterX
                local dy = projCenterY - enemyCenterY
                local distSq = dx * dx + dy * dy
                if distSq < enemyHitRadius * enemyHitRadius then
                    hitEnemyIndex = j
                    break
                end
            end
        end

        local shouldRemove = false

        if hitEnemyIndex ~= nil then
            local enemy = enemies[hitEnemyIndex]
            enemy.health = (enemy.health or 0) - 1
            TriangleShooterPlayerProgress.addXp(1)
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
                Entity.set_global_pos(enemy.entity, -1000, -1000)
                table.remove(enemies, hitEnemyIndex)
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

                if proj.x < 0 then
                    hitEdge = true
                    edgeX = "left"
                    proj.x = 0
                elseif proj.x + projectileSize > screenW then
                    hitEdge = true
                    edgeX = "right"
                    proj.x = screenW - projectileSize
                end

                if proj.y < 0 then
                    hitEdge = true
                    edgeY = "top"
                    proj.y = 0
                elseif proj.y + projectileSize > screenH then
                    hitEdge = true
                    edgeY = "bottom"
                    proj.y = screenH - projectileSize
                end

                if hitEdge then
                    if proj.bounceRemaining > 0 then
                        proj.bounceRemaining = proj.bounceRemaining - 1
                        proj.hitEnemies = {}

                        local newProjCenterX = proj.x + projectileSize/2
                        local newProjCenterY = proj.y + projectileSize/2
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
    
    -- Check collision between enemies and player
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    for i = 1, #enemies do
        local enemy = enemies[i]
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
            FlashPlayer()
            damageCooldown = damageCooldownDuration
            if playerDamageSfxEntity then
                AudioComponent.play(playerDamageSfxEntity)
            end
            break
        end
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
        enemyProjectilesEnabled, enemyShootIntervalSeconds,
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
                Sprite.set_image_width(enemy.sprite, math.floor(currentSize * scale))
                Sprite.set_image_height(enemy.sprite, math.floor(currentSize * scale))
            end
        end
    else
        for i = 1, #enemies do
            local enemy = enemies[i]
            local state = enemy.teleportState
            local isTeleporting = state == "shrinking" or state == "growing" or state == "teleporting"
            if enemy and enemy.sprite and not isTeleporting then
                local currentSize = enemy.displaySize or enemy.baseSize or enemy.size or enemyBaseImageSize
                Sprite.set_image_width(enemy.sprite, math.floor(currentSize))
                Sprite.set_image_height(enemy.sprite, math.floor(currentSize))
            end
        end
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
        local sprite = Entity.add_sprite_component(proj, assets.textures.Ghast_Tear, enemyProjectileSize, enemyProjectileSize, 5)
        Sprite.set_color(sprite, 128, 0, 255) -- Purple
        projData = { entity = proj, sprite = sprite }
    end
    
    local eSize = enemy.size or enemySize
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
    
    table.insert(enemyProjectiles, projData)
end

function SpawnEnemyProjectile(enemy)
    local eSize = enemy.size or enemySize
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
