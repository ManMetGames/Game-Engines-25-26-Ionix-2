local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
--Game Objects
local Background
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

    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
end
return ExampleScript