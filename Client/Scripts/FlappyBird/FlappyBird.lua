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
-- Background scrolling (main menu)
local BG_BASE_W, BG_BASE_H = 960, 610
local BG_PAD = 20                -- oversize to hide seams/edges

local floorTileSprites = {}
local BG_TILE_W = BG_BASE_W      
local BG_SCROLL_SPEED = -18      
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

-- --------------------------------------------------------------------------------
-- Language settings
-- --------------------------------------------------------------------------------
local Localisation = require("Scripts.FlappyBird.Localisation")
local function T(key) return Localisation.t(key) end

local language = Json.load_setting(GAME_ID, "ui.language", "en") or "en"
Localisation.set_language(language)

local UI_FONT_BOLD = "ImGuiDefaultBold"
local UI_FONT_REG  = "ImGuiDefault"

-- Language-specific 
local function ApplyLanguageFonts()
    -- use the actual language variable
    local lang_local = language or Localisation.get_language() or "en"

    if lang_local == "ja" then
      UI_FONT_REG   = "JFDotJP"
      UI_FONT_BOLD  = "JFDotBoldJP"
      UI_FONT_SUB = "JFDotSubJP"
      UI_FONT_HEADER = "JFDotHeaderJP"
      UI_FONT_TITLE = "JFDotTitleJP"
    else
      UI_FONT_REG   = "ImGuiDefault"
      UI_FONT_BOLD  = "ImGuiDefaultBold"
      UI_FONT_SUB = "ImGuiSub"
      UI_FONT_HEADER = "ImGuiHeader"
      UI_FONT_TITLE = "ImGuiTitle"
    end
end

ApplyLanguageFonts()
-- --------------------------------------------------------------------------------
-- Saved settings 
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

-- Owned/equipped cosmetics
local ownedBackgrounds = CsvToSet(LoadSetting("owned.backgrounds", "bg_classic"))
local ownedBirds       = CsvToSet(LoadSetting("owned.birds", "bird_classic"))
local equippedBackground = LoadSetting("equipped.background", "bg_classic")
local equippedBird       = LoadSetting("equipped.bird", "bird_classic")

-- Customise screen tab
local customiseTab = "backgrounds" -- "backgrounds" or "birds"

-- Shop items 
local STORE_BACKGROUNDS = {
    { id = "bg_classic", name = "Classic Sky", price = 0,  textureKey = "Background" },
    { id = "bg_night",   name = "Night Sky",   price = 30, textureKey = "Background" }, -- tint style
}

local STORE_BIRDS = {
    { id = "bird_classic", name = "Classic Bird", price = 0,  textureKey = "FlappyBird" },
    { id = "bird_gold",    name = "Gold Bird",    price = 50, textureKey = "FlappyBird" }, -- tint skin
    { id = "bird_purple",  name = "Purple Bird",  price = 75, textureKey = "FlappyPurple" },
}

local BG_STYLES
local BIRD_STYLES


local function EnsureDefaultsOwned()
if not BG_STYLES[equippedBackground] then
    equippedBackground = "bg_classic"
    SaveSetting("equipped.background", equippedBackground)
end
if not BIRD_STYLES[equippedBird] then
    equippedBird = "bird_classic"
    SaveSetting("equipped.bird", equippedBird)
end

    if not ownedBackgrounds["bg_classic"] then ownedBackgrounds["bg_classic"] = true end
    if not ownedBirds["bird_classic"] then ownedBirds["bird_classic"] = true end
end

BG_STYLES = {
    bg_classic = { tint = {255, 255, 255}, textureKey = "Background" },
    bg_night   = { tint = {150, 170, 255}, textureKey = "Background" }, -- night tint
}


BIRD_STYLES = {
    bird_classic = { tint = {255, 255, 255}, textureKey = "FlappyBird" },
    bird_gold    = { tint = {255, 220, 120}, textureKey = "FlappyBird" },
    bird_purple  = { tint = {255, 255, 255}, textureKey = "FlappyPurple" },
}


local function ApplyBackgroundStyle()
    local style = BG_STYLES[equippedBackground] or BG_STYLES["bg_classic"]

    if (backgroundSprite or backgroundSprite2) and Sprite and Sprite.set_texture and assets and assets.textures then
        local key = (style and style.textureKey) or "Background"
        local tex = assets.textures[key] or assets.textures["Background"]
        if tex then
            if backgroundSprite then pcall(Sprite.set_texture, backgroundSprite, tex) end
            if backgroundSprite2 then pcall(Sprite.set_texture, backgroundSprite2, tex) end
        end
    end

    local t = (style and style.tint) or {255, 255, 255}
    if Sprite and Sprite.set_color then
        if backgroundSprite then Sprite.set_color(backgroundSprite, t[1], t[2], t[3]) end
        if backgroundSprite2 then Sprite.set_color(backgroundSprite2, t[1], t[2], t[3]) end
    end
end


local function ApplyBirdStyle()
    local style = BIRD_STYLES[equippedBird] or BIRD_STYLES["bird_classic"]

    if playerSprite and Sprite and Sprite.set_texture and assets and assets.textures then
        local key = (style and style.textureKey) or "FlappyBird"
        local tex = assets.textures[key] or assets.textures["FlappyBird"]
        if tex then
            pcall(Sprite.set_texture, playerSprite, tex)
        end
    end

    local t = (style and style.tint) or {255, 255, 255}
    if playerSprite and Sprite and Sprite.set_color then
        Sprite.set_color(playerSprite, t[1], t[2], t[3])
    end
end


EnsureDefaultsOwned()

-- Pipes
local pipe, pipeT, pipe2, pipeT2, pipe3, pipeT3
local pipeSets = {}
local pipesList = {} 
local pipeSpeed = -3
local pipeOffScreenLeft = -100 

-- Pipe randomness (in pixels)
local pipeHeight = 500       -- pipe sprite height in pixels
local pipeStartGap = 330     -- starting gap size in pixels (easier)
local pipeMinGap   = 210     -- minimum gap size in pixels (harder)
local pipeShrinkGap = 3      -- gap shrinks by this many pixels per point

local pipesetMinGap = 150    -- minimum Y position for gap center (in pixels from top)
local pipesetMaxGap = 400    -- maximum Y position for gap center (in pixels from top)

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
local pipeScoreText = T("fb.score_label") .. "0"
local scoreText = T("fb.coins") .. "0"
local text1 = T("fb.press_space_start")
local text2 = T("fb.press_space_restart")
local finalScoreText = T("fb.final_score") .. "0"
local coinsText = T("fb.coins_collected")
local topScore = T("fb.highscore")

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


-- Window
Window.set_size_centered(960, 600)


-- Base SFX volumes
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
            AudioComponent.play(musicEntity, 0, -1)
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
-- UI scaling helpers
-- ----------------------------------------------------------
local UI_BASE_W, UI_BASE_H = 960, 600
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

-- Settings menu back navigation
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

    -- Calculate gap between pipes (topPos.y + pipeHeight is bottom of top pipe, bottomPos.y is top of bottom pipe)
    local gapTop = Mafs.get_vec_y(topPos) + pipeHeight
    local gapBottom = Mafs.get_vec_y(bottomPos)

    -- Place coin randomly in the gap
    local randY = math.random() * (gapBottom - gapTop - 100) + gapTop + 10
    local yRandomOffset = (math.random() * 2 - 1) * 80
    randY = randY + yRandomOffset

    local randX = Mafs.get_vec_x(bottomPos) + offsetX
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
    pipeScoreText = T("fb.score_label") .. "0"
    scoreText = T("fb.coins") .. "0"
    text1 = T("fb.press_space_start")
    finalScoreText = T("fb.final_score") .. "0"
    coinsText = T("fb.coins_collected")
    topScore = T("fb.highscore")
    highscore = Json.load_high_score(GAME_ID)

    -- Reset player
    Fysics.set_pos(player1, 100, 300)
    Fysics.set_gravity_scale(player1, 0)
    Fysics.set_linear_velocity(player1, 0, 0)

    -- Reset pipes
    local function resetPipe(pipeEntity, xPos, yPos)
        Fysics.set_pos(pipeEntity, xPos, yPos)
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
    finalScoreText = T("fb.final_score") .. tostring(Pscore)
    topScore = T("fb.highscore") .. tostring(highscore)
    coinsText = T("fb.coins_collected") .. tostring(score)

    bankCoins = (bankCoins or 0) + (score or 0)
    SaveSetting("coins.total", bankCoins)
end

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    ------------------------------------------------------
    -- Pick texture for left / middle / right
    ------------------------------------------------------
    local tileSize = 64
    local floorY = 550
    -- Floor collision tiles
    for i = 0, 30 do

        local tile = Entity.create_entity()
        local xPos = i * tileSize

        local s = Entity.add_sprite_component(tile, assets.textures.Sand, tileSize, tileSize, -5)
        floorTileSprites[#floorTileSprites+1] = s
        Sprite.set_columns(s, 1)

        Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)
        Fysics.add_sprite_collider(tile, false, 1)
        Fysics.set_pos(tile, xPos, floorY)
    end

    ------------------------------------------------------
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
	
    playerSprite = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 64, 64, 10)
    Sprite.set_columns(playerSprite,1)
    ApplyBirdStyle()

    -- PLAYER 1 PHYSICS
    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true) -- dynamic body
    Fysics.add_sprite_collider(player1,false, 0.35)
    Fysics.set_pos(player1, x, 300)

    -- Freeze bird
    Fysics.set_gravity_scale(player1, 0)

	------------------------------------------------------
	-- Create pipe obstacle
	------------------------------------------------------
    local function createPipeSet(bottomX, bottomY, topX, topY)
        local bottomPipe = Entity.create_entity()
        Entity.add_sprite_component(bottomPipe, assets.textures.FlappyPipe, 60, 500, 0)
        Entity.add_fysics_component(bottomPipe, enums.bodytype.kinematicBody, false)
        Fysics.add_sprite_collider(bottomPipe, false, 1)
        Fysics.set_pos(bottomPipe, bottomX, bottomY)

        local topPipe = Entity.create_entity()
        Entity.add_sprite_component(topPipe, assets.textures.FlappyPipe2, 60, 500, 0)
        Entity.add_fysics_component(topPipe, enums.bodytype.kinematicBody, false)
        Fysics.add_sprite_collider(topPipe, false, 1)
        Fysics.set_pos(topPipe, topX, topY)
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
    --Entity.add_audio_component(musicEntity, "", false)
    
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
-- Helper: Localise item display names
----------------------------------------------------------
local function LocalisedItemName(item)
    if not item or not item.id then return item and item.name or "" end
    if string.sub(item.id, 1, 3) == "bg_" then
        if item.id == "bg_classic" then return T("backgrounds.classic") end
        if item.id == "bg_night" then return T("backgrounds.classicnight") end
    elseif string.sub(item.id, 1, 5) == "bird_" then
        if item.id == "bird_classic" then return T("birds.classic") end
        if item.id == "bird_gold" then return T("birds.gold") end
        if item.id == "bird_purple" then return T("birds.purple") end
    end
    return item.name or item.id
end

----------------------------------------------------------
-- Flappy UI
----------------------------------------------------------

local function DrawMainMenu_C(windowW, windowH)
    UI.add_panel(0, 0, windowW, windowH, 0.10, 0, 0, 0, 0)

    local cx = windowW / 2

    -- Header
    UI.add_centered_label(cx, 55, T("fb.title"), UI_FONT_TITLE, 1)

    -- Top-left stats
    UI.add_label(18, 16, 0, 0, T("fb.best_score") .. tostring(highscore), UI_FONT_REG, 1)
    UI.add_label(18, 40, 0, 0, T("fb.coins") .. tostring(bankCoins or 0), UI_FONT_REG, 1)

    -- Big play button
    local playW, playH = 320, 74
    local playX = math.floor((windowW - playW) / 2)
    local playY = math.floor(windowH * 0.44)

    UI.add_button(
        playX, playY, playW, playH,
        T("fb.play"), "fb_play",
        UI_FONT_BOLD, 1.15,
        playH / 2, true,
        70, 200, 120, 0.95
    )

    -- Bottom nav strip
    local navBtnW, navBtnH = 150, 44
    local navGap = 14
    local totalW = navBtnW * 3 + navGap * 2
    local startX = math.floor((windowW - totalW) / 2)
    local navY = windowH - navBtnH - 26

    UI.add_button(startX + (navBtnW + navGap) * 0, navY, navBtnW, navBtnH,
        T("fb.customise"), "fb_nav_customise",
        UI_FONT_BOLD, 1.0, navBtnH / 2, true,
        80, 170, 255, 0.92
    )

    UI.add_button(startX + (navBtnW + navGap) * 1, navY, navBtnW, navBtnH,
        T("fb.settings"), "fb_nav_settings",
        UI_FONT_BOLD, 1.0, navBtnH / 2, true,
        255, 170, 80, 0.92
    )

    UI.add_button(startX + (navBtnW + navGap) * 2, navY, navBtnW, navBtnH,
        T("fb.exit"), "fb_nav_exit",
        UI_FONT_BOLD, 1.0, navBtnH / 2, true,
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

    UI.begin_child(panelX, panelY, panelW, panelH, "FB_SettingsPanel",
        true, 0, true, 0.80, 12, 26, 245, 225
    )

    local cx = panelW / 2
    UI.add_centered_label(cx, 26, T("settings.settings"), UI_FONT_BOLD, 1.9)

    local contentX = 30
    local contentY = 85
    local contentW = panelW - 60  
    local contentH = panelH - 157

    UI.begin_child(contentX, contentY, contentW, contentH, "FB_SettingsContent",
        false, 128, false
    )

    local leftPad = 12
    local x = leftPad
    local w = contentW - (leftPad * 2) 
    local y = 0

    -- Center X inside inner child
    local cxLocal = contentW / 2

    -- Header for the audio block 
    UI.add_centered_label(cxLocal, y + 8, T("settings.audio"), UI_FONT_HEADER, 1)
    y = y + 36

    -- for future sliders
    local sliderStyle = {
        height = 18,
        rounding = 10,
        grab_size = 16,
        track = { 60, 80, 100, 0.35 },
        grab  = { 255, 200, 80, 0.95 }
    }

    local checkboxStyle = {
        size = 18,
        rounding = 6,
        off = { 60, 80, 100, 0.35 },
        on  = { 255, 200, 80, 0.95 },
        check = { 255, 255, 255, 0.95 }
    }

    -- Use a fixed block spacing so sliders are equally spaced
    local blockGap = 66

    -- Music toggle + slider 
    UI.add_checkbox_styled(x, y, 0, 0, T("settings.musiccb"), "fb_music_on", s_musicOn, UI_FONT_REG, 1, checkboxStyle)
    y = y + 28
    UI.add_label(x, y, 0, 0, T("settings.musicvol"), UI_FONT_REG, 1.0)
    y = y + 23
    UI.add_slider_styled(x, y, w, "", "fb_music_vol", 0, 100, s_musicVol, nil, nil, "%.0f", sliderStyle)
    y = y + blockGap

    -- SFX toggle + slider
    UI.add_checkbox_styled(x, y, 0, 0, T("settings.sfxcb"), "fb_sfx_on", s_sfxOn, UI_FONT_REG, 1, checkboxStyle)
    y = y + 28
    UI.add_label(x, y, 0, 0, T("settings.sfxvol"), UI_FONT_REG, 1.0)
    y = y + 23
    UI.add_slider_styled(x, y, w, "", "fb_sfx_vol", 0, 100, s_sfxVol, nil, nil, "%.0f", sliderStyle)
    y = y + blockGap

    -- Master volume
    UI.add_label(x, y, 0, 0, T("settings.mastervol"), UI_FONT_REG, 1.0)
    y = y + 23
    UI.add_slider_styled(x, y, w, "", "fb_master_vol", 0, 100, s_masterVol, nil, nil, "%.0f", sliderStyle)
    y = y + 88 -- extra space before language header

    -- Apply changes 
    if UI.was_checkbox_changed("fb_music_on") then
        s_musicOn = UI.get_checkbox("fb_music_on")
        if s_musicOn == nil then s_musicOn = true end
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
        s_sfxOn = UI.get_checkbox("fb_sfx_on")
        if s_sfxOn == nil then s_sfxOn = true end
        SaveSetting("audio.sfxOn", s_sfxOn)
        ApplySfxVolumes()

        if (not s_sfxOn) and AudioComponent and AudioComponent.terminate then
            AudioComponent.terminate(birdJumpSound)
            AudioComponent.terminate(hitSound)
            AudioComponent.terminate(coinSound)
            AudioComponent.terminate(gameOverSound)
        end
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

    local ddStyle = {
        height = 32,
        rounding = 10,
        popup_rounding = 10,
        border_size = 1,

        frame = { 60, 80, 100, 0.35 },

        frame_hover  = { 255, 200, 80, 0.95 },
        frame_active = { 255, 200, 80, 0.95 },

        popup_bg = { 20, 20, 20, 0.95 },

        item        = { 255, 200, 80, 0.95 },
        item_hover  = { 255, 200, 80, 0.95 },
        item_active = { 255, 200, 80, 0.95 },
        text = { 255, 255, 255, 1.0 },
    }

    local opts = { "English", "日本語" }
    local defaultIndex = (language == "ja") and 1 or 0

    UI.add_centered_label(x + w/2, y, T("settings.language"), UI_FONT_HEADER, 1.0)
    y = y + 36
    local langDropdownFont = "ImGuiDefaultJP" -- use JP fontto support both EN and JA characters
    UI.add_dropdown_styled(x, y, w, 32, "", "fb_lang", opts, defaultIndex, langDropdownFont, 1.0, ddStyle)

    if UI.was_dropdown_changed("fb_lang") then
        local idx = UI.get_dropdown_index("fb_lang") or 0
        language = (idx == 1) and "ja" or "en"
        Json.save_setting(GAME_ID, "ui.language", language)
        Localisation.set_language(language)
        ApplyLanguageFonts()
    end

    UI.end_child() -- end inner content child

    -- Footer / Back button 
    local bw, bh = 200, 46
    local bx = math.floor((panelW - bw) / 2)
    local by = panelH - bh - 26

    UI.add_button(bx, by, bw, bh, T("settings.back"), "fb_settings_back",
        UI_FONT_BOLD, 1.0, bh / 2, true,
        80, 170, 255, 0.92
    )

    if UI.was_button_pressed("fb_settings_back") then
        menuContext = settingsBackContext
    end

    UI.end_child() -- end outer child (panel)
end

local function DrawCustomiseMenu_C(windowW, windowH)
    UI.add_panel(0, 0, windowW, windowH, 0.18, 0, 0, 0, 0)

    local panelW = math.floor(math.min(680, windowW * 0.92))
    local panelH = math.floor(math.min(520, windowH * 0.86))
    local panelX = math.floor((windowW - panelW) / 2)
    local panelY = math.floor((windowH - panelH) / 2)

    UI.add_panel(panelX, panelY, panelW, panelH, 0.80, 26, 245, 225, 170)

    local cx = windowW / 2
    UI.add_centered_label(cx, panelY + 24, T("customise.customise"), UI_FONT_BOLD, 1.9)
    UI.add_centered_label(cx, panelY + 54, T("customise.coins") .. tostring(bankCoins or 0), UI_FONT_BOLD, 1.2)

    -- Tabs
    local tabW, tabH = 160, 40
    local tabY = panelY + 88
    local tabX = math.floor(cx - tabW - 10)
    local tabX2 = math.floor(cx + 10)

    local function tabBtn(label, id, active, x)
        local r, g, b = 80, 170, 255
        local a = active and 0.95 or 0.70
        UI.add_button(x, tabY, tabW, tabH, label, id,
            UI_FONT_BOLD, 1.0, tabH / 2, true,
            r, g, b, a
        )
    end

    tabBtn(T("customise.backgrounds"), "fb_tab_bg", customiseTab == "backgrounds", tabX)
    tabBtn(T("customise.birds"), "fb_tab_bird", customiseTab == "birds", tabX2)

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

        local label = LocalisedItemName(item)
        if equipped then
            label = label .. "  " .. T("customise.equipped")
        elseif owned then
            label = label .. "  " .. T("customise.owned")
        else
            label = label .. "  (" .. tostring(item.price) .. T("customise.price") .. ")"
        end

        UI.add_label(listX, y + 10, 0, 0, label, UI_FONT_BOLD, 1.15)

        local bw, bh = 140, 38
        local bx = listX + rowW - bw
        local by = y + 8

        local btnText, canPress, br, bg, bb, ba

        if equipped then
            btnText = T("customise.selected")
            canPress = false
            br, bg, bb, ba = 70, 200, 120, 0.92
        elseif owned then
            btnText = T("customise.equip")
            canPress = true
            br, bg, bb, ba = 160, 160, 160, 0.85
        else
            btnText = T("customise.buy")
            canPress = (bankCoins or 0) >= (item.price or 0)
            if canPress then
                br, bg, bb, ba = 255, 170, 80, 0.92
            else
                br, bg, bb, ba = 120, 120, 120, 0.65
            end
        end

        UI.add_button(bx, by, bw, bh, btnText, "fb_buy_" .. item.id,
            UI_FONT_BOLD, 1.0, bh / 2, true,
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
        for i, item in ipairs(STORE_BACKGROOUNDS or STORE_BACKGROUNDS) do
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
    UI.add_button(bx, by, bw, bh, T("customise.back"), "fb_customise_back",
        UI_FONT_BOLD, 1.0, bh / 2, true,
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
    -- Scroll the background horizontally on main menu
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
        UI.add_centered_label(cx, S(34), T("fb.paused"), UI_FONT_TITLE, 1.0)

        local btnW, btnH = S(260), S(46)
        local btnX = (panelW - btnW) / 2
        local startY = S(90)
        local gapY = S(14)

        local function AddBtn(row, text, id, r, g, b, a)
            UI.add_button(btnX, startY + (btnH + gapY) * row, btnW, btnH, text, id,
                UI_FONT_BOLD, 1.0, btnH / 2, true,
                r, g, b, a
            )
        end

        AddBtn(0, T("fb.resume"),   "fb_pause_resume",   80, 170, 255, 0.92)
        AddBtn(1, T("fb.restart"),  "fb_pause_restart",  255, 170, 80, 0.92)
        AddBtn(2, T("fb.settings"), "fb_pause_settings", 120, 220, 140, 0.90)
        AddBtn(3, T("fb.returntomainmenu"),"fb_pause_main",     255, 170, 80, 0.92)
        AddBtn(4, T("fb.exit"),     "fb_pause_exit",     220, 80, 80, 0.90)

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

        -- Escape to pause
        if Input.get_key_down(Keys.ionix_escape) then
            pauseGame(false)
            inMainMenu = false
            print("Resumed Game (Escape)")
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

        -- Escape to resume
        if Input.get_key_down(Keys.ionix_escape) then
            pauseGame(true)
            inMainMenu = true
            menuContext = "ingame"
            print("Paused Game (Escape)")
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
        UI.add_centered_label(lcx, 6, T("fb.leaderboard"), UI_FONT_HEADER, 1.0)

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
       local gap = 28

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
    for i, set in ipairs(pipeSets) do
        local pipeX = Mafs.get_vec_x(Fysics.get_pos(set.bottom))

        if pipeX < -60 then

            -- Added difficulty: shrink pipe gap as score increases (in pixels)
            local gapSize = pipeStartGap - (Pscore * pipeShrinkGap)
            if gapSize < pipeMinGap then
                gapSize = pipeMinGap
            end

            -- Random gap center Y position (in pixels)
            local gapCenter = pipesetMinGap +
                math.random() * (pipesetMaxGap - pipesetMinGap)

            if set.lastGapCenter then
                local delta = gapCenter - set.lastGapCenter
                if math.abs(delta) < 40 then
                    gapCenter = gapCenter + (delta > 0 and 60 or -60)
                end
            end
            set.lastGapCenter = gapCenter

            -- Calculate pipe positions (in pixels)
            -- bottomY is where the TOP edge of bottom pipe should be
            local bottomY = gapCenter + (gapSize / 2)
            -- topY is where the BOTTOM edge of top pipe should be (pipe extends upward)
            local topY = gapCenter - (gapSize / 2) - pipeHeight

            local spawnX = windowW + 60

            -- Apply positions
            Fysics.set_pos(set.bottom, spawnX, bottomY)
            Fysics.set_pos(set.top, spawnX, topY)

            set.passed = false
            
            -- Respawn the coin associated with this pipe set
            local c = coins[i]
            if c then
                spawnCoins(c, set, 140)
                
                local s = Entity.get_sprite_component(c)
                if s then
                    Sprite.set_width(s, 16)
                    Sprite.set_height(s, 16)
                end
                
                coinHidden[c] = false
                Fysics.set_linear_velocity(c, coinSpeed, 0)
            end
        end
    end

    -- Pipe passing logic
    local birdX = Mafs.get_vec_x(Fysics.get_pos(player1))
    for _, set in ipairs(pipeSets) do
        local pipeX = Mafs.get_vec_x(Fysics.get_pos(set.bottom))
        if birdX > pipeX and not set.passed then
            Pscore = Pscore + 1
            pipeScoreText = T("fb.score_label") .. tostring(Pscore)
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
    UI.add_label(10, 10, 1000, 1000, pipeScoreText)
    UI.add_label(10, 40, 1000, 1000, scoreText)

    ------------------------------
    -----------Raycast------------
    ---------------------------------
    local playerPos = Entity.get_center_pos(player1)
    local mousePos = Input.get_mouse_pos()
    local hit, info = Fysics.raycast(playerPos, mousePos)
    Fysics.draw_raycast(playerPos, mousePos, false)
    local hitEntity = Raycast.entity(info)
    if hit and hitEntity then
        -- Check if the hit entity is a coin
        local isCoin = false
        for _, c in ipairs(coins) do
            if c == hitEntity then
                isCoin = true
                break
            end
        end
        
        -- Only collect coins via raycast
        if isCoin and not coinHidden[hitEntity] then
            Fysics.draw_raycast(playerPos, mousePos, true)
            
            -- Hide the coin sprite
            local s = Entity.get_sprite_component(hitEntity)
            if s then
                Sprite.set_width(s, 0)
                Sprite.set_height(s, 0)
            end
            
            -- Move the coin physics body off-screen
            Fysics.set_pos(hitEntity, -1000, -1000)
            Fysics.set_linear_velocity(hitEntity, 0, 0)
            
            coinHidden[hitEntity] = true
            score = score + 1
            scoreText = T("fb.coins") .. tostring(score)
            
            -- Play coin sfx
            if coinSound then
                AudioComponent.play(coinSound)
            end
            
            -- Show point effect
            showPointEffect()
        end
    end



    -- UI.add_label(10, 10, 1000, 1000, pipeScoreText, UI_FONT_SUB, 1)
    -- UI.add_label(10, 40, 1000, 1000, scoreText, UI_FONT_SUB, 1)
    -- UI.add_centered_label(cx, cy, text1, UI_FONT_HEADER, 1)
    -- THIS MIGHT HAVE BEEN THE UI FUNCTIONS I SHOULD HAVE KEPT IN THE MERGE COMMIT
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
            
            -- Move the coin physics body off-screen
            Fysics.set_pos(c, -1000, -1000)
            Fysics.set_linear_velocity(c, 0, 0)
            
            coinHidden[c] = true
            score = score + 1
            scoreText = T("fb.coins") .. tostring(score)

            -- Output coin sfx
            if coinSound then
                AudioComponent.play(coinSound)
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