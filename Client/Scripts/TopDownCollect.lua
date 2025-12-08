local EngineShowcase = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

local player
local rotatingBox
local bouncingBall
local staticPlatform

local rotationSpeed = 2
local time = 0
local particleTimer = 0
local particles = {}

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function EngineShowcase:OnStart()
    
    ------------------------------------------------------
    -- Background
    ------------------------------------------------------
    local bg = Entity.create_entity()
    Entity.set_global_pos(bg, 480, 320)
    local bgSprite = Entity.add_sprite_component(bg, assets.textures.Background, 960, 640, 0)
    Sprite.set_columns(bgSprite, 1)
    
    ------------------------------------------------------
    -- Player 
    ------------------------------------------------------
    player = Entity.create_entity()
    Entity.set_global_pos(player, 200, 300)
    
    local playerSprite = Entity.add_sprite_component(player, assets.textures.FlappyBird, 48, 48, 10)
    Sprite.set_columns(playerSprite, 1)
    
    Entity.add_fysics_component(player, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player, false, 1)
    Fysics.set_gravity_scale(player, 0)
    Fysics.set_fixed_rotation(player, true)
    
    ------------------------------------------------------
    -- Rotating Box 
    ------------------------------------------------------
    rotatingBox = Entity.create_entity()
    Entity.set_global_pos(rotatingBox, 480, 200)
    
    local boxSprite = Entity.add_sprite_component(rotatingBox, assets.textures.Sand, 80, 80, 5)
    Sprite.set_columns(boxSprite, 1)
    
    Entity.add_fysics_component(rotatingBox, enums.bodytype.kinematicBody, false)
    Fysics.add_sprite_collider(rotatingBox, false, 1)
    Fysics.set_gravity_scale(rotatingBox, 0)
    
    ------------------------------------------------------
    -- Bouncing Ball 
    ------------------------------------------------------
    bouncingBall = Entity.create_entity()
    Entity.set_global_pos(bouncingBall, 700, 100)
    
    local ballSprite = Entity.add_sprite_component(bouncingBall, assets.textures.Coin, 40, 40, 8)
    Sprite.set_rows(ballSprite, 1)
    Sprite.set_columns(ballSprite, 5)
    Sprite.set_width(ballSprite, 40)
    Sprite.set_height(ballSprite, 40)
    
    Entity.add_fysics_component(bouncingBall, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(bouncingBall, false, 1)
    Fysics.set_gravity_scale(bouncingBall, 1)
    
    ------------------------------------------------------
    -- Static Platform 
    ------------------------------------------------------
    staticPlatform = Entity.create_entity()
    Entity.set_global_pos(staticPlatform, 700, 500)
    
    local platformSprite = Entity.add_sprite_component(staticPlatform, assets.textures.Sand, 200, 40, 3)
    Sprite.set_columns(platformSprite, 1)
    
    Entity.add_fysics_component(staticPlatform, enums.bodytype.staticBody, false)
    Fysics.add_sprite_collider(platformSprite, false, 1)
    
    ------------------------------------------------------
    -- Spawn Some Particles
    ------------------------------------------------------
    for i = 1, 5 do
        local particle = Entity.create_entity()
        local x = math.random(100, 400)
        local y = math.random(400, 550)
        Entity.set_global_pos(particle, x, y)
        
        local pSprite = Entity.add_sprite_component(particle, assets.textures.Coin, 20, 20, 2)
        Sprite.set_rows(pSprite, 1)
        Sprite.set_columns(pSprite, 5)
        Sprite.set_width(pSprite, 20)
        Sprite.set_height(pSprite, 20)
        
        Entity.add_fysics_component(particle, enums.bodytype.kinematicBody, false)
        Fysics.set_gravity_scale(particle, 0)
        
        table.insert(particles, {
            entity = particle,
            baseY = y,
            offset = math.random(0, 100) / 100
        })
    end
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function EngineShowcase:OnUpdate()
    
    time = time + Mafs.delta_time()
    
    ------------------------------------------------------
    -- UI 
    ------------------------------------------------------
    UI.Add_label(10, 10, 500, 50, "IONIX ENGINE FEATURE SHOWCASE")
    UI.Add_label(10, 40, 400, 50, "Mouse: Player Follows Cursor")
    UI.Add_label(10, 70, 400, 50, "Space: Jump Ball")
    UI.Add_label(10, 100, 400, 50, "Click: Spawn object")
    
    -- Feature indicators
    UI.Add_label(10, 150, 300, 50, "Physics: Gravity & Collision")
    UI.Add_label(10, 180, 300, 50, "Input: Keyboard & Mouse")
    UI.Add_label(10, 210, 300, 50, "Math: Sin/Cos Animation")
    UI.Add_label(10, 240, 300, 50, "Entities: Dynamic Creation")
    
    -- Show time
    UI.Add_label(10, 280, 300, 50, "Time: " .. math.floor(time) .. "s")
    
    ------------------------------------------------------
    -- Player Follows Mouse 
    ------------------------------------------------------
    local mouseX = Input.get_mouse_x()
    local mouseY = Input.get_mouse_y()
    
    -- Get current player position
    local playerPos = Fysics.get_pos(player)
    local playerX = Mafs.get_vec_x(playerPos) * 64 
    local playerY = Mafs.get_vec_y(playerPos) * 64
    
   
    local dirX = mouseX - playerX
    local dirY = mouseY - playerY
    
    
    local distance = Mafs.square_root(dirX * dirX + dirY * dirY)
    
    -- Only move if not too close to mouse
    if distance > 5 then
        -- Normalize and apply speed
        local speed = 5
        local vx = (dirX / distance) * speed
        local vy = (dirY / distance) * speed
        Fysics.set_linear_velocity(player, vx, vy)
    else
        Fysics.set_linear_velocity(player, 0, 0)
    end
    
    ------------------------------------------------------
    -- Rotating Box 
    ------------------------------------------------------
    local currentAngle = Fysics.get_angle(rotatingBox)
    local newAngle = currentAngle + (rotationSpeed * Mafs.delta_time())
    Fysics.set_angle(rotatingBox, newAngle)
    
    ------------------------------------------------------
    -- Make ball jump on spacebar 
    ------------------------------------------------------
    if Input.get_key_down(Keys.ionix_space) then
        Fysics.set_linear_velocity(bouncingBall, 0, -8)
        print("Ball jump!")
    end
    
    -- Reset ball if it falls too far
    local ballPos = Fysics.get_pos(bouncingBall)
    if Mafs.get_vec_y(ballPos) > 10 then
        Fysics.set_pos(bouncingBall, 11, 2)
        Fysics.set_linear_velocity(bouncingBall, 0, 0)
    end
    
    ------------------------------------------------------
    -- Animate Particles 
    ------------------------------------------------------
    for i, p in ipairs(particles) do
        local floatY = p.baseY + Mafs.sin(time * 2 + p.offset) * 20
        Fysics.set_pos(p.entity, Mafs.get_vec_x(Fysics.get_pos(p.entity)), floatY / 64)
    end
    
    ------------------------------------------------------
    -- Mouse Click to Spawn 
    ------------------------------------------------------
    if Input.get_mouse_button_down(1) then
        local mx = Input.get_mouse_x()
        local my = Input.get_mouse_y()
        
        local newObj = Entity.create_entity()
        Entity.set_global_pos(newObj, mx, my)
        
        local sprite = Entity.add_sprite_component(newObj, assets.textures.FlappyPipe, 60, 60, 7)
        Sprite.set_columns(sprite, 1)
        
        Entity.add_fysics_component(newObj, enums.bodytype.dynamicBody, true)
        Fysics.add_sprite_collider(newObj, false, 1)
        Fysics.set_gravity_scale(newObj, 0.5)
    end
end

----------------------------------------------------------
-- OnCollisionEnter 
----------------------------------------------------------
function EngineShowcase:OnCollisionEnter(entity1, entity2)
    print("Collision detected!")
    
    -- Make ball bounce higher when it hits platform
    if (entity1 == bouncingBall or entity2 == bouncingBall) then
        if entity1 == staticPlatform or entity2 == staticPlatform then
            Fysics.set_linear_velocity(bouncingBall, 0, -250)
        end
    end
end

return EngineShowcase