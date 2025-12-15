local TriangleShooterAbilities = {}

local function basicAbility(tipX, tipY, aimX, aimY)
    return {
        { offsetX = 0, offsetY = 0, dirX = aimX, dirY = aimY }
    }
end

local function dualAbility(tipX, tipY, aimX, aimY, projectileSize)
    local sideX = -aimY
    local sideY = aimX
    local offset = projectileSize * 0.6

    return {
        { offsetX = sideX * offset, offsetY = sideY * offset, dirX = aimX, dirY = aimY },
        { offsetX = -sideX * offset, offsetY = -sideY * offset, dirX = aimX, dirY = aimY },
    }
end

local function tripleAbility(tipX, tipY, aimX, aimY, projectileSize)
    local sideX = -aimY
    local sideY = aimX
    local offset = projectileSize * 0.6

    return {
        { offsetX = 0, offsetY = 0, dirX = aimX, dirY = aimY },
        { offsetX = sideX * offset, offsetY = sideY * offset, dirX = aimX, dirY = aimY },
        { offsetX = -sideX * offset, offsetY = -sideY * offset, dirX = aimX, dirY = aimY },
    }
end

local function wideAbility(tipX, tipY, aimX, aimY)
    local count = 4
    local totalSpreadDeg = 40
    local baseAngle = math.atan(aimY, aimX)
    local halfSpread = math.rad(totalSpreadDeg) / 2
    local step = (count == 1) and 0 or (2 * halfSpread / (count - 1))

    local shots = {}
    for i = 0, count - 1 do
        local angle = baseAngle - halfSpread + step * i
        local dirX = math.cos(angle)
        local dirY = math.sin(angle)
        shots[#shots + 1] = { offsetX = 0, offsetY = 0, dirX = dirX, dirY = dirY }
    end

    return shots
end

local abilityDispatch = {
    basic = function(tipX, tipY, aimX, aimY, projectileSize)
        return basicAbility(tipX, tipY, aimX, aimY)
    end,
    dual = dualAbility,
    triple = tripleAbility,
    wide = function(tipX, tipY, aimX, aimY, projectileSize)
        return wideAbility(tipX, tipY, aimX, aimY)
    end,
}

function TriangleShooterAbilities.getShots(name, tipX, tipY, aimX, aimY, projectileSize)
    local func = abilityDispatch[name] or abilityDispatch.basic
    return func(tipX, tipY, aimX, aimY, projectileSize)
end

return TriangleShooterAbilities
