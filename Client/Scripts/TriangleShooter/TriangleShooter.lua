local TriangleShooter = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

-- Player (triangle)
local player
local playerSprite

-- Player settings
local playerSize = 48
local playerX = 400
local playerY = 300
local followSpeed = 18  -- Higher = faster catch-up

-- Projectile settings
local projectiles = {}
local projectileSize = 16
local projectileSpeed = 10

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
    
    -- Smooth follow using lerp (linear interpolation)
    -- dt would be ideal here, but we'll use a fixed factor
    local lerpFactor = followSpeed * 0.016  -- Assuming ~60fps, adjust followSpeed to tune
    
    playerX = playerX + (targetX - playerX) * lerpFactor
    playerY = playerY + (targetY - playerY) * lerpFactor
    
    Entity.set_global_pos(player, playerX, playerY)
    
    -- Rotate triangle to face center of screen
    local centerX = 400  -- Assuming 800 width screen
    local centerY = 300  -- Assuming 600 height screen
    local dx = centerX - (playerX + playerSize/2)
    local dy = centerY - (playerY + playerSize/2)
    local angleRadians = math.atan(dy, dx)
    local angleDegrees = math.deg(angleRadians) + 90  -- +90 because triangle points up by default
    Entity.set_global_rot(player, angleDegrees)
    
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
    local proj = Entity.create_entity()
    
    -- Spawn at tip of triangle (top center, assuming triangle points up)
    local spawnX = playerX + playerSize/2 - projectileSize/2
    local spawnY = playerY - projectileSize
    
    Entity.set_global_pos(proj, spawnX, spawnY)
    
    local sprite = Entity.add_sprite_component(proj, assets.textures.Ghast_Tear, projectileSize, projectileSize, 5)
    Sprite.set_columns(sprite, 1)
    
    -- Store projectile with its velocity (firing upward for now)
    table.insert(projectiles, {
        entity = proj,
        x = spawnX,
        y = spawnY,
        vx = 0,
        vy = -projectileSpeed  -- Negative Y = upward
    })
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
        
        -- Remove if off screen
        if proj.y < -50 or proj.y > 700 or proj.x < -50 or proj.x > 1000 then
            -- TODO: destroy entity when that API is available
            table.remove(projectiles, i)
        end
    end
end

return TriangleShooter
