local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local Background
local player1
local x = 100
local gameOver = false
local highscore = Json.load_high_score()
local newHighScore = false
local submitted = false -- For Highscore submission
local playerName = ""
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

-- Point effect
local pointEffect
local pointEffectTimer = 0
local pointEffectDuration = 40

local pointX = 0
local pointY = 0

-- Pipe point effect
local pipeEffect
local pipeEffectTimer = 0
local pipeEffectDuration = 40

-- Text
local pipeScoreText = "Score: 0"
local scoreText = "Coins: 0"
local text1 = "Press SPACE to start!"
local text2 = "Press SPACE to restart"
local finalScoreText = "Final Score: 0"
local coinsText = "Coins Collected: "
local topScore = "Highscore: "
local gameOver = false

-- Audio
local birdJumpSound
local coinSound

local topLeaderboard = nil
local leaderboardFetched = false

-- Window
Window.set_size_centered(960, 640)

----------------------------------------------------------
-- Show point effect
----------------------------------------------------------
local function showPointEffect()
    local birdPos = Entity.get_global_pos(player1)
    local birdX = Mafs.get_vec_x(birdPos)
    local birdY = Mafs.get_vec_y(birdPos)

    -- Offset
    pointX = birdX + 25
    pointY = birdY - 25

    -- Set position
    Entity.set_global_pos(pointEffect, pointX, pointY)

    local pointE = Entity.get_sprite_component(pointEffect)
    if pointE then
        Sprite.set_width(pointE, 32)
        Sprite.set_height(pointE, 32)
    end

    pointEffectTimer = pointEffectDuration
end

local function showPipeEffect()
    local birdPos = Entity.get_global_pos(player1)
    local bx = Mafs.get_vec_x(birdPos)
    local by = Mafs.get_vec_y(birdPos)

    Entity.set_global_pos(pipeEffect, bx + 25, by - 25)

    local pipeE = Entity.get_sprite_component(pipeEffect)
    if pipeE then
        Sprite.set_width(pipeE, 32)
        Sprite.set_height(pipeE, 32)
    end

    pipeEffectTimer = pipeEffectDuration
end

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
    Fysics.set_pos(player1, 1, 3)
    Fysics.set_gravity_scale(player1, 0)
    Fysics.set_linear_velocity(player1, 0, 0)

    -- Reset pipes
    local function resetPipe(pipeEntity, xPos, yPos)
        Fysics.set_pos(pipeEntity, xPos / 100, yPos / 100)
        Fysics.set_linear_velocity(pipeEntity, 0, 0)
    end

    resetPipe(pipe, 400, 400)
    resetPipe(pipeT, 400, 0)
    resetPipe(pipe2, 750, 400)
    resetPipe(pipeT2, 750, -40)
    resetPipe(pipe3, 1100, 360)
    resetPipe(pipeT3, 1100, -40)

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

    newHighScore = false
    submitted = false
    playerName = ""
    topLeaderboard = nil
    leaderboardFetched = false
    UI.clear_input("player_name")
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

    pipe, pipeT = createPipeSet(400, 430, 400, 0)
    pipe2, pipeT2 = createPipeSet(750, 400, 750, -40)
    pipe3, pipeT3 = createPipeSet(1100, 360, 1100, -40)
    
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

    ------------------------------------------------------
	-- Create point effect
	------------------------------------------------------
    pointEffect = Entity.create_entity()

	local point_Effect = Entity.add_sprite_component(pointEffect, assets.textures.PointEffect, 25, 25, 0)

    Sprite.set_columns(point_Effect,1)
    Sprite.set_width(point_Effect, 0)
    Sprite.set_height(point_Effect, 0)

    ------------------------------
    -- Create pipe point effect
    ------------------------------
    pipeEffect = Entity.create_entity()

    local pipe_Effect = Entity.add_sprite_component(pipeEffect, assets.textures.PipeEffect, 25, 25, 0)

    Sprite.set_columns(pipe_Effect, 1)
    Sprite.set_width(pipe_Effect, 0)
    Sprite.set_height(pipe_Effect, 0)

    ------------------------------------------------------
	-- Audio / Sound Effects
	------------------------------------------------------
    -- Bird Jump SFX
    birdJumpSound = Entity.create_entity()
    Entity.add_audio_component(birdJumpSound, "Jump", false)
    AudioComponent.change_volume(birdJumpSound, 100)

    -- Coin SFX
    coinSound = Entity.create_entity()
    Entity.add_audio_component(coinSound, "Jump", false)
    AudioComponent.change_volume(coinSound, 100)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    ------------------
	-- Window
	------------------
    Window.set_size_centered(960, 640)
    local windowW = Window.get_width()
    local windowH = Window.get_height()

    ------------------
	-- New button/checkbox test
	------------------
    -- UI.add_button(20, 20, 120, 35, "Restart", "restart_btn")
    -- if UI.was_button_pressed("restart_btn") then
    --     resetGame()
    -- end

    UI.add_checkbox(20, 60, 0, 0, "Music", "music_chk", true)

    if UI.get_checkbox("music_chk") then
        -- music on
    end

    if UI.was_checkbox_changed("music_chk") then
        print("toggled to:", UI.get_checkbox("music_chk"))
    end


    ------------------
	-- Point effect
	------------------
    -- Update point effect timer
    if pointEffectTimer > 0 then
        pointEffectTimer = pointEffectTimer - 1

        -- Make it go up slightly
        local pos = Entity.get_global_pos(pointEffect)
        local x = Mafs.get_vec_x(pos)
        local y = Mafs.get_vec_y(pos)
        Entity.set_global_pos(pointEffect, x, y - 1)

        -- Hide when timer ends
        if pointEffectTimer <= 0 then
            local pointE = Entity.get_sprite_component(pointEffect)
            if pointE then
                Sprite.set_width(pointE, 0)
                Sprite.set_height(pointE, 0)
            end
        end
    end

    -----------------------
    -- Pipe point effect
    -----------------------
    if pipeEffectTimer > 0 then
        pipeEffectTimer = pipeEffectTimer - 1

        local pos = Entity.get_global_pos(pipeEffect)
        local x = Mafs.get_vec_x(pos)
        local y = Mafs.get_vec_y(pos)
        Entity.set_global_pos(pipeEffect, x, y - 1)

        if pipeEffectTimer <= 0 then
            local pipeE = Entity.get_sprite_component(pipeEffect)
            if pipeE then
                Sprite.set_width(pipeE, 0)
                Sprite.set_height(pipeE, 0)
            end
        end
    end

    ------------------
	-- Score
	------------------

    if gameOver then
        -- Fetch leaderboard once --
        if not leaderboardFetched then
            topLeaderboard = Firebase.retrieve_high_score(5)
            leaderboardFetched = true
        end

        -- Leaderboard (top-left) --
        local lbX, lbY = 5, 5
        local lbW, lbH = 260, 170
        UI.add_panel(lbX, lbY, lbW, lbH, 0.75, 10, 70, 160, 115)

        UI.Add_label(lbX + 10, lbY + 10, 0, 0, "Leaderboard Ranking")

        if topLeaderboard then
          for i, e in ipairs(topLeaderboard) do
            local line = string.format("%d. %s - %d", i, e.name, e.score)
            UI.Add_label(lbX + 10, lbY + 35 + (i-1)*22, 0, 0, line)
          end
        end

        -- Center Game Over panel
        local panelW, panelH = 460, 205
        local panelX = (windowW - panelW) / 2
        local panelY = 200

        UI.add_panel(panelX, panelY, panelW, panelH, 0.75, 12, 95, 150, 165)

        local y0 = panelY + 20
        local gap = 26
        local centerX = panelX + (panelW / 2)

        UI.add_centered_label(centerX, y0 + gap*0, text1)
        UI.add_centered_label(centerX, y0 + gap*1, finalScoreText)
        UI.add_centered_label(centerX, y0 + gap*2, coinsText)
        UI.add_centered_label(centerX, y0 + gap*3, topScore)
        UI.add_centered_label(centerX, y0 + gap*5.5, text2)


        -- Show TextInput only if new high score
        if newHighScore and not submitted then
            local nhW, nhH = 520, 85
            local nhX = (windowW - nhW) / 2
            local nhY = panelY + panelH + 12   -- 12px gap under the main panel

            UI.add_panel(nhX, nhY, nhW, nhH+15, 0.75, 12, 95, 150, 165)

            local centerX = nhX + (nhW / 2)

            -- Title centered (uses centered label function)
            UI.add_centered_label(centerX, nhY + 10, "New Highscore! Enter your name")
            UI.add_centered_label(centerX, nhY + 35, "to be added to the leaderboard:")

            -- Input box (put it centered-ish under the title)
            local inputW = 260
            local inputX = nhX + (nhW - inputW) / 2
            local inputY = nhY + 45

            UI.add_input_text(inputX, inputY, inputW, "", "player_name", 16)

            if UI.was_input_committed("player_name") then
                local name = UI.get_input_text("player_name")
                if name == "" then name = "Anon" end
    
                Firebase.submit_high_score(name, highscore)
                submitted = true

                -- Refresh leaderboard next frame
                leaderboardFetched = false
            end
        end

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

        if birdJumpSound then
            AudioComponent.play(birdJumpSound)
            print("Bird jump sfx played")
        end

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
        if pipeX < -0.6 then

            -- Reset pipes
            Fysics.set_pos(set.bottom, (windowW+60)/100, 4 + (math.random(2, 3) / 10 * (math.random(1, 2) == 1 and -1 or 1)))
            Fysics.set_pos(set.top, (windowW+60)/100, -0.5 + (math.random(2, 3) / 10 * (math.random(1, 2) == 1 and -1 or 1)))

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

            -- Show pipe point effect
            showPipeEffect()
        end
        if pipeX > birdX then
            set.passed = false
        end
    end

    -- UI
    UI.Add_label(10, 10, 1000, 1000, pipeScoreText)
    UI.Add_label(10, 40, 1000, 1000, scoreText)

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
            score = score + 1
            scoreText = "Coins: " .. tostring(score)

            -- Output coin sfx
            if coinSound then
            AudioComponent.play(coinSound)
            print("Coin sfx played")
            end

            -- Show point effect
            showPointEffect()

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
    
    newHighScore = (Pscore > highscore)

    if newHighScore then
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

