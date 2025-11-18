local ball = {}
local xSpeed = 2
local ySpeed = 2
local x, y
local screenWidth, screenHeight
local ballEntity
local spriteSize = 32
local spriteHalf = spriteSize / 2

local function safe_call(fn, ...)
    if not ballEntity then
        print("[Lua] ballEntity is nil; skipping engine call")
        return nil
    end
    print("[Lua] ballEntity pointer:", tostring(ballEntity))
    local ok, res = pcall(fn, ...)
    if not ok then
        print("[Lua] engine call failed:", res, "entity:", tostring(ballEntity))
        return nil
    end
    return res
end

function ball:OnStart()
    screenWidth = Window.get_width()
    screenHeight = Window.get_height()

    ballEntity = Entity.create_entity()
    if not ballEntity then
        print("[Lua] Entity.create_entity() returned nil")
        return
    end

    safe_call(function() Entity.add_sprite_component(ballEntity, "PimBall", spriteSize, spriteSize, 255) end)

    x = screenWidth / 2
    y = screenHeight / 2
    safe_call(function() Entity.set_entity_pos(ballEntity, x, y) end)
end

function ball:OnUpdate()
    screenWidth = Window.get_width()
    screenHeight = Window.get_height()

    x = x + xSpeed
    y = y + ySpeed

    local minX, maxX = spriteHalf, screenWidth - spriteHalf
    local minY, maxY = spriteHalf, screenHeight - spriteHalf

    if x > maxX then x = maxX; xSpeed = -xSpeed
    elseif x < minX then x = minX; xSpeed = -xSpeed end

    if y > maxY then y = maxY; ySpeed = -ySpeed
    elseif y < minY then y = minY; ySpeed = -ySpeed end

    safe_call(function() Entity.set_entity_pos(ballEntity, x, y) end)
end

function ball:OnDestroy()
    -- clear reference so Lua won't call into a freed entity
    if ballEntity then
        print("[Lua] OnDestroy clearing ballEntity:", tostring(ballEntity))
    end
    ballEntity = nil
end

return ball