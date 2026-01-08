local PongButBetter = {}

local assets = require("Scripts.Assets")

local ball
local background

local ballX = 400
local ballY = 300
local ballVelX = 200
local ballVelY = 150
local paddleSpeed = 4

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

    
    if ballY <= 10 or ballY >= 590 then
        ballVelY = -ballVelY
    end

    Entity.set_global_pos(ball, ballX, ballY)

    self:MovePaddles(dt)
    self:OnCollisionEnter()
end

function PongButBetter:MovePaddles(dt)
    local leftVelY = 0
    local rightVelY = 0
    
    if Input.get_key_held(Keys.arrow_up) then
        rightVelY = -paddleSpeed
    elseif Input.get_key_held(Keys.arrow_down) then
        rightVelY = paddleSpeed
    end

    local rightPaddleY = Mafs.get_vec_y(Entity.get_global_pos(rightPaddle))
    
    if (rightPaddleY <= 40 and rightVelY < 0) or (rightPaddleY >= 520 and rightVelY > 0) then
        rightVelY = 0
    end    
    
    Fysics.set_linear_velocity(rightPaddle, 0, rightVelY)
end

function PongButBetter:OnCollisionEnter()
    local rightY = Mafs.get_vec_y(Entity.get_global_pos(rightPaddle))

    if ballX >= 750 and ballX <= 780 and ballY <= rightY + 80 and ballX >= ballY then
        ballVelX = -Mafs.abs(ballVelX)
    end    
end

return PongButBetter