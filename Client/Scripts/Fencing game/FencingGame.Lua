local FencingGame = {}

local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

local player
local ai

local ground

local playerIdle
local playerWalk
local playerAttack

local playerIdleSprite
local playerWalkSprite
local playerAttackSprite

local playerState = "idle"
local attackTimer = 0

local playerStartX = 200
local playerStartY = 400
local aiStartX = 700
local aiStartY = 400

local playerSpeed = 70.0

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
    local playerColliderSprite = Entity.add_sprite_component(player, assets.textures.Player, 128, 128, 5)
    Sprite.set_rows(playerColliderSprite, 1)
    Sprite.set_columns(playerColliderSprite, 1)
    Sprite.set_width(playerColliderSprite, 0)
    Sprite.set_height(playerColliderSprite, 0)

    Entity.add_fysics_component(player, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player, false, 1)

    playerIdle = Entity.create_entity()
    Entity.set_global_pos(playerIdle, playerStartX, playerStartY)
    playerIdleSprite = Entity.add_sprite_component(playerIdle, assets.textures.Idle, 128, 128, 10)
    Sprite.set_rows(playerIdleSprite, 1)
    Sprite.set_columns(playerIdleSprite, 4)

    playerWalk = Entity.create_entity()
    Entity.set_global_pos(playerWalk, playerStartX, playerStartY)
    playerWalkSprite = Entity.add_sprite_component(playerWalk, assets.textures.Walk, 128, 128, 10)
    Sprite.set_rows(playerWalkSprite, 1)
    Sprite.set_columns(playerWalkSprite, 8)

    playerAttack = Entity.create_entity()
    Entity.set_global_pos(playerAttack, playerStartX, playerStartY)
    playerAttackSprite = Entity.add_sprite_component(playerAttack, assets.textures.Attack_3, 128, 128, 10)
    Sprite.set_rows(playerAttackSprite, 1)
    Sprite.set_columns(playerAttackSprite, 4)

    Sprite.set_width(playerIdleSprite, 128)
    Sprite.set_height(playerIdleSprite, 128)
    Sprite.set_width(playerWalkSprite, 0)
    Sprite.set_height(playerWalkSprite, 0)
    Sprite.set_width(playerAttackSprite, 0)
    Sprite.set_height(playerAttackSprite, 0)

    ai = Entity.create_entity()
    Entity.set_global_pos(ai, aiStartX, aiStartY)

    local aiSprite = Entity.add_sprite_component(ai, assets.textures.FlappyBird, 64, 64, 10)
    Sprite.set_columns(aiSprite, 1)

    Entity.add_fysics_component(ai, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(ai, false, 1)

    ground = Entity.create_entity()
    Entity.set_global_pos(ground, 960 / 2, 640 - 5)
    Entity.add_fysics_component(ground, enums.bodytype.staticBody, false)
    Fysics.add_box_collider(ground, 960 / 2, 10, false)

    updateTexts()
end

function FencingGame:OnUpdate()
    if not player or not ai then
        return
    end

    local vx = 0
    local vy = 0

    if Input.get_key_held(Keys.ionix_d) then
        vx = playerSpeed
    elseif Input.get_key_held(Keys.ionix_a) then
        vx = -playerSpeed
    end

    Fysics.set_linear_velocity(player, vx, vy)

    local pos = Fysics.get_pos(player)
    local px = Mafs.get_vec_x(pos)
    local py = Mafs.get_vec_y(pos)

    if playerIdle then
        Entity.set_global_pos(playerIdle, px, py)
    end
    if playerWalk then
        Entity.set_global_pos(playerWalk, px, py)
    end
    if playerAttack then
        Entity.set_global_pos(playerAttack, px, py)
    end

    local isMoving = vx ~= 0

    if attackTimer > 0 then
        attackTimer = attackTimer - Mafs.delta_time()
        if attackTimer <= 0 then
            if isMoving then
                playerState = "walk"
            else
                playerState = "idle"
            end
        end
    else
        if Input.get_mouse_button_down(1) then
            playerState = "attack"
            attackTimer = 0.4
        else
            if isMoving then
                playerState = "walk"
            else
                playerState = "idle"
            end
        end
    end

    if playerIdleSprite and playerWalkSprite and playerAttackSprite then
        if playerState == "idle" then
            Sprite.set_width(playerIdleSprite, 128)
            Sprite.set_height(playerIdleSprite, 128)
            Sprite.set_width(playerWalkSprite, 0)
            Sprite.set_height(playerWalkSprite, 0)
            Sprite.set_width(playerAttackSprite, 0)
            Sprite.set_height(playerAttackSprite, 0)
        elseif playerState == "walk" then
            Sprite.set_width(playerIdleSprite, 0)
            Sprite.set_height(playerIdleSprite, 0)
            Sprite.set_width(playerWalkSprite, 128)
            Sprite.set_height(playerWalkSprite, 128)
            Sprite.set_width(playerAttackSprite, 0)
            Sprite.set_height(playerAttackSprite, 0)
        elseif playerState == "attack" then
            Sprite.set_width(playerIdleSprite, 0)
            Sprite.set_height(playerIdleSprite, 0)
            Sprite.set_width(playerWalkSprite, 0)
            Sprite.set_height(playerWalkSprite, 0)
            Sprite.set_width(playerAttackSprite, 128)
            Sprite.set_height(playerAttackSprite, 128)
        end
    end

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

