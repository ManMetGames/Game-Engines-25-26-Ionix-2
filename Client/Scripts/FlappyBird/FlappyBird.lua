local ExampleScript = {}
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
    local BgBackground = Entity.add_sprite_component(Background, "Background", 0,0, 0)
    Sprite.set_playback_mode(BgBackground, 4)
    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity(0)

    Entity.set_entity_pos(player1, x, 500)
	
    local playerSprite1 = Entity.add_sprite_component(player1, "FlappyBird", 64, 64, 0)

    Sprite.set_sprite_width(playerSprite1, 64)
    Sprite.set_sprite_height(playerSprite1, 64)
	Sprite.set_playback_mode(playerSprite1, 4)

    -- PLAYER 1 PHYSICS
    Entity.add_fysics_component(player1, 2, false) -- dynamic body
    --Fysics.add_sprite_collider(player1, false)
    Fysics.add_box_collider(player1, 1, 1.5, 0, 0, 0, false)


    local tileSize = 64
    local floorY = 600

	------------------------------------------------------
	-- pick texture for left / middle / right
	------------------------------------------------------
	local tex = "middle"

	for i = 0, 30 do
		local tile = Entity.create_entity(0)
		local xPos = i * tileSize

		------------------------------------------------------
		-- place sprite
		------------------------------------------------------
		Entity.set_entity_pos(tile, xPos, floorY)

		-- sprite as single frame (4 = manual/no anim)
		local s = Entity.add_sprite_component(tile, "Sand", tileSize, tileSize, 0)
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
	pipe = Entity.create_entity(0)
	Entity.set_entity_pos(pipe, 400, 400)

	local pipeSprite = Entity.add_sprite_component(pipe, "BottomPipe", 0, 0, 0)
	Sprite.set_playback_mode(pipeSprite, 1)

	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipe, 1, false)
	Fysics.add_sprite_collider(pipe, false)
	------------------------------------------------------
	-- pick texture for left / middle / right
	------------------------------------------------------
	local tex = "middle"

	------------------------------------------------------
	-- place sprite
	------------------------------------------------------
	local floor = Entity.create_entity()
	Entity.set_entity_pos(floor, 0, floorY)

	-- sprite as single frame (4 = manual/no anim)
	local s = Entity.add_sprite_component(floor, "Sand", Window.get_width(), tileSize, 0)
	Sprite.set_playback_mode(s, 4)

	------------------------------------------------------
	-- add physics body + collider
	------------------------------------------------------
	Entity.add_fysics_component(floor, 0, false)  -- static
	Fysics.add_sprite_collider(floor, false)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    -- get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)
    
    -- Constant rightward movement
    local vx = 0
    local vy1 = vel1.y

	if Input.get_key_down(Keys.ionix_space) then
        -- Set velocity directly to cancel out falling momentum
        vy1 = -5  -- Jump velocity for player1
	end
	
	if Input.get_key_down(Keys.ionix_a) then
        Entity.set_entity_pos(player1, xPos, floorY)
	end

    Fysics.set_linear_velocity(player1, vx, vy1)
end

return ExampleScript
