local ExampleScript = {}
local assets = require("Scripts.Assets")


local Background
local ScreenH = Window.get_width()
local ScreenW = Window.get_height()

local Player
local PlayerStartPosX = Window.get_height() / 2
local PlayerStartPosY = Window.get_width() - 150
local PlayerSprite

local Bullet
local BulletSprite
local BulletSpeed

local StartTimer = true

local t = 10.0

--Duck Variables
local DuckLeft
local DuckLeftSprite
local DuckLeftFlySpeed

local DuckRight
local DuckRightSprite
local DuckRightFlySpeed

local SpawnPointLeft = Mafs.vector2(-75,450)
local SpawnPointRight = Mafs.vector2(Window.get_width(), 450)

local SendLeftDuck = true
local SendRightDuck = false

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


    ------------------------------------------------------
    -- Duck Left
    ------------------------------------------------------

    DuckLeft = Entity.create_entity()

    Entity.set_entity_pos(DuckLeft, Mafs.vector2_x(SpawnPointLeft), Mafs.vector2_y(SpawnPointLeft))  
	
    local DuckLeftSprite = Entity.add_sprite_component(DuckLeft, assets.textures.DuckLeft, 75, 75, 1)
    Sprite.set_columns(DuckLeftSprite,1)
    -- DUCK 1 PHYSICS
    Entity.add_fysics_component(DuckLeft, 2, true) -- dynamic body
    Fysics.add_sprite_collider(DuckLeft ,false, 1)
    -- Freeze bird
    Fysics.set_gravity_scale(DuckLeft, 0)



 ------------------------------------------------------
    -- Duck Right
    ------------------------------------------------------

    DuckRight = Entity.create_entity()
    Entity.set_entity_pos(DuckRight, Mafs.vector2_x(SpawnPointRight), Mafs.vector2_y(SpawnPointRight) - 1500)  
    local DuckRightSprite = Entity.add_sprite_component(DuckRight, assets.textures.DuckRight, 75, 75, 1)
    Sprite.set_columns(DuckRightSprite,1)
    -- DUCK 1 PHYSICS
    Entity.add_fysics_component(DuckRight, 2, true) -- dynamic body
    Fysics.add_sprite_collider(DuckRight ,false, 1)
    -- Freeze bird
    Fysics.set_gravity_scale(DuckRight, 0)




end
----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    if Input.get_key_down(Keys.ionix_space) then
        Fysics.set_gravity_scale(DuckLeft, 1)
         Fysics.add_force(DuckLeft, 220,-250,1,1)         
	end

    local DuckLeftPos = Mafs.vector2_y(Entity.get_entity_pos(DuckLeft))
    if DuckLeftPos >= Window.get_height()  then
       Fysics.set_pos(DuckLeft, Mafs.vector2_x(SpawnPointLeft), Mafs.vector2_y(SpawnPointLeft)- 1500)
       Fysics.set_gravity_scale(DuckLeft, 0)
       Fysics.clear_forces(DuckLeft)
       SendRightDuck = true
       StartTimer = true
    end

    local DuckRightPos = Mafs.vector2_y(Entity.get_entity_pos(DuckRight))
      if DuckRightPos >= Window.get_height()  then
       Fysics.set_pos(DuckRight, Mafs.vector2_x(SpawnPointRight), Mafs.vector2_y(SpawnPointRight) - 1500)
       Fysics.set_gravity_scale(DuckRight, 0)
       Fysics.clear_forces(DuckRight)
       SendLeftDuck = true
       StartTimer = true
    end

    if StartTimer == true then
        t = t + Mafs.delta_time()
        print(t)
        if t >= 1.5 then
            t = 0
            print("Done")
            StartTimer = false
            if SendLeftDuck then
                SendOffLeftDuck()
            end
            if SendRightDuck then
                SendOffRightDuck()
            end
        end
    end
end

function SendOffLeftDuck()
   SendLeftDuck = false
   local YPositionOffset = math.random(0,180)
   Fysics.set_pos(DuckLeft, Mafs.vector2_x(SpawnPointLeft), Mafs.vector2_y(SpawnPointLeft) - YPositionOffset)
   Fysics.set_gravity_scale(DuckLeft, 1)
   Fysics.add_force(DuckLeft, 220,-250,1,1)  
   print(YPositionOffset)
end

function SendOffRightDuck()
    SendRightDuck = false
    local YPositionOffset = math.random(0,180)
    Fysics.set_pos(DuckRight, Mafs.vector2_x(SpawnPointRight), Mafs.vector2_y(SpawnPointRight) - YPositionOffset)
    Fysics.set_gravity_scale(DuckRight, 1)
    Fysics.add_force(DuckRight, -220,-250,1,1) 
    print(YPositionOffset)
end

return ExampleScript