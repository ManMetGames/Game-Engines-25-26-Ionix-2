local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
--Game Objects
local Background
local Floor
local Player
--Player Settings
local playerX = 150
local playerY = 400
local playerWidth = 50
local playerHeight = 50
--Floor Settings
local FloorY = 500
local FloorHeight = 20
local x = 200
local y = 300
local t = 10
local coinCount = 0

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
end
return ExampleScript