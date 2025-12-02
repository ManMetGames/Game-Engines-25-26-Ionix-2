local TriangleShooter = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

-- Player (triangle)
local player
local playerSprite

-- Player settings
local playerSize = 48

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function TriangleShooter:OnStart()
    -- Create player triangle
    player = Entity.create_entity()
    
    -- Start at center of screen
    Entity.set_global_pos(player, 400, 300)
    
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
    
    -- Move triangle to cursor position (centered on cursor)
    Entity.set_global_pos(player, mouseX - playerSize/2, mouseY - playerSize/2)
end

return TriangleShooter
