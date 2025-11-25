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
local coin

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
    Texture.add_texture("./Assets/Coin", "Coin")

    ------------------------------------------------------
	-- Background Texture
	------------------------------------------------------
    Background = Entity.create_entity()
    local BgBackground = Entity.add_sprite_component(Background, "Background", 960,640, 0)
    Sprite.set_width(BgBackground, 1280)
    Sprite.set_height(BgBackground, 1280)
    Sprite.set_playback_mode(BgBackground, 4)
    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()
    Entity.set_entity_pos(player1, x, 300)
    local playerSprite1 = Entity.add_sprite_component(player1, "FlappyBird", 100, 100, 0)
    Sprite.set_width(playerSprite1, 64)
    Sprite.set_height(playerSprite1, 64)
	Sprite.set_playback_mode(playerSprite1, 4)

    -- PLAYER 1 PHYSICS
    Entity.add_fysics_component(player1, 2, false) -- dynamic body
    Fysics.add_sprite_collider(player1, false, 0.5)
    Fysics.set_gravity_scale(player1, 0)
    local tileSize = 64
    local floorY = 600

    ------------------------------------------------------
    -- Create Coins 
    ------------------------------------------------------
    coin = Entity.create_entity()
    Entity.set_entity_pos(coin, x, 300)

    local coinSprite = Entity.add_sprite_component(coin, "Coin", 80, 16, 0)
    Sprite.set_width(coinSprite, 80)
    Sprite.set_height(coinSprite, 16)
	Sprite.set_playback_mode(coinSprite, 4)
    ------------------------------------------------------
	-- add physics body + collider
	------------------------------------------------------
	Entity.add_fysics_component(coin, 1, false)  -- static
    Fysics.add_sprite_collider(coin, false)


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
    local vy1 = vel1.y

	if Input.get_key_down(Keys.ionix_space) then
        Fysics.set_gravity_scale(player1, 1)
        vy1 = -5  -- Jump velocity for player1
	end
	
	if Input.get_key_down(Keys.ionix_a) then
        Entity.set_entity_pos(player1, xPos, floorY)
	end

    Fysics.set_linear_velocity(player1, vx, vy1)
    --UI.draw_label("Press any key to play", 20, 20, 300, 300, "Bold")
end

return ExampleScript
