local ExampleScript = {}

local player1
local goal
local playerSprite
local goalSprite
local x = 200
local pipex = 600
local goalX = 500
local goalY = 500
local y = 300
local t = 10
local pipes = {}
local pipeSpeed = 150
local spawnDistance = 300
local minGapY = 200
local maxGapY = 400
local gapSize = 150
local screenWidth = 800
local nextSpawnX = 800

----------------------------------------------------------
-- Pipe Creation Functions
----------------------------------------------------------
local function CreatePipe(xPos, gapY, gapSize)
    local pipeWidth = 64
    local pipeHeight = 400
    
    -- Create top pipe
    local topPipe = Entity.create_entity()
    Entity.set_entity_pos(topPipe, xPos, gapY - gapSize/2 - pipeHeight)
    
    local topSprite = Entity.add_sprite_component(topPipe, "middle", pipeWidth, pipeHeight, 0)
    Sprite.set_width(topSprite, pipeWidth)
    Sprite.set_height(topSprite, pipeHeight)
    Sprite.set_playback_mode(topSprite, 4)
    
    -- Create bottom pipe
    local bottomPipe = Entity.create_entity()
    Entity.set_entity_pos(bottomPipe, xPos, gapY + gapSize/2)
    
    local bottomSprite = Entity.add_sprite_component(bottomPipe, "middle", pipeWidth, pipeHeight, 0)
    Sprite.set_width(bottomSprite, pipeWidth)
    Sprite.set_height(bottomSprite, pipeHeight)
    Sprite.set_playback_mode(bottomSprite, 4)
    
    return {top = topPipe, bottom = bottomPipe, x = xPos}
end

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    -- Load textures
    --Texture.add_texture("./Assets/left.png", "left")
    --Texture.add_texture("./Assets/middle.png", "middle")
	--Texture.add_texture("./Assets/right.png", "right")
	--Texture.add_texture("./Assets/player1.png", "player1")
    Texture.add_texture("./Assets/pipe.png", "pipe")
	--Texture.add_texture("./Assets/key.png", "key")
    Texture.add_texture("./Assets/left.png", "left")
    Texture.add_texture("./Assets/middle.png", "middle")
    Texture.add_texture("./Assets/right.png", "right")
    Texture.add_texture("./Assets/player1.png", "player1")
    Texture.add_texture("./Assets/key.png", "key")
    Texture.add_texture("./Assets/FlappyBird.png", "FlappyBird")
    --Texture.add_texture("./Assets/Background.png", "Background")
    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    Texture.add_texture("./Assets/Background.png", "Background")
    
    -- Create player
    player1 = Entity.create_entity()
    pipe1 = Entity.create_entity()

    Entity.set_entity_pos(player1, x, 300)
    Entity.set_entity_pos(pipe1, pipex, 350)
	
    Entity.set_entity_pos(player1, 200, 300)
    
    local playerSprite1 = Entity.add_sprite_component(player1, "FlappyBird", 64, 64, 0)
    local pipeSprite1 = Entity.add_sprite_component(pipe1, "pipe", 128, 300, 0)

    Sprite.set_width(playerSprite1, 320)
    Sprite.set_height(playerSprite1, 320)
    Sprite.set_width(pipeSprite1, 375)
    Sprite.set_height(pipeSprite1, 475)

    -- Still image --
	Sprite.set_playback_mode(playerSprite1, 4)
    Sprite.set_playback_mode(pipeSprite1, 4)

    -- PLAYER 1 PHYSICS
    Sprite.set_width(playerSprite1, 64)
    Sprite.set_height(playerSprite1, 64)
    Sprite.set_playback_mode(playerSprite1, 4)
    
    -- Player physics
    Entity.add_fysics_component(player1, 2, false) -- dynamic body
    Fysics.add_sprite_collider(player1, false)

    --Entity.add_fysics_component(pipe1, 2, true)
    --Fysics.add_sprite_collider(pipe1, false)

    
    -- Create floor
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
    
    for i = 0, 30 do
        local tile = Entity.create_entity()
        local xPos = i * tileSize
        
        Entity.set_entity_pos(tile, xPos, floorY)
        
        local s = Entity.add_sprite_component(tile, "middle", tileSize, tileSize, 0)
        Sprite.set_playback_mode(s, 4)
        
        Entity.add_fysics_component(tile, 0, false) -- static
        Fysics.add_sprite_collider(tile, false)
    end
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    -- get current velocity
    -- Player controls
    local vel1 = Fysics.get_linear_velocity(player1)
    
    local vx = 0
    local vy1 = vel1.y

    -- Moving Pipes across screen --
    Entity.set_entity_pos(pipe1, pipex-10, 350)

	if Input.get_key_down(Keys.ionix_space) then
        -- Set velocity directly to cancel out falling momentum
        vy1 = -5  -- Jump velocity for player1
	end
	
	if Input.get_key_down(Keys.ionix_a) then
        Entity.set_entity_pos(player1, xPos, floorY)
	end

    
    if Input.get_key_down(Keys.ionix_space) then
        vy1 = -5 -- Jump velocity
    end
    
    Fysics.set_linear_velocity(player1, vx, vy1)
end

return ExampleScript