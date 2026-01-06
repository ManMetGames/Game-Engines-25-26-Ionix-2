local PongButBetter = {}

local ball
local leftPaddle
local rightPaddle

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function PongButBetter:OnStart()
    Window.set_size(800, 600)
    ------------------------------------------------------
    -- Load textures
    ------------------------------------------------------
    --Texture.add_texture("./Assets/left.png", "left")
    Texture.add_texture("./Assets/balls.png", "ball")
    ------------------------------------------------------
    -- Create ball
    ------------------------------------------------------
    ball = Entity.create_entity()
    Entity.set_global_pos(ball, 400, 300)
    ballSprite = Entity.add_sprite_component(ball, "ball", 20, 20, 0)

    ------------------------------------------------------
    -- Create paddles
    ------------------------------------------------------
    leftPaddle = Entity.create_entity()
    Entity.set_global_pos(leftPaddle, 30, 300)
    leftPaddleSprite = Entity.add_sprite_component(leftPaddle, "ball", 15, 80, 0)

    rightPaddle = Entity.create_entity()
    Entity.set_global_pos(rightPaddle, 770, 300)
    rightPaddleSprite = Entity.add_sprite_component(rightPaddle, "ball", 15, 80, 0)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function PongButBetter:OnUpdate()
    
end

return PongButBetter
