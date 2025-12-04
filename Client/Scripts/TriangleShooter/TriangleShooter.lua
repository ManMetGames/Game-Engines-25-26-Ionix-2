local TriangleShooter = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local TriangleShooterLevels = require("Scripts.TriangleShooter.TriangleShooterLevels")
local TriangleShooterEnemy = require("Scripts.TriangleShooter.TriangleShooterEnemy")

-- Screen bounds (updated each frame from window size)
local screenW = 1920
local screenH = 1080

-- Wall settings
local wallPingPongEnabled = true
local wallMaxShrinkX = 760        -- Max pixels each horizontal wall can shrink (1920 - 400 = 1520, /2 = 760)
local wallMaxShrinkY = 340        -- Max pixels each vertical wall can shrink (1080 - 400 = 680, /2 = 340)
local wallShrinkSpeed = 0.2       -- Pixels per frame each wall shrinks
local wallExpandDuration = 180    -- 3 seconds at 60fps
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

-- Player (triangle)
local player
local playerSprite
local playerSize = 48
local playerX = 400
local playerY = 300
local playerSpeed = 0.5  -- Mouse sensitivity multiplier
local playerHealth = 100

-- Player flash effect
local playerFlashTimer = 0
local playerFlashDuration = 10  -- frames

-- Damage cooldown (0.5s = 30 frames at 60fps)
local damageCooldown = 0
local damageCooldownDuration = 30

-- Projectile settings
local projectiles = {}      -- Active projectiles
local projectilePool = {}   -- Inactive projectiles (reusable)
local projectileSize = 24
local projectileSpeed = 4
local projectileLifetime = 300  -- frames (~5 seconds at 60fps)

-- Current aim direction (updated each frame)
local aimDirX = 0
local aimDirY = -1  -- Default: pointing up

-- Enemy (cube)
local enemySize = 48

local enemies = {}
local levelEnemyHealth = 50

local function CreateEnemy(x, y, health)
    return TriangleShooterEnemy.createEnemy(x, y, health, enemySize, playerX, playerY, playerSize)
end

local function ClearEnemies()
    TriangleShooterEnemy.clearEnemies(enemies)
    enemies = {}
end

-- Enemy projectile settings
local enemyProjectiles = {}
local enemyProjectilePool = {}
local enemyProjectileSize = 24
local enemyProjectileSpeed = 3
local enemyShootInterval = 75  -- 1.25 seconds at 60fps
local enemyProjectilesEnabled = true

-- Collision settings
local collisionRadius = 24  -- Half of enemy size for circle collision

-- Flash effect
local flashTimer = 0
local flashDuration = 10  -- frames

local knockbackTimer = 0
local knockbackDuration = 45
local knockbackBaseSpeed = 6
local knockbackDirX = 0
local knockbackDirY = 0

local musicEntity
local musicVolume = 64

local bpm = 34 -- not real bpm
local framesPerBeat = 3600 / bpm
local beatTimer = 0
local bopDurationFrames = 8
local bopTimer = 0

local beatStartDelayFrames = 3402
local beatStartDelayCounter = 0

local playerBaseImageWidth = playerSize
local playerBaseImageHeight = playerSize
local enemyBaseImageSize = enemySize

local globalFrame = 0

-- Level settings
local currentLevel = 1
local levelTimer = 0

local playerLevel = 1
local xp = 0
local xpToNextLevel = 100
local currentShootAbility = "basic"

local shootAbilities = {}

shootAbilities.basic = function(tipX, tipY, aimX, aimY)
    return {
        { offsetX = 0, offsetY = 0, dirX = aimX, dirY = aimY }
    }
end

shootAbilities.dual = function(tipX, tipY, aimX, aimY)
    local sideX = -aimY
    local sideY = aimX
    local offset = projectileSize * 0.6

    return {
        { offsetX = sideX * offset, offsetY = sideY * offset, dirX = aimX, dirY = aimY },
        { offsetX = -sideX * offset, offsetY = -sideY * offset, dirX = aimX, dirY = aimY },
    }
end

shootAbilities.triple = function(tipX, tipY, aimX, aimY)
    local sideX = -aimY
    local sideY = aimX
    local offset = projectileSize * 0.6

    return {
        { offsetX = 0, offsetY = 0, dirX = aimX, dirY = aimY },
        { offsetX = sideX * offset, offsetY = sideY * offset, dirX = aimX, dirY = aimY },
        { offsetX = -sideX * offset, offsetY = -sideY * offset, dirX = aimX, dirY = aimY },
    }
end

shootAbilities.wide = function(tipX, tipY, aimX, aimY)
    local count = 4
    local totalSpreadDeg = 40
    local baseAngle = math.atan(aimY, aimX)
    local halfSpread = math.rad(totalSpreadDeg) / 2
    local step = (count == 1) and 0 or (2 * halfSpread / (count - 1))

    local shots = {}
    for i = 0, count - 1 do
        local angle = baseAngle - halfSpread + step * i
        local dirX = math.cos(angle)
        local dirY = math.sin(angle)
        shots[#shots + 1] = { offsetX = 0, offsetY = 0, dirX = dirX, dirY = dirY }
    end

    return shots
end

local function GetXpForNextLevel(level)
    local n = level - 1
    return math.floor(100 + 40 * n + 10 * n * math.max(n - 1, 0))
end

local function OnLevelUp()
    playerLevel = playerLevel + 1
    xpToNextLevel = GetXpForNextLevel(playerLevel)

    if playerLevel == 2 then
        currentShootAbility = "dual"
    elseif playerLevel == 3 then
        currentShootAbility = "triple"
    elseif playerLevel == 4 then
        currentShootAbility = "wide"
    end
end

local function AddXp(amount)
    xp = xp + amount
    while xp >= xpToNextLevel do
        xp = xp - xpToNextLevel
        OnLevelUp()
    end
end

local function LoadLevel(index)
    local cfg = TriangleShooterLevels.getLevelConfig(index)
    if not cfg then
        return
    end

    currentLevel = index
    levelTimer = cfg.timeLimitFrames or 0

    wallPingPongEnabled = cfg.wallPingPong and true or false

    enemyProjectilesEnabled = cfg.enemyProjectiles and true or false

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
    playerX = screenW / 2 - playerSize / 2
    playerY = screenH / 2 - playerSize / 2
    Entity.set_global_pos(player, playerX, playerY)
    Sprite.set_color(playerSprite, 255, 255, 255)
    playerFlashTimer = 0
    damageCooldown = 0
end

local function OnEnemyKilled()
    local nextIndex = currentLevel + 1
    if TriangleShooterLevels.getLevelConfig(nextIndex) then
        LoadLevel(nextIndex)
    else
        LoadLevel(currentLevel)
    end
end

local function OnLevelTimeout()
    LoadLevel(currentLevel)
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
    
    LoadLevel(1)

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

    -- Update wall lerps
    UpdateWallLerps()
    
    -- Update screen bounds from window
    screenW = Window.get_width()
    screenH = Window.get_height()
    
    if levelTimer > 0 then
        levelTimer = levelTimer - 1
    end

    if Input.get_key_down(Keys.ionix_space) then
        print(string.format("[BeatMarker] globalFrame=%d", globalFrame))
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
    UI.draw_label("Player Lv: " .. tostring(playerLevel) .. "  XP: " .. tostring(xp) .. " / " .. tostring(xpToNextLevel), 220, 45, 740, 60, "")

    local enemiesAlive = #enemies > 0
    if playerHealth <= 0 then
        LoadLevel(currentLevel)
    elseif not enemiesAlive then
        OnEnemyKilled()
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
    local abilityFunc = shootAbilities[currentShootAbility] or shootAbilities.basic
    local shots = abilityFunc(tipX, tipY, aimDirX, aimDirY)
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
            AddXp(1)
            FlashEnemy(enemy)
            if enemy.health <= 0 then
                Entity.set_global_pos(enemy.entity, -1000, -1000)
                table.remove(enemies, hitEnemyIndex)
            end
            Entity.set_global_pos(proj.entity, -1000, -1000)
            table.insert(projectilePool, table.remove(projectiles, i))
        else
            -- Increment age and remove if expired or off screen
            proj.age = proj.age + 1
            if proj.age > projectileLifetime or proj.y < -50 or proj.y > screenH + 50 or proj.x < -50 or proj.x > screenW + 50 then
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
    Sprite.set_color(enemy.sprite, 255, 0, 0)
    enemy.flashTimer = flashDuration
end

function UpdateFlash()
    -- Enemy flash
    for i = 1, #enemies do
        local enemy = enemies[i]
        if enemy.flashTimer and enemy.flashTimer > 0 then
            enemy.flashTimer = enemy.flashTimer - 1
            if enemy.flashTimer <= 0 then
                Sprite.set_color(enemy.sprite, 255, 255, 255)
            end
        end
    end
    
    -- Player flash
    if playerFlashTimer > 0 then
        playerFlashTimer = playerFlashTimer - 1
        if playerFlashTimer <= 0 then
            Sprite.set_color(playerSprite, 255, 255, 255)
        end
    end
    
    -- Damage cooldown
    if damageCooldown > 0 then
        damageCooldown = damageCooldown - 1
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
    if beatStartDelayCounter < beatStartDelayFrames then
        beatStartDelayCounter = beatStartDelayCounter + 1
        return
    end

    beatTimer = beatTimer + 1
    if beatTimer >= framesPerBeat then
        beatTimer = beatTimer - framesPerBeat
        bopTimer = bopDurationFrames
    end

    if bopTimer > 0 then
        bopTimer = bopTimer - 1
        local t = bopTimer / bopDurationFrames
        local scale = 1.0 + 0.45 * t

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

----------------------------------------------------------
-- Update enemy projectiles
----------------------------------------------------------
function UpdateEnemyProjectiles()
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
            proj.age = proj.age + 1
            if proj.age > projectileLifetime or proj.y < -50 or proj.y > screenH + 50 or proj.x < -50 or proj.x > screenW + 50 then
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
    
    -- Update left wall: always shrinking, expand when hit
    if leftWallExpandTimer > 0 then
        leftWallExpandTimer = leftWallExpandTimer - 1
        leftWallOffset = leftWallOffset - wallShrinkSpeed * wallExpandSpeedMultiplier
        if leftWallOffset < 0 then leftWallOffset = 0 end
    else
        leftWallOffset = leftWallOffset + wallShrinkSpeed
        if leftWallOffset > wallMaxShrinkX then leftWallOffset = wallMaxShrinkX end
    end
    
    -- Update right wall
    if rightWallExpandTimer > 0 then
        rightWallExpandTimer = rightWallExpandTimer - 1
        rightWallOffset = rightWallOffset - wallShrinkSpeed * wallExpandSpeedMultiplier
        if rightWallOffset < 0 then rightWallOffset = 0 end
    else
        rightWallOffset = rightWallOffset + wallShrinkSpeed
        if rightWallOffset > wallMaxShrinkX then rightWallOffset = wallMaxShrinkX end
    end
    
    -- Update top wall
    if topWallExpandTimer > 0 then
        topWallExpandTimer = topWallExpandTimer - 1
        topWallOffset = topWallOffset - wallShrinkSpeed * wallExpandSpeedMultiplier
        if topWallOffset < 0 then topWallOffset = 0 end
    else
        topWallOffset = topWallOffset + wallShrinkSpeed
        if topWallOffset > wallMaxShrinkY then topWallOffset = wallMaxShrinkY end
    end
    
    -- Update bottom wall
    if bottomWallExpandTimer > 0 then
        bottomWallExpandTimer = bottomWallExpandTimer - 1
        bottomWallOffset = bottomWallOffset - wallShrinkSpeed * wallExpandSpeedMultiplier
        if bottomWallOffset < 0 then bottomWallOffset = 0 end
    else
        bottomWallOffset = bottomWallOffset + wallShrinkSpeed
        if bottomWallOffset > wallMaxShrinkY then bottomWallOffset = wallMaxShrinkY end
    end
    
    -- Capture initial position on first frame
    if windowInitialX == nil then
        windowInitialX = 0
        windowInitialY = 0
        windowBaseWidth = 1920
        windowBaseHeight = 1080
        originalWindowWidth = 1920
        originalWindowHeight = 1080
        Window.set_pos(windowInitialX, windowInitialY)
        Window.set_size(windowBaseWidth, windowBaseHeight)
    end
    
    -- Calculate new window bounds
    local newX = math.floor(windowInitialX + leftWallOffset)
    local newY = math.floor(windowInitialY + topWallOffset)
    local newWidth = math.floor(windowBaseWidth - leftWallOffset - rightWallOffset)
    local newHeight = math.floor(windowBaseHeight - topWallOffset - bottomWallOffset)
    
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
