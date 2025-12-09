local ExampleScript = {}
local assets = require("Scripts.Assets")
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
local h = 500
local w = 400
local text = "Hello, World!"
local font = assets.fonts.DefaultFont
local Apple
local AppleX = 400
local AppleY= 500
local AppleSprite
local AppleSound

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    Background = Entity.create_entity()
    local BgBackground = Entity.add_sprite_component(Background, assets.textures.Background,960 , 640, 0)   

   ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()

    Entity.set_local_pos(player1, x, y)
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.player1, 32, 32, 10)
    Sprite.set_columns(playerSprite1,1)
    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, 2, true) -- dynamic body
    --Fysics.add_sprite_collider(player1, false)
    Fysics.add_sprite_collider(player1,false,1)
    -- Freeze bird
    Fysics.set_gravity_scale(player1, 0) 

      local tileSize = 64
    local floorY = -67
    local CeilingY = 650
    local floorx = 300
    
  
    --Create Apple

    Apple = Entity.create_entity()

    Entity.set_local_pos(Apple, AppleX, AppleY)

    local AppleSprite = Entity.add_sprite_component(Apple, assets.textures.Apple, 32, 32, 10)
    Sprite.set_columns(AppleSprite,1)

    AppleSound = Entity.create_entity()
    Entity.add_audio_component(AppleSound, "AppleCollide", false)
 
    
    Entity.add_fysics_component(Apple, 2, true) -- dynamic body
    --Fysics.add_sprite_collider(player1, false)
    Fysics.add_sprite_collider(Apple,true,1)

    Fysics.set_gravity_scale(Apple, 0) 

    -- pick texture for left / middle / right
	------------------------------------------------------
	local tex = "middle"

	for i = 0, 30 do
		local Top = Entity.create_entity()
        local Bottom = Entity.create_entity()
        local Left = Entity.create_entity()
        local Right = Entity.create_entity()
		local xPos = i * tileSize
        local yPos = i * tileSize


        --Bottom
		------------------------------------------------------
		-- place sprite
		------------------------------------------------------
		Entity.set_local_pos(Bottom, xPos, floorY)
	    local s = Entity.add_sprite_component(Bottom, assets.textures.sand, tileSize, tileSize, 1)
        Sprite.set_columns(s,1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(Bottom, 0, false)  -- static
		Fysics.add_sprite_collider(Bottom, false,1)
        if setColliderTrigger then setColliderTrigger(Bottom, true) end

        
        --Top
        
        Entity.set_local_pos(Top, xPos, CeilingY)
	    local s = Entity.add_sprite_component(Top, assets.textures.sand, tileSize, tileSize, 1)
        Sprite.set_columns(s,1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(Top, 0, false)  -- static
		Fysics.add_sprite_collider(Top, false,1)
        if setColliderTrigger then setColliderTrigger(Top, true) end

        --Left 
        Entity.set_local_pos(Left, -67, yPos)
	    local s = Entity.add_sprite_component(Left, assets.textures.sand, tileSize, tileSize, 1)
        Sprite.set_columns(s,1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(Left, 0, false)  -- static
		Fysics.add_sprite_collider(Left, false,1)
        if setColliderTrigger then setColliderTrigger(Left, true) end


        --Right
        Entity.set_local_pos(Right, 967, yPos)
	    local s = Entity.add_sprite_component(Right, assets.textures.sand, tileSize, tileSize, 1)
        Sprite.set_columns(s,1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(Right, 0, false)  -- static
		Fysics.add_sprite_collider(Right, false,1)

        if setColliderTrigger then setColliderTrigger(Right, true) 
		
		end


end



----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()

    local Speed = 5
    local vx = 0
    local vy = 0

    local playerPos = Entity.get_global_pos(player1)
    local px = Mafs.get_vec_x(playerPos)
    local py = Mafs.get_vec_y(playerPos)

    local minX = 0
    local maxX = 930
    local minY = 0
    local maxY = 610
    
    if px <= minX or px >= maxX or py <= minY or py >= maxY then
        Fysics.set_pos(player1, 200 / 100, 320 / 100)
        Fysics.set_linear_velocity(player1, 0, 0)
    end

  -- basic movment using the WASD keys
    if Input.get_key_down(Keys.ionix_d) then
        vx = Speed
        vy = 0
         Fysics.set_linear_velocity(player1, vx, vy)
    end    
    if Input.get_key_down(Keys.ionix_a) then
        vx = -Speed
        vy = 0
         Fysics.set_linear_velocity(player1, vx, vy)
    end
    if Input.get_key_down(Keys.ionix_w) then
        vy = -Speed
        vx = 0
         Fysics.set_linear_velocity(player1, vx, vy)
    end    
    if Input.get_key_down(Keys.ionix_s) then
        vy = Speed
        vx = 0
         Fysics.set_linear_velocity(player1, vx, vy)
    end   




end

function ExampleScript:OnTriggerEnter(collision1, collision2)
    if(collision2 == Apple) then
        Entity.destroy_entity(collision2)
        AudioComponent.play(AppleSound)
        AudioComponent.change_volume(AppleSound, 64)
    end
end

return ExampleScript