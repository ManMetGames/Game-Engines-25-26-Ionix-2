local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local Background
local Background2
local backgroundSprite
local backgroundSprite2
local bgBaseX, bgBaseY = 0, 0
local bgScrollX = 0
local player1
local playerSprite
-- Background scrolling (main menu). Keep world authored at 960x640 ("virtual world").
local BG_BASE_W, BG_BASE_H = 960, 640
local BG_PAD = 20                -- oversize to hide seams/edges
local BG_TILE_W = BG_BASE_W      -- wrap distance
local BG_SCROLL_SPEED = -18      -- pixels/sec (menu only)
local x = 100
local gameOver = false
local GAME_ID = "flappy_bird"
local highscore = Json.load_high_score(GAME_ID)
local newHighScore = false
local submitted = false -- For Highscore submission
local function SanitiseName(name)
    name = tostring(name or "")
    name = name:gsub("^%s+", ""):gsub("%s+$", "")
    return name
end

local playerName = SanitiseName(Json.load_player_name())
local showSettings = showSettings or false

-- UI fonts (baked sizes; avoid scaling blur)
local UI_FONT_REG    = "ImGuiDefault"
local UI_FONT_SUB    = "ImGuiSub"
local UI_FONT_HEADER = "ImGuiHeader"
local UI_FONT_TITLE  = "ImGuiTitle"

-- String table scaffold (EN for now; ready for localisation later)
local STR = {
    ["fb.leaderboard"]           = "Leaderboard",
    ["fb.press_space_start"]     = "Press SPACE to start!",
    ["fb.press_space_restart"]   = "Press SPACE to restart",
    ["fb.game_over_try_again"]   = "GAME OVER!! TRY AGAIN",
    ["fb.new_highscore_line1"]   = "New Highscore! Enter your name",
    ["fb.new_highscore_line2"]   = "to be added to the leaderboard:",
}
local function T(key)
    return STR[key] or key
end



-- --------------------------------------------------------------------------------
-- Saved settings / economy (stored via Json settings)
-- --------------------------------------------------------------------------------
local function LoadSetting(key, default)
    if Json and Json.load_setting then
        local v = Json.load_setting(GAME_ID, key, default)
        if v == nil then return default end
        return v
    end
    return default
end

local function SaveSetting(key, value)
    if Json and Json.save_setting then
        Json.save_setting(GAME_ID, key, value)
    end
end

-- Audio settings (0..100)
local s_masterVol = LoadSetting("audio.masterVol", 100)
local s_sfxVol    = LoadSetting("audio.sfxVol", 100)
local s_musicVol  = LoadSetting("audio.musicVol", 70)
local s_musicOn   = LoadSetting("audio.musicOn", true)
local s_sfxOn     = LoadSetting("audio.sfxOn", true)
local musicEntity
local musicStarted = false
local BASE_VOL_MUSIC = 80

-- Currency (total coins owned across runs)
local bankCoins = LoadSetting("coins.total", 0)

-- Simple CSV helpers for owned items
local function CsvToSet(csv)
    local set = {}
    if type(csv) ~= "string" then return set end
    for token in string.gmatch(csv, "([^,]+)") do
        set[token] = true
    end
    return set
end

local function SetToCsv(set)
    local t = {}
    for k, v in pairs(set) do
        if v then table.insert(t, k) end
    end
    table.sort(t)
    return table.concat(t, ",")
end

-- Owned/equipped cosmetics (IDs). Defaults are always owned.
local ownedBackgrounds = CsvToSet(LoadSetting("owned.backgrounds", "bg_classic"))
local ownedBirds       = CsvToSet(LoadSetting("owned.birds", "bird_classic"))
local equippedBackground = LoadSetting("equipped.background", "bg_classic")
local equippedBird       = LoadSetting("equipped.bird", "bird_classic")

-- Customise screen tab
local customiseTab = "backgrounds" -- "backgrounds" or "birds"

-- Shop items (placeholder IDs; hook up textures/sprites later)
local STORE_BACKGROUNDS = {
    { id = "bg_classic", name = "Classic Sky", price = 0 },
    { id = "bg_sunset",  name = "Sunset Sky",  price = 50 },
    { id = "bg_night",   name = "Night Sky",   price = 120 },
}

local STORE_BIRDS = {
    { id = "bird_classic", name = "Classic Bird", price = 0 },
    { id = "bird_red",     name = "Red Bird",     price = 80 },
    { id = "bird_gold",    name = "Gold Bird",    price = 250 },
}

local function EnsureDefaultsOwned()
    if not ownedBackgrounds["bg_classic"] then ownedBackgrounds["bg_classic"] = true end
    if not ownedBirds["bird_classic"] then ownedBirds["bird_classic"] = true end
end
-- --------------------------------------------------------------------------------
-- Cosmetic styles (visual-only for now).
-- Later, when you add new textures, you can update these tables to point at them.
-- --------------------------------------------------------------------------------
local BG_STYLES = {
    bg_classic = { tint = {255, 255, 255} },
    bg_sunset  = { tint = {255, 210, 170} },
    bg_night   = { tint = {170, 190, 255} },
}

local BIRD_STYLES = {
    bird_classic = { tint = {255, 255, 255} },
    bird_red     = { tint = {255, 140, 140} },
    bird_gold    = { tint = {255, 225, 120} },
}

local function ApplyBackgroundStyle()
    local style = BG_STYLES[equippedBackground] or BG_STYLES["bg_classic"]
    local t = (style and style.tint) or {255, 255, 255}
    if backgroundSprite then Sprite.set_color(backgroundSprite, t[1], t[2], t[3]) end
    if backgroundSprite2 then Sprite.set_color(backgroundSprite2, t[1], t[2], t[3]) end
end

local function ApplyBirdStyle()
    local style = BIRD_STYLES[equippedBird] or BIRD_STYLES["bird_classic"]
    local t = (style and style.tint) or {255, 255, 255}
    if playerSprite then Sprite.set_color(playerSprite, t[1], t[2], t[3]) end
end

EnsureDefaultsOwned()

-- Pipes
local pipe, pipeT, pipe2, pipeT2, pipe3, pipeT3
local pipeSets = {}
local pipesList = {} 
local pipeSpeed = -3
local pipeOffScreenLeft = -100 

-- Pipe randomness
local pipeHeight = 3.0      
local pipeStartGap = 3.3   -- start easier
local pipeMinGap   = 2.1   -- end harder
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
local text1 = T("fb.press_space_start")
local text2 = T("fb.press_space_restart")
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


-- Base SFX volumes (0..100). Master/SFX settings scale these.
local BASE_VOL_JUMP    = 100
local BASE_VOL_HIT     = 50
local BASE_VOL_COIN    = 100
local BASE_VOL_GAMEOVER= 30

local function ApplyMusicVolume()
    if not musicEntity or not AudioComponent or not AudioComponent.change_volume then return end
    local master = (s_masterVol or 100) / 100.0
    local musicMul = (s_musicOn and (s_musicVol or 100) or 0) / 100.0
    local v = math.floor((BASE_VOL_MUSIC * master * musicMul) + 0.5)
    AudioComponent.change_volume(musicEntity, v)
end

local function UpdateMusicPlayback()
    if not musicEntity or not AudioComponent then return end

    if s_musicOn then
        if not musicStarted then
            AudioComponent.play(musicEntity, 0, -1) -- loop forever (like SystemShooter)
            musicStarted = true
        elseif AudioComponent.resume then
            AudioComponent.resume(musicEntity)
        end
    else
        if musicStarted and AudioComponent.pause then
            AudioComponent.pause(musicEntity)
        end
    end
end

local function ApplySfxVolumes()
    local master = (s_masterVol or 100) / 100.0
    local sfxMul = (s_sfxOn and (s_sfxVol or 100) or 0) / 100.0

    local function setVol(ent, base)
        if ent ~= nil and AudioComponent and AudioComponent.change_volume then
            local v = math.floor((base * master * sfxMul) + 0.5)
            AudioComponent.change_volume(ent, v)
        end
    end

    setVol(birdJumpSound, BASE_VOL_JUMP)
    setVol(hitSound,      BASE_VOL_HIT)
    setVol(coinSound,     BASE_VOL_COIN)
    setVol(gameOverSound, BASE_VOL_GAMEOVER)
end
----------------------------------------------------------
-- Main Menu
----------------------------------------------------------
local inMainMenu = true
local menuContext = "main"

----------------------------------
-- Pause game when in main menu
----------------------------------
-- ----------------------------------------------------------
-- UI scaling helpers (virtual UI space 960x640, letterboxed)
-- ----------------------------------------------------------
local UI_BASE_W, UI_BASE_H = 960, 640
local uiScale = 1.0
local uiOffX, uiOffY = 0, 0

local function UpdateUiScale(windowW, windowH)
    uiScale = math.min(windowW / UI_BASE_W, windowH / UI_BASE_H)
    uiOffX = (windowW - (UI_BASE_W * uiScale)) / 2
    uiOffY = (windowH - (UI_BASE_H * uiScale)) / 2
end

local function S(px)
    return math.floor((px * uiScale) + 0.5)
end

local function Ux(px)
    return math.floor((uiOffX + (px * uiScale)) + 0.5)
end

local function Uy(py)
    return math.floor((uiOffY + (py * uiScale)) + 0.5)
end

-- Settings menu back navigation (so pause -> settings -> back returns to pause)
local settingsBackContext = "main"

-- Cached motion during pause so Resume restores correctly
local pausedPlayerVel = nil
local pausedPlayerGravity = nil
local pausedPipeVels = {}
local pausedCoinVels = {}

local function pauseGame(pause)
    gamePaused = pause

    -- Safety: if the world/entities are not ready, just set the flag
    if player1 == nil then return end

    if pause then
        -- Cache current motion so Resume restores correctly
        pausedPlayerVel = Fysics.get_linear_velocity(player1)
        pausedPlayerGravity = Fysics.get_gravity_scale(player1)

        Fysics.set_gravity_scale(player1, 0)
        Fysics.set_linear_velocity(player1, 0, 0)

        pausedPipeVels = {}
        for i, p in ipairs(pipesList) do
            pausedPipeVels[i] = Fysics.get_linear_velocity(p)
            Fysics.set_linear_velocity(p, 0, 0)
        end

        pausedCoinVels = {}
        for i, c in ipairs(coins) do
            pausedCoinVels[i] = Fysics.get_linear_velocity(c)
            Fysics.set_linear_velocity(c, 0, 0)
        end
    else
        -- Restore cached motion
        if pausedPlayerGravity ~= nil then
            Fysics.set_gravity_scale(player1, pausedPlayerGravity)
        end
        if pausedPlayerVel ~= nil then
            Fysics.set_linear_velocity_v(player1, pausedPlayerVel)
        end

        for i, p in ipairs(pipesList) do
            local v = pausedPipeVels[i]
            if v ~= nil then
                Fysics.set_linear_velocity_v(p, v)
            end
        end

        for i, c in ipairs(coins) do
            local v = pausedCoinVels[i]
            if v ~= nil then
                Fysics.set_linear_velocity_v(c, v)
            end
        end
    end
end

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
    text1 = T("fb.press_space_start")
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
    playerName = SanitiseName(Json.load_player_name())
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

    for _, p in ipairs(pipesList) do
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

    text1 = T("fb.game_over_try_again")
    finalScoreText = "Final Score: " .. tostring(Pscore)
    topScore = "Highscore: " .. tostring(highscore)
    coinsText = "Coins Collected: " .. tostring(score)

    -- Bank run coins so cosmetics can be purchased from the main menu / customise screen
    bankCoins = (bankCoins or 0) + (score or 0)
    SaveSetting("coins.total", bankCoins)
end

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()------------------------------------------------------
-- Background Texture (two tiles for menu scrolling)
------------------------------------------------------
Background = Entity.create_entity()
Background2 = Entity.create_entity()

backgroundSprite = Entity.add_sprite_component(Background, assets.textures.Background, BG_BASE_W, BG_BASE_H, 0)
backgroundSprite2 = Entity.add_sprite_component(Background2, assets.textures.Background, BG_BASE_W, BG_BASE_H, 0)

-- Slightly oversize so scrolling doesn't show edges
if backgroundSprite then
    Sprite.set_width(backgroundSprite, BG_BASE_W + BG_PAD)
    Sprite.set_height(backgroundSprite, BG_BASE_H + BG_PAD)
end
if backgroundSprite2 then
    Sprite.set_width(backgroundSprite2, BG_BASE_W + BG_PAD)
    Sprite.set_height(backgroundSprite2, BG_BASE_H + BG_PAD)
end

bgBaseX, bgBaseY = -(BG_PAD / 2), -(BG_PAD / 2)
bgScrollX = 0

Entity.set_global_pos(Background, bgBaseX, bgBaseY)
Entity.set_global_pos(Background2, bgBaseX + BG_TILE_W, bgBaseY)

ApplyBackgroundStyle()
------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()

    Entity.set_global_pos(player1, x, 300)
	
    playerSprite = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 64, 64, 10)
    Sprite.set_columns(playerSprite,1)
    ApplyBirdStyle()

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

    -- populate cached pipes list once
    pipesList = { pipe, pipeT, pipe2, pipeT2, pipe3, pipeT3 }

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
    Entity.add_audio_component(coinSound, "coin", false)
    AudioComponent.change_volume(coinSound, 100)

    -- Game over SFX
    gameOverSound = Entity.create_entity()
    Entity.add_audio_component(gameOverSound, "gameOver", false)
    AudioComponent.change_volume(gameOverSound, 30)

    --    musicEntity = Entity.create_entity()
    --Entity.add_audio_component(musicEntity, "technoSong", false)
    
    -- Apply saved audio settings
    ApplyMusicVolume()
    UpdateMusicPlayback()
    ApplySfxVolumes()

    -- Freeze everything on main menu
    Fysics.set_gravity_scale(player1, 0)
    Fysics.set_linear_velocity(player1, 0, 0)

    for _, p in ipairs(pipesList) do
        Fysics.set_linear_velocity(p, 0, 0)
    end

    for _, c in ipairs(coins) do
        Fysics.set_linear_velocity(c, 0, 0)
    end
end


----------------------------------------------------------
-- Flappy UI (Layout C)
----------------------------------------------------------

local function DrawMainMenu_C(windowW, windowH)
    -- Light overlay so UI reads without looking like a dark sci‑fi panel
    UI.add_panel(0, 0, windowW, windowH, 0.10, 0, 0, 0, 0)

    local cx = windowW / 2

    -- Header
    UI.add_centered_label(cx, 55, "FLAPPY BIRD", "ImGuiDefaultBold", 2.2)

    -- Top-left stats
    UI.add_label(18, 16, 0, 0, "Best: " .. tostring(highscore), "ImGuiDefaultBold", 1.2)
    UI.add_label(18, 40, 0, 0, "Coins: " .. tostring(bankCoins or 0), "ImGuiDefaultBold", 1.2)

    -- Big play button
    local playW, playH = 320, 74
    local playX = math.floor((windowW - playW) / 2)
    local playY = math.floor(windowH * 0.44)

    UI.add_button(
        playX, playY, playW, playH,
        "PLAY", "fb_play",
        "ImGuiDefaultBold", 1.15,
        playH / 2, true,
        70, 200, 120, 0.95
    )

    UI.add_centered_label(cx, playY + playH + 28, "Press SPACE to flap", "", 1.15)

    -- Bottom nav strip
    local navBtnW, navBtnH = 150, 44
    local navGap = 14
    local totalW = navBtnW * 3 + navGap * 2
    local startX = math.floor((windowW - totalW) / 2)
    local navY = windowH - navBtnH - 26

    -- (No heavy panel; just “mobile-style” buttons)
    UI.add_button(startX + (navBtnW + navGap) * 0, navY, navBtnW, navBtnH,
        "Customise", "fb_nav_customise",
        "ImGuiDefaultBold", 1.0, navBtnH / 2, true,
        80, 170, 255, 0.92
    )

    UI.add_button(startX + (navBtnW + navGap) * 1, navY, navBtnW, navBtnH,
        "Settings", "fb_nav_settings",
        "ImGuiDefaultBold", 1.0, navBtnH / 2, true,
        255, 170, 80, 0.92
    )

    UI.add_button(startX + (navBtnW + navGap) * 2, navY, navBtnW, navBtnH,
        "Exit", "fb_nav_exit",
        "ImGuiDefaultBold", 1.0, navBtnH / 2, true,
        220, 80, 80, 0.90
    )

    -- Actions
    if UI.was_button_pressed("fb_play") then
        if Background ~= nil then
            Entity.set_global_pos(Background, bgBaseX, bgBaseY)
        end

        inMainMenu = false
        resetGame()
        print("Started Game")
    elseif UI.was_button_pressed("fb_nav_customise") then
        menuContext = "customise"
    elseif UI.was_button_pressed("fb_nav_settings") then
        settingsBackContext = "main"
        menuContext = "settings"
    elseif UI.was_button_pressed("fb_nav_exit") then
        Window.quit()
    end
end

local function DrawSettingsMenu_C(windowW, windowH)
    UI.add_panel(0, 0, windowW, windowH, 0.18, 0, 0, 0, 0)

    local panelW = math.floor(math.min(560, windowW * 0.85))
    local panelH = math.floor(math.min(420, windowH * 0.80))
    local panelX = math.floor((windowW - panelW) / 2)
    local panelY = math.floor((windowH - panelH) / 2)

    -- Soft “cloudy” panel
    UI.add_panel(panelX, panelY, panelW, panelH, 0.72, 24, 60, 140, 190)

    local cx = windowW / 2
    UI.add_centered_label(cx, panelY + 26, "SETTINGS", "ImGuiDefaultBold", 1.9)

    local x = panelX + 40
    local w = panelW - 80
    local y = panelY + 85

    -- Music toggle + slider (slider still useful once you add BGM)
    UI.add_checkbox(x, y, 0, 0, "Music", "fb_music_on", s_musicOn)
    y = y + 28
    UI.add_label(x, y, "Music Volume", UI_FONT_REG, 1.0)
    y = y + 18
    UI.add_slider(x, y, w, "", "fb_music_vol", 0, 100, s_musicVol, nil, nil, "%.0f")
    y = y + 60

    -- SFX toggle + slider
    UI.add_checkbox(x, y, 0, 0, "SFX", "fb_sfx_on", s_sfxOn)
    y = y + 28
    UI.add_label(x, y, "SFX Volume", UI_FONT_REG, 1.0)
    y = y + 18
    UI.add_slider(x, y, w, "", "fb_sfx_vol", 0, 100, s_sfxVol, nil, nil, "%.0f")
    y = y + 60

    -- Master volume
    UI.add_label(x, y, "Master Volume", UI_FONT_REG, 1.0)
    y = y + 18
    UI.add_slider(x, y, w, "", "fb_master_vol", 0, 100, s_masterVol, nil, nil, "%.0f")

    -- Apply changes
    if UI.was_checkbox_changed("fb_music_on") then
        s_musicOn = UI.get_checkbox("fb_music_on") or s_musicOn
        SaveSetting("audio.musicOn", s_musicOn)
        ApplyMusicVolume()
        UpdateMusicPlayback()
    end

    if UI.was_slider_changed("fb_music_vol") then
        s_musicVol = UI.get_slider("fb_music_vol") or s_musicVol
        SaveSetting("audio.musicVol", s_musicVol)
        ApplyMusicVolume()
    end

    if UI.was_checkbox_changed("fb_sfx_on") then
        s_sfxOn = UI.get_checkbox("fb_sfx_on") or s_sfxOn
        SaveSetting("audio.sfxOn", s_sfxOn)
        ApplySfxVolumes()
    end

    if UI.was_slider_changed("fb_sfx_vol") then
        s_sfxVol = UI.get_slider("fb_sfx_vol") or s_sfxVol
        SaveSetting("audio.sfxVol", s_sfxVol)
        ApplySfxVolumes()
    end



    if UI.was_slider_changed("fb_master_vol") then
        s_masterVol = UI.get_slider("fb_master_vol") or s_masterVol
        SaveSetting("audio.masterVol", s_masterVol)
        ApplySfxVolumes()
        ApplyMusicVolume()
    end

    -- Back button
    local bw, bh = 200, 46
    local bx = math.floor((windowW - bw) / 2)
    local by = panelY + panelH - bh - 26
    UI.add_button(bx, by, bw, bh, "Back", "fb_settings_back",
        "ImGuiDefaultBold", 1.0, bh / 2, true,
        80, 170, 255, 0.92
    )

    if UI.was_button_pressed("fb_settings_back") then
        menuContext = settingsBackContext
    end
end

local function DrawCustomiseMenu_C(windowW, windowH)
    UI.add_panel(0, 0, windowW, windowH, 0.18, 0, 0, 0, 0)

    local panelW = math.floor(math.min(680, windowW * 0.92))
    local panelH = math.floor(math.min(520, windowH * 0.86))
    local panelX = math.floor((windowW - panelW) / 2)
    local panelY = math.floor((windowH - panelH) / 2)

    UI.add_panel(panelX, panelY, panelW, panelH, 0.72, 24, 60, 140, 190)

    local cx = windowW / 2
    UI.add_centered_label(cx, panelY + 24, "CUSTOMISE", "ImGuiDefaultBold", 1.9)
    UI.add_centered_label(cx, panelY + 54, "Coins: " .. tostring(bankCoins or 0), "ImGuiDefaultBold", 1.2)

    -- Tabs
    local tabW, tabH = 160, 40
    local tabY = panelY + 88
    local tabX = math.floor(cx - tabW - 10)
    local tabX2 = math.floor(cx + 10)

    local function tabBtn(label, id, active, x)
        local r, g, b = 80, 170, 255
        local a = active and 0.95 or 0.70
        UI.add_button(x, tabY, tabW, tabH, label, id,
            "ImGuiDefaultBold", 1.0, tabH / 2, true,
            r, g, b, a
        )
    end

    tabBtn("Backgrounds", "fb_tab_bg", customiseTab == "backgrounds", tabX)
    tabBtn("Birds",       "fb_tab_bird", customiseTab == "birds", tabX2)

    if UI.was_button_pressed("fb_tab_bg") then customiseTab = "backgrounds" end
    if UI.was_button_pressed("fb_tab_bird") then customiseTab = "birds" end

    local listX = panelX + 40
    local listY = tabY + 60
    local rowH = 54
    local rowW = panelW - 80

    local function drawItemRow(item, ownedSet, equippedIdKey, currentEquipped, rowIndex)
        local y = listY + (rowIndex - 1) * rowH
        if y + rowH > panelY + panelH - 90 then return end

        local owned = ownedSet[item.id] == true
        local equipped = (currentEquipped == item.id)

        local label = item.name
        if equipped then
            label = label .. "  (Equipped)"
        elseif owned then
            label = label .. "  (Owned)"
        else
            label = label .. "  (" .. tostring(item.price) .. " coins)"
        end

        UI.add_label(listX, y + 10, 0, 0, label, "ImGuiDefaultBold", 1.15)

        local bw, bh = 140, 38
        local bx = listX + rowW - bw
        local by = y + 8

        local btnText, canPress, br, bg, bb, ba

        if equipped then
            btnText = "Selected"
            canPress = false
            br, bg, bb, ba = 120, 120, 120, 0.65
        elseif owned then
            btnText = "Equip"
            canPress = true
            br, bg, bb, ba = 70, 200, 120, 0.92
        else
            btnText = "Buy"
            canPress = (bankCoins or 0) >= (item.price or 0)
            if canPress then
                br, bg, bb, ba = 255, 170, 80, 0.92
            else
                br, bg, bb, ba = 120, 120, 120, 0.65
            end
        end

        UI.add_button(bx, by, bw, bh, btnText, "fb_buy_" .. item.id,
            "ImGuiDefaultBold", 1.0, bh / 2, true,
            br, bg, bb, ba
        )

        if canPress and UI.was_button_pressed("fb_buy_" .. item.id) then
            if owned then
                -- equip
                if customiseTab == "backgrounds" then
                    equippedBackground = item.id
                    SaveSetting("equipped.background", equippedBackground)
                    ApplyBackgroundStyle()
                else
                    equippedBird = item.id
                    SaveSetting("equipped.bird", equippedBird)
                    ApplyBirdStyle()
                end
            else
                -- buy
                bankCoins = (bankCoins or 0) - (item.price or 0)
                SaveSetting("coins.total", bankCoins)

                ownedSet[item.id] = true
                if customiseTab == "backgrounds" then
                    SaveSetting("owned.backgrounds", SetToCsv(ownedBackgrounds))
                else
                    SaveSetting("owned.birds", SetToCsv(ownedBirds))
                end
            end
        end
    end

    if customiseTab == "backgrounds" then
        for i, item in ipairs(STORE_BACKGROUNDS) do
            drawItemRow(item, ownedBackgrounds, "equipped.background", equippedBackground, i)
        end
    else
        for i, item in ipairs(STORE_BIRDS) do
            drawItemRow(item, ownedBirds, "equipped.bird", equippedBird, i)
        end
    end

    -- Back button
    local bw, bh = 200, 46
    local bx = math.floor((windowW - bw) / 2)
    local by = panelY + panelH - bh - 26
    UI.add_button(bx, by, bw, bh, "Back", "fb_customise_back",
        "ImGuiDefaultBold", 1.0, bh / 2, true,
        80, 170, 255, 0.92
    )

    if UI.was_button_pressed("fb_customise_back") then
        menuContext = "main"
    end
end


----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
----------------------------------------------------------
-- Main Menu / Pause / Settings / Customise (Layout C)
----------------------------------------------------------

-- Cache window size once per frame
local windowW = Window.get_width()
local windowH = Window.get_height()

if inMainMenu then
    -- Scroll the background horizontally on the MAIN menu only (classic Flappy vibe).
    if Background ~= nil then
        if menuContext == "main" then
            local dt = Mafs.delta_time()
            local driftY = math.cos(Mafs.time() * 0.12) * 4

            bgScrollX = bgScrollX + (BG_SCROLL_SPEED * dt)
            if bgScrollX <= -BG_TILE_W then
                bgScrollX = bgScrollX + BG_TILE_W
            end

            Entity.set_global_pos(Background,  bgBaseX + bgScrollX,              bgBaseY + driftY)
            if Background2 ~= nil then
                Entity.set_global_pos(Background2, bgBaseX + bgScrollX + BG_TILE_W, bgBaseY + driftY)
            end
        else
            -- Keep it static when in sub-menus (settings/customise) so UI feels stable
            bgScrollX = 0
            Entity.set_global_pos(Background, bgBaseX, bgBaseY)
            if Background2 ~= nil then
                Entity.set_global_pos(Background2, bgBaseX + BG_TILE_W, bgBaseY)
            end
        end
    end


    if menuContext == "main" then
        DrawMainMenu_C(windowW, windowH)
    elseif menuContext == "settings" then
        DrawSettingsMenu_C(windowW, windowH)
    elseif menuContext == "customise" then
        DrawCustomiseMenu_C(windowW, windowH)
    elseif menuContext == "ingame" then
        -- Redesigned pause menu (Layout C style)
        UpdateUiScale(windowW, windowH)

        UI.add_panel(0, 0, windowW, windowH, 0.35, 0, 0, 0, 0)

        local baseW, baseH = 520, 390
        local panelW, panelH = S(baseW), S(baseH)
        local panelX = Ux((UI_BASE_W - baseW) / 2)
        local panelY = Uy((UI_BASE_H - baseH) / 2)

        UI.begin_child(panelX, panelY, panelW, panelH, "FB_PauseMenu",
            true, 0,
            true, 0.85, 10, 70, 160, 115
        )

        local cx = panelW / 2
        UI.add_centered_label(cx, S(34), "Paused", UI_FONT_TITLE, 1.0)

        local btnW, btnH = S(260), S(46)
        local btnX = (panelW - btnW) / 2
        local startY = S(90)
        local gapY = S(14)

        local function AddBtn(row, text, id, r, g, b, a)
            UI.add_button(btnX, startY + (btnH + gapY) * row, btnW, btnH, text, id,
                "ImGuiDefaultBold", 1.0, btnH / 2, true,
                r, g, b, a
            )
        end

        AddBtn(0, "Resume",   "fb_pause_resume",   80, 170, 255, 0.92)
        AddBtn(1, "Restart",  "fb_pause_restart",  255, 170, 80, 0.92)
        AddBtn(2, "Settings", "fb_pause_settings", 120, 220, 140, 0.90)
        AddBtn(3, "Main Menu","fb_pause_main",     255, 170, 80, 0.92)
        AddBtn(4, "Exit",     "fb_pause_exit",     220, 80, 80, 0.90)

        UI.end_child()

        if UI.was_button_pressed("fb_pause_resume") then
            pauseGame(false)
            inMainMenu = false
            print("Resumed Game")
        elseif UI.was_button_pressed("fb_pause_restart") then
            pauseGame(false)
            inMainMenu = false
            resetGame()
            print("Restarted Game")
        elseif UI.was_button_pressed("fb_pause_settings") then
            settingsBackContext = "ingame"
            menuContext = "settings"
        elseif UI.was_button_pressed("fb_pause_main") then
            pauseGame(false)
            inMainMenu = true
            menuContext = "main"
            resetGame()
            print("Switched to main menu")
        elseif UI.was_button_pressed("fb_pause_exit") then
            Window.quit()
            print("Quitting Game")
        end
    end

    return
end

-----------------------------------------
-- Open main menu button in play mode
-----------------------------------------
    if not inMainMenu then

        local btnSize = 36
        local margin = 10
        local btnX = windowW - btnSize - margin
        local btnY = margin

        UI.add_button(btnX, btnY, btnSize, btnSize, "=", "menuButton")

        -- Open main menu
        if UI.was_button_pressed("menuButton") then
            pauseGame(true)
            inMainMenu = true   
            menuContext = "ingame"
        end

    end
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

        UI.begin_child(lbX, lbY, lbW, lbH, "FB_Leaderboard",
            true, 0,
            true, 0.75, 10, 70, 160, 115
        )

        local lcx = lbW / 2
        UI.add_centered_label(lcx, 12, T("fb.leaderboard"), UI_FONT_HEADER, 1.0)

        if topLeaderboard then
            for i, e in ipairs(topLeaderboard) do
                local line = string.format("%d. %s - %d", i, e.name, e.score)
                UI.add_label(10, 42 + (i-1)*20, 0, 0, line, UI_FONT_REG, 1.0)
            end
        end

        UI.end_child()


        -- Center Game Over panel
        local panelW, panelH = 460, 205
        local panelX = (windowW - panelW) / 2
        local panelY = 200

       UI.begin_child(panelX, panelY, panelW, panelH, "FB_GameOver",
           true, 0,
           true, 0.75, 12, 95, 150, 165
       )

       local cx = panelW / 2
       local y0 = 14
       local gap = 22

       UI.add_centered_label(cx, y0, text1, UI_FONT_HEADER, 1.0)
       UI.add_centered_label(cx, y0 + gap*2, topScore, UI_FONT_SUB, 1.0)
       UI.add_centered_label(cx, y0 + gap*3, finalScoreText, UI_FONT_SUB, 1.0)
       UI.add_centered_label(cx, y0 + gap*4, coinsText, UI_FONT_SUB, 1.0)
       UI.add_centered_label(cx, panelH - 34, text2, UI_FONT_SUB, 1.0)

       UI.end_child()

    -- Show TextInput only if new high score
    if newHighScore and not submitted then

        -- If we already have a saved name, submit immediately once
        if playerName ~= nil and playerName ~= "" then
            Firebase.submit_high_score(GAME_ID, playerName, highscore)
            submitted = true
            leaderboardFetched = false
        else
            -- Otherwise prompt for name
            local nhW, nhH = 520, 85
            local nhX = (windowW - nhW) / 2
            local nhY = panelY + panelH + 12

            UI.begin_child(nhX, nhY, nhW, nhH+15, "FB_NewHighscore",
                true, 0,
                true, 0.75, 10, 70, 160, 115
            )

            local cx = nhW / 2
            UI.add_centered_label(cx, 12, T("fb.new_highscore_line1"), UI_FONT_SUB, 1.0)
            UI.add_centered_label(cx, 34, T("fb.new_highscore_line2"), UI_FONT_REG, 1.0)

            local inputW = 260
            local inputX = (nhW - inputW) / 2
            local inputY = 52
            UI.add_input_text(inputX, inputY, inputW, "", "player_name", 16)

            UI.end_child()

            if UI.was_input_committed("player_name") then
                local getName = UI.get_input_text_live or UI.get_input_text
                local name = SanitiseName(getName("player_name"))
                if name == "" then name = "Anon" end

                playerName = name
                Json.save_player_name(playerName)

                Firebase.submit_high_score(GAME_ID, playerName, highscore)
                submitted = true
                leaderboardFetched = false
            end
        end
    end


-- Count down once per frame (never below 0)
        restartDelayFrames = math.max(0, restartDelayFrames - 1)

        -- Only allow restart when delay is done AND name entry isn't active
        local canRestart = (restartDelayFrames == 0) and not (newHighScore and not submitted)

        text2 = canRestart and T("fb.press_space_restart") or ""


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

        for _, p in ipairs(pipesList) do
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
   local cx = windowW / 2
   local cy = windowH / 2
    -- UI
    UI.add_label(10, 10, 1000, 1000, pipeScoreText, UI_FONT_SUB, 1)
    UI.add_label(10, 40, 1000, 1000, scoreText, UI_FONT_SUB, 1)
    UI.add_centered_label(cx, cy, text1, UI_FONT_HEADER, 1)
end

----------------------------------------------------------
-- Coins collision + score
----------------------------------------------------------
function ExampleScript:OnTriggerEnter(a, b)
    if inMainMenu or gameOver then return end
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
    if inMainMenu or gameOver then return end
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
