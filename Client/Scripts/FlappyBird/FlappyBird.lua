local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local Background
local player1
local player2
local player3
local player4
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
    local BgBackground = Entity.add_sprite_component(Background, assets.textures.OutsideSmilingOffice,1920 , 1280, 0)
    

    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()

    Entity.set_global_pos(player1, x, 200)
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.Pim, 128, 128, 10)
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
	
    local playerSprite2 = Entity.add_sprite_component(player2, assets.textures.Charlie, 160, 160, 10)
    Sprite.set_columns(playerSprite2,1)
    -- PLAYER 2 PHYSICS

    Entity.add_fysics_component(player2, enums.bodytype.dynamicBody, true) -- dynamic body
    --Fysics.add_sprite_collider(player2, false)
    Fysics.add_sprite_collider(player2,false,1)

    ------------------------------------------------------
    -- Create player3
    ------------------------------------------------------
    player3 = Entity.create_entity()

    Entity.set_global_pos(player3, x + 100, 200)
	
    local playerSprite3 = Entity.add_sprite_component(player3, assets.textures.Allan, 128, 200, 10)
    Sprite.set_columns(playerSprite3,1)
    -- PLAYER 3 PHYSICS

    Entity.add_fysics_component(player3, enums.bodytype.dynamicBody, true) -- dynamic body
    --Fysics.add_sprite_collider(player3, false)
    Fysics.add_sprite_collider(player3,false,1)

    ------------------------------------------------------
    -- Create player4
    ------------------------------------------------------
    player4 = Entity.create_entity()

    Entity.set_global_pos(player4, x + 150, 200)
	
    local playerSprite4 = Entity.add_sprite_component(player4, assets.textures.Glep, 96, 96, 10)
    Sprite.set_columns(playerSprite4,1)
    -- PLAYER 4 PHYSICS

    Entity.add_fysics_component(player4, enums.bodytype.dynamicBody, true) -- dynamic body
    --Fysics.add_sprite_collider(player4, false)
    Fysics.add_sprite_collider(player4,false,1)

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
    local tileSprite = Entity.add_sprite_component(tile, assets.textures.Platform, 1000, 100, 1)
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
    local platformSprite = Entity.add_sprite_component(tile, assets.textures.Platform, 1000, 100, 1)
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
    local vx1 = Mafs.get_vec_x(vel1)
    local vy1 = Mafs.get_vec_y(vel1)

    -- get current velocity
    local vel2 = Fysics.get_linear_velocity(player2)
    local vx2 = Mafs.get_vec_x(vel2)
    local vy2 = Mafs.get_vec_y(vel2)

    -- get current velocity
    local vel3 = Fysics.get_linear_velocity(player3)
    local vx3 = Mafs.get_vec_x(vel3)
    local vy3 = Mafs.get_vec_y(vel3)

    -- get current velocity
    local vel4 = Fysics.get_linear_velocity(player4)
    local vx4 = Mafs.get_vec_x(vel4)
    local vy4 = Mafs.get_vec_y(vel4)

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
        vx1 = 2.5 * Input.get_left_stick_x(0)
    else
        vx1 = 0
    end
	
    if Input.get_left_stick_x(1) then
        vx2 = 2.5 * Input.get_left_stick_x(1)
    else
        vx2 = 0
    end

    if Input.get_left_stick_x(2) then
        vx3 = 2.5 * Input.get_left_stick_x(2)
    else
        vx3 = 0
    end

    if Input.get_left_stick_x(3) then
        vx4 = 2.5 * Input.get_left_stick_x(3)
    else
        vx4 = 0
    end

    Fysics.set_linear_velocity(player1, vx1, vy1)
    Fysics.set_linear_velocity(player2, vx2, vy2)
    Fysics.set_linear_velocity(player3, vx3, vy3)
    Fysics.set_linear_velocity(player4, vx4, vy4)
     
end

    function ExampleScript:OnCollisionEnter()
        if Fysics.col(player1, tile) or Fysics.col(player1, platform1) then
                jumpCount1 = 0
                print("grounded")
            end
        if Fysics.col(player2, tile) or Fysics.col(player2, platform1) then
                jumpCount1 = 0
                print("grounded")
            end
        if Fysics.col(player3, tile) or Fysics.col(player3, platform1) then
                jumpCount1 = 0
                print("grounded")
            end
        if Fysics.col(player4, tile) or Fysics.col(player4, platform1) then
                jumpCount1 = 0
                print("grounded")
            end
    end

    function ExampleScript:OnTriggerEnter()
    end

    function ExampleScript:OnTriggerExit()
    end

    function ExampleScript:OnCollisionExit()
    end

return ExampleScript