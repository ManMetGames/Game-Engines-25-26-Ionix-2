local TriangleShooterAbilities = {}

local SPREAD_ANGLE_DEG = 10

 --=====================================================================
 --  [INTERNAL] Dynamic Pattern Generation
 --=====================================================================
local function generateShotPattern(bulletCount, aimX, aimY, projectileSize)
    local shots = {}
    local baseAngle = math.atan(aimY, aimX)

    if bulletCount == 1 then
        shots[1] = { offsetX = 0, offsetY = 0, dirX = aimX, dirY = aimY }

    elseif bulletCount == 2 then
        local sideX = -aimY
        local sideY = aimX
        local offset = projectileSize * 0.6
        shots[1] = { offsetX = sideX * offset, offsetY = sideY * offset, dirX = aimX, dirY = aimY }
        shots[2] = { offsetX = -sideX * offset, offsetY = -sideY * offset, dirX = aimX, dirY = aimY }

    elseif bulletCount == 3 then
        local sideX = -aimY
        local sideY = aimX
        local offset = projectileSize * 0.6
        shots[1] = { offsetX = 0, offsetY = 0, dirX = aimX, dirY = aimY }
        shots[2] = { offsetX = sideX * offset, offsetY = sideY * offset, dirX = aimX, dirY = aimY }
        shots[3] = { offsetX = -sideX * offset, offsetY = -sideY * offset, dirX = aimX, dirY = aimY }

    else
        local halfSpread = math.rad(SPREAD_ANGLE_DEG) / 2
        local step = (2 * halfSpread) / (bulletCount - 1)

        for i = 0, bulletCount - 1 do
            local angle = baseAngle - halfSpread + step * i
            local dirX = math.cos(angle)
            local dirY = math.sin(angle)
            shots[#shots + 1] = { offsetX = 0, offsetY = 0, dirX = dirX, dirY = dirY }
        end
    end

    return shots
end

 --=====================================================================
 --  [PUBLIC API] Resolve Shots
 --=====================================================================
function TriangleShooterAbilities.getShots(bulletCount, tipX, tipY, aimX, aimY, projectileSize)
    return generateShotPattern(bulletCount, aimX, aimY, projectileSize)
end

return TriangleShooterAbilities
