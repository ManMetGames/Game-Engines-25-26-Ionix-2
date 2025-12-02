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
local followSpeed = 15  -- Higher = faster catch-up

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
    
    -- Target position (centered on cursor)
    local targetX = mouseX - playerSize/2
    local targetY = mouseY - playerSize/2
    
    -- Smooth follow using lerp (linear interpolation)
    -- dt would be ideal here, but we'll use a fixed factor
    local lerpFactor = followSpeed * 0.016  -- Assuming ~60fps, adjust followSpeed to tune
    
    playerX = playerX + (targetX - playerX) * lerpFactor
    playerY = playerY + (targetY - playerY) * lerpFactor
    
    Entity.set_global_pos(player, playerX, playerY)
end

return TriangleShooter
