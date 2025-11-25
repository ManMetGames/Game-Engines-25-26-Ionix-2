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
	
    local playerSprite1 = Entity.add_sprite_component(player1, "FlappyBird", 64, 64, 0)

    Sprite.set_width(playerSprite1, 64)
    Sprite.set_height(playerSprite1, 64)
	Sprite.set_playback_mode(playerSprite1, 4)

    -- PLAYER 1 PHYSICS
    Entity.add_fysics_component(player1, 2, false) -- dynamic body
    Fysics.add_sprite_collider(player1, false)

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
    
    -------------------------------
    --Polygon Testing
    --------------------------------
    local polygon = Entity.create_entity()
    Entity.set_entity_pos(polygon, 400, 300)
    local s = Entity.add_sprite_component(polygon, "Sand", tileSize, tileSize, 0)
    Sprite.set_playback_mode(s, 4)
    Entity.add_fysics_component(polygon, 0, false)
    --Fysics.add_sprite_collider(polygon)
    Fysics.add_polygon_collider(polygon, 64)
    
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
