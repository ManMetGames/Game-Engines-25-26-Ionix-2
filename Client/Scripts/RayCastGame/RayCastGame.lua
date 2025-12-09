local ExampleScript = {}
local assets = require("Scripts.Assets")


local Background
local ScreenH = 960
local ScreenW = 640

local Player
local PlayerStartPosX = ScreenH / 2
local PlayerStartPosY = ScreenW - 150
local PlayerSprite

local Bullet
local BulletSprite
local BulletSpeed

local t = 10

--Duck Variables
local Duck
local DuckSprite
local DuckFlySpeed

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    ------------------------------------------------------
	-- Background Texture
	------------------------------------------------------
    Background = Entity.create_entity()
    local BgBackground = Entity.add_sprite_component(Background, assets.textures.backgroundDuckHunt,ScreenH , ScreenW, 0)

    ------------------------------------------------------
    -- Create Player (Shooter)
    ------------------------------------------------------
    Player = Entity.create_entity()

    Entity.set_entity_pos(Player, PlayerStartPosX, PlayerStartPosY)  
	
    local PlayerSprite = Entity.add_sprite_component(Player, assets.textures.NEWcrosshair, 25, 23, 1)
    Sprite.set_columns(PlayerSprite,1)
    -- PLAYER 1 PHYSICS
    Entity.add_fysics_component(Player, 2, false) -- dynamic body
    Fysics.add_sprite_collider(Player ,false, 1)
    -- Freeze bird
    Fysics.set_gravity_scale(Player, 0)













end
----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------

    

return ExampleScript