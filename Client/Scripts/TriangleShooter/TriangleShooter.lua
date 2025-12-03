local TriangleShooter = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

-- Screen bounds (updated each frame from window size)
local screenW = 960
local screenH = 640

-- Player (triangle)
local player
local playerSprite
local playerSize = 48
local playerX = 400
local playerY = 300
local playerSpeed = 0.35  -- Mouse sensitivity multiplier
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
local dashSpeed = 4      -- pixels per frame while moving
local dashDirX = 0
local dashDirY = 0
local collisionRadius = 24  -- Half of enemy size for circle collision

-- Enemy projectile settings
local enemyProjectiles = {}
local enemyProjectilePool = {}
local enemyProjectileSize = 24
local enemyProjectileSpeed = 2.5
local enemyShootCooldown = 0
local enemyShootInterval = 125  -- 2 seconds at 60fps


-- Flash effect
local flashTimer = 0
local flashDuration = 10  -- frames

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

    -- Initial enemy direction towards player
    local enemyCenterX = enemyX + enemySize/2
    local enemyCenterY = enemyY + enemySize/2
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    local dx = playerCenterX - enemyCenterX
    local dy = playerCenterY - enemyCenterY
    local dist = math.sqrt(dx * dx + dy * dy)
    if dist > 0 then
        dashDirX = dx / dist
        dashDirY = dy / dist
    else
        dashDirX = 0
        dashDirY = 0
    end
    local angle = math.deg(math.atan(dashDirY, dashDirX)) + 90
    Entity.set_global_rot(enemy, angle)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function TriangleShooter:OnUpdate()
    -- Update screen bounds from window
    screenW = Window.get_width()
    screenH = Window.get_height()
    
    -- Get mouse delta (relative movement)
    local delta = Input.get_mouse_delta()
    local deltaX = delta.x
    local deltaY = delta.y
    
    -- Move player by delta (allows knockback since not snapping to cursor)
    playerX = playerX + deltaX * playerSpeed
    playerY = playerY + deltaY * playerSpeed
    
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
-- Enemy behavior: continuous dashing with wall bounces and shooting
----------------------------------------------------------
function UpdateEnemyDash()
    -- Wall boundaries
    local minX = 0
    local maxX = screenW - enemySize
    local minY = 0
    local maxY = screenH - enemySize

    -- Ensure we have some direction; if not, aim at player
    if dashDirX == 0 and dashDirY == 0 then
        local enemyCenterX = enemyX + enemySize/2
        local enemyCenterY = enemyY + enemySize/2
        local playerCenterX = playerX + playerSize/2
        local playerCenterY = playerY + playerSize/2
        local dx = playerCenterX - enemyCenterX
        local dy = playerCenterY - enemyCenterY
        local dist = math.sqrt(dx * dx + dy * dy)
        if dist > 0 then
            dashDirX = dx / dist
            dashDirY = dy / dist
        end
    end

    -- Move in current direction
    enemyX = enemyX + dashDirX * dashSpeed
    enemyY = enemyY + dashDirY * dashSpeed

    -- Shoot projectiles on a simple timer
    enemyShootCooldown = enemyShootCooldown + 1
    if enemyShootCooldown >= enemyShootInterval then
        SpawnEnemyProjectile()
        enemyShootCooldown = 0
    end

    -- Check wall collision and bounce
    local hitX = false
    local hitY = false

    if enemyX <= minX then
        enemyX = minX
        hitX = true
    elseif enemyX >= maxX then
        enemyX = maxX
        hitX = true
    end
    if enemyY <= minY then
        enemyY = minY
        hitY = true
    elseif enemyY >= maxY then
        enemyY = maxY
        hitY = true
    end

    -- Bounce off walls (continuous)
    if hitX or hitY then
        -- Bounce: reverse appropriate direction
        if hitX then dashDirX = -dashDirX end
        if hitY then dashDirY = -dashDirY end

        -- Update rotation to match new direction
        local newAngle = math.deg(math.atan(dashDirY, dashDirX)) + 90
        Entity.set_global_rot(enemy, newAngle)
    end

    Entity.set_global_pos(enemy, enemyX, enemyY)
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

return TriangleShooter
