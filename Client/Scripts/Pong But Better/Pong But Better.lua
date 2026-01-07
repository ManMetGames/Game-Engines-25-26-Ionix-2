local PongButBetter = {}

local assets = require("Scripts.Assets")

local ball
local background

local ballX = 400
local ballY = 300
local ballVelX = 200
local ballVelY = 150

function PongButBetter:OnStart()
    Window.set_size(800, 600)

    -- Create Background
    background = Entity.create_entity()
    Entity.set_global_pos(background, 0, 0)
    backgroundSprite = Entity.add_sprite_component(background, assets.textures.BackgroundPong, 800, 600, 0)

    -- Create Left Paddle
    leftPaddle = Entity.create_entity()
    Entity.set_global_pos(leftPaddle, 30, 300)
    leftPaddleSprite = Entity.add_sprite_component(leftPaddle, assets.textures.PongPaddle, 20, 80, 0)
    Entity.add_fysics_component(leftPaddle, 1, false)
    Fysics.add_box_collider(leftPaddle, 20, 80, 0, 0, 0, false)
    
    -- Create Right Paddle
    rightPaddle = Entity.create_entity()
    Entity.set_global_pos(rightPaddle, 770, 300)
    rightPaddleSprite = Entity.add_sprite_component(rightPaddle, assets.textures.PongPaddle, 20, 80, 0)
    Entity.add_fysics_component(rightPaddle, 1, false)
    Fysics.add_box_collider(rightPaddle, 20, 80, 0, 0, 0, false)
        
    -- Create Walls
    topWall = Entity.create_entity()
    Entity.set_global_pos(topWall, 400, 0)
    Entity.add_fysics_component(topWall, 0, false)
    Fysics.add_box_collider(topWall, 800, 20, 0, 0, 0, false)

    bottomWall = Entity.create_entity()
    Entity.set_global_pos(bottomWall, 400, 600)
    Entity.add_fysics_component(bottomWall, 0, false)
    Fysics.add_box_collider(bottomWall, 800, 20, 0, 0, 0, false)

    leftWall = Entity.create_entity()
    Entity.set_global_pos(leftWall, 0, 300)
    Entity.add_fysics_component(leftWall, 0, false)
    Fysics.add_box_collider(leftWall, 200, 600, 0, 0, 0, false)

    rightWall = Entity.create_entity()
    Entity.set_global_pos(rightWall, 800, 300)
    Entity.add_fysics_component(rightWall, 0, false)
    Fysics.add_box_collider(rightWall, 200, 600, 0, 0, 0, false)

    -- Create ball
    ball = Entity.create_entity()
    Entity.set_global_pos(ball, ballX, ballY)
    ballSprite = Entity.add_sprite_component(ball, assets.textures.PongBall, 20, 20, 0)
    Sprite.set_playback_mode(ballSprite, 4)

end

function PongButBetter:OnUpdate()
    local dt = Mafs.delta_time()

    if dt > 0.1 then
        dt = 0.016 -- ~60fps
    end

    ballX = ballX + ballVelX * dt
    ballY = ballY + ballVelY * dt

    if ballY <= 20 or ballY >= 580 then
        ballVelY = -ballVelY
    end

    if ballX <= 20 or ballX >= 780 then
        ballVelX = -ballVelX
    end

    Entity.set_global_pos(ball, ballX, ballY)
end

return PongButBetter