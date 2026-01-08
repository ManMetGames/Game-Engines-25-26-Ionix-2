local PongButBetter = {}

local GameState = {
    MENU = 1,
    SERVE = 2,
    PLAY = 3,
    GAMEOVER = 4
}

local GameMode = {
    PVP = 1,
    PVC = 2
}

local state = GameState.MENU
local mode = GameMode.PVP
local assets = require("Scripts.Assets")
local ballAudio
local scoreAudio

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

local AISpeed = 3.6

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
    ballAudio = Entity.add_audio_component(ball, "PongHit", false)
    scoreAudio = Entity.add_audio_component(ball, "PongScore", false)
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
        UI.add_label(260, 330, 0, 0, "Press 1 for 1 Player (vs AI)")
        UI.add_label(260, 360, 0, 0, "Press 2 for 2 Player")
        UI.add_label(260, 390, 0, 0, "Current: " .. ((mode == GameMode.PVC) and "1P and AI" or "2P PVP"))
    elseif state == GameState.GAMEOVER then
        local winner = (leftScore >= winScore) and "LEFT WINS!" or "RIGHT WINS!"
        UI.add_label(340, 100, 0, 0, winner)
        UI.add_label(270, 140, 0, 0, "Press R to return to menu")
        UI.add_label(300, 180, 0, 0, "or Press Esc to quit")
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

    if mode == GameMode.PVP then
        if Input.get_key_held(Keys.ionix_w) then
            leftVelY = -paddleSpeed
        elseif Input.get_key_held(Keys.ionix_s) then
            leftVelY = paddleSpeed
        end
    else
        leftVelY = self:GetAIVelocity(dt)
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

    if ballX >= 750 and ballX <= 780 and ballY <= rightY + 80 and ballY >= rightY - 20 and ballVelX > 0 then
        AudioComponent.play_one_shot(ball, "PongHit", 1.0)
        ballVelX = -Mafs.abs(ballVelX) * speedMult
        local hitOffset = (ballY - (rightY + 40)) / 40
        ballVelY = hitOffset * 250 * speedMult
    end

    if ballX >= 20 and ballX <= 50 and ballY <= leftY + 80 and ballY >= leftY - 20 and ballVelX < 0 then
        AudioComponent.play_one_shot(ball, "PongHit", 1.0)
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
    elseif Input.get_key_down(Keys.ionix_1) then
        mode = GameMode.PVC
    elseif Input.get_key_down(Keys.ionix_2) then
        mode = GameMode.PVP
    end    
end


function PongButBetter:UpdatePlay(dt)
    ballX = ballX + ballVelX * dt
    ballY = ballY + ballVelY * dt
    
    local ballHalf = 10
    local topLimit = ballHalf
    local bottomLimit = 600 - ballHalf
    
    if ballY <= topLimit then
        AudioComponent.play_one_shot(ball, "PongHit", 1.0)
        ballY = topLimit + 0.001
        ballVelY = Mafs.abs(ballVelY)
    elseif ballY >= bottomLimit then
        AudioComponent.play_one_shot(ball, "PongHit", 1.0)
        ballY = bottomLimit - 0.001
        ballVelY = -Mafs.abs(ballVelY)
    end
    
    if ballX < -20 then
        rightScore = rightScore + 1
        AudioComponent.play_one_shot(ball, "PongScore", 1.0)
        if rightScore >= winScore then
            state = GameState.GAMEOVER
            self:ResetBall(-1, true)
        else
            self:BeginServe(-1)
        end    
    
    elseif ballX > 800 then
        leftScore = leftScore + 1
        AudioComponent.play_one_shot(ball, "PongScore", 1.0)
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
    elseif Input.get_key_down(Keys.ionix_escape) then
        Window.quit()
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

function PongButBetter:GetAIVelocity(dt)
    local leftPaddleY = Mafs.get_vec_y(Entity.get_global_pos(leftPaddle))
    local dy = ballY - leftPaddleY

    if Mafs.abs(dy) < 8 then return 0 end

    if ballVelX >= 0 then
        local centreDy = 300 - leftPaddleY
        if Mafs.abs(centreDy) < 8 then return 0 end
        return (centreDy > 0) and AISpeed or -AISpeed 
    end
    
    return (dy > 0) and AISpeed or -AISpeed
end

return PongButBetter