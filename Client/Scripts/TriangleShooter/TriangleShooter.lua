local TriangleShooter = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local TriangleShooterLevels = require("Scripts.TriangleShooter.TriangleShooterLevels")
local TriangleShooterEnemy = require("Scripts.TriangleShooter.TriangleShooterEnemy")
local TriangleShooterAbilities = require("Scripts.TriangleShooter.TriangleShooterAbilities")
local TriangleShooterPlayerProgress = require("Scripts.TriangleShooter.TriangleShooterPlayerProgress")

-- SCREEN BOUNDS (UPDATED EACH FRAME FROM WINDOW SIZE)
local screenW = 1920
local screenH = 1080

-- WALL SETTINGS
local wallPingPongEnabled = true
local wallMaxShrinkX = 600        -- Max pixels each horizontal wall can shrink (1920 - 400 = 1520, /2 = 760)
local wallMaxShrinkY = 200        -- Max pixels each vertical wall can shrink (1080 - 400 = 680, /2 = 340)
local wallShrinkSpeed = 0.35       -- Pixels per frame each wall shrinks
local wallExpandDuration = 120    -- 2 seconds at 60fps
local wallExpandSpeedMultiplier = 4.0

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

-- Original window size (never changes, used for speed scaling)
local originalWindowWidth = 1920
local originalWindowHeight = 1080
local windowTransitionActive = false
local windowTransitionTimer = 0
local windowTransitionDurationFrames = 60
local windowTransitionStartW = 0
local windowTransitionStartH = 0
local windowTransitionTargetW = 0
local windowTransitionTargetH = 0
local pendingLevelIndex = nil
local pendingResetPlayerState = false

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
local playerFlashDuration = 0.005  -- seconds

-- DAMAGE COOLDOWN (0.5S = 30 FRAMES AT 60FPS)
local damageCooldown = 0
local damageCooldownDuration = 0.5  -- seconds

-- PROJECTILE SETTINGS
local projectiles = {}      -- Active projectiles
local projectilePool = {}   -- Inactive projectiles (reusable)
local projectileSize = 24
local projectileSpeed = 4
local projectileLifetime = 300  -- frames (~5 seconds at 60fps)

-- Current aim direction (updated each frame)
local aimDirX = 0
local aimDirY = -1  -- Default: pointing up

-- ENEMY (CUBE)
local enemySize = 48

local enemies = {}
local levelEnemyHealth = 50
local StartLevel
local LoadLevel

local function CreateEnemy(x, y, health)
    return TriangleShooterEnemy.createEnemy(x, y, health, enemySize, playerX, playerY, playerSize)
end

function UpdateWindowTransition()
    if not windowTransitionActive then
        return
    end

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

    windowTransitionTimer = windowTransitionTimer - 1
    local t = 1.0 - (windowTransitionTimer / windowTransitionDurationFrames)
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

-- ENEMY PROJECTILE SETTINGS
local enemyProjectiles = {}
local enemyProjectilePool = {}
local enemyProjectileSize = 24
local enemyProjectileSpeed = 3
local enemyShootInterval = 75  -- 1.25 seconds at 60fps
local enemyProjectilesEnabled = true

-- COLLISION SETTINGS
local collisionRadius = 24  -- Half of enemy size for circle collision

-- FLASH EFFECT
local flashTimer = 0
local flashDuration = 0.2  -- seconds

local knockbackTimer = 0
local knockbackDuration = 45
local knockbackBaseSpeed = 6
local knockbackDirX = 0
local knockbackDirY = 0

-- MUSIC CONTROL
local musicEntity
local musicVolume = 64
local musicMuted = false

local bpm = 133 -- not real bpm
local framesPerBeat = 60.0 / bpm
local beatTimer = 0
local bopDurationFrames = 8 / 60.0
local bopTimer = 0
local bopScale = 0.25

local beatStartDelayFrames = (8 * 4) * framesPerBeat
local beatStartDelayCounter = 0

local function GetDt()
    return Mafs.delta_time()
end

local playerBaseImageWidth = playerSize
local playerBaseImageHeight = playerSize
local enemyBaseImageSize = enemySize

local globalFrame = 0
local peaceTimerFrames = 0
local peaceDurationFrames = 60 * 15

-- LEVEL SETTINGS
local currentLevel = 1
local levelTimer = 0

LoadLevel = function(index, resetPlayerState)
    local cfg = TriangleShooterLevels.getLevelConfig(index)
    if not cfg then
        return
    end

    currentLevel = index
    levelTimer = cfg.timeLimitFrames or 0

    wallPingPongEnabled = cfg.wallPingPong and true or false

    enemyProjectilesEnabled = cfg.enemyProjectiles and true or false

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

    local enemyCount = cfg.enemyCount or 1
    levelEnemyHealth = cfg.enemyHealth or levelEnemyHealth

    local centerX = screenW / 2 - enemySize / 2
    local centerY = screenH / 2 - enemySize / 2

    if enemyCount == 1 then
        local e = CreateEnemy(centerX, centerY, levelEnemyHealth)
        table.insert(enemies, e)
    else
        local radius = 120
        local playerCenterX = screenW / 2
        local playerCenterY = screenH / 2
        for i = 1, enemyCount do
            local angle = (2 * math.pi * (i - 1)) / enemyCount
            local ex = playerCenterX + math.cos(angle) * radius - enemySize / 2
            local ey = playerCenterY + math.sin(angle) * radius - enemySize / 2
            local e = CreateEnemy(ex, ey, levelEnemyHealth)
            table.insert(enemies, e)
        end
    end

    playerHealth = 100
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
    windowTransitionTimer = windowTransitionDurationFrames
    windowTransitionStartW = currentWidth
    windowTransitionStartH = currentHeight
    windowTransitionTargetW = targetWidth
    windowTransitionTargetH = targetHeight
    pendingLevelIndex = index
    pendingResetPlayerState = resetPlayerState
end

local function OnEnemyKilled()
    local nextIndex = currentLevel + 1
    if TriangleShooterLevels.getLevelConfig(nextIndex) then
        StartLevel(nextIndex, false)
    else
        StartLevel(currentLevel, false)
    end
end

local function OnLevelTimeout()
    StartLevel(currentLevel, true)
end

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
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
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function TriangleShooter:OnUpdate()
    globalFrame = globalFrame + 1

    if windowTransitionActive then
        UpdateWindowTransition()
        return
    end

    -- Update wall lerps
    UpdateWallLerps()
    
    -- Update screen bounds from window
    screenW = Window.get_width()
    screenH = Window.get_height()
    
    if levelTimer > 0 and #enemies > 0 then
        levelTimer = levelTimer - 1
    end

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
        local speed = knockbackBaseSpeed * factor
        playerX = playerX + knockbackDirX * speed
        playerY = playerY + knockbackDirY * speed
        
        knockbackTimer = knockbackTimer - 1
    end
    
    -- Clamp to screen bounds
    playerX = math.max(0, math.min(screenW - playerSize, playerX))
    playerY = math.max(0, math.min(screenH - playerSize, playerY))
    
    Entity.set_global_pos(player, playerX, playerY)
    
    -- Rotate triangle to face the nearest enemy cube (if any)
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
    
    -- Spawn projectile on LMB click
    if Input.get_mouse_button_down(1) then
        SpawnProjectile()
    end
    
    -- Update all projectiles
    UpdateProjectiles()
    UpdateEnemyProjectiles()
    
    -- Update flash effect
    UpdateFlash()
    
    -- Check enemy-player collision and apply damage
    UpdateEnemyCollision()
    
    -- Update enemy dash behavior
    UpdateEnemyDash()

    UpdateBeatBop()

    local levelCfg = TriangleShooterLevels.getLevelConfig(currentLevel)
    if levelCfg ~= nil then
        local enemyCount = levelCfg.enemyCount or 1
        local maxEnemyHealthTotal = (levelCfg.enemyHealth or levelEnemyHealth) * enemyCount
        local currentEnemyHealthTotal = 0
        for i = 1, #enemies do
            currentEnemyHealthTotal = currentEnemyHealthTotal + (enemies[i].health or 0)
        end
        if maxEnemyHealthTotal < 1 then
            maxEnemyHealthTotal = 1
        end
        UI.draw_progress_bar(20, 20, 200, 20, maxEnemyHealthTotal, currentEnemyHealthTotal, 1)
        if levelCfg.timeLimitFrames ~= nil and levelCfg.timeLimitFrames > 0 then
            UI.draw_progress_bar(20, 50, 200, 10, levelCfg.timeLimitFrames, levelTimer, 3)
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

    UI.draw_label("Stage: " .. tostring(currentLevel), 140, 140, 400, 20, "")

    UI.draw_progress_bar(screenW - 220, 20, 200, 20, 100, playerHealth, 2)
    local level, xp, xpToNextLevel = TriangleShooterPlayerProgress.getProgress()
    UI.draw_label("Player Lv: " .. tostring(level) .. "  XP: " .. tostring(xp) .. " / " .. tostring(xpToNextLevel), 220, 45, 740, 60, "")

    -- Peace progress bar (only during inter-level peace)
    if #enemies == 0 and peaceTimerFrames > 0 then
        local elapsed = peaceDurationFrames - peaceTimerFrames
        if elapsed < 0 then
            elapsed = 0
        end
        UI.draw_progress_bar(
            screenW / 2 - 100, 80, 200, 10,
            peaceDurationFrames, elapsed, 4
        )
    end

    local enemiesAlive = #enemies > 0
    if enemiesAlive and peaceTimerFrames > 0 then
        peaceTimerFrames = 0
    end

    if playerHealth <= 0 then
        StartLevel(currentLevel, true)
    elseif not enemiesAlive then
        if peaceTimerFrames <= 0 then
            peaceTimerFrames = peaceDurationFrames
        else
            peaceTimerFrames = peaceTimerFrames - 1
            if peaceTimerFrames <= 0 then
                OnEnemyKilled()
            end
        end
    elseif levelTimer <= 0 and enemiesAlive then
        OnLevelTimeout()
    end
end

----------------------------------------------------------
-- Spawn a projectile from the tip of the triangle
----------------------------------------------------------
local function SpawnSingleProjectile(spawnX, spawnY, dirX, dirY)
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

    table.insert(projectiles, projData)
end

function SpawnProjectile()
    -- Spawn at tip of triangle (offset in aim direction)
    local centerX = playerX + playerSize/2
    local centerY = playerY + playerSize/2
    local tipX = centerX + aimDirX * (playerSize/2)
    local tipY = centerY + aimDirY * (playerSize/2)
    local abilityName = TriangleShooterPlayerProgress.getCurrentShootAbility()
    local shots = TriangleShooterAbilities.getShots(abilityName, tipX, tipY, aimDirX, aimDirY, projectileSize)
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
        SpawnSingleProjectile(spawnX, spawnY, dirX, dirY)
    end
end

----------------------------------------------------------
-- Update all active projectiles
----------------------------------------------------------
function UpdateProjectiles()
    local dt = GetDt()
    for i = #projectiles, 1, -1 do
        local proj = projectiles[i]
        
        -- Move projectile
        proj.x = proj.x + proj.vx
        proj.y = proj.y + proj.vy
        
        Entity.set_global_pos(proj.entity, proj.x, proj.y)
        
        -- Check collisionRadius with enemies
        local projCenterX = proj.x + projectileSize/2
        local projCenterY = proj.y + projectileSize/2
        local hitRadius = collisionRadius + projectileSize/2
        local hitEnemyIndex = nil
        for j = #enemies, 1, -1 do
            local enemy = enemies[j]
            local enemyCenterX = enemy.x + enemySize/2
            local enemyCenterY = enemy.y + enemySize/2
            local dx = projCenterX - enemyCenterX
            local dy = projCenterY - enemyCenterY
            local distSq = dx * dx + dy * dy
            if distSq < hitRadius * hitRadius then
                hitEnemyIndex = j
                break
            end
        end

        if hitEnemyIndex ~= nil then
            local enemy = enemies[hitEnemyIndex]
            enemy.health = (enemy.health or 0) - 1
            TriangleShooterPlayerProgress.addXp(1)
            FlashEnemy(enemy)
            if enemy.health <= 0 then
                Entity.set_global_pos(enemy.entity, -1000, -1000)
                table.remove(enemies, hitEnemyIndex)
            end
            Entity.set_global_pos(proj.entity, -1000, -1000)
            table.insert(projectilePool, table.remove(projectiles, i))
        else
            -- Increment age and remove if expired or off screen
            proj.age = proj.age + dt
            if proj.age > projectileLifetime / 60.0 or proj.y < -50 or proj.y > screenH + 50 or proj.x < -50 or proj.x > screenW + 50 then
                -- Move entity off-screen and return to pool
                Entity.set_global_pos(proj.entity, -1000, -1000)
                table.insert(projectilePool, table.remove(projectiles, i))
            end
        end
    end
end

----------------------------------------------------------
-- Flash effect (frame-based, no coroutines)
----------------------------------------------------------
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

                local r = 255
                local g = math.floor(255 * (1.0 - t) + 0.5)
                local b = math.floor(255 * (1.0 - t) + 0.5)
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

function UpdateEnemyDash()
    TriangleShooterEnemy.updateEnemyDash(
        enemies,
        playerX, playerY, playerSize,
        screenW, screenH,
        originalWindowWidth, originalWindowHeight,
        enemyProjectilesEnabled, enemyShootInterval,
        SpawnEnemyProjectile,
        TriggerWallLerp
    )
end

function UpdateBeatBop()
    local dt = GetDt()
    if beatStartDelayCounter < beatStartDelayFrames then
        beatStartDelayCounter = beatStartDelayCounter + dt
        return
    end

    beatTimer = beatTimer + dt
    if beatTimer >= framesPerBeat then
        beatTimer = beatTimer - framesPerBeat
        bopTimer = bopDurationFrames
    end

    if bopTimer > 0 then
        bopTimer = bopTimer - dt
        local t = bopTimer / bopDurationFrames
        local scale = 1.0 + bopScale * t

        if playerSprite then
            Sprite.set_image_width(playerSprite, math.floor(playerBaseImageWidth * scale))
            Sprite.set_image_height(playerSprite, math.floor(playerBaseImageHeight * scale))
        end
    else
        if playerSprite then
            Sprite.set_image_width(playerSprite, playerBaseImageWidth)
            Sprite.set_image_height(playerSprite, playerBaseImageHeight)
        end
    end
end

----------------------------------------------------------
-- Enemy projectile spawning
----------------------------------------------------------
function SpawnEnemyProjectile(enemy)
    local projData
    
    -- Try to reuse a pooled projectile
    if #enemyProjectilePool > 0 then
        projData = table.remove(enemyProjectilePool)
        Sprite.set_color(projData.sprite, 128, 0, 255)  -- Purple
    else
        -- Create new entity
        local proj = Entity.create_entity()
        local sprite = Entity.add_sprite_component(proj, assets.textures.Ghast_Tear, enemyProjectileSize, enemyProjectileSize, 5)
        Sprite.set_color(sprite, 128, 0, 255)  -- Purple
        projData = { entity = proj, sprite = sprite }
    end
    
    -- Direction towards player
    local enemyCenterX = enemy.x + enemySize/2
    local enemyCenterY = enemy.y + enemySize/2
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    local dx = playerCenterX - enemyCenterX
    local dy = playerCenterY - enemyCenterY
    local dist = math.sqrt(dx * dx + dy * dy)
    
    local dirX, dirY = 0, 0
    if dist > 0 then
        dirX = dx / dist
        dirY = dy / dist
    end
    
    -- Spawn at enemy center
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

-- Update enemy projectiles
----------------------------------------------------------
function UpdateEnemyProjectiles()
    local dt = GetDt()
    for i = #enemyProjectiles, 1, -1 do
        local proj = enemyProjectiles[i]
        
        -- Move projectile
        proj.x = proj.x + proj.vx
        proj.y = proj.y + proj.vy
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
            if proj.age > projectileLifetime / 60.0 or proj.y < -50 or proj.y > screenH + 50 or proj.x < -50 or proj.x > screenW + 50 then
                Entity.set_global_pos(proj.entity, -1000, -1000)
                table.insert(enemyProjectilePool, table.remove(enemyProjectiles, i))
            end
        end
    end
end

----------------------------------------------------------
-- Trigger a wall to start expanding (resets timer to full duration)
----------------------------------------------------------
function TriggerWallLerp(wall)
    if not wallPingPongEnabled then return end
    
    if wall == "left" then
        leftWallExpandTimer = wallExpandDuration
    elseif wall == "right" then
        rightWallExpandTimer = wallExpandDuration
    elseif wall == "top" then
        topWallExpandTimer = wallExpandDuration
    elseif wall == "bottom" then
        bottomWallExpandTimer = wallExpandDuration
    end
end

----------------------------------------------------------
-- Update wall offsets: always shrinking unless expand timer is active
----------------------------------------------------------
function UpdateWallLerps()
    if not wallPingPongEnabled then return end
    if peaceTimerFrames > 0 and #enemies == 0 then return end
    
    local minWindowWidth = 200
    local minWindowHeight = 200
    local maxShrinkX = math.min(wallMaxShrinkX, math.max(0, (windowBaseWidth - minWindowWidth) * 0.5))
    local maxShrinkY = math.min(wallMaxShrinkY, math.max(0, (windowBaseHeight - minWindowHeight) * 0.5))

    -- Update left wall: always shrinking, expand when hit
    if leftWallExpandTimer > 0 then
        leftWallExpandTimer = leftWallExpandTimer - 1
        leftWallOffset = leftWallOffset - wallShrinkSpeed * wallExpandSpeedMultiplier
        if leftWallOffset < 0 then leftWallOffset = 0 end
    else
        leftWallOffset = leftWallOffset + wallShrinkSpeed
        if leftWallOffset > maxShrinkX then leftWallOffset = maxShrinkX end
    end
    
    -- Update right wall
    if rightWallExpandTimer > 0 then
        rightWallExpandTimer = rightWallExpandTimer - 1
        rightWallOffset = rightWallOffset - wallShrinkSpeed * wallExpandSpeedMultiplier
        if rightWallOffset < 0 then rightWallOffset = 0 end
    else
        rightWallOffset = rightWallOffset + wallShrinkSpeed
        if rightWallOffset > maxShrinkX then rightWallOffset = maxShrinkX end
    end
    
    -- Update top wall
    if topWallExpandTimer > 0 then
        topWallExpandTimer = topWallExpandTimer - 1
        topWallOffset = topWallOffset - wallShrinkSpeed * wallExpandSpeedMultiplier
        if topWallOffset < 0 then topWallOffset = 0 end
    else
        topWallOffset = topWallOffset + wallShrinkSpeed
        if topWallOffset > maxShrinkY then topWallOffset = maxShrinkY end
    end
    
    -- Update bottom wall
    if bottomWallExpandTimer > 0 then
        bottomWallExpandTimer = bottomWallExpandTimer - 1
        bottomWallOffset = bottomWallOffset - wallShrinkSpeed * wallExpandSpeedMultiplier
        if bottomWallOffset < 0 then bottomWallOffset = 0 end
    else
        bottomWallOffset = bottomWallOffset + wallShrinkSpeed
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
    
    -- Clamp to real screen bounds (top and bottom edges)
    local realScreenHeight = Window.get_display_height()
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
