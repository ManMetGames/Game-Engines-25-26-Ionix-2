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
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.Debug, 32, 32, 10)
    Sprite.set_columns(playerSprite1,1)
    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, 2, true) -- dynamic body
    --Fysics.add_sprite_collider(player1, false)
    Fysics.add_sprite_collider(player1,false,1)
    -- Freeze bird
    Fysics.set_gravity_scale(player1, 0) 

      local tileSize = 64
    local floorY = 600
  
    -- pick texture for left / middle / right
	------------------------------------------------------
	local tex = "middle"

	for i = 0, 30 do
		local tile = Entity.create_entity()
		local xPos = i * tileSize

		------------------------------------------------------
		-- place sprite
		------------------------------------------------------
		Entity.set_local_pos(tile, xPos, floorY)
		local s = Entity.add_sprite_component(tile, assets.textures.Sand, tileSize, tileSize, 1)
        Sprite.set_columns(s,1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, 0, false)  -- static
		Fysics.add_sprite_collider(tile, false,1)

end


----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()

    local vx = 0
    local vy = 0

  -- basic movment using the WASD keys
    if Input.get_key_down(Keys.ionix_d) then
        vx = 5
        vy = 0
         Fysics.set_linear_velocity(player1, vx, vy)
    end    
    if Input.get_key_down(Keys.ionix_a) then
        vx = -5
        vy = 0
         Fysics.set_linear_velocity(player1, vx, vy)
    end
    if Input.get_key_down(Keys.ionix_w) then
        vy = -5
        vx = 0
         Fysics.set_linear_velocity(player1, vx, vy)
    end    
    if Input.get_key_down(Keys.ionix_s) then
        vy = 5
        vx = 0
         Fysics.set_linear_velocity(player1, vx, vy)
    end   

     end
      
end

return ExampleScript