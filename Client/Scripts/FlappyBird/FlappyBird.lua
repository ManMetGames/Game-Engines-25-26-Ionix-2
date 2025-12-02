local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
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
local coinCount = 0

-- Pipe variables
local pipe
local pipeT
local pipeSpeed = -3
local pipeStartX = 900
local pipeOffScreenLeft = -100

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

    Entity.set_global_pos(player1, x, 300)
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 32, 32, 10)
    Sprite.set_columns(playerSprite1,1)
    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true) -- dynamic body
    --Fysics.add_sprite_collider(player1, false)
    Fysics.add_sprite_collider(player1,false,1)
    -- Freeze bird
    Fysics.set_gravity_scale(player1, 0)



    -----------------------------
    ------Coins
    ------------
    coin = Entity.create_entity()
    Entity.set_global_pos(coin, 200, 200)
    local coinSprite = Entity.add_sprite_component(coin, assets.textures.Coin, 100, 32, 10)
    Sprite.set_columns(coinSprite, 1)
    Entity.add_fysics_component(coin, enums.bodytype.kinematicBody, false)
    Fysics.add_sprite_collider(coin, true, 1)

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
		Entity.set_global_pos(tile, xPos, floorY)
		local s = Entity.add_sprite_component(tile, assets.textures.Sand, tileSize, tileSize, 1)
        Sprite.set_columns(s,1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)  -- static
		Fysics.add_sprite_collider(tile, false,1)
	end

	------------------------------------------------------
	-- Create pipe obstacle
	------------------------------------------------------
    --BOTTOM PIPE
	pipe = Entity.create_entity()
	Entity.set_global_pos(pipe, 640, 400)

	local pipeSprite = Entity.add_sprite_component(pipe, assets.textures.FlappyPipe, 80, 300, 0)
    Sprite.set_columns(pipeSprite,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipe, enums.bodytype.kinematicBody, false)
	Fysics.add_sprite_collider(pipe, false,1)


    -- TOP PIPE
    pipeT = Entity.create_entity()
	Entity.set_global_pos(pipeT, 640, 0)

	local pipeSpriteT = Entity.add_sprite_component(pipeT,assets.textures.FlappyPipe , 80, 300, 0)
    Sprite.set_columns(pipeSpriteT,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipeT, enums.bodytype.kinematicBody, false)
	Fysics.add_sprite_collider(pipeT, false,1)

    if Input.get_key_down(Keys.ionix_a) then
        Entity.set_global_pos(pipe, xPos, floorY)
	end

end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    -- get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)
    local vy1 = Fysics.get_linear_velocity(pipe)
    -- Constant rightward movement
    local vx = 0
    local vy1 = Mafs.get_vec_y(vel1)

	if Input.get_key_down(Keys.ionix_space) then
        -- Bird move if space is pressed (allow gravity)
        Fysics.set_gravity_scale(player1, 1)
        Fysics.set_linear_velocity(pipe, pipeSpeed, 0)
        Fysics.set_linear_velocity(pipeT, pipeSpeed, 0)
        -- Set velocity directly to cancel out falling momentum
        vy1 = -5  -- Jump velocity for player1
	end

    Fysics.set_linear_velocity(player1, vx, vy1)

    -- Pipe movement
    local pipePos = Fysics.get_pos(pipe)
    if Mafs.get_vec_x(pipePos) < pipeOffScreenLeft then
        Fysics.set_pos(pipe, pipeStartX, pipePos.y)
        Fysics.set_pos(pipeT, pipeStartX, pipePos.y)
     end
     
end

    function ExampleScript:OnCollisionEnter()
        if Fysics.col(player1, pipe) then
                print("CollisionPipe")
            end
    end

    function ExampleScript:OnTriggerEnter()
        if Fysics.col(player1, coin) then
                Entity.destroy_entity(coin)
                coinCount = coinCount + 1
                print(coinCount)
        end
    end

    function ExampleScript:OnTriggerExit()
        if Fysics.col(player1, coin) then
            print("ExitCoinCollision")
        end
    end

    function ExampleScript:OnCollisionExit()
        if Fysics.col(player1, pipe) then
            print("ExitCollisionPipe")
        end
    end

return ExampleScript