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
local platformTiles = {}  -- Store all platform tiles
local scrollSpeed = 2.5  -- Speed at which platforms move left

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

    -- PLAYER 1 PHYSICS (Category: 0x0002, Mask: 0xFFFD - ignores category 0x0002)
    Entity.add_fysics_component(player1, 2, false) -- dynamic body
    Fysics.add_box_collider(player1, .25, .25, 0, 0, 0, false, 0x0002, 0xFFFD)

    -- PLAYER 2 PHYSICS (Category: 0x0002, Mask: 0xFFFD - ignores category 0x0002)
    Entity.add_fysics_component(player2, 2, false) -- dynamic body
    Fysics.add_box_collider(player2, .25, .25, 0, 0, 0, false, 0x0002, 0xFFFD)

    ------------------------------------------------------
    -- Create one large platform covering the screen width
    ------------------------------------------------------
    local platformWidth = 500  -- Very large width for extended gameplay
    local platformHeight = 2  -- Height of the platform
    local floorY = 600
    
    -- Position platform so players start in the middle of it
    local platform = Entity.create_entity()
    Entity.set_entity_pos(platform, 250, floorY)  -- Center at x=250 so it extends from 0 to 500
    
    -- Add sprite (you can adjust or remove this if you don't need visual)
    local platformSprite = Entity.add_sprite_component(platform, "Sand", platformWidth * 100, platformHeight * 100, 0)
    Sprite.set_playback_mode(platformSprite, 4)
    
    -- Add physics: kinematic body (can move but not affected by collisions)
    Entity.add_fysics_component(platform, 1, false)  -- 1 = kinematic
    Fysics.add_box_collider(platform, platformWidth, platformHeight, 0, 0, 0, false)
    
    -- Store platform reference
    table.insert(platformTiles, platform)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    -- get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)
    local vel2 = Fysics.get_linear_velocity(player2)
    
    -- Players only move vertically (no horizontal movement)
    local vx = 0.033
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
    
    -- Move all platforms to the left to simulate forward movement
    for i, tile in ipairs(platformTiles) do
        Fysics.set_linear_velocity(tile, -scrollSpeed, 0)
    end
	
	-- To do...
	CheckGoalProximity(player1, goal, 50, x, y)
    CheckGoalProximity(player2, goal, 50, x, y)
end

return ExampleScript
