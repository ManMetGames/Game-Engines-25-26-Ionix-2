local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
--Game Objects
local Background
local Floor
local Player
local Enemies = {}
--Player Settings
local playerX = 150
local playerY = 400
local playerWidth = 50
local playerHeight = 50
local isJumping = false
local jumpForce = -15000
--Floor Settings
local FloorY = 500
local FloorHeight = 20
-- Enemy Settings
local enemySpawnX = 1000
local enemyWidth = 30
local enemyHeight = 50

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    --Create Background
    Background = Entity.create_entity()
    local BgBackground = Entity.add_sprite_component(Background, assets.textures.Background,960 , 640, 0)
    Entity.set_global_pos(Background, 480, 320)

    -- Create Floor
    Floor = Entity.create_entity()
    Entity.add_sprite_component(Floor, assets.textures.Sand, 960, floorHeight, 1)
    Entity.set_global_pos(Floor, 480, floorY)

    -- Add physics to floor
    Entity.add_fysics_component(Floor)
    Fysics.add_box_collider(Floor, 480, floorHeight/2, 0, 0, 0, false)
    Fysics.set_gravity_scale(Floor, 0)
    Fysics.set_linear_velocity(Floor, 0, 0)
    ------------------------------------------------------
    -- Create Player
    ------------------------------------------------------
    Player = Entity.create_entity()
    Entity.add_sprite_component(Player, assets.textures.FlappyBird, playerWidth, playerHeight, 2)
    Entity.set_global_pos(Player, playerX, playerY)

    -- Add physics to player
    Entity.add_fysics_component(Player)
    Fysics.add_box_collider(Player, playerWidth/2, playerHeight/2, 0, 0, 0, false)
    Fysics.set_fixed_rotation(Player, true)
    Fysics.set_linear_damping(Player, 0.5)

    -- Setup collision detection
    Fysics.add_to_collision_map(Player, Floor)

    print("Player and Floor created successfully!")
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()

    -- Player Jump Input
    if Input.get_key_down(Keys.ionix_space) then

        -- Check if player is on ground
        local vel = Fysics.get_linear_velocity(Player)
        if math.abs(vel.y) < 1.0 then
            Fysics.add_impulse_to_center(Player, 0, jumpForce)
            print("Jump!")
        end
    end
    -- Update and cleanup enemies
    local i = 1
    while i <= #Enemies do
        local enemy = Enemies[i]
        
        if enemy ~= nil then
            -- Move enemy left
            local pos = Entity.get_global_pos(enemy)
            Entity.set_global_pos(enemy, pos.x - gameSpeed * Mafs.delta_time(), pos.y)
            
            -- Check collision with player
            if Fysics.col(Player, enemy) then
                print("Collision! Game Over!")
                gameOver = true
            end
            
            -- Remove enemy if off screen
            if pos.x < -50 then
                Entity.destroy_entity(enemy)
                table.remove(Enemies, i)
            else
                i = i + 1
            end
        else
            i = i + 1
        end
    end
end

----------------------------------------------------------
-- Spawn Enemy
----------------------------------------------------------
function ExampleScript:SpawnEnemy()
    local enemy = Entity.create_entity()
    Entity.add_sprite_component(enemy, assets.textures.PimBall, enemyWidth, enemyHeight, 2)
    Entity.set_global_pos(enemy, enemySpawnX, floorY - enemyHeight/2 - floorHeight/2)
    
    -- Add physics to enemy
    Entity.add_fysics_component(enemy)
    Fysics.add_box_collider(enemy, enemyWidth/2, enemyHeight/2, 0, 0, 0, false)
    Fysics.set_gravity_scale(enemy, 0)
    Fysics.set_fixed_rotation(enemy, true)
    
    -- Setup collision detection with player
    Fysics.add_to_collision_map(Player, enemy)
    
    table.insert(Enemies, enemy)
    print("Enemy spawned! Total enemies: " .. #Enemies)
end

return ExampleScript