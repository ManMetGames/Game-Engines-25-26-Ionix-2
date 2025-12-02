local PacMan = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local Background
local player1
local playerSprite
local x = 200
local y = 300
-- Dots
local Dots = {}

------------------------------------------------------
	-- Create Dots class
------------------------------------------------------
function Dots:new(xPos, yPos)
    local dots = {}
    --create entity
    dots = Entity.create_entity()
    --sprite component
    local dots = Entity.add_sprite_component(dots, assets.textures.Coint,640 , 640 , 1)
    --fysics for collision
    Entity.add_fysics_component(dot.entity, enums.bodytype.staticBody, false)
    Fysics.add_sprite_collider(dot.entity, true, 1)
    --Dots properties

    return Dots
end

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

    Entity.set_global_pos(player1, x, 300)
	

    -- PLAYER 1 PHYSICS

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
		-- place sprite
		------------------------------------------------------
		
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)  -- static
		Fysics.add_sprite_collider(tile, false,1)
	end
------------------------------------------------------
    -- Create Grid of Dots
------------------------------------------------------
    local dotSpacingX = 80
    local dotSpacingY = 80
    local startX = 100
    local startY = 100
    local rows = 6
    local cols = 10
    
    for row = 0, rows - 1 do
        for col = 0, cols - 1 do
            local dotX = startX + (col * dotSpacingX)
            local dotY = startY + (row * dotSpacingY)
            
            local newDot = Dot:new(dotX, dotY)
            table.insert(allDots, newDot)
        end
    end
    
    print("Created " .. #allDots .. " dots!")

end



----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function PacMan:OnUpdate()
    -- get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)
    
    -- Constant rightward movement
    local vx = 0
    local vy1 = Mafs.get_vec_y(vel1)

	

    Fysics.set_linear_velocity(player1, vx, vy1)

    -- Pipe movement
   
     
     
end

return PacMan