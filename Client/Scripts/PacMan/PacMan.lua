local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local player1
local x = 200
local y = 300

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()
    local tileSize = 128
    local YPos = Window.get_height()/6

    	------------------------------------------------------
		-- makes map by looping through and making a 3x3 map grid, can be easily adjustable
		------------------------------------------------------
	for i = 1, 3 do
        for j = 1, 3 do
		local tile = Entity.create_entity()
        local xPos = (j * tileSize) + 128
		------------------------------------------------------
		-- place sprite
		------------------------------------------------------
		Entity.set_global_pos(tile, xPos, YPos)
		local s = Entity.add_sprite_component(tile, assets.textures.MapBox, tileSize, tileSize, 1)
        Sprite.set_columns(s,1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)  -- static
		Fysics.add_sprite_collider(tile, true, 1)
        end
        YPos = YPos + tileSize
	end
        ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()

    Entity.set_global_pos(player1, x, y)
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.PacMan, 32, 32, 10) --Pac man sprite render + animation
    Sprite.set_columns(playerSprite1, 2)
    Sprite.rows(playerSprite1, 2)
    Sprite.set_height(playerSprite1, 32)
    Sprite.set_width(playerSprite1, 32)
    Sprite.set_playback_mode(playerSprite1, 3)

    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true) -- dynamic body, last value is rotation lock
    Fysics.add_sprite_collider(player1,false,1)
    Fysics.set_gravity_scale(player1, 0)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    -- get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)
    -- Constant rightward movement
    local vx = Mafs.get_vec_x(vel1);
    local vy1 = Mafs.get_vec_y(vel1)
    
    --if Input.get_key_down(Kyes.ionix_d) then Fysics.set_linear_velocity(player1, 20, 0) end

	if Input.get_key_down(Keys.ionix_w) then -- move up
        vy1 = -1
        vx = 0
	end
    if Input.get_key_down(Keys.ionix_s) then -- move down
         vy1 = 1
          vx = 0
    end
    if Input.get_key_down(Keys.ionix_a) then -- move left
        vx = -1
        vy1 = 0
    end
    if Input.get_key_down(Keys.ionix_d) then -- move right
        vx = 1
        vy1 = 0
    end
    Fysics.set_linear_velocity(player1, vx, vy1)
end

return ExampleScript