local TriangleShooter = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local TriangleShooterLevels = require("Scripts.TriangleShooter.TriangleShooterLevels")

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
local enemy
local enemySprite
local enemySize = 48
local enemyX = 400  -- Center of screen
local enemyY = 300
local enemyHealth = 50
local enemyRotation = 0
local enemySpinVelocity = 0      -- Current spin speed (degrees per frame)
local enemySpinMaxVelocity = 36  -- Soft cap (~360 RPM at 60fps)
local enemySpinBoost = 1         -- Spin added on each bounce (much lower)
local enemySpinDecayBase = 0.0005
local enemySpinDecayExtra = 0.01

-- Enemy state machine: "aiming" (tracks player) -> "dashing" -> "aiming"
local enemyState = "aiming"
local dashCooldown = 0

-- Timing (at 60fps)
local dashCooldownDuration = 180  -- 3 seconds between dashes
local dashSpeed = 5               -- pixels per frame while dashing
local dashDirX = 0
local dashDirY = 0
local enemyBounceSteer = 0.4
local enemySteerStrength = 0.0
-- Continuous bouncing (no max bounces)

-- Enemy projectile settings
local enemyProjectiles = {}
local enemyProjectilePool = {}
local enemyProjectileSize = 24
local enemyProjectileSpeed = 3
local enemyShootCooldown = 0
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

-- Level settings
local currentLevel = 1
local levelTimer = 0

local function LoadLevel(index)
    local cfg = TriangleShooterLevels.getLevelConfig(index)
    if not cfg then
        return
    end

    currentLevel = index
    levelTimer = cfg.timeLimitFrames or 0

    wallPingPongEnabled = cfg.wallPingPong and true or false

    enemyProjectilesEnabled = cfg.enemyProjectiles and true or false

    enemyHealth = cfg.enemyHealth or enemyHealth
    enemyX = screenW / 2 - enemySize / 2
    enemyY = screenH / 2 - enemySize / 2
    Entity.set_global_pos(enemy, enemyX, enemyY)

    enemyShootCooldown = 0
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
    
    -- Create enemy cube at center of screen
    enemy = Entity.create_entity()
    Entity.set_global_pos(enemy, enemyX, enemyY)
    enemySprite = Entity.add_sprite_component(enemy, assets.textures.Cube, enemySize, enemySize, 5)
    Sprite.set_columns(enemySprite, 1)

    LoadLevel(1)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function TriangleShooter:OnUpdate()
    -- Update wall lerps
    UpdateWallLerps()
    
    -- Update screen bounds from window
    screenW = Window.get_width()
    screenH = Window.get_height()
    
    if levelTimer > 0 then
        levelTimer = levelTimer - 1
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
    
    -- Rotate triangle to face the enemy cube
    local dx = (enemyX + enemySize/2) - (playerX + playerSize/2)
    local dy = (enemyY + enemySize/2) - (playerY + playerSize/2)
    local angleRadians = math.atan(dy, dx)
    local angleDegrees = math.deg(angleRadians) + 90  -- +90 because triangle points up by default
    Entity.set_global_rot(player, angleDegrees)
    
    -- Store normalized aim direction for projectiles
    local dist = math.sqrt(dx * dx + dy * dy)
    if dist > 0 then
        aimDirX = dx / dist
        aimDirY = dy / dist
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

    if enemyHealth <= 0 then
        OnEnemyKilled()
    elseif levelTimer <= 0 and enemyHealth > 0 then
        OnLevelTimeout()
    end
end

----------------------------------------------------------
-- Spawn a projectile from the tip of the triangle
----------------------------------------------------------
function SpawnProjectile()
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
    
    -- Spawn at tip of triangle (offset in aim direction)
    local centerX = playerX + playerSize/2
    local centerY = playerY + playerSize/2
    local spawnX = centerX + aimDirX * (playerSize/2) - projectileSize/2
    local spawnY = centerY + aimDirY * (playerSize/2) - projectileSize/2
    
    -- Set position and rotation
    Entity.set_global_pos(projData.entity, spawnX, spawnY)
    local projAngle = math.deg(math.atan(aimDirY, aimDirX)) + 90
    Entity.set_global_rot(projData.entity, projAngle)
    
    -- Initialize projectile data
    projData.x = spawnX
    projData.y = spawnY
    projData.vx = aimDirX * projectileSpeed
    projData.vy = aimDirY * projectileSpeed
    projData.age = 0
    
    table.insert(projectiles, projData)
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
        
        -- Check collisionRadius with enemy cube
        local projCenterX = proj.x + projectileSize/2
        local projCenterY = proj.y + projectileSize/2
        local enemyCenterX = enemyX + enemySize/2
        local enemyCenterY = enemyY + enemySize/2
        
        local dx = projCenterX - enemyCenterX
        local dy = projCenterY - enemyCenterY
        local distSq = dx * dx + dy * dy
        local hitRadius = collisionRadius + projectileSize/2
        
        if distSq < hitRadius * hitRadius then
            -- Collision! Flash enemy and return projectile to pool
            enemyHealth = enemyHealth - 1
            FlashEnemy()
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
function FlashEnemy()
    Sprite.set_color(enemySprite, 255, 0, 0)
    flashTimer = flashDuration
    if enemyHealth <= 0 then
        Entity.set_global_pos(enemy, -1000, -1000)
    end
end

function UpdateFlash()
    -- Enemy flash
    if flashTimer > 0 then
        flashTimer = flashTimer - 1
        if flashTimer <= 0 then
            Sprite.set_color(enemySprite, 255, 255, 255)
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
    
    -- Check collision between enemy and player
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    local enemyCenterX = enemyX + enemySize/2
    local enemyCenterY = enemyY + enemySize/2
    
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
        if dashDirX ~= 0 or dashDirY ~= 0 then
            local sideLX = -dashDirY
            local sideLY = dashDirX
            local sideRX = dashDirY
            local sideRY = -dashDirX
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
    end
end

function FlashPlayer()
    Sprite.set_color(playerSprite, 255, 0, 0)
    playerFlashTimer = playerFlashDuration
    if playerHealth <= 0 then
        Entity.set_global_pos(player, -1000, -1000)
    end
end

----------------------------------------------------------
-- Enemy behavior: aiming (tracks player) <-> dashing
----------------------------------------------------------
function UpdateEnemyDash()
    -- Wall boundaries
    local minX = 0
    local maxX = screenW - enemySize
    local minY = 0
    local maxY = screenH - enemySize
    
    -- Calculate direction to player (used in aiming state)
    local enemyCenterX = enemyX + enemySize/2
    local enemyCenterY = enemyY + enemySize/2
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    local dx = playerCenterX - enemyCenterX
    local dy = playerCenterY - enemyCenterY
    
    if enemyState == "aiming" then
        -- Always face the player
        local angle = math.deg(math.atan(dy, dx)) + 90
        Entity.set_global_rot(enemy, angle)
        
        -- Count down cooldown
        if dashCooldown > 0 then
            dashCooldown = dashCooldown - 1
        else
            -- Cooldown done, lock direction and dash
            local dist = math.sqrt(dx * dx + dy * dy)
            if dist > 0 then
                dashDirX = dx / dist
                dashDirY = dy / dist
            else
                dashDirX = 0
                dashDirY = 0
            end
            enemyState = "dashing"
            enemyShootCooldown = 0
        end
        
    elseif enemyState == "dashing" then
        if enemySteerStrength > 0 then
            local distToPlayer = math.sqrt(dx * dx + dy * dy)
            if distToPlayer > 0 then
                local toPlayerDirX = dx / distToPlayer
                local toPlayerDirY = dy / distToPlayer
                local steer = enemySteerStrength
                local newDirX = dashDirX * (1 - steer) + toPlayerDirX * steer
                local newDirY = dashDirY * (1 - steer) + toPlayerDirY * steer
                local newLen = math.sqrt(newDirX * newDirX + newDirY * newDirY)
                if newLen > 0 then
                    dashDirX = newDirX / newLen
                    dashDirY = newDirY / newLen
                end
            end
        end
        -- Scale speed based on current arena size vs original
        local scaleX = screenW / originalWindowWidth
        local scaleY = screenH / originalWindowHeight
        local scale = (scaleX + scaleY) / 2
        local currentSpeed = dashSpeed * scale
        
        -- Move in locked direction (no rotation change)
        enemyX = enemyX + dashDirX * currentSpeed
        enemyY = enemyY + dashDirY * currentSpeed
        
        -- Shoot projectiles while dashing
        if enemyProjectilesEnabled then
            enemyShootCooldown = enemyShootCooldown + 1
            if enemyShootCooldown >= enemyShootInterval then
                SpawnEnemyProjectile()
                enemyShootCooldown = 0
            end
        end
        
        -- Check wall collision and bounce
        local hitLeft = false
        local hitRight = false
        local hitTop = false
        local hitBottom = false
        
        if enemyX <= minX then
            enemyX = minX
            hitLeft = true
        elseif enemyX >= maxX then
            enemyX = maxX
            hitRight = true
        end
        if enemyY <= minY then
            enemyY = minY
            hitTop = true
        elseif enemyY >= maxY then
            enemyY = maxY
            hitBottom = true
        end
        
        -- Bounce off walls toward player and trigger wall lerp
        if hitLeft or hitRight or hitTop or hitBottom then
            if hitLeft or hitRight then
                dashDirX = -dashDirX
            end
            if hitTop or hitBottom then
                dashDirY = -dashDirY
            end
            local len = math.sqrt(dashDirX * dashDirX + dashDirY * dashDirY)
            if len > 0 then
                dashDirX = dashDirX / len
                dashDirY = dashDirY / len
            end
            local enemyCenterX = enemyX + enemySize/2
            local enemyCenterY = enemyY + enemySize/2
            local toPlayerX = playerCenterX - enemyCenterX
            local toPlayerY = playerCenterY - enemyCenterY
            local dist = math.sqrt(toPlayerX * toPlayerX + toPlayerY * toPlayerY)
            if dist > 0 then
                local toPlayerDirX = toPlayerX / dist
                local toPlayerDirY = toPlayerY / dist
                local steer = enemyBounceSteer
                local newDirX = dashDirX * (1 - steer) + toPlayerDirX * steer
                local newDirY = dashDirY * (1 - steer) + toPlayerDirY * steer
                local newLen = math.sqrt(newDirX * newDirX + newDirY * newDirY)
                if newLen > 0 then
                    dashDirX = newDirX / newLen
                    dashDirY = newDirY / newLen
                end
            end
            
            -- Trigger wall lerps
            if hitLeft then TriggerWallLerp("left") end
            if hitRight then TriggerWallLerp("right") end
            if hitTop then TriggerWallLerp("top") end
            if hitBottom then TriggerWallLerp("bottom") end
            
            -- Add spin on bounce
            enemySpinVelocity = enemySpinVelocity + enemySpinBoost
            if enemySpinVelocity > enemySpinMaxVelocity then
                enemySpinVelocity = enemySpinMaxVelocity
            end
        end
        
        -- Apply spin rotation
        local speed = math.abs(enemySpinVelocity)
        local t = speed / enemySpinMaxVelocity
        if t > 1 then t = 1 end
        local decay = enemySpinDecayBase + enemySpinDecayExtra * t
        enemySpinVelocity = enemySpinVelocity - enemySpinVelocity * decay
        if math.abs(enemySpinVelocity) < 0.01 then
            enemySpinVelocity = 0
        end
        enemyRotation = enemyRotation + enemySpinVelocity
        Entity.set_global_rot(enemy, enemyRotation)
        
        Entity.set_global_pos(enemy, enemyX, enemyY)
    end
end

----------------------------------------------------------
-- Enemy projectile spawning
----------------------------------------------------------
function SpawnEnemyProjectile()
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
    local enemyCenterX = enemyX + enemySize/2
    local enemyCenterY = enemyY + enemySize/2
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
