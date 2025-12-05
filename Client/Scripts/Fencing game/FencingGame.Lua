local FencingGame = {}

local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

local player
local ai

local ground
local screenW
local screenH

local playerStartX = 200
local playerStartY = 400
local aiStartX = 700
local aiStartY = 400

local playerSpeed = 3.0

local currentRound = 1
local maxRounds = 5
local playerScore = 0
local aiScore = 0
local roundActive = true
local matchOver = false

local roundText = "Round: 1 / 5"
local scoreText = "Player 0  -  0 AI"
local resultText = ""

local function resetFighters()
    if player then
        Entity.set_global_pos(player, playerStartX, playerStartY)
        Fysics.set_linear_velocity(player, 0, 0)
    end

    if ai then
        Entity.set_global_pos(ai, aiStartX, aiStartY)
        Fysics.set_linear_velocity(ai, 0, 0)
    end
end

local function updateTexts()
    roundText = "Round: " .. tostring(currentRound) .. " / " .. tostring(maxRounds)
    scoreText = "Player " .. tostring(playerScore) .. "  -  " .. tostring(aiScore) .. " AI"
end

local function endRound(winner)
    if not roundActive or matchOver then
        return
    end

    roundActive = false

    if winner == "player" then
        playerScore = playerScore + 1
        resultText = "Player wins the round!"
    elseif winner == "ai" then
        aiScore = aiScore + 1
        resultText = "AI wins the round!"
    else
        resultText = "Round ended."
    end

    if playerScore > maxRounds / 2 or aiScore > maxRounds / 2 or currentRound >= maxRounds then
        matchOver = true
        if playerScore > aiScore then
            resultText = resultText .. "  Match over: Player wins!"
        elseif aiScore > playerScore then
            resultText = resultText .. "  Match over: AI wins!"
        else
            resultText = resultText .. "  Match over: Draw!"
        end
    else
        currentRound = currentRound + 1
        resetFighters()
        roundActive = true
    end

    updateTexts()
end

function FencingGame:OnStart()
    player = Entity.create_entity()
    Entity.set_global_pos(player, playerStartX, playerStartY)

    local playerSprite = Entity.add_sprite_component(player, assets.textures.FlappyBird, 64, 64, 10)
    Sprite.set_columns(playerSprite, 1)

    Entity.add_fysics_component(player, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player, false, 1)

    ai = Entity.create_entity()
    Entity.set_global_pos(ai, aiStartX, aiStartY)

    local aiSprite = Entity.add_sprite_component(ai, assets.textures.FlappyBird, 64, 64, 10)
    Sprite.set_columns(aiSprite, 1)

    Entity.add_fysics_component(ai, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(ai, false, 1)

    screenW = Window.get_width()
    screenH = Window.get_height()

    ground = Entity.create_entity()
    Entity.set_global_pos(ground, screenW / 2, screenH - 20)
    Entity.add_fysics_component(ground, enums.bodytype.staticBody, false)
    Fysics.add_box_collider(ground, screenW / 2, 10, false)

    updateTexts()
end

function FencingGame:OnUpdate()
    if not player or not ai then
        return
    end

    local vel = Fysics.get_linear_velocity(player)
    local vx = vel.x
    local vy = vel.y

    if Input.get_key_held(Keys.ionix_d) then
        vx = playerSpeed
    elseif Input.get_key_held(Keys.ionix_a) then
        vx = -playerSpeed
    else
        vx = 0
    end

    Fysics.set_linear_velocity(player, vx, vy)

    if roundActive and not matchOver then
    end

    UI.Add_label(20, 20, 400, 40, roundText)
    UI.Add_label(20, 60, 400, 40, scoreText)

    if resultText ~= "" then
        UI.Add_label(200, 20, 800, 60, resultText)
    end
end

function FencingGame:OnTriggerEnter(c1, c2)
end

return FencingGame

