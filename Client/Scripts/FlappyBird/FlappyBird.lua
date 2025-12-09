local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local Background
local player1
local player2

--PLayer Properties
local playeHealth = 20
local playerSizeX = 32
local playerSizeY = 32
local playerSpeed = 2.5


local x = 300
local jumpCount1 = 0
local jumpCount2 = 0
local tile
local platform1

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    ------------------------------------------------------
	-- Background Texture
	------------------------------------------------------
    Background = Entity.create_entity()
    local BgBackground = Entity.add_sprite_component(Background, assets.textures.Background,960 , 640, 0)
    

    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()

    Entity.set_global_pos(player1, x, 200)
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.FlappyBird, playerSizeX, playerSizeY, 10)
    Sprite.set_columns(playerSprite1,1)
    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true) -- dynamic body
    --Fysics.add_sprite_collider(player1, false)
    Fysics.add_sprite_collider(player1,false,1)

    ------------------------------------------------------
    -- Create player2
    ------------------------------------------------------
    player2 = Entity.create_entity()

    Entity.set_global_pos(player2, x + 50, 200)
	
    local playerSprite2 = Entity.add_sprite_component(player2, assets.textures.FlappyBird, playerSizeX, playerSizeY, 10)
    Sprite.set_columns(playerSprite2,1)
    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player2, enums.bodytype.dynamicBody, true) -- dynamic body
    --Fysics.add_sprite_collider(player2, false)
    Fysics.add_sprite_collider(player2,false,1)

    local tileSize = 64
    local floorY = 500
    
	------------------------------------------------------
	-- pick texture for left / middle / right
	------------------------------------------------------
	local tex = "middle"

	tile = Entity.create_entity()
		------------------------------------------------------
		-- place sprite
		------------------------------------------------------
	Entity.set_global_pos(tile, 250, floorY)
    local tileSprite = Entity.add_sprite_component(tile, assets.textures.Sand, 500, 50, 1)
    Sprite.set_columns(tileSprite, 1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
	Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)  -- static
	Fysics.add_sprite_collider(tile, false, 1)

    platform1 = Entity.create_entity()
		------------------------------------------------------
		-- place sprite
		------------------------------------------------------
	Entity.set_global_pos(platform1, 300, 400)
    local platformSprite = Entity.add_sprite_component(tile, assets.textures.Sand, 500, 50, 1)
    Sprite.set_columns(platformSprite, 1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
	Entity.add_fysics_component(platform1, enums.bodytype.staticBody, false)  -- static
	Fysics.add_edge_collider(platform1, 20, 30, 50, 30, false)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    -- get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)
    local vx2 = Mafs.get_vec_x(vel1)
    local vy1 = Mafs.get_vec_y(vel1)

    -- get current velocity
    local vel2 = Fysics.get_linear_velocity(player2)
    local vx2 = Mafs.get_vec_x(vel2)
    local vy2 = Mafs.get_vec_y(vel2)

	if Input.get_button_down(0, Buttons.ionix_a) then
        jumpCount1 = jumpCount1 + 1
        Fysics.add_force_to_center(player1, 0, -30 / jumpCount1)
	end
    if Input.get_button_down(1, Buttons.ionix_a) and jumpCount2 <= 1 then
        jumpCount2 = jumpCount2 + 1
        Fysics.add_force_to_center(player2, 0, -30  / jumpCount2)
	end 
    
    if Input.get_key_down(Keys.ionix_m) then
        Entity.destroy_entity(coin)
    end

    if Input.get_left_stick_x(0) then
        vx1 = playerSpeed * Input.get_left_stick_x(0)
    else
        vx1 = 0
    end
	
    if Input.get_left_stick_x(1) then
        vx2 = playerSpeed * Input.get_left_stick_x(1)
    else
        vx2 = 0
    end

    Fysics.set_linear_velocity(player1, vx1, vy1)
    Fysics.set_linear_velocity(player2, vx2, vy2)
     
end

    function ExampleScript:OnCollisionEnter()
        if Fysics.col(player1, tile) or Fysics.col(player1, platform1) then
                jumpCount1 = 0
                print("grounded")
            end
        if Fysics.col(player2, tile) or Fysics.col(player1, platform1) then
                jumpCount1 = 0
                print("grounded")
            end
    end

    --Collision between players 
   function ExampleScript:OnHitCollision()
       -- Get distance between player and other player 
       -- make hit radius 
       -- if hit radius equals distance of collisionradius 
       -- knockback player 
       -- playerHealth - playerHealth - 2
   end 
    

    function ExampleScript:OnTriggerEnter()
    end

    function ExampleScript:OnTriggerExit()
    end

    function ExampleScript:OnCollisionExit()
    end

return ExampleScript