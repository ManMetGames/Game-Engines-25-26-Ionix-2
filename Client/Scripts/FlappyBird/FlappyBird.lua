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
local pipeT
local pipeSpeed = -3
local pipeStartX = 900

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    ------------------------------------------------------
    -- Load textures
    ------------------------------------------------------
    Texture.add_texture("./Assets/Background.png", "Background")
    Texture.add_texture("./Assets/FlappyBird.png", "FlappyBird")
    Texture.add_texture("./Assets/left.png", "left")
    Texture.add_texture("./Assets/middle.png", "middle")
	Texture.add_texture("./Assets/right.png", "right")
	Texture.add_texture("./Assets/player1.png", "player1")
	Texture.add_texture("./Assets/key.png", "key")
    Texture.add_texture("./Assets/FlappyPipe.png", "FlappyPipe")
    Texture.add_texture("./Assets/FlappyPipe2.png", "FlappyPipe2")

    ------------------------------------------------------
	-- Background Texture
	------------------------------------------------------
    Background = Entity.create_entity()
    local BgBackground = Entity.add_sprite_component(Background, "Background", 960, 640, 0)
    Sprite.set_width(BgBackground, 1280)
    Sprite.set_height(BgBackground, 1280)
    Sprite.set_playback_mode(BgBackground, 4)

    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()

    Entity.set_entity_pos(player1, x, 300)
	
    local playerSprite1 = Entity.add_sprite_component(player1, "FlappyBird", 0, 0, 0)

	Sprite.set_playback_mode(playerSprite1, 4)

    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, 2, false) -- dynamic body
    Fysics.add_sprite_collider(player1, false)

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
    --BOTTOM PIPE
	pipe = Entity.create_entity()
	Entity.set_entity_pos(pipe, 640, 400)

	local pipeSprite = Entity.add_sprite_component(pipe, "FlappyPipe", 80, 185, 0)
    Sprite.set_width(pipeSprite, 480)
    Sprite.set_height(pipeSprite, 1845)
	Sprite.set_playback_mode(pipeSprite, 4)

	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipe, 1, false)
	Fysics.add_sprite_collider(pipe, false)


    -- TOP PIPE
    pipeT = Entity.create_entity()
	Entity.set_entity_pos(pipeT, 640, 0)

	local pipeSpriteT = Entity.add_sprite_component(pipeT, "FlappyPipe2", 80, 185, 0)
    Sprite.set_width(pipeSpriteT, 480)
    Sprite.set_height(pipeSpriteT, 1845)
	Sprite.set_playback_mode(pipeSpriteT, 4)

	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipeT, 1, false)
	Fysics.add_sprite_collider(pipeT, false)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()

    -- get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)
    local v = Fysics.get_linear_velocity(pipe)
    local vy = Fysics.get_linear_velocity(pipeT)
    -- Constant rightward movement
    local vx = vel1.x
    local vy1 = vel1.y
    local v1 = v.x

    local pipePos = Fysics.get_pos(pipe)
	if pipePos.x < 0 then
       Entity.set_entity_pos(pipe, 640, 400)
       --Fysics.set_linear_velocity(pipe, 640, 0)
	end
    
    --local pipePos = Fysics.get_pos(pipeT)
    --if pipePos.x < 0 then
      --Entity.set_entity_pos(pipe, 640, 400)
    --end

	if Input.get_key_down(Keys.ionix_space) then
        -- Bird moves once space key is pressed (allows gravity)
        Fysics.set_gravity_scale(player1, 1)
        -- Pipe movement
        Fysics.set_linear_velocity(pipe, pipeSpeed, 0)
        Fysics.set_linear_velocity(pipeT, pipeSpeed, 0)
        -- Set velocity directly to cancel out falling momentum
        vy1 = -5  -- Jump velocity for player1
	end
	
    Fysics.set_linear_velocity(player1, vx, vy1)

    --Draws button
    UI.Add_label(250, 250, 100, 100, "Some Text!")
    --UI.draw_button("But", 30, 10, 100, 100)
    --UI.Add_label("Press any button to play", 200, 200, 200 ,200, "Bold")
    --UI.Add_label(50, 50, 100, 50, "Press any button to play")
end

return ExampleScript
