-- TopDownCollect.lua
local TopDownCollect = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

-- Entities
local background
local player
local enemy
local coins = {}
local coinHidden = {}
local coinCount = 8

-- Gameplay
local speed = 180           -- player speed
local score = 0
local scoreText = "Score: 0"
local startText = "Use ARROWS or WASD to move. Collect coins."
local arenaW = 960
local arenaH = 640

-- enemy
local enemySpeed = 120
local enemyDir = 1 -- 1 = right, -1 = left
local enemyLeftX = 120
local enemyRightX = 840

local function clamp(v, a, b)
    if v < a then return a end
    if v > b then return b end
    return v
end

function TopDownCollect:OnStart()
    -- Background
    background = Entity.create_entity()
    if assets.textures.Background then
        local bg = Entity.add_sprite_component(background, assets.textures.Background, 960, 640, -20)
        Sprite.set_columns(bg, 1)
    end

    -- Player
    player = Entity.create_entity()
    Entity.set_global_pos(player, arenaW * 0.5, arenaH * 0.6)
    local pSprite = Entity.add_sprite_component(player,assets.textures.FlappyBird, 48, 48, -5)
    Sprite.set_columns(pSprite, 1)

    Entity.add_fysics_component(player, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player, false, 1)

    -- keep player upright
    Fysics.set_gravity_scale(player, 0)
    Fysics.set_fixed_rotation(player, true)

    -- Enemy
    enemy = Entity.create_entity()
    Entity.set_global_pos(enemy, enemyLeftX, arenaH * 0.35)
    local eSprite = Entity.add_sprite_component(enemy,assets.textures.FlappyPipe, 56, 56, -10)
    Sprite.set_columns(eSprite, 1)
    Entity.add_fysics_component(enemy, enums.bodytype.kinematicBody, false)
    Fysics.add_sprite_collider(enemy, false, 1)

    -- Create coins placed around the arena
    for i = 1, coinCount do
        local c = Entity.create_entity()
        local margin = 80
        local cx = math.random(margin, arenaW - margin)
        local cy = math.random(margin + 60, arenaH - margin)
        Entity.set_global_pos(c, cx, cy)

        local coinSprite = Entity.add_sprite_component(c, assets.textures.Coin or assets.textures.Coin, 24, 24, 8)
        Sprite.set_rows(coinSprite, 1)
        Sprite.set_columns(coinSprite, 5)
        Sprite.set_width(coinSprite, 16)
        Sprite.set_height(coinSprite, 16)

        Entity.add_fysics_component(c, enums.bodytype.kinematicBody, false)
        -- make coin a trigger so player can pass through and collect
        Fysics.add_sprite_collider(c, true, 1)

        table.insert(coins, c)
        coinHidden[c] = false
    end

  

    
end

function TopDownCollect:OnUpdate()
    -- show score
    scoreText = "Score: " .. tostring(score)
    UI.Add_label(10, 10, 200, 30, scoreText)

    -- Player movement: read input and set linear velocity
    local vx = 0
    local vy = 0
    if Input.get_key_held(Keys.ionix_left) or Input.get_key_held(Keys.ionix_a) then
        vx = vx - 1
    end
    if Input.get_key_held(Keys.ionix_right) or Input.get_key_held(Keys.ionix_d) then
        vx = vx + 1
    end
    if Input.get_key_held(Keys.ionix_up) or Input.get_key_held(Keys.ionix_w) then
        vy = vy - 1
    end
    if Input.get_key_held(Keys.ionix_down) or Input.get_key_held(Keys.ionix_s) then
        vy = vy + 1
    end
    
    local pPos = Fysics.get_pos(player)
    print("Player X:", Mafs.get_vec_x(pPos))

    -- normalize to avoid faster diagonal speed
    local mag = math.sqrt(vx * vx + vy * vy)
    if mag > 0 then
        vx = (vx / mag) * speed
        vy = (vy / mag) * speed
    end

    Fysics.set_linear_velocity(player, vx, vy)

    -- clamp player position to arena
    local pPos = Fysics.get_pos(player)
    local px = Mafs.get_vec_x(pPos)
    local py = Mafs.get_vec_y(pPos)

    px = math.max(24, math.min(arenaW - 24, px))
    py = math.max(48, math.min(arenaH - 24, py))

    Fysics.set_pos(player, px, py)

    -- Enemy patrol logic (move horizontally; if reached bounds, reverse)
    local eVelX = enemySpeed * enemyDir
    Fysics.set_linear_velocity(enemy, eVelX, 0)

    local ePos = Fysics.get_pos(enemy)
    local ex = Mafs.get_vec_x(ePos)
    if ex >= enemyRightX then
        enemyDir = -1
    elseif ex <= enemyLeftX then
        enemyDir = 1
    end
end

-- Collision when player physically hits enemy
function TopDownCollect:OnCollisionEnter(a, b)
    -- If either is player and other is enemy -> punish
    if (a == player and b == enemy) or (b == player and a == enemy) then
        print("Player hit enemy! Resetting player and score.")
        -- respawn player to center
        Fysics.set_pos(player, arenaW * 0.5, arenaH * 0.6)
        Fysics.set_linear_velocity(player, 0, 0)
        score = 0
    end
end

-- Trigger based collisions (coins)
function TopDownCollect:OnTriggerEnter(c1, c2)
    -- ensure coinHidden exists
    coinHidden = coinHidden or {}

    local playerEntity, other
    if c1 == player then
        playerEntity = c1
        other = c2
    elseif c2 == player then
        playerEntity = c2
        other = c1
    end

    if not playerEntity then
        return
    end

    -- check if 'other' is one of our coins and not already collected
    for i, coin in ipairs(coins) do
        if other == coin and (coinHidden[coin] == false or coinHidden[coin] == nil) then
            -- collect
            print("Coin collected!")
            local s = Entity.get_sprite_component(coin)
            if s then
                Sprite.set_width(s, 0)
                Sprite.set_height(s, 0)
            end
            coinHidden[coin] = true
            score = score + 5
            scoreText = "Score: " .. tostring(score)
            break
        end
    end
end

return TopDownCollect