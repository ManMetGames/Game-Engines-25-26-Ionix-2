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
local highscore

-- Pipe variables
local pipe
local pipeT
local pipe2
local pipeT2
local pipeSets = {}

-- Pipe movement and positioning
local pipeSpeed = -3
local pipeStartX = 900
local pipe2StartX = 1200
local pipeOffScreenLeft = -100 
local xPos = 0  -- Initialize xPos to avoid undefined global warning

local coins = {}
local coinCount = 6
local coinSpacing = 200
local coinSpeed = -3
local coinHidden = {}
local score = 0
local Pscore = 0
local pipeScoreText = "Score: 0"
local scoreText = "Coins: 0"
local text1 = "Press SPACE to start!"
local text2 = "Press SPACE to restart"
local finalScoreText = "Final Score: 0"
local coinsText = "Coins Collected: "

local gameOver = false

local function resetGame()
    --Reset game state
    gameOver = false
    score = 0
    Pscore = 0
    pipeScoreText = "Score: 0"
    scoreText = "Coins: 0"
    text1 = "Press SPACE to start!"
    finalScoreText = "Final Score: 0"
    coinsText = "Coins Collected: "

    --Reset player
    Fysics.set_pos(player1, 2, 3)
    Fysics.set_gravity_scale(player1, 0)
    Fysics.set_linear_velocity(player1, 0, 0)

    --Reset pipes
    local function resetPipe(pipeEntity, xPos, yPos)
        Fysics.set_pos(pipeEntity, xPos / 100, yPos / 100)
        Fysics.set_linear_velocity(pipeEntity, 0, 0)
    end

    resetPipe(pipe, 640, 400)
    resetPipe(pipeT, 640, 0)
    resetPipe(pipe2, 940, 400)
    resetPipe(pipeT2, 940, -40)
    resetPipe(pipe3, 1240, 360)
    resetPipe(pipeT3, 1240, -40)

    --Reset coins
    for i, c in ipairs(coins) do
        local startX = pipeStartX + (i - 1) * coinSpacing
        local cy
        if (i % 3) == 1 then cy = 240
        elseif (i % 3) == 2 then cy = 120
        else cy = 520 end

        Entity.set_global_pos(c, startX, cy)
        Fysics.set_linear_velocity(c, 0, 0)
        
        local s = Entity.get_sprite_component(c)
        if s then
            Sprite.set_width(s, 16)
            Sprite.set_height(s, 16)
        end
        coinHidden[c] = false
    end
end

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
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 48, 48, 10)
    Sprite.set_columns(playerSprite1,1)
    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true) -- dynamic body
    --Fysics.add_sprite_collider(player1, false)
    Fysics.add_sprite_collider(player1,false, 0.5)
    -- Freeze bird
    Fysics.set_gravity_scale(player1, 0)

	------------------------------------------------------
	-- pick texture for left / middle / right
	------------------------------------------------------
    local tileSize = 64
    local floorY = 600
    
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

	---------------------------
	-- Pipe Set 1
	---------------------------
    --BOTTOM PIPE
	pipe = Entity.create_entity()
	Entity.set_global_pos(pipe, 640, 430)

	local pipeSprite = Entity.add_sprite_component(pipe, assets.textures.FlappyPipe, 60, 300, 0)
    Sprite.set_columns(pipeSprite,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipe, enums.bodytype.kinematicBody, false)
	Fysics.add_sprite_collider(pipe, false,1)


    -- TOP PIPE
    pipeT = Entity.create_entity()
	Entity.set_global_pos(pipeT, 640, 0)

	local pipeSpriteT = Entity.add_sprite_component(pipeT,assets.textures.FlappyPipe2, 60, 300, 0)
    Sprite.set_columns(pipeSpriteT,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipeT, enums.bodytype.kinematicBody, false)
	Fysics.add_sprite_collider(pipeT, false, 1)

    if Input.get_key_down(Keys.ionix_a) then
        Entity.set_global_pos(pipe, xPos, floorY)
	end
    ---------------------------
	-- Pipe Set 2
	---------------------------
    --BOTTOM PIPE
	pipe2 = Entity.create_entity() 
	Entity.set_global_pos(pipe2, 940, 400)

	local pipeSprite2 = Entity.add_sprite_component(pipe2, assets.textures.FlappyPipe, 60, 300, 0)
    Sprite.set_columns(pipeSprite2,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipe2, enums.bodytype.kinematicBody, false)
	Fysics.add_sprite_collider(pipe2, false,1)

    -- TOP PIPE
    pipeT2 = Entity.create_entity()
	Entity.set_global_pos(pipeT2, 940, -40)

	local pipeSpriteT2 = Entity.add_sprite_component(pipeT2,assets.textures.FlappyPipe2, 60, 300, 0)
    Sprite.set_columns(pipeSpriteT2,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipeT2, enums.bodytype.kinematicBody, false)
	Fysics.add_sprite_collider(pipeT2, false,1)

    if Input.get_key_down(Keys.ionix_a) then
        Entity.set_global_pos(pipe2, xPos, floorY)
	end

    ---------------------------
	-- Pipe Set 3
	---------------------------
    --BOTTOM PIPE
	pipe3 = Entity.create_entity() 
	Entity.set_global_pos(pipe3, 1240, 360)

	local pipeSprite3 = Entity.add_sprite_component(pipe3, assets.textures.FlappyPipe, 60, 300, 0)
    Sprite.set_columns(pipeSprite3,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipe3, enums.bodytype.kinematicBody, false)
	Fysics.add_sprite_collider(pipe3, false,1)

    -- TOP PIPE
    pipeT3 = Entity.create_entity()
	Entity.set_global_pos(pipeT3, 1240, -40)

	local pipeSpriteT3 = Entity.add_sprite_component(pipeT3,assets.textures.FlappyPipe2, 60, 300, 0)
    Sprite.set_columns(pipeSpriteT3,1)
	-- Kinematic body so it moves but isn't affected by gravity
	Entity.add_fysics_component(pipeT3, enums.bodytype.kinematicBody, false)
	Fysics.add_sprite_collider(pipeT3, false,1)

    	if Input.get_key_down(Keys.ionix_a) then
		Entity.set_global_pos(pipe3, xPos, floorY)
	end

    pipeSets = {
    { bottom = pipe,  top = pipeT,  passed = false },
    { bottom = pipe2, top = pipeT2, passed = false },
    { bottom = pipe3, top = pipeT3, passed = false }
    }

	------------------------------------------------------
	-- Create coins
	------------------------------------------------------
	for i = 1, coinCount do
		local c = Entity.create_entity()
		-- base X spaced to the right; add small jitter so coins don't line up exactly
		local startX = pipeStartX + (i - 1) * coinSpacing + math.random(-20, 20)
		-- alternate Y positions: some inside the pipe gap, some above, some below
		local cy
		if (i % 3) == 1 then
			cy = 240
		elseif (i % 3) == 2 then
			cy = 120
		else
			cy = 520
		end
		Entity.set_global_pos(c, startX, cy)

		local coinSprite = Entity.add_sprite_component(c, assets.textures.Coin, 32, 32, 0)
		Sprite.set_rows(coinSprite, 1)
		Sprite.set_columns(coinSprite, 5)
		Sprite.set_width(coinSprite, 16)
		Sprite.set_height(coinSprite, 16)

		Entity.add_fysics_component(c, enums.bodytype.kinematicBody, false)
		-- Changed to use trigger collider (true parameter)
		Fysics.add_sprite_collider(c, true, 1)

		
		table.insert(coins, c)
		coinHidden[c] = false  -- Initialize as not hidden
	end

end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    ------------------
	-- Score
	------------------

    if gameOver then
        --Score and Game Over text
        UI.Add_label(350, 235, 1000, 1000, text1)

        --Display final score
        UI.Add_label(375, 260, 1000, 1000, finalScoreText)

        --Display retry text
        UI.Add_label(350, 285, 1000, 1000, text2)

        if gameOver and Input.get_key_down(Keys.ionix_space) then
        resetGame()
        end
        return
    end

    ------------------------------------------------------
	-- Player
	------------------------------------------------------
    -- Get current velocity
    local vel1 = Fysics.get_linear_velocity(player1)
    local vy1 = Fysics.get_linear_velocity(pipe)
    local vy1 = Fysics.get_linear_velocity(pipe2)

    -- Constant rightward movement
    local vx = 0
    local vy1 = Mafs.get_vec_y(vel1)
    
    -- Display instruction text at the start of the game
    if text1 ~= "" then
        UI.Add_label(20, 20, 1000, 1000, pipeScoreText)

        -- UI - Display score in top-left corner
        UI.Add_label(20, 50, 1000, 1000, scoreText)

        -- Display instruction text (only at start)
        UI.Add_label(300, 250, 1000, 1000, text1)

    else
        UI.Add_label(20, 20, 1000, 1000, pipeScoreText)

        -- UI - Display score in top-left corner
        UI.Add_label(20, 50, 1000, 1000, scoreText)

    end

	if Input.get_key_down(Keys.ionix_space) and (not gameOver) then
        -- Bird move if space is pressed (allow gravity)
        Fysics.set_gravity_scale(player1, 0.75)
        -- Set velocity directly to cancel out falling momentum
        vy1 = -3  -- Jump velocity for player1

        -- Pipes move left if space if pressed
        Fysics.set_linear_velocity(pipe, pipeSpeed, 0)
        Fysics.set_linear_velocity(pipeT, pipeSpeed, 0)
        Fysics.set_linear_velocity(pipe2, pipeSpeed, 0)
        Fysics.set_linear_velocity(pipeT2, pipeSpeed, 0)
        Fysics.set_linear_velocity(pipe3, pipeSpeed, 0)
        Fysics.set_linear_velocity(pipeT3, pipeSpeed, 0)
        for _, c in ipairs(coins) do
            Fysics.set_linear_velocity(c, coinSpeed, 0)
        end

        text1 = ""
	end
    Fysics.set_linear_velocity(player1, vx, vy1)

    ------------------------------------------------------
	-- Resetting pipe back to start logic
	------------------------------------------------------

    -- Set 1
    local pipePos = Fysics.get_pos(pipe)
    local pipePosX = Mafs.get_vec_x(pipePos)
    if pipePosX < 0 then
        local random1 = math.random(2, 3)
        local offset = random1/10
        local plusOrMinus = math.random(1, 2)
        if plusOrMinus < 2 then
            offset = offset*-1
        end
        Fysics.set_pos(pipe, 10, 4+offset)
        random1 = math.random(2, 3)
        offset = random1/10
        plusOrMinus = math.random(1, 2)
        if plusOrMinus < 2 then
            offset = offset*-1
        end
        Fysics.set_pos(pipeT, 10, -0.5-offset)
    end
     
    -- Set 2
    local pipePos2 = Fysics.get_pos(pipe2)
    local pipePos2X = Mafs.get_vec_x(pipePos2)
    if pipePos2X < 0 then
        random1 = math.random(2, 3)
        offset = random1/10
        plusOrMinus = math.random(1, 2)
        if plusOrMinus < 2 then
            offset = offset*-1
        end
        Fysics.set_pos(pipe2, 10, 4+offset)
        random1 = math.random(2, 3)
        offset = random1/10
        plusOrMinus = math.random(1, 2)
        if plusOrMinus < 2 then
            offset = offset*-1
        end
        Fysics.set_pos(pipeT2, 10, -0.5+offset)
    end
     
    -- Set 3
    local pipePos3 = Fysics.get_pos(pipe3)
    local pipePos3X = Mafs.get_vec_x(pipePos3)
    if pipePos3X < 0 then
        random1 = math.random(2, 3)
        offset = random1/10
        plusOrMinus = math.random(1, 2)
        if plusOrMinus < 2 then
            offset = offset*-1
        end
        Fysics.set_pos(pipe3, 10, 4+offset)
        random1 = math.random(2, 3)
        offset = random1/10
        plusOrMinus = math.random(1, 2)
        if plusOrMinus < 2 then
            offset = offset*-1
        end
        Fysics.set_pos(pipeT3, 10, -0.5+offset)
    end

    local farthestX = -1e9
	for _, c in ipairs(coins) do
		local p = Fysics.get_pos(c)
		local px = Mafs.get_vec_x(p)
		if px > farthestX then farthestX = px end
	end
	for _, c in ipairs(coins) do
		local p = Fysics.get_pos(c)
		if Mafs.get_vec_x(p) < pipeOffScreenLeft then
			farthestX = farthestX + coinSpacing
			Fysics.set_pos(c, farthestX, p.y)
					-- If this coin was hidden (collected), restore its sprite size so it becomes visible again
			if coinHidden[c] ~= nil then  -- More explicit nil check
				local s = Entity.get_sprite_component(c)
				if s then
					Sprite.set_width(s, 16)
					Sprite.set_height(s, 16)
				end
				coinHidden[c] = false  -- Reset to false instead of nil to maintain the key
			end
		end
	end

    -- Bird X position
local birdPos = Fysics.get_pos(player1)
local birdX = Mafs.get_vec_x(birdPos)

-- Pipe passing logic
for _, set in ipairs(pipeSets) do
    local pipePos = Fysics.get_pos(set.bottom)
    local pipeX = Mafs.get_vec_x(pipePos)

    -- Bird passed pipe midpoint
    if (birdX > pipeX) and (set.passed == false) then
        Pscore = Pscore + 1
        pipeScoreText = "Score: " .. tostring(Pscore)
        print("Passed Pipe! Score = " .. Pscore)
        set.passed = true
    end

    -- When pipe resets behind screen, allow scoring again
    if pipeX > birdX then
        set.passed = false
    end
end


    --Coin respawn logic
    for _, c in ipairs(coins) do
    local p = Fysics.get_pos(c)
    local px = Mafs.get_vec_x(p)

    --If the coin is offscreen
    if px < 0 then  

        --New positions for the coins
        local newX = 10 + math.random(4, 10)
        
        --random Y positions
        local newY = math.random(1, 4)

        Fysics.set_pos(c, newX, newY)

        --unhide the coin sprite
        local s = Entity.get_sprite_component(c)
        if s then
            Sprite.set_width(s, 16)
            Sprite.set_height(s, 16)
        end

        coinHidden[c] = false
    end
end
    
    ------------------------------------------------------
	-- UI
	------------------------------------------------------
end

    function ExampleScript:OnTriggerEnter(collision1, collision2)
        -- Initialize coinHidden table if it doesn't exist
        coinHidden = coinHidden or {}
        
        -- Check if either parameter is the player and the other is a coin
        local player, coin
        if collision1 == player1 then
            player = collision1
            coin = collision2
        elseif collision2 == player1 then
            player = collision2
            coin = collision1
        end
        
        -- If we found a player-coin collision
        if player and coin and coins then
            -- Verify the coin is in our coins table and not already collected
            for i, c in ipairs(coins) do
                if coin == c and (coinHidden[c] == nil or coinHidden[c] == false) then
                    print("Coin collected!")
                    
                    -- Hide the coin's sprite
                    local s = Entity.get_sprite_component(coin)
                    if s then
                        Sprite.set_width(s, 0)
                        Sprite.set_height(s, 0)
                    end
                    
                    -- Mark the coin as collected
                    coinHidden[coin] = true
                    
                    -- Update score
                    score = score + 10  -- 10 points per coin
                    scoreText = "Coins: " .. tostring(score)
                    print(scoreText)  -- Debug output
                    
                    break
                end
            end
        end
    end

    ------------------------------------------------------
	-- Game Over
	------------------------------------------------------
    local function triggerGameOver()
    gameOver = true

    --Stop all pipe
    Fysics.set_linear_velocity(pipe, 0, 0)
    Fysics.set_linear_velocity(pipeT, 0, 0)
    Fysics.set_linear_velocity(pipe2, 0, 0)
    Fysics.set_linear_velocity(pipeT2, 0, 0)
    Fysics.set_linear_velocity(pipe3, 0, 0)
    Fysics.set_linear_velocity(pipeT3, 0, 0)

    --Stop all coins & hide coins
    for _, c in ipairs(coins) do
        Fysics.set_linear_velocity(c, 0, 0)
        local s = Entity.get_sprite_component(c)
        if s then Sprite.set_width(s, 0); Sprite.set_height(s, 0) end
    end

    text1 = "GAME OVER!! TRY AGAIN"
    finalScoreText = "Final Score: " .. tostring(Pscore)
    local finalScoreText = "Final Score: 0"
    local coinsCollected = ""
    local text2 = ""
end
    ------------------------------------------------------
	--Collision
	------------------------------------------------------
function ExampleScript:OnCollisionEnter(a, b)
    if gameOver then return end

    --If player touches any pipe then game over
    if (a == player1 and (b == pipe or b == pipeT or b == pipe2 or b == pipeT2 or b == pipe3 or b == pipeT3))
    or (b == player1 and (a == pipe or a == pipeT or a == pipe2 or a == pipeT2 or a == pipe3 or a == pipeT3)) then
        print("GAME OVER: Hit pipe! Try Again")
        triggerGameOver()
    end
end

return ExampleScript

