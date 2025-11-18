local ExampleScript = {}

local player1
local player2
local goal
local playerSprite
local goalSprite
local x = 200
local goalX = 500
local goalY = 500
local y = 300
local t = 10

local function CheckGoalProximity(player1, goal, threshold, respawnX, respawnY)
local function CheckGoalProximity(player2, goal, threshold, respawnX, respawnY)
    
end

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
    Texture.add_texture("./Assets/FlappyBird.png", "FlapyBird")
    Texture.add_texture("./Assets/Background.png", "Background")
    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()
    player2 = Entity.create_entity()

    Entity.set_entity_pos(player1, x, 300)
    Entity.set_entity_pos(player2, x, 200)

    playerSprite = Entity.add_sprite_component(player1, "FlappyBird", 100, 100, 0)
    playerSprite = Entity.add_sprite_component(player2, "FlappyBird", 100, 100, 0)

    Sprite.set_width(playerSprite, 64)
    Sprite.set_height(playerSprite, 64)
	Sprite.set_playback_mode(playerSprite, 4)

    Entity.add_fysics_component(player, 2, false) -- dynamic body
    Fysics.add_box_collider(player, .5, .4, 0, 0, 0, false)
    
    -- Get and log the current gravity scale
    local currentGravityScale = Fysics.get_gravity_scale(player)
    print("Previous gravity scale: " .. currentGravityScale)
    
    -- Set gravity scale to 1 (default)
    Fysics.set_gravity_scale(player, 1)
    print("New gravity scale: 1")

    Entity.add_fysics_component(player1, 2, false) -- dynamic body
    Fysics.add_box_collider(player1, .5, .4, 0, 0, 0, false)

   -- PLAYER 2 PHYSICS 
    Entity.add_fysics_component(player2, 2, false) -- dynamic body
    Fysics.add_box_collider(player2, .5, .4, 0, 0, 0, false)

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
		Fysics.add_box_collider(tile, 1, 1, 0, 0, 0, false)  -- not a trigger
	end
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    -- get current velocity
    local vel = Fysics.get_linear_velocity(player1)
    local vel = Fysics.get_linear_velocity(player2)
    local vx = vel.x
    local vy = vel.y

	if Input.get_key_down(Keys.ionix_space) then
        -- Set velocity directly to cancel out falling momentum
        vy = -5  -- Jump velocity (negative is up)
	end
    ------------------------------------------------------
    -- movement
    ------------------------------------------------------
    if Input.get_key_held(Keys.ionix_d) then
        vx = 2.5
    elseif Input.get_key_held(Keys.ionix_a) then
        vx = -2.5
    else
        vx = 0
    end

    Fysics.set_linear_velocity(player1, vx, vy)
    Fysics.set_linear_velocity(player2, vx, vy)
	
	-- To do...
	CheckGoalProximity(player1, goal, 50, x, y)
    CheckGoalProximity(player2, goal, 50, x, y)
end

return ExampleScript
