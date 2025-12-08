local CoinCollectorGame = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

-- Game entities
local player
local coins = {}
local obstacles = {}
local Background

-- Game state
local score = 0
local gameStarted = false
local gameOver = false
local playerSpeed = 300
local obstacleSpeed = 150

-- Spawn timers
local coinSpawnTimer = 0
local coinSpawnInterval = 1.5
local obstacleSpawnTimer = 0
local obstacleSpawnInterval = 3.0

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function CoinCollectorGame:OnStart()
    
    ------------------------------------------------------
    -- Background
    ------------------------------------------------------
    Background = Entity.create_entity()
    Entity.set_global_pos(Background, 480, 320)
    local bgSprite = Entity.add_sprite_component(Background, assets.textures.Background, 960, 640, 0)
    Sprite.set_columns(bgSprite, 1)
    
    ------------------------------------------------------
    -- Create Player
    ------------------------------------------------------
    player = Entity.create_entity()
    Entity.set_global_pos(player, 480, 500)
    
    local playerSprite = Entity.add_sprite_component(player, assets.textures.FlappyBird, 48, 48, 10)
    Sprite.set_columns(playerSprite, 1)
    
    -- Add physics to player
    Entity.add_fysics_component(player, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player, false, 1)
    Fysics.set_gravity_scale(player, 0) -- No gravity for top-down movement
    Fysics.set_fixed_rotation(player, true) -- Don't rotate
    
    ------------------------------------------------------
    -- Create Floor (boundaries)
    ------------------------------------------------------
    local tileSize = 64
    local floorY = 600
    
    for i = 0, 15 do
        local tile = Entity.create_entity()
        local xPos = i * tileSize
        Entity.set_global_pos(tile, xPos, floorY)
        local s = Entity.add_sprite_component(tile, assets.textures.Sand, tileSize, tileSize, 1)
        Sprite.set_columns(s, 1)
        Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)
        Fysics.add_sprite_collider(tile, false, 1)
    end
    
    -- Top boundary
    for i = 0, 15 do
        local tile = Entity.create_entity()
        local xPos = i * tileSize
        Entity.set_global_pos(tile, xPos, 0)
        local s = Entity.add_sprite_component(tile, assets.textures.Sand, tileSize, tileSize, 1)
        Sprite.set_columns(s, 1)
        Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)
        Fysics.add_sprite_collider(tile, false, 1)
    end
    
    ------------------------------------------------------
    -- Spawn initial coins
    ------------------------------------------------------
    for i = 1, 3 do
        spawnCoin()
    end
end

----------------------------------------------------------
-- Spawn Coin Function
----------------------------------------------------------
function spawnCoin()
    local coin = Entity.create_entity()
    
    -- Random position within play area
    local x = math.random(100, 860)
    local y = math.random(100, 500)
    Entity.set_global_pos(coin, x, y)
    
    local coinSprite = Entity.add_sprite_component(coin, assets.textures.Coin, 32, 32, 5)
    Sprite.set_rows(coinSprite, 1)
    Sprite.set_columns(coinSprite, 5)
    Sprite.set_width(coinSprite, 32)
    Sprite.set_height(coinSprite, 32)
    
    -- Add physics with trigger collider
    Entity.add_fysics_component(coin, enums.bodytype.kinematicBody, false)
    Fysics.add_sprite_collider(coin, true, 1)
    Fysics.set_gravity_scale(coin, 0)
    
    table.insert(coins, {entity = coin, collected = false})
end

----------------------------------------------------------
-- Spawn Obstacle Function
----------------------------------------------------------
function spawnObstacle()
    local obstacle = Entity.create_entity()
    
    -- Spawn on right side, move left
    local x = 1000
    local y = math.random(100, 500)
    Entity.set_global_pos(obstacle, x, y)
    
    local obstacleSprite = Entity.add_sprite_component(obstacle, assets.textures.FlappyPipe, 60, 100, 3)
    Sprite.set_columns(obstacleSprite, 1)
    
    -- Add physics
    Entity.add_fysics_component(obstacle, enums.bodytype.kinematicBody, false)
    Fysics.add_sprite_collider(obstacle, false, 1)
    Fysics.set_gravity_scale(obstacle, 0)
    Fysics.set_linear_velocity(obstacle, -obstacleSpeed, 0)
    
    table.insert(obstacles, {entity = obstacle, active = true})
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function CoinCollectorGame:OnUpdate()
    
    local dt = Mafs.delta_time()
    
    ------------------------------------------------------
    -- UI Display
    ------------------------------------------------------
    if not gameStarted then
        UI.Add_label(300, 250, 400, 100, "Press SPACE to Start!")
        UI.Add_label(250, 300, 500, 100, "Arrow Keys to Move")
        UI.Add_label(250, 350, 500, 100, "Collect Coins, Avoid Pipes!")
        
        if Input.get_key_down(Keys.ionix_space) then
            gameStarted = true
        end
        return
    end
    
    if gameOver then
        UI.Add_label(350, 250, 300, 100, "GAME OVER!")
        UI.Add_label(300, 300, 400, 100, "Final Score: " .. score)
        UI.Add_label(300, 350, 400, 100, "Press R to Restart")
        
        if Input.get_key_down(Keys.ionix_r) then
            -- Simple restart by resetting score
            score = 0
            gameOver = false
            gameStarted = false
        end
        return
    end
    
    -- Display score
    UI.Add_label(20, 20, 200, 50, "Score: " .. score)
    
    ------------------------------------------------------
    -- Player Movement
    ------------------------------------------------------
    local vel = Mafs.vector2(0, 0)
    
    if Input.get_key_held(Keys.ionix_left) or Input.get_key_held(Keys.ionix_a) then
        vel = Mafs.vector2(-playerSpeed, Mafs.get_vec_y(vel))
    end
    if Input.get_key_held(Keys.ionix_right) or Input.get_key_held(Keys.ionix_d) then
        vel = Mafs.vector2(playerSpeed, Mafs.get_vec_y(vel))
    end
    if Input.get_key_held(Keys.ionix_up) or Input.get_key_held(Keys.ionix_w) then
        vel = Mafs.vector2(Mafs.get_vec_x(vel), -playerSpeed)
    end
    if Input.get_key_held(Keys.ionix_down) or Input.get_key_held(Keys.ionix_s) then
        vel = Mafs.vector2(Mafs.get_vec_x(vel), playerSpeed)
    end
    
    Fysics.set_linear_velocity(player, Mafs.get_vec_x(vel), Mafs.get_vec_y(vel))
    
    ------------------------------------------------------
    -- Spawn Coins
    ------------------------------------------------------
    coinSpawnTimer = coinSpawnTimer + dt
    if coinSpawnTimer >= coinSpawnInterval then
        coinSpawnTimer = 0
        spawnCoin()
    end
    
    ------------------------------------------------------
    -- Spawn Obstacles
    ------------------------------------------------------
    obstacleSpawnTimer = obstacleSpawnTimer + dt
    if obstacleSpawnTimer >= obstacleSpawnInterval then
        obstacleSpawnTimer = 0
        spawnObstacle()
    end
    
    ------------------------------------------------------
    -- Clean up off-screen obstacles
    ------------------------------------------------------
    for i = #obstacles, 1, -1 do
        local obs = obstacles[i]
        if obs.active then
            local pos = Fysics.get_pos(obs.entity)
            if Mafs.get_vec_x(pos) < -100 then
                obs.active = false
                table.remove(obstacles, i)
            end
        end
    end
    
    ------------------------------------------------------
    -- Remove collected coins
    ------------------------------------------------------
    for i = #coins, 1, -1 do
        if coins[i].collected then
            table.remove(coins, i)
        end
    end
end
----------------------------------------------------------
-- OnTriggerEnter - Coin Collection
----------------------------------------------------------
function CoinCollectorGame:OnTriggerEnter(entity1, entity2)
    if gameOver then return end
    
    -- Check if player collided with a coin
    local player_entity, other_entity
    
    if entity1 == player then
        player_entity = entity1
        other_entity = entity2
    elseif entity2 == player then
        player_entity = entity2
        other_entity = entity1
    end
    
    if player_entity then
        -- Check if it's a coin
        for i, coinData in ipairs(coins) do
            if coinData.entity == other_entity and not coinData.collected then
                -- Collect coin
                coinData.collected = true
                score = score + 10
                
                -- Hide coin sprite
                local sprite = Entity.get_sprite_component(other_entity)
                if sprite then
                    Sprite.set_width(sprite, 0)
                    Sprite.set_height(sprite, 0)
                end
                
                print("Coin collected! Score: " .. score)
                break
            end
        end
    end
end

----------------------------------------------------------
-- OnCollisionEnter - Obstacle Hit
----------------------------------------------------------
function CoinCollectorGame:OnCollisionEnter(entity1, entity2)
    if gameOver then return end
    
    -- Check if player hit an obstacle
    if entity1 == player or entity2 == player then
        for _, obsData in ipairs(obstacles) do
            if entity1 == obsData.entity or entity2 == obsData.entity then
                gameOver = true
                print("Game Over! Hit obstacle!")
                break
            end
        end
    end
end

return CoinCollectorGame