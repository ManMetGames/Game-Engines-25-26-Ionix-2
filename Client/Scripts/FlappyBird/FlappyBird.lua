local ExampleScript = {}
local assets = require("Scripts.Assets")
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

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    ------------------------------------------------------
	-- Background Texture
	------------------------------------------------------
    Background = Entity.create_entity()
    local BgBackground = Entity.add_sprite_component(Background, assets.textures.Background,0 , 0, 0)
    Sprite.set_width(BgBackground, 1280)
    Sprite.set_height(BgBackground, 1280)
    Sprite.set_playback_mode(BgBackground, 4)

    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()

    Entity.set_entity_pos(player1, x, 300)
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 0, 0, 10)

	Sprite.set_playback_mode(playerSprite1, 4)

    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, 2, false) -- dynamic body
    --Fysics.add_sprite_collider(player1, false)
    Fysics.add_box_collider(player1, 1, 1, 0, 0, 0, false)
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
		Entity.set_entity_pos(tile, xPos, floorY)

		-- sprite as single frame (4 = manual/no anim)
		local s = Entity.add_sprite_component(tile, assets.textures.Sand, 0, 0, 1)
		Sprite.set_playback_mode(s, 4)

		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, 0, false)  -- static
		Fysics.add_sprite_collider(tile, false)
	end

	------------------------------------------------------
	-- Create pipe obstacle
	------------------------------------------------------
    --BOTTOM PIPE
	pipe = Entity.create_entity()
	Entity.set_entity_pos(pipe, 640, 400)

	local pipeSprite = Entity.add_sprite_component(pipe, assets.textures.FlappyPipe, 0, 0, 2)
    Sprite.set_width(pipeSprite, 480)
    Sprite.set_height(pipeSprite, 1845)
	Sprite.set_playback_mode(pipeSprite, 4)

	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipe, 1, false)
	Fysics.add_sprite_collider(pipe, false)


    -- TOP PIPE
    pipeT = Entity.create_entity()
	Entity.set_entity_pos(pipeT, 640, 0)

	local pipeSpriteT = Entity.add_sprite_component(pipeT,assets.textures.FlappyPipe , 0, 0, 3)
    Sprite.set_width(pipeSpriteT, 480)
    Sprite.set_height(pipeSpriteT, 1845)
	Sprite.set_playback_mode(pipeSpriteT, 4)

	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipeT, 1, false)
	Fysics.add_sprite_collider(pipeT, false)

    if Input.get_key_down(Keys.ionix_a) then
        Entity.set_entity_pos(pipe, xPos, floorY)
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
    local vx = 0
    local vy1 = vel1.y

	if Input.get_key_down(Keys.ionix_space) then
        -- Bird move if space is pressed (allow gravity)
        Fysics.set_gravity_scale(player1, 1)
        Fysics.set_linear_velocity(pipe, pipeSpeed, 0)
        Fysics.set_linear_velocity(pipeT, pipeSpeed, 0)
        -- Set velocity directly to cancel out falling momentum
        vy1 = -5  -- Jump velocity for player1
	end

    Fysics.set_linear_velocity(player1, vx, vy1)

    -- Pipe movement
    local pipePos = Fysics.get_pos(pipe)
    local pipePos = Fysics.get_pos(pipeT)
    if pipePos.x < pipeOffScreenLeft then
        Fysics.set_pos(pipe, pipeStartX, pipePos.y)
        Fysics.set_pos(pipeT, pipeStartX, pipePos.y)
     end
     
end

return ExampleScript