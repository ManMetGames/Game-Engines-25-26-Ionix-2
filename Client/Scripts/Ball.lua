local ball = {}

local ballEntity
local speedX = 6
local speedY = 6
local ballSize = 32

----------------------------------------------------------
-- Create walls around the screen
----------------------------------------------------------
local function CreateWall(x, y, w, h)
    local wall = Entity.create_entity()
    Entity.set_entity_pos(wall, x, y)

    -- Static physics body
    Entity.add_fysics_component(wall, 0, false)

    -- Convert pixel size to collider scale (Box2D units)
    Fysics.add_box_collider(wall, w / 100, h / 100, 0, 0, 0, false)

    return wall
end

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ball:OnStart()
    local screenW = Window.get_width()
    local screenH = Window.get_height()

    ------------------------------------------------------
    -- Create the ball
    ------------------------------------------------------
    ballEntity = Entity.create_entity()

    -- Center it on screen
    Entity.set_entity_pos(ballEntity, screenW / 2, screenH / 2)

    Entity.add_sprite_component(ballEntity, "PimBall", ballSize, ballSize, 1)

    ------------------------------------------------------
    -- Add physics body (dynamic)
    ------------------------------------------------------
    Entity.add_fysics_component(ballEntity, 2, false)
    Fysics.add_box_collider(ballEntity, 0.16, 0.16, 0, 0, 0, false)
    -- collider size ~ ballSize / 200 if ballSize = 32

    ------------------------------------------------------
    -- Build screen boundary walls
    ------------------------------------------------------
    -- Left wall
    CreateWall(-20, screenH/2, 40, screenH)

    -- Right wall
    CreateWall(screenW + 20, screenH/2, 40, screenH)

    -- Top wall
    CreateWall(screenW/2, -20, screenW, 40)

    -- Bottom wall
    CreateWall(screenW/2, screenH + 20, screenW, 40)

    ------------------------------------------------------
    -- Initial velocity
    ------------------------------------------------------
    Fysics.set_linear_velocity(ballEntity, speedX, speedY)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ball:OnUpdate()
    -- Read current velocity
    local vel = Fysics.get_linear_velocity(ballEntity)

    ------------------------------------------------------
    -- Maintain constant speed after bouncing
    ------------------------------------------------------
    local finalX = speedX
    local finalY = speedY

    ------------------------------------------------------
    -- Check if velocity reversed by collision
    ------------------------------------------------------
    if vel.x ~= 0 then
        -- Maintain sign of bounce
        finalX = (vel.x > 0) and math.abs(speedX) or -math.abs(speedX)
    end

    if vel.y ~= 0 then
        finalY = (vel.y > 0) and math.abs(speedY) or -math.abs(speedY)
    end

    ------------------------------------------------------
    -- Apply stable velocity (prevents Box2D slowdown)
    ------------------------------------------------------
    Fysics.set_linear_velocity(ballEntity, finalX, finalY)
end

return ball
