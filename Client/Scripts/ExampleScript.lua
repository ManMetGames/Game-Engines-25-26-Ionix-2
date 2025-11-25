local ExampleScript = {}

local player
local player2
local goal
local objectSprite
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
	Texture.add_texture("./Assets/player.png", "player")
	Texture.add_texture("./Assets/key.png", "key")
    ------------------------------------------------------
    -- Create player
    ------------------------------------------------------
    player = Entity.create_entity()
    Entity.set_entity_pos(player, x, y)
    playerSprite = Entity.add_sprite_component(player, "ball", 75, 75, 0)
	Sprite.set_playback_mode(playerSprite, 4)

    Entity.add_fysics_component(player, 2, true) -- dynamic body
    Fysics.add_sprite_collider(player, false)
  
    player2 = Entity.create_entity()
    Entity.set_entity_pos(player2, x+100, y+100)
    playerSprite = Entity.add_sprite_component(player2, "player", 75, 75, 0)
	Sprite.set_playback_mode(playerSprite, 4)

    Entity.add_fysics_component(player2, 2, true) -- dynamic body
    Fysics.add_sprite_collider(player2, false)

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
		Entity.set_entity_pos(tile, xPos, floorY)

		-- sprite as single frame (4 = manual/no anim)
		local s = Entity.add_sprite_component(tile, tex, tileSize, tileSize, 0)
		Sprite.set_playback_mode(s, 4)

		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, 0, false)  -- static
        Fysics.add_sprite_collider(tile, false)
	end
   ------------------------------------------------------

    --Fysics.create_prismatic_joint(player2, player, 0, 0, 5, 10, true, 50 , 20, true) --Prismatic Joint
    
    --Fysics.create_weld_joint(player2, player) --Weld Joint
    
    --Fysics.create_pulley_joint(player2, player, x, y, x+300, y, 4, 1, 2) --Pulley Joint
    
    --Fysics.create_revolute_joint(player2,player, false, 5, 8, true, 20, 60) --Revolute Joint

    --Fysics.create_distance_joint(player2, player, 10) --Distance Joint
   
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    -- get current velocity
    local vel = Fysics.get_linear_velocity(player)
    local vx = vel.x
    local vy = vel.y

	if Input.get_key_down(Keys.ionix_space) then
        Fysics.add_force_to_center(player, 0, -45)
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
    if Input.get_key_down(Keys.ionix_q) then
      -- Fysics.set_damping(0,0) --Joint ID used to find the correct joint (just the position of the joint in the world joint list)
    end
     if Input.get_key_down(Keys.ionix_e) then

      -- Fysics.set_damping(0,100)
    end

    Fysics.set_linear_velocity(player, vx, vy)
	
	-- To do...
	CheckGoalProximity(player, goal, 50, x, y)
end

return ExampleScript
