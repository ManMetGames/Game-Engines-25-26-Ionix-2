local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local Background
local player1
local x = 100
local gameOver = false
local GAME_ID = "flappy_bird"
local highscore = Json.load_high_score(GAME_ID)
local newHighScore = false
local submitted = false -- For Highscore submission
local playerName = Json.load_player_name()
if playerName == "" then playerName = "Anon" end
local showSettings = showSettings or false

-- Pipes
local pipe, pipeT, pipe2, pipeT2, pipe3, pipeT3
local pipeSets = {}
local pipeSpeed = -3
local pipeOffScreenLeft = -100 

-- Pipe randomness
local pipeHeight = 3.0      
local pipeStartGap = 2.2
local pipeMinGap = 3.3 
local pipeShrinkGap = 0.03

local pipesetMinGap = 1
local pipesetMaxGap = 3.5

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

-- Audio
local birdJumpSound
local coinSound
local hitSound
local gameOverSound

-- Leaderboard
local topLeaderboard = nil
local leaderboardFetched = false

-- For delaying restart
local RESTART_DELAY_FRAMES = 90   -- equivalent 1s at 60fps
local restartDelayFrames = 0

--[[
-- Settings test
local settings_loaded = false
local s_difficulty = 1
local s_skin = 0
local s_tint = {1,1,1,1}
local s_music = true
local s_volume = 0.75
]]

-- Window
Window.set_size_centered(960, 640)

----------------------------------------------------------
-- Main Menu
----------------------------------------------------------
local inMainMenu = true
local showMenuButton = true

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
    restartDelayFrames = 0
    gameOver = false
    score = 0
    Pscore = 0
    pipeScoreText = "Score: 0"
    scoreText = "Coins: 0"
    text1 = "Press SPACE to start!"
    finalScoreText = "Final Score: 0"
    coinsText = "Coins Collected: "
    topScore = "Highscore: "
    highscore = Json.load_high_score(GAME_ID)

    -- Reset player
    Fysics.set_pos(player1, 1, 3)
    Fysics.set_gravity_scale(player1, 0)
    Fysics.set_linear_velocity(player1, 0, 0)

    -- Reset pipes
    local function resetPipe(pipeEntity, xPos, yPos)
        Fysics.set_pos(pipeEntity, xPos / 100, yPos / 100)
        Fysics.set_linear_velocity(pipeEntity, 0, 0)
    end

    resetPipe(pipe, 400, 360)
    resetPipe(pipeT, 400, -250)
    resetPipe(pipe2, 750, 300)
    resetPipe(pipeT2, 750, -350)
    resetPipe(pipe3, 1100, 400)
    resetPipe(pipeT3, 1100, -200)

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
    playerName = Json.load_player_name()
    if playerName == "" then playerName = "Anon" end
    topLeaderboard = nil
    leaderboardFetched = false
    UI.clear_input("player_name")
end

------------------------------------------------------
-- Game Over
------------------------------------------------------
    local function triggerGameOver()
    gameOver = true
    restartDelayFrames = RESTART_DELAY_FRAMES

    if gameOverSound then
    AudioComponent.play(gameOverSound)
    print("GameOver sound played")
    end

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
        Json.save_high_score(GAME_ID, highscore)
    end

    text1 = "GAME OVER!! TRY AGAIN"
    finalScoreText = "Final Score: " .. tostring(Pscore)
    topScore = "Highscore: " .. tostring(highscore)
    coinsText = "Coins Collected: " .. tostring(score)
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
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 64, 64, 10)
    Sprite.set_columns(playerSprite1,1)

    -- PLAYER 1 PHYSICS
    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true) -- dynamic body
    Fysics.add_sprite_collider(player1,false, 0.4)

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
        Entity.add_sprite_component(bottomPipe, assets.textures.FlappyPipe, 60, 500, 0)
        Entity.add_fysics_component(bottomPipe, enums.bodytype.kinematicBody, false)
        Fysics.add_sprite_collider(bottomPipe, false, 1)

        local topPipe = Entity.create_entity()
        Entity.set_global_pos(topPipe, topX, topY)
        Entity.add_sprite_component(topPipe, assets.textures.FlappyPipe2, 60, 500, 0)
        Entity.add_fysics_component(topPipe, enums.bodytype.kinematicBody, false)
        Fysics.add_sprite_collider(topPipe, false, 1)
        return bottomPipe, topPipe
    end

    pipe, pipeT = createPipeSet(400, 360, 400, -250)
    pipe2, pipeT2 = createPipeSet(750, 300, 750, -350)
    pipe3, pipeT3 = createPipeSet(1100, 400, 1100, -200)
    
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

    -- Collision SFX
    hitSound = Entity.create_entity()
    Entity.add_audio_component(hitSound, "Hit", false)
    AudioComponent.change_volume(hitSound, 50)

    -- Coin SFX
    coinSound = Entity.create_entity()
    Entity.add_audio_component(coinSound, "Coin", false)
    AudioComponent.change_volume(coinSound, 100)

    -- Game over SFX
    gameOverSound = Entity.create_entity()
    Entity.add_audio_component(gameOverSound, "gameOver", false)
    AudioComponent.change_volume(gameOverSound, 30)

    -- Freeze everything on main menu
    Fysics.set_gravity_scale(player1, 0)
    Fysics.set_linear_velocity(player1, 0, 0)

    for _, p in ipairs({pipe, pipeT, pipe2, pipeT2, pipe3, pipeT3}) do
        Fysics.set_linear_velocity(p, 0, 0)
    end

    for _, c in ipairs(coins) do
        Fysics.set_linear_velocity(c, 0, 0)
    end
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()

    ----------------------------------------------------------
    -- Main Menu 
    ----------------------------------------------------------

    if inMainMenu then
        local windowW = Window.get_width()
        local windowH = Window.get_height()

        -- Center button
        local buttonW, buttonH = 200, 50
        local centerX = (windowW - buttonW) / 2
        local centerY = (windowH - buttonH) / 2
        local gap = 20

        -- Play button
        UI.add_button(centerX, centerY - (buttonH / 2) - gap, buttonW, buttonH, "Play", "playButton")

        -- Exit button
        UI.add_button(centerX, centerY + (buttonH / 2) - gap + 20, buttonW, buttonH, "Exit", "exitButton")

        -- Start game
        if UI.was_button_pressed("playButton") then
            inMainMenu = false
            resetGame()
            print("Pressed Play Button: Started Game")
        end

        -- Exit game
        if UI.was_button_pressed("exitButton") then
            print("Pressed Exit Buttom: Quitting Game")
            os.exit()
        end

        return
    end

    -----------------------------------------
    -- Open main menu button in play mode
    -----------------------------------------
    if not inMainMenu then
        local windowW = Window.get_width()

        local btnSize = 36
        local margin = 10
        local btnX = windowW - btnSize - margin
        local btnY = margin

        UI.add_button(btnX, btnY, btnSize, btnSize, "=", "menuButton")

        -- Open main menu
        if UI.was_button_pressed("menuButton") then
            inMainMenu = true   
        end
    end

    ------------------------
	-- Settings test load
	------------------------

    if not settings_loaded then
      s_difficulty = Json.load_setting(GAME_ID, "ui.difficulty", 1)
      s_skin       = Json.load_setting(GAME_ID, "ui.skin_index", 0)
      s_tint       = Json.load_setting(GAME_ID, "ui.tint", {1,1,1,1})
      s_music      = Json.load_setting(GAME_ID, "audio.music", true)
      s_volume     = Json.load_setting(GAME_ID, "audio.volume", 0.75)
      settings_loaded = true
    end

    ------------------
	-- Window
	------------------
    Window.set_size_centered(960, 640)
    local windowW = Window.get_width()
    local windowH = Window.get_height()

    -----------------------------------
    -- Out of bounds check (Game Over)
    -----------------------------------
    if not gameOver then
        local pos = Entity.get_global_pos(player1)
        local birdY = Mafs.get_vec_y(pos)

        local topLimit = -23

        -- Game Over if flappybird is outside of window size
        if birdY < topLimit then
            triggerGameOver()
            return
        end
    end

    -- ===================================================================================================== 
	-- User Interface - button/checkbox/sliderFloat/radio button/dropdown/colour picker/child panel examples
	-- ===================================================================================================== 

    ------------------
	-- Button
	------------------
    --UI.add_button(20, 20, 120, 35, "Restart", "restart_btn")
     --if UI.was_button_pressed("restart_btn") then
     --    resetGame()
     --end

    --UI.add_button(100, 50, 160, 35, "Retry", "retry_btn", "ImGuiDefaultBold", 1.0, 12, true, 70, 130, 180, 1)
    ------------------
	-- Checkbox
	------------------
     --UI.add_checkbox(20, 60, 0, 0, "Music", "music_chk", true)

     --if UI.get_checkbox("music_chk") then
     --   -- music on
     --end

     --if UI.was_checkbox_changed("music_chk") then
     --    print("toggled to:", UI.get_checkbox("music_chk"))
     --end

    ------------------
	-- Slider
	------------------
    --UI.add_slider(20, 80, 110, "Volume", "volume", 0.0, 1.0, 0.75)

    --local v = UI.get_slider("volume")
    --if UI.was_slider_changed("volume") then
    --    print("volume now:", v)
    --end

    ------------------
	-- Radio toggle
	------------------
    -- Three options in the same group "difficulty"
    --UI.add_radio(20, 120, 0, 0, "Easy",   "difficulty", 0, 1, false) -- defaultValue = 1
    --UI.add_radio(20, 140, 0, 0, "Normal", "difficulty", 1, 1, false)
    --UI.add_radio(20, 160, 0, 0, "Hard",   "difficulty", 2, 1, false)

    --local diff = UI.get_radio("difficulty")

    --if UI.was_radio_changed("difficulty") then
    --  print("difficulty now:", diff)
    --end

    ------------------
	-- Dropdown
	------------------
    --UI.add_dropdown(
    --  20, 200, 220, 0,
    --  "Bird Skin",
    --  "bird_skin_dd",
    --  { "Classic", "Blue", "Red", "Gold" },
    --  0 -- defaultIndex
    --)

    --local idx = UI.get_dropdown_index("bird_skin_dd")

    --if UI.was_dropdown_changed("bird_skin_dd") then
    --  print("dropdown index:", idx)
    --end

    ---- *string* option in Lua:
    --local options = { "Classic", "Blue", "Red", "Gold" }
    --local selected = options[idx + 1]  -- Lua is 1-based, index is 0-based

    ------------------
	-- Colour picker
	------------------
    --UI.add_color_picker(20, 250, 0, 0, "Tint", "bird_tint",
    --  1.0, 1.0, 1.0, 1.0   -- r,g,b,a default
    --)
    --local c = UI.get_color("bird_tint")  -- table: { [1]=r, [2]=g, [3]=b, [4]=a }

    --if UI.was_color_changed("bird_tint") then
    --  print("color:", c[1], c[2], c[3], c[4])
    --end

    ------------------
	-- Child panel
	------------------

    --UI.add_button(20, 20, 120, 35, "Restart", "restart_btn")
     --if UI.was_button_pressed("restart_btn") then
     --    resetGame()
     --end

    ---- A child region positioned at (20, 100) sized 300x260 with a coloured background
    --UI.begin_child(20, 95, 320, 300, "FB_Settings", true, 0, true, 0.75, 10, 70, 160, 115) -- hasBg alpha rounding r  g   b

    --UI.add_label(10, 10, 0, 0, "Settings", "ImGuiDefaultBold", 1.3)
    --UI.add_radio(10, 40, 0, 0, "Easy", "difficulty", 0, 1, false)
    --UI.add_radio(10, 60, 0, 0, "Normal", "difficulty", 1, 1, false)
    --UI.add_radio(10, 80, 0, 0, "Hard", "difficulty", 2, 1, false)

    --UI.add_dropdown(10, 120, 240, 0, "Bird Skin", "bird_skin_dd",
    --  { "Classic", "Blue", "Red", "Gold" }, 0)

    --UI.add_color_picker(10, 160, 0, 0, "Tint", "bird_tint", 1, 1, 1, 1)

    --UI.end_child()

    ------------------
	-- Example Settings Panel showcase with button to toggle
    --(Have to uncomment the top 'settings test' block too)
	------------------
   --[[
    UI.add_button(150, 20, 160, 35, showSettings and "Hide Settings" or "Show Settings", "settings_btn", "ImGuiDefaultBold", 1.0, 12, true, 95, 150, 165, 0.75)
    if UI.was_button_pressed("settings_btn") then
      showSettings = not showSettings
    end

    if showSettings then
      UI.begin_child(20, 70, 320, 300, "SettingsChild", true, 0,
                     true, 0.75, 5, 95, 150, 165)

      UI.add_label(10, 10, 0, 0, "Settings", "ImGuiDefaultBold", 1.3)

      -- Radio: difficulty 
      UI.add_radio(10, 45, 0, 0, "Easy",   "difficulty", 0, s_difficulty, false)
      UI.add_radio(10, 65, 0, 0, "Normal", "difficulty", 1, s_difficulty, false)
      UI.add_radio(10, 85, 0, 0, "Hard",   "difficulty", 2, s_difficulty, false)

      -- Dropdown: skin 
      UI.add_dropdown(10, 115, 240, 0, "Bird Skin", "bird_skin_dd",
                      { "Classic", "Blue", "Red", "Gold" }, s_skin)

      -- Color picker: tint 
      UI.add_color_picker(10, 150, 0, 0, "Tint", "bird_tint",
                          s_tint[1], s_tint[2], s_tint[3], s_tint[4])

      -- Checkbox + slider examples 
      UI.add_checkbox(10, 220, 0, 0, "Music", "music_chk", s_music)
      UI.add_slider(10, 245, 200, "Volume", "volume", 0.0, 1.0, s_volume)

      -- Save on change
      if UI.was_radio_changed("difficulty") then
        local v = UI.get_radio("difficulty")
        Json.save_setting(GAME_ID, "ui.difficulty", v)
        s_difficulty = v
      end

      if UI.was_dropdown_changed("bird_skin_dd") then
        local idx = UI.get_dropdown_index("bird_skin_dd")
        Json.save_setting(GAME_ID, "ui.skin_index", idx)
        s_skin = idx
      end

      if UI.was_color_changed("bird_tint") then
        local c = UI.get_color("bird_tint")
        Json.save_setting(GAME_ID, "ui.tint", c)
        s_tint = c
      end

      if UI.was_checkbox_changed("music_chk") then
        local m = UI.get_checkbox("music_chk")
        Json.save_setting(GAME_ID, "audio.music", m)
        s_music = m
      end

      if UI.was_slider_changed("volume") then
        local vol = UI.get_slider("volume")
        Json.save_setting(GAME_ID, "audio.volume", vol)
        s_volume = vol
      end

      UI.end_child()
    end
    ]]

    -- =====================================================================================================
    -- End of UI examples
    -- =====================================================================================================

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
            topLeaderboard = Firebase.retrieve_high_score(GAME_ID, 5)
            leaderboardFetched = true
        end

        -- Leaderboard (top-left) --
        local lbX, lbY = 5, 5
        local lbW, lbH = 260, 170
        UI.add_panel(lbX, lbY, lbW, lbH, 0.75, 10, 70, 160, 115)

        UI.add_label(lbX + 10, lbY + 10, 0, 0, "Leaderboard Ranking", "ImGuiDefaultBold", 1.8)

        if topLeaderboard then
          for i, e in ipairs(topLeaderboard) do
            local line = string.format("%d. %s - %d", i, e.name, e.score)
            UI.add_label(lbX + 10, lbY + 40 + (i-1)*22, 0, 0, line,  "", 1.5)
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

        UI.add_centered_label(centerX, y0 + gap*-0.25, text1,  "ImGuiDefaultBold", 1.8) 
        UI.add_centered_label(centerX, y0 + gap*1.5, topScore, "", 1.5)
        UI.add_centered_label(centerX, y0 + gap*2.5, finalScoreText, "", 1.5)
        UI.add_centered_label(centerX, y0 + gap*3.5, coinsText, "", 1.5)
        UI.add_centered_label(centerX, y0 + gap*5.5, text2, "ImGuiDefaultBold", 1.8)


        -- Show TextInput only if new high score
        if newHighScore and not submitted then
            local nhW, nhH = 520, 85
            local nhX = (windowW - nhW) / 2
            local nhY = panelY + panelH + 12   -- 12px gap under the main panel

            UI.add_panel(nhX, nhY, nhW, nhH+15, 0.75, 10, 70, 160, 115)

            local centerX = nhX + (nhW / 2)

            -- Title centered (uses centered label function)
            UI.add_centered_label(centerX, nhY + 10, "New Highscore! Enter your name", "", 1.5)
            UI.add_centered_label(centerX, nhY + 35, "to be added to the leaderboard:", "", 1.5)

            -- Input box (put it centered-ish under the title)
            local inputW = 260
            local inputX = nhX + (nhW - inputW) / 2
            local inputY = nhY + 45

            UI.add_input_text(inputX, inputY, inputW, "", "player_name", 16)

            if UI.was_input_committed("player_name") then
                local name = UI.get_input_text("player_name")
                if name == "" then name = "Anon" end

                -- persist name locally
                playerName = name
                Json.save_player_name(playerName)

                -- submit to the correct game's leaderboard
                Firebase.submit_high_score(GAME_ID, playerName, highscore)

                submitted = true
                leaderboardFetched = false
            end
        end

        -- Count down once per frame (never below 0)
        restartDelayFrames = math.max(0, restartDelayFrames - 1)

        -- Only allow restart when delay is done AND name entry isn't active
        local canRestart = (restartDelayFrames == 0) and not (newHighScore and not submitted)

        text2 = canRestart and "Press SPACE to restart" or ""


        if canRestart and Input.get_key_down(Keys.ionix_space) then
            resetGame()
        end


        return
    end

    -- Player input
    local vel = Fysics.get_linear_velocity(player1)
    local vx, vy = 0, Mafs.get_vec_y(vel)

    if not inMainMenu and Input.get_key_down(Keys.ionix_space) then
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

    ------------------------------------
    -- Pipe reset w randomness 
    ------------------------------------
    for _, set in ipairs(pipeSets) do
        local pipeX = Mafs.get_vec_x(Fysics.get_pos(set.bottom))

        if pipeX < -0.6 then

            -- Added difficulty: shrink pipe gap as score increases
            local gapSize = pipeStartGap - (Pscore * pipeShrinkGap)
            if gapSize < pipeMinGap then
                gapSize = pipeMinGap
            end

            -- Random gap
            local gapCenter = pipesetMinGap +
                math.random() * (pipesetMaxGap - pipesetMinGap)

            if set.lastGapCenter then
                local delta = gapCenter - set.lastGapCenter
                if math.abs(delta) < 0.4 then
                    gapCenter = gapCenter + (delta > 0 and 0.6 or -0.6)
                end
            end
            set.lastGapCenter = gapCenter

            -- Calculate pipe positions
            local bottomY = gapCenter + (gapSize / 2)
            local topY = gapCenter - (gapSize / 2) - pipeHeight

            local spawnX = (windowW + 60) / 100

            -- Apply positions
            Fysics.set_pos(set.bottom, spawnX, bottomY)
            Fysics.set_pos(set.top, spawnX, topY)

            set.passed = false
        end
    end

    -- Coin reset
    for i, c in ipairs(coins) do
        local coinX = Mafs.get_vec_x(Fysics.get_pos(c))

        if coinX < -0.6 then
            local pipeSet = pipeSets[i]

            spawnCoins(c, pipeSet, 140)

            local s = Entity.get_sprite_component(c)
            if s then
                Sprite.set_width(s, 16)
                Sprite.set_height(s, 16)
            end

            coinHidden[c] = false
            Fysics.set_linear_velocity(c, coinSpeed, 0)
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
    UI.add_label(10, 10, 1000, 1000, pipeScoreText, "", 1.5)
    UI.add_label(10, 40, 1000, 1000, scoreText, "", 1.5)
    UI.add_label(350, 290, 1000, 1000, text1, "", 2)
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
-- Collision
------------------------------------------------------
function ExampleScript:OnCollisionEnter(a, b)
    if gameOver then return end

    local other = nil
    if a == player1 then other = b
    elseif b == player1 then other = a end
    if not other then return end

    
    -- If player touches any pipe then game over
    if other == pipe or other == pipeT or other == pipe2 or other == pipeT2 or other == pipe3 or other == pipeT3 then

        if hitSound then 
        AudioComponent.play(hitSound)
        print("Hit sfx played")
        end
        
        print("GAME OVER: Hit pipe! Try Again")
        triggerGameOver()
        return
    end

    -- Hit floor or anything that is not a coin then game over
    for _, c in ipairs(coins) do
        if other == c then
            return 
        end
    end

    if hitSound then 
    AudioComponent.play(hitSound) 
    end

    print("GAME OVER: Hit pipe! Try Again")
    triggerGameOver()
end

return ExampleScript

