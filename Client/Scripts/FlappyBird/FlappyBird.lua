local ExampleScript = {}

local player1
local goal
local playerSprite
local goalSprite
local x = 200
local goalX = 500
local goalY = 500
local y = 300
local t = 10

local floorY = 600
local isGrounded = false
local playerHalfHeight = 32

    Texture.add_texture("./Assets/FlappyBird.png", "FlappyBird")
    Texture.add_texture("./Assets/Background.png", "Background")
    ------------------------------------------------------
    -- Create player1
----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    ------------------------------------------------------
    -- Load textures
    ------------------------------------------------------
    Texture.add_texture("./Assets/left.png", "left")
    Texture.add_texture("./Assets/middle.png", "middle")
	Texture.add_texture("./Assets/right.png", "right")
	Texture.add_texture("./Assets/player1.png", "player1")
	Texture.add_texture("./Assets/key.png", "key")
    ------------------------------------------------------
	local Background = Entity.create_entity()
	Entity.set_entity_pos(Background, 0, 0)
	local bgSprite = Entity.add_sprite_component(Background, "Background", 960, 640, -1)
	Sprite.set_width(bgSprite, 1250)
	Sprite.set_height(bgSprite, 1250)

    player1 = Entity.create_entity()

    Entity.set_entity_pos(player1, x, 300)
	
    local playerSprite1 = Entity.add_sprite_component(player1, "FlappyBird", 64, 64, 0)

    Sprite.set_width(playerSprite1, 64)
    Sprite.set_height(playerSprite1, 64)
	Sprite.set_playback_mode(playerSprite1, 4)

    -- PLAYER 1 PHYSICS
    Entity.add_fysics_component(player1, 2, true) -- dynamic body
    Fysics.add_sprite_collider(player1, false)

    local tileSize = 64

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
end


----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    -- get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)
    
    -- Constant rightward movement
    local vx = 0
    local vy1 = vel1.y or 0

	local px, py = Entity.get_entity_pos(player1)
	if py == nil then
		isGrounded = false
	else
		isGrounded = (py >= (floorY - playerHalfHeight))
	end


	if Input.get_key_held(Keys.ionix_space) and isGrounded then 
        -- Set velocity directly to cancel out falling momentum
        vy1 = -5  -- Jump velocity for player1
		isGrounded = false
	end
	
	if Input.get_key_held(Keys.ionix_a) then
		vx = -5
        --Entity.set_entity_pos(player1, vx, floorX)
		Entity.set_entity_pos(player1, vx, floorX)
	end

	if Input.get_key_held(Keys.ionix_d) then
		vx = 5
        --Entity.set_entity_pos(player1, vx, floorX)
		Entity.set_entity_pos(player1, vx, floorX)
	end

    Fysics.set_linear_velocity(player1, vx, vy1)
end

return ExampleScript
