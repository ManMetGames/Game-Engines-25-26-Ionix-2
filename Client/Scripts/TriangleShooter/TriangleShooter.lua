local TriangleShooter = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

-- Player (triangle)
local player
local playerSprite
local playerSize = 48
local playerX = 400
local playerY = 300
local followSpeed = 18  -- Higher = faster catch-up

-- Projectile settings
local projectiles = {}      -- Active projectiles
local projectilePool = {}   -- Inactive projectiles (reusable)
local projectileSize = 16
local projectileSpeed = 10
local projectileLifetime = 300  -- frames (~5 seconds at 60fps)

-- Current aim direction (updated each frame)
local aimDirX = 0
local aimDirY = -1  -- Default: pointing up

-- Enemy (cube)
local enemy
local enemySize = 48
local enemyX = 400  -- Center of screen
local enemyY = 300

-- Collision settings
local collisionRadius = 24  -- Half of enemy size for circle collision

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function TriangleShooter:OnStart()
    -- Create player triangle
    player = Entity.create_entity()
   
    
    -- Start at center of screen
    Entity.set_global_pos(player, playerX, playerY)
    
    -- Add sprite component 
    playerSprite = Entity.add_sprite_component(player, assets.textures.Triangle, playerSize, playerSize, 10)
    Sprite.set_columns(playerSprite, 1)
    
    -- Create enemy cube at center of screen
    enemy = Entity.create_entity()
    Entity.set_global_pos(enemy, enemyX, enemyY)
    local enemySprite = Entity.add_sprite_component(enemy, assets.textures.Cube, enemySize, enemySize, 5)
    Sprite.set_columns(enemySprite, 1)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function TriangleShooter:OnUpdate()
    -- Get mouse position
    local mouseX = Input.get_mouse_x()
    local mouseY = Input.get_mouse_y()
     Input.show_cursor(false)
    
    -- Target position (centered on cursor)
    local targetX = mouseX - playerSize/2
    local targetY = mouseY - playerSize/2
    
    -- Smooth follow using lerp 
    -- dt would be ideal here, but i'll use a fixed factor
    local lerpFactor = followSpeed * 0.016  -- adjust followSpeed to tune
    
    playerX = playerX + (targetX - playerX) * lerpFactor
    playerY = playerY + (targetY - playerY) * lerpFactor
    
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
        
        -- Check collision with enemy cube
        local projCenterX = proj.x + projectileSize/2
        local projCenterY = proj.y + projectileSize/2
        local enemyCenterX = enemyX + enemySize/2
        local enemyCenterY = enemyY + enemySize/2
        
        local dx = projCenterX - enemyCenterX
        local dy = projCenterY - enemyCenterY
        local distSq = dx * dx + dy * dy
        local hitRadius = collisionRadius + projectileSize/2
        
        if distSq < hitRadius * hitRadius then
            -- Collision! Return projectile to pool
            Entity.set_global_pos(proj.entity, -1000, -1000)
            table.insert(projectilePool, table.remove(projectiles, i))
        else
            -- Increment age and remove if expired or off screen
            proj.age = proj.age + 1
            if proj.age > projectileLifetime or proj.y < -50 or proj.y > 700 or proj.x < -50 or proj.x > 1000 then
                -- Move entity off-screen and return to pool
                Entity.set_global_pos(proj.entity, -1000, -1000)
                table.insert(projectilePool, table.remove(projectiles, i))
            end
        end
    end
end

return TriangleShooter
