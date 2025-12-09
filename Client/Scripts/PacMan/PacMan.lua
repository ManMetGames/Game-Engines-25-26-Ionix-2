local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local Background
local player1
local goal
local playerSprite
local goalSprite
local x = 200
local goalX = 500
local goalY = 500
local y = 300
local t = 10

-- Pipe variables
local pipe
local pipeT
local pipeSpeed = -3
local pipeStartX = 900
local pipeOffScreenLeft = -100

--PacMan
local xDir -- -1 goes left, 1 goes right
local yDir -- -1 goes up, 1 goes down

local windowSize

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    local pie = Mafs.pi()
    windowSize = Window.get_height()
    print(windowSize)    
    ------------------------------------------------------
	-- Background Texture
	------------------------------------------------------
    Background = Entity.create_entity()
    local BgBackground = Entity.add_sprite_component(Background, assets.textures.Background,960 , 640, 0)
    

    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()

    Entity.set_global_pos(player1, x, 300)
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.PacMan, 32, 32, 10) --Pac man sprite render + animation
    Sprite.set_columns(playerSprite1, 2)
    Sprite.rows(playerSprite1, 2)
    Sprite.set_height(playerSprite1, 32)
    Sprite.set_width(playerSprite1, 32)
    Sprite.set_playback_mode(playerSprite1, 4)
    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, false) -- dynamic body, last value is rotation lock
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
		Entity.set_global_pos(tile, xPos, floorY)
		local s = Entity.add_sprite_component(tile, assets.textures.Sand, tileSize, tileSize, 1)
        Sprite.set_columns(s,1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)  -- static
		Fysics.add_sprite_collider(tile, false,1)
	end

	------------------------------------------------------
	-- Create pipe obstacle
	------------------------------------------------------
    --BOTTOM PIPE
	pipe = Entity.create_entity()
	Entity.set_global_pos(pipe, 640, 400)

	local pipeSprite = Entity.add_sprite_component(pipe, assets.textures.FlappyPipe, 80, 300, 0)
    Sprite.set_columns(pipeSprite,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipe, enums.bodytype.kinematicBody, false)
	Fysics.add_sprite_collider(pipe, false,1)


    -- TOP PIPE
    pipeT = Entity.create_entity()
	Entity.set_global_pos(pipeT, 640, 0)

	local pipeSpriteT = Entity.add_sprite_component(pipeT,assets.textures.FlappyPipe , 80, 300, 0)
    Sprite.set_columns(pipeSpriteT,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipeT, enums.bodytype.kinematicBody, false)
	Fysics.add_sprite_collider(pipeT, false,1)

    if Input.get_key_down(Keys.ionix_a) then
        Entity.set_global_pos(pipe, xPos, floorY)
	end
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    -- get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)
    local vy1 = Fysics.get_linear_velocity(pipe)
    local vy1 = Fysics.get_linear_velocity(pipeT)
    -- Constant rightward movement
    local vx = Mafs.get_vec_x(vel1);
    local vy1 = Mafs.get_vec_y(vel1)
    
    --if Input.get_key_down(Kyes.ionix_d) then Fysics.set_linear_velocity(player1, 20, 0) end

	if Input.get_key_down(Keys.ionix_w) then -- move up
        vy1 = -1  
        vx = 0
        Fysics.set_angle(player1, 270 * (Mafs.pi() / 180))
	end
    if Input.get_key_down(Keys.ionix_s) then -- move down
         vy1 = 1
          vx = 0
        Fysics.set_angle(player1, 90 * (Mafs.pi() / 180))
    end
    if Input.get_key_down(Keys.ionix_a) then -- move left
        vx = -1
        vy1 = 0
        Fysics.set_angle(player1, Mafs.pi())
    end
    if Input.get_key_down(Keys.ionix_d) then -- move right
        vx = 1
        vy1 = 0
        Fysics.set_angle(player1, 0)
    end


    Fysics.set_linear_velocity(player1, vx, vy1)

    -- Pipe movement
    local pipePos = Fysics.get_pos(pipe)
    local pipePos = Fysics.get_pos(pipeT)
    if Mafs.get_vec_x(pipePos) < pipeOffScreenLeft then
        Fysics.set_pos(pipe, pipeStartX, pipePos.y)
        Fysics.set_pos(pipeT, pipeStartX, pipePos.y)
     end
     
end

return ExampleScript