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
local h = 300
local w = 300






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

    Entity.set_entity_pos(player1, x, 300)
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 32, 32, 10)
    Sprite.set_columns(playerSprite1,1)
    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, 2, true) -- dynamic body
    Fysics.add_sprite_collider(player1, false)
    Fysics.add_sprite_collider(player1,false,1)
    -- Freeze bird
    

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
		Entity.set_entity_pos(tile, xPos, floorY)
		local s = Entity.add_sprite_component(tile, assets.textures.Sand, tileSize, tileSize, 1)
        Sprite.set_columns(s,1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, 0, false)  -- static
		Fysics.add_sprite_collider(tile, false,1)

end

pipe = Entity.create_entity()
	Entity.set_entity_pos(pipe, 440, 550)

	local pipeSprite = Entity.add_sprite_component(pipe, assets.textures.FlappyPipe, 80, 50, 0)
    Sprite.set_columns(pipeSprite,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipe, 1, false)
	Fysics.add_sprite_collider(pipe, false,1)


end
  



----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()

local score = 42
  -- text, w, h, x, y, font (font currently unused/placeholder)
  UI.draw_label(text, w, h, x, y, font)

  

 -- get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)

 
   
    -- Constant rightward movement
    local vx = 0
    local vy1 = vel1.y

	if Input.get_key_down(Keys.ionix_space) then
      
        vy1 = -5  -- Jump velocity for player1
  end
    if Input.get_key_held(Keys.ionix_a) then
       
        vx = -5
    end        

 if Input.get_key_held(Keys.ionix_d) then
       
        vx = 5
    end        


 Fysics.set_linear_velocity(player1, vx, vy1)


      
end

return ExampleScript