local ExampleScript = {}

local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

local Background
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

    Background = Entity.create_entity()
    local BgBackground = Entity.add_sprite_component(Background, assets.textures.Background,960 , 640, 0)
    ------------------------------------------------------
    ---
    ------------------------------------------------------
    player1 = Entity.create_entity()
    Entity.set_global_pos(player1, x, y)
    playerSprite = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 32, 32, 10)
	Sprite.set_playback_mode(playerSprite, 4)

    Entity.add_fysics_component(player1, 2, false) -- dynamic body
    --Fysics.add_box_collider(player1, .5, .5, 0, 0, 0, false)

    ------------------------------------------------------
    -- Create player 2
    ------------------------------------------------------
    --player2 = Entity.create_entity()
    --Entity.set_global_pos(player2, x + 50, y)
    --playerSprite = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 32, 32, 10)

    --Entity.add_fysics_component(player2, 2, false) -- dynamic body
    --Fysics.add_box_collider(player2, .5, .5, 0, 0, 0, false)

    local tileSize = 32
    local floorY = 610

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

		-- sprite as single frame (4 = manual/no anim)
		local s = Entity.add_sprite_component(tile, tex, tileSize, tileSize, 0)
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

    local vx = 0
    local vy1 = Mafs.get_vec_y(vel1)

	if Input.get_key_down(Keys.ionix_space) then
        -- Bird move if space is pressed (allow gravity)
        Fysics.set_linear_velocity(player1, 2, 0)
    end
    local jump1 = true

     --get current velocity
    local vel = Fysics.get_linear_velocity(player2)
    local vx2 = vel.x
    local vy2 = vel.y
    local jump2 = true


	if Input.get_button_down(0, Buttons.ionix_a) and jump1 == true then
        Fysics.add_force_to_center(player1, 0, -45)
        jump1 = false
	end

    -- if Input.get_button_down(0, Buttons.ionix_a) and jump1 == true then
    --     Fysics.add_force_to_center(player1, 0, -45)
    --     jump1 = false
	-- end

    
	--if Input.get_button_down(1, Buttons.ionix_a) and jump2 == true then
        --Fysics.add_force_to_center(player2, 0, -45)
        --jump2 = false
	--end
    ------------------------------------------------------
    -- movement
    ------------------------------------------------------
    if Input.get_left_stick_x(0) then
        vx1 = 2.5 * Input.get_left_stick_x()
    else
        vx1 = 0
    end
	
    if Input.get_left_stick_x(1) then
        vx2 = 2.5 * Input.get_left_stick_x(1)
    else
        vx2 = 0
    end

    --Fysics.set_linear_velocity(player1, vx1, vy1)
    --Fysics.set_linear_velocity(player2, vx2, vy2)
	
	-- To do...
	CheckGoalProximity(player1, goal, 50, x, y)
end

return ExampleScript
