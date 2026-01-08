local PongButBetter = {}

local GameState = {
    MENU = 1,
    SERVE = 2,
    PLAY = 3,
    GAMEOVER = 4
}

local state = GameState.MENU
local assets = require("Scripts.Assets")

local ball
local background

local ballX = 400
local ballY = 300
local ballVelX = 200
local ballVelY = 150
local paddleSpeed = 4

local rightScore = 0
local leftScore = 0
local winScore = 5

local serveTimer = 0
local serveDuration = 3

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

    state = GameState.MENU
end

function PongButBetter:OnUpdate()
    local dt = Mafs.delta_time()

    if dt > 0.1 then
        dt = 0.016 -- ~60fps
    end

    if state == GameState.MENU then
        self:UpdateMenu(dt)
    elseif state == GameState.PLAY then
        self:UpdatePlay(dt)
    elseif state == GameState.GAMEOVER then
        self:UpdateGameOver(dt)
    elseif state == GameState.SERVE then
        self:UpdateServe(dt)
    end    

    self:DrawUI()

    if state == GameState.PLAY or state == GameState.SERVE then
        self:MovePaddles(dt)
    end
end

function PongButBetter:DrawUI()
    UI.add_label(0, 0, 0, 0, "Left: " .. tostring(leftScore))
    UI.add_label(700, 0, 0, 0, "Right: " .. tostring(rightScore))

    if state == GameState.MENU then
        UI.add_label(300, 250, 0, 0, "PONG BUT BETTER")
        UI.add_label(280, 290, 0, 0, "PRESS SPACE TO START")
    elseif state == GameState.GAMEOVER then
        local winner = (leftScore >= winScore) and "LEFT WINS!" or "RIGHT WINS!"
        UI.add_label(340, 250, 0, 0, winner)
        UI.add_label(260, 290, 0, 0, "Press R to return to menu")
    elseif state == GameState.SERVE then
        local count = Mafs.round(serveTimer * 1)
        count = Mafs.max(1, count)
        if count < 1 then count = 1 end
        UI.add_label(320,250,0,0,"SERVING IN: " .. tostring(count))
    end
end

function PongButBetter:MovePaddles(dt)
    local leftVelY = 0
    local rightVelY = 0

    if Input.get_key_held(Keys.arrow_up) then
        rightVelY = -paddleSpeed
    elseif Input.get_key_held(Keys.arrow_down) then
        rightVelY = paddleSpeed
    end

    if Input.get_key_held(Keys.ionix_w) then
        leftVelY = -paddleSpeed
    elseif Input.get_key_held(Keys.ionix_s) then
        leftVelY = paddleSpeed
    end

    local rightPaddleY = Mafs.get_vec_y(Entity.get_global_pos(rightPaddle))
    local leftPaddleY = Mafs.get_vec_y(Entity.get_global_pos(leftPaddle))

    if (rightPaddleY <= 40 and rightVelY < 0) or (rightPaddleY >= 520 and rightVelY > 0) then
        rightVelY = 0
    end

    if (leftPaddleY <= 40 and leftVelY < 0) or (leftPaddleY >= 520 and leftVelY > 0) then
        leftVelY = 0
    end

    Fysics.set_linear_velocity(rightPaddle, 0, rightVelY)
    Fysics.set_linear_velocity(leftPaddle, 0, leftVelY)
end

function PongButBetter:OnCollisionEnter()
    local rightY = Mafs.get_vec_y(Entity.get_global_pos(rightPaddle))
    local leftY = Mafs.get_vec_y(Entity.get_global_pos(leftPaddle))
    
    local speedMult = 1.05

    if ballX >= 750 and ballX <= 780 and ballY <= rightY + 80 and ballY >= rightY then
        ballVelX = -Mafs.abs(ballVelX) * speedMult
        local hitOffset = (ballY - (rightY + 40)) / 40
        ballVelY = hitOffset * 250 * speedMult
    end

    if ballX >= 20 and ballX <= 50 and ballY <= leftY + 80 and ballY >= leftY then
        ballVelX = Mafs.abs(ballVelX) * speedMult
        local hitOffset = (ballY - (leftY + 40)) / 40
        ballVelY = hitOffset * 250 * speedMult
    end
end

function PongButBetter:ResetBall(dir, hold)
    ballX, ballY = 400, 300
    
    if hold then
        ballVelX, ballVelY = 0, 0
    else
        ballVelX = 200 * dir
        ballVelY = math.random(-120, 120)
        print(ballVelY)
    end

    Entity.set_global_pos(ball, ballX, ballY)
end

function PongButBetter:UpdateMenu(dt)
    if Input.get_key_down(Keys.ionix_space) then
        leftScore, rightScore = 0, 0
        local dir = (math.random(0, 1) == 0) and -1 or 1
        self:BeginServe(dir)
    end
end


function PongButBetter:UpdatePlay(dt)
    ballX = ballX + ballVelX * dt
    ballY = ballY + ballVelY * dt
    
    if ballY <= 10 or ballY >= 590 then
        ballVelY = -ballVelY
    end
    
    if ballX < -20 then
        rightScore = rightScore + 1
        if rightScore >= winScore then
            state = GameState.GAMEOVER
            self:ResetBall(-1, true)
        else
            self:BeginServe(-1)
        end    
    
    elseif ballX > 800 then
        leftScore = leftScore + 1
        if leftScore >= winScore then
            state = GameState.GAMEOVER
            self:ResetBall(1, true)
        else
            self:BeginServe(1)
        end
    end
    self:OnCollisionEnter()
    
    Entity.set_global_pos(ball, ballX, ballY)
end    

function PongButBetter:UpdateGameOver(dt)
    if Input.get_key_down(Keys.ionix_r) then
        leftScore, rightScore = 0, 0
        state = GameState.MENU
        self:ResetBall(1, true)
    end
end

function PongButBetter:BeginServe(dir)
    serveDir = dir
    serveTimer = serveDuration
    state = GameState.SERVE
    self:ResetBall(dir, true)
end

function PongButBetter:UpdateServe(dt)
    serveTimer = serveTimer - dt
    
    if serveTimer <= 0 then
        ballVelX = 200 * serveDir
        ballVelY = math.random(-120, 120)
        state = GameState.PLAY
    end
end

return PongButBetter