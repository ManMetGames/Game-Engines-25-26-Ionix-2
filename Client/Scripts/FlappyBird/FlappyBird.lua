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

local function CheckGoalProximity(player, goal, threshold, respawnX, respawnY)
    
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
    Texture.add_texture("./Assets/FlappyBird.png", "FlappyBird")
    Texture.add_texture("./Assets/Background.png", "Background")
    Texture.add_texture("./Assets/Sand.png", "Sand")

    background = Entity.create_entity()
    Entity.set_entity_pos(background, 0, 0)
    local bgSprite = Entity.add_sprite_component(background, "Background", -1, -1, 0)
    Sprite.set_width(bgSprite, 1920)
    Sprite.set_height(bgSprite, 1080)
    Sprite.set_playback_mode(bgSprite, 0)   -- static image
    
    Sprite.set_width(bg, 1920)
    Sprite.set_height(bg, 1080)
    Sprite.set_playback_mode(bg, 4)

    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()
    Entity.set_entity_pos(player1, x, 300)
    local playerSprite1 = Entity.add_sprite_component(player1, "FlappyBird", 100, 100, 0)
    Sprite.set_width(playerSprite1, 64)
    Sprite.set_height(playerSprite1, 64)
	Sprite.set_playback_mode(playerSprite1, 4)

    ------------------------------------------------------
    -- Create player2
    ------------------------------------------------------
    player2 = Entity.create_entity()
    Entity.set_entity_pos(player2, x, 200)
    local playerSprite2 = Entity.add_sprite_component(player2, "FlappyBird", 100, 100, 0)
    Sprite.set_width(playerSprite2, 64)
    Sprite.set_height(playerSprite2, 64)
	Sprite.set_playback_mode(playerSprite2, 4)

    -- PLAYER 1 PHYSICS
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
    local vel1 = Fysics.get_linear_velocity(player1)
    local vel2 = Fysics.get_linear_velocity(player2)
    
    -- Constant rightward movement
    local vx = 2.5
    local vy1 = vel1.y
    local vy2 = vel2.y

	if Input.get_key_down(Keys.ionix_space) then
        -- Set velocity directly to cancel out falling momentum
        vy1 = -5  -- Jump velocity for player1
	end

    if Input.get_key_down(Keys.ionix_w) then
        -- Set velocity directly to cancel out falling momentum
        vy2 = -5  -- Jump velocity for player2
	end

    Fysics.set_linear_velocity(player1, vx, vy1)
    Fysics.set_linear_velocity(player2, vx, vy2)
	
	-- To do...
	CheckGoalProximity(player1, goal, 50, x, y)
    CheckGoalProximity(player2, goal, 50, x, y)


    ------------------------------------------------------
    -- CAMERA & SCROLLING
    ------------------------------------------------------
    -- Follow the leading player (or average both)
    local leadX = math.max(Entity.get_entity_pos_x(player1), Entity.get_entity_pos_x(player2))
    local camX = leadX - 400  -- keep players in view (adjust 400 as needed)

    -- Scroll background with parallax (slower = farther away)
    Entity.set_entity_pos_x(background, -camX * 0.4)

    -- Scroll floor seamlessly
    local floorOffset = camX % (31 * tileSize)
    for i = 0, 30 do
        local x = i * tileSize - floorOffset
        Entity.set_entity_pos_x(floorTiles[i], x)
    end
end

return ExampleScript
