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

local leftMarker
local rightMarker

local playerState = "idle"
local attackTimer = 0
local playerHitRegistered = false

local aiAttackTimer = 0
local aiAttackCooldownTimer = 0
local aiHitRegistered = false

local playerStartX = 200
local playerStartY = 400
local aiStartX = 700
local aiStartY = 400

local aiSpeed = 70.0
local playerSpeed = 70.0
local aiSpeed = 5.0

local aiLeftX = 400
local aiRightX = 700
local aiDirection = -1

local currentRound = 1
local maxRounds = 5
local playerScore = 0
local aiScore = 0
local roundActive = true
local matchOver = false

local roundText = "Round: 1 / 5"
local scoreText = "Player 0  -  0 AI"
local resultText = ""

local maxHP = 1
local playerHP = maxHP
local aiHP = maxHP

local function resetFighters()
    if player then
        Entity.set_global_pos(player, playerStartX, playerStartY)
        Fysics.set_linear_velocity(player, 0, 0)
    end

    if ai then
        Entity.set_global_pos(ai, aiStartX, aiStartY)
    end

    playerState = "idle"
    attackTimer = 0
    playerHitRegistered = false

    aiAttackTimer = 0
    aiAttackCooldownTimer = 0
    aiHitRegistered = false

    playerHP = maxHP
    aiHP = maxHP
end

local function updateTexts()
    roundText = "Round: " .. tostring(currentRound) .. " / " .. tostring(maxRounds)
    scoreText = "Player " .. tostring(playerScore) .. "  -  " .. tostring(aiScore) .. " AI"
end

local function endRound(winner)
    if not roundActive or matchOver then
        return
    end

    roundActive = true

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

    -- Debug markers to visualise attack range around the player
    leftMarker = Entity.create_entity()
    rightMarker = Entity.create_entity()

    Entity.set_global_pos(leftMarker, playerStartX - attackRange, playerStartY)
    Entity.set_global_pos(rightMarker, playerStartX + attackRange, playerStartY)

    local leftSprite = Entity.add_sprite_component(leftMarker, assets.textures.Player, 4, 64, 5)
    Sprite.set_columns(leftSprite, 1)
    Sprite.set_width(leftSprite, 4)
    Sprite.set_height(leftSprite, 64)

    local rightSprite = Entity.add_sprite_component(rightMarker, assets.textures.Player, 4, 64, 5)
    Sprite.set_columns(rightSprite, 1)
    Sprite.set_width(rightSprite, 4)
    Sprite.set_height(rightSprite, 64)

    ai = Entity.create_entity()
    Entity.set_global_pos(ai, aiStartX, aiStartY)

    local aiSprite = Entity.add_sprite_component(ai, assets.textures.FlappyBird, 64, 64, 10)
    Sprite.set_columns(aiSprite, 1)

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

    if matchOver or not roundActive then
        UI.Add_label(20, 20, 400, 40, roundText)
        UI.Add_label(20, 60, 400, 40, scoreText)

        if resultText ~= "" then
            UI.Add_label(200, 20, 800, 60, resultText)
        end
        return
    end

    local vx = 0

    if Input.get_key_held(Keys.ionix_d) then
        vx = playerSpeed
    elseif Input.get_key_held(Keys.ionix_a) then
        vx = -playerSpeed
    end

    -- Preserve vertical velocity so gravity and jumping (if added later) still work
    local vel = Fysics.get_linear_velocity(player)
    local vy = Mafs.get_vec_y(vel)

    Fysics.set_linear_velocity(player, vx, vy)

    -- Lock player to a fixed Y so they don't stick to ceiling or fall
    local pos = Fysics.get_pos(player)
    local px = Mafs.get_vec_x(pos)
    local py = playerStartY
    Fysics.set_pos(player, px, py)

    if playerIdle then
        Entity.set_global_pos(playerIdle, px, py)
    end
    if playerWalk then
        Entity.set_global_pos(playerWalk, px, py)
    end
    if playerAttack then
        Entity.set_global_pos(playerAttack, px, py)
    end

    -- Get AI position (kinematic entity) and lock to fixed Y
    local aiPos = Entity.get_global_pos(ai)
    local aix = Mafs.get_vec_x(aiPos)
    local aiy = aiStartY

    -- AI behaviour (kinematic horizontal movement)
    -- If AI is to the right of the player, move left; if left, move right
    local dx = aix - px
    local absDx = Mafs.abs(dx)

    local aiMoveDir = 0

    -- Spacing controller:
    -- - If too far: step toward player
    -- - If too close: step away from player (prevents player slipping behind)
    -- - Otherwise: hold position (attack / idle)
    local chaseStart = attackRange
    local retreatStart = attackRange * 0.55

    if absDx > chaseStart then
        if dx > 0 then
            aiMoveDir = -1   -- AI is right of player: move left toward player
        else
            aiMoveDir = 1    -- AI is left of player: move right toward player
        end
    elseif absDx < retreatStart then
        if dx > 0 then
            aiMoveDir = 1    -- AI is right of player: move further right (retreat)
        else
            aiMoveDir = -1   -- AI is left of player: move further left (retreat)
        end
    else
        aiMoveDir = 0
    end

    local dt = Mafs.delta_time()

    if aiAttackCooldownTimer > 0 then
        aiAttackCooldownTimer = aiAttackCooldownTimer - dt
        if aiAttackCooldownTimer < 0 then
            aiAttackCooldownTimer = 0
        end
    end

    if aiMoveDir ~= 0 then
        local newAix = aix + aiMoveDir * aiSpeed * dt

        -- Keep AI roughly inside the window horizontally
        if newAix < 0 then newAix = 0 end
        if newAix > 960 then newAix = 960 end

        Entity.set_global_pos(ai, newAix, aiy)
    end

    -- Move debug markers to show current attack range around the player
    if leftMarker then
        Entity.set_global_pos(leftMarker, px - attackRange, py)
    end
    if rightMarker then
        Entity.set_global_pos(rightMarker, px + attackRange, py)
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
            playerHitRegistered = false
        else
            if isMoving then
                playerState = "walk"
            else
                playerState = "idle"
            end
        end
    end

    -- Player hit check (simple distance-based hitbox during a small attack window)
    if playerState == "attack" and attackTimer > 0 and not playerHitRegistered then
        local inHitWindow = attackTimer < 0.28 and attackTimer > 0.12
        if inHitWindow and absDx <= attackRange then
            aiHP = aiHP - 1
            playerHitRegistered = true
            if aiHP <= 0 then
                endRound("player")
                return
            end
        end
    end

    -- AI attack logic + hit check
    if aiAttackTimer > 0 then
        aiAttackTimer = aiAttackTimer - dt
        if aiAttackTimer <= 0 then
            aiAttackTimer = 0
        end
    else
        if aiAttackCooldownTimer <= 0 and absDx <= attackRange then
            aiAttackTimer = 0.4
            aiAttackCooldownTimer = 0.8
            aiHitRegistered = false
        end
    end

    if aiAttackTimer > 0 and not aiHitRegistered then
        local inHitWindow = aiAttackTimer < 0.28 and aiAttackTimer > 0.12
        if inHitWindow and absDx <= attackRange then
            playerHP = playerHP - 1
            aiHitRegistered = true
            if playerHP <= 0 then
                endRound("ai")
                return
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
        local aiPos = Fysics.get_pos(ai)
        local ax = Mafs.get_vec_x(aiPos)
        local ay = Mafs.get_vec_y(aiPos)

        if ax <= aiLeftX then
            ax = aiLeftX
            aiDirection = 1
        elseif ax >= aiRightX then
            ax = aiRightX
            aiDirection = -1
        end

        Entity.set_global_pos(ai, ax, ay)

        local avx = aiSpeed * aiDirection
        Fysics.set_linear_velocity(ai, avx, 0)
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

