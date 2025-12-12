local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local Background
local player1
local x = 200
local gameOver = false
local highscore = Json.load_high_score()

-- Pipes
local pipe, pipeT, pipe2, pipeT2, pipe3, pipeT3
local pipeSets = {}
local pipeSpeed = -3
local pipeOffScreenLeft = -100 

-- Coins
local coins = {}
local coinSpeed = -3
local coinHidden = {}
local score = 0
local Pscore = 0

-- Text
local pipeScoreText = "Score: 0"
local scoreText = "Coins: 0"
local text1 = "Press SPACE to start!"
local text2 = "Press SPACE to restart"
local finalScoreText = "Final Score: 0"
local coinsText = "Coins Collected: "
local topScore = "Highscore: "
local gameOver = false

-- Window
Window.set_size_centered(960, 640)

----------------------------------------------------------
-- Spawn coins between pipes
----------------------------------------------------------
local function spawnCoins(coin, pipeSet, offsetX)
    local bottomPos = Fysics.get_pos(pipeSet.bottom)
    local topPos = Fysics.get_pos(pipeSet.top)

    local pipeHeight = 3.5
    local gapTop = Mafs.get_vec_y(topPos) + pipeHeight
    local gapBottom = Mafs.get_vec_y(bottomPos)

    local randY = math.random() * (gapBottom - gapTop - 1) + gapTop + 0.1
    local yRandomOffset = (math.random() * 2 - 1) * 0.8
    randY = randY + yRandomOffset

    local randX = Mafs.get_vec_x(bottomPos) + (offsetX / 100)
    Fysics.set_pos(coin, randX, randY)
end

----------------------------------------------------------
-- Reset Game
----------------------------------------------------------
local function resetGame()
    -- Reset game state
    gameOver = false
    score = 0
    Pscore = 0
    pipeScoreText = "Score: 0"
    scoreText = "Coins: 0"
    text1 = "Press SPACE to start!"
    finalScoreText = "Final Score: 0"
    coinsText = "Coins Collected: "
    topScore = "Highscore: "
    highscore = Json.load_high_score()

    -- Reset player
    Fysics.set_pos(player1, 2, 3)
    Fysics.set_gravity_scale(player1, 0)
    Fysics.set_linear_velocity(player1, 0, 0)

    -- Reset pipes
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

    -- Reset coins
    for i, c in ipairs(coins) do
        local pipeSet = pipeSets[i]
        spawnCoins(c, pipeSet, 140)
        local s = Entity.get_sprite_component(c)
        if s then
            Sprite.set_width(s, 16)
            Sprite.set_height(s, 16)
        end
        coinHidden[c] = false
        Fysics.set_linear_velocity(c, 0, 0)
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
    Fysics.add_sprite_collider(player1,false, 0.5)

    -- Freeze bird
    Fysics.set_gravity_scale(player1, 0)

	------------------------------------------------------
	-- Pick texture for left / middle / right
	------------------------------------------------------
    local tileSize = 64
    local floorY = 600
    
	local tex = "middle"

	for i = 0, 30 do
		local tile = Entity.create_entity()
		local xPos = i * tileSize

		------------------------------------------------------
		-- Place sprite
		------------------------------------------------------
		Entity.set_global_pos(tile, xPos, floorY)
		local s = Entity.add_sprite_component(tile, assets.textures.Sand, tileSize, tileSize, 1)
        Sprite.set_columns(s,1)

		------------------------------------------------------
		-- Add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)  -- static
		Fysics.add_sprite_collider(tile, false,1)
	end

	------------------------------------------------------
	-- Create pipe obstacle
	------------------------------------------------------
    local function createPipeSet(bottomX, bottomY, topX, topY)
        local bottomPipe = Entity.create_entity()
        Entity.set_global_pos(bottomPipe, bottomX, bottomY)
        Entity.add_sprite_component(bottomPipe, assets.textures.FlappyPipe, 60, 300, 0)
        Entity.add_fysics_component(bottomPipe, enums.bodytype.kinematicBody, false)
        Fysics.add_sprite_collider(bottomPipe, false, 1)

        local topPipe = Entity.create_entity()
        Entity.set_global_pos(topPipe, topX, topY)
        Entity.add_sprite_component(topPipe, assets.textures.FlappyPipe2, 60, 300, 0)
        Entity.add_fysics_component(topPipe, enums.bodytype.kinematicBody, false)
        Fysics.add_sprite_collider(topPipe, false, 1)
        return bottomPipe, topPipe
    end

    pipe, pipeT = createPipeSet(640, 430, 640, 0)
    pipe2, pipeT2 = createPipeSet(940, 400, 940, -40)
    pipe3, pipeT3 = createPipeSet(1240, 360, 1240, -40)

    pipeSets = {
    { bottom = pipe,  top = pipeT,  passed = false },
    { bottom = pipe2, top = pipeT2, passed = false },
    { bottom = pipe3, top = pipeT3, passed = false }
    }

	------------------------------------------------------
	-- Create coins
	------------------------------------------------------
    for i, pipeSet in ipairs(pipeSets) do
        local c = Entity.create_entity()
        local coinSprite = Entity.add_sprite_component(c, assets.textures.Coin, 32, 32, 0)
        Sprite.set_rows(coinSprite, 1)
        Sprite.set_columns(coinSprite, 5)
        Sprite.set_width(coinSprite, 16)
        Sprite.set_height(coinSprite, 16)
        Entity.add_fysics_component(c, enums.bodytype.kinematicBody, false)
        Fysics.add_sprite_collider(c, true, 1)

        table.insert(coins, c)
        coinHidden[c] = false
        spawnCoins(c, pipeSet, 140)

        -- Associate coin with pipe sets
        pipeSet.coin = c
    end
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    ------------------
	-- Score
	------------------
    Window.set_size_centered(960, 640)
    local windowW = Window.get_width()
    local windowH = Window.get_height()

    ------------------
	-- Score
	------------------

    if gameOver then
        --Score and Game Over text
        UI.Add_label(320, 215, 1000, 1000, text1)

        --Display final score
        UI.Add_label(365, 240, 1000, 1000, finalScoreText)

        --Display coins collected
        UI.Add_label(335, 265, 1000, 1000, coinsText)

        --Display retry text
        UI.Add_label(320, 290, 1000, 1000, text2)

        --Display Highscore
        UI.Add_label(100, windowH/2, 1000, 1000, topScore)
        
        if Input.get_key_down(Keys.ionix_space) then
            resetGame()
        end
        return
    end

    -- Player input
    local vel = Fysics.get_linear_velocity(player1)
    local vx, vy = 0, Mafs.get_vec_y(vel)

    if Input.get_key_down(Keys.ionix_space) then
        Fysics.set_gravity_scale(player1, 0.75)
        vy = -3
        for _, p in ipairs({pipe, pipeT, pipe2, pipeT2, pipe3, pipeT3}) do
            Fysics.set_linear_velocity(p, pipeSpeed, 0)
        end
        for _, c in ipairs(coins) do
            Fysics.set_linear_velocity(c, coinSpeed, 0)
        end
        text1 = ""
    end
    Fysics.set_linear_velocity(player1, vx, vy)

    -- Pipe & coin reset
    for _, set in ipairs(pipeSets) do
        local pipeX = Mafs.get_vec_x(Fysics.get_pos(set.bottom))
        if pipeX < 0 then

            -- Reset pipes
            Fysics.set_pos(set.bottom, 10, 4 + (math.random(2, 3) / 10 * (math.random(1, 2) == 1 and -1 or 1)))
            Fysics.set_pos(set.top, 10, -0.5 + (math.random(2, 3) / 10 * (math.random(1, 2) == 1 and -1 or 1)))

            -- Reset coin
            if set.coin then
                spawnCoins(set.coin, set, 140)
                local s = Entity.get_sprite_component(set.coin)
                if s then
                    Sprite.set_width(s, 16)
                    Sprite.set_height(s, 16)
                end
                coinHidden[set.coin] = false
                Fysics.set_linear_velocity(set.coin, coinSpeed, 0)
            end
        end
    end

    -- Pipe passing logic
    local birdX = Mafs.get_vec_x(Fysics.get_pos(player1))
    for _, set in ipairs(pipeSets) do
        local pipeX = Mafs.get_vec_x(Fysics.get_pos(set.bottom))
        if birdX > pipeX and not set.passed then
            Pscore = Pscore + 1
            pipeScoreText = "Score: " .. tostring(Pscore)
            set.passed = true
        end
        if pipeX > birdX then
            set.passed = false
        end
    end

    -- UI
    UI.Add_label(20, 20, 1000, 1000, pipeScoreText)
    UI.Add_label(20, 50, 1000, 1000, scoreText)
end

----------------------------------------------------------
-- Coins collision + score
----------------------------------------------------------
function ExampleScript:OnTriggerEnter(a, b)
    local player, coin
    if a == player1 then player, coin = a, b
    elseif b == player1 then player, coin = b, a end
    if not coin then return end

    for _, c in ipairs(coins) do
        if c == coin and not coinHidden[c] then
            local s = Entity.get_sprite_component(c)
            if s then
                Sprite.set_width(s, 0)
                Sprite.set_height(s, 0)
            end
            coinHidden[c] = true
            score = score + 10
            scoreText = "Coins: " .. tostring(score)
            break
        end
    end
end

    ------------------------------------------------------
	-- Game Over
	------------------------------------------------------
    local function triggerGameOver()
    gameOver = true
    for _, p in ipairs({pipe, pipeT, pipe2, pipeT2, pipe3, pipeT3}) do
        Fysics.set_linear_velocity(p, 0, 0)
    end
    for _, c in ipairs(coins) do
        Fysics.set_linear_velocity(c, 0, 0)
        local s = Entity.get_sprite_component(c)
        if s then Sprite.set_width(s, 0); Sprite.set_height(s, 0) end
    end
    
    if Pscore > highscore then 
        highscore = Pscore 
        Json.save_high_score(highscore)
    end

    text1 = "GAME OVER!! TRY AGAIN"
    finalScoreText = "Final Score: " .. tostring(Pscore)
    topScore = "Highscore: " .. tostring(highscore)
    coinsText = "Coins Collected: " .. tostring(score)
    local finalScoreText = "Final Score: 0"
    local coinsText = ""
    local text2 = ""
    local topScore = "Highscore: "
    
end
    ------------------------------------------------------
	-- Collision
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

