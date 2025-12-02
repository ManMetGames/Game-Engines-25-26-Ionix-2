local PacMan = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local Background
local player1
local winWidth = Window.get_width()
local winHeight = Window.get_height()

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function PacMan:OnStart()

    ------------------------------------------------------
	-- Background Texture
	------------------------------------------------------
    Background = Entity.create_entity()
    local BgBackground = Entity.add_sprite_component(Background, assets.textures.PacManBackground,960 , 640, 0)
    
    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()
    local pacman = Entity.add_sprite_component(player1, assets.textures.PacMan, 40, 40, 1)

    Sprite.set_width(pacman, 840)
    Sprite.set_height(pacman, 680)

    Entity.set_global_pos(player1, winWidth/2-20,  winHeight/2+15)
	
    -- PLAYER 1 PHYSICs
    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true) -- dynamic body
    --Fysics.add_sprite_collider(player1, false)
    Fysics.add_sprite_collider(player1,false,1)
    -- Freeze bird
    Fysics.set_gravity_scale(player1, 0)


    local tileSize = 64
    local floorY = 600
	------------------------------------------------------
	-- pick texture for left / middle / right
	------------------------------------------------------
	local tex = "middle"

	for i = 0, 30 do
		local tile = Entity.create_entity()
		local xPos = i * tileSize
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)  -- static
		Fysics.add_sprite_collider(tile, false,1)
	end
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function PacMan:OnUpdate()

end

return PacMan