local TriangleShooterAbilities = {}

local SPREAD_ANGLE_DEG = 10

 --=====================================================================
 --  [INTERNAL] Dynamic Pattern Generation
 --=====================================================================
local function generateShotPattern(firepower, aimX, aimY, projectileSize)
    local shots = {}
    local baseAngle = math.atan(aimY, aimX)

    -- Firepower 5+: golden bullet progression
    if firepower >= 5 then
        if firepower == 5 then
            -- Single golden bullet, 5 damage, 2.5x size
            shots[1] = { 
                offsetX = 0, 
                offsetY = 0, 
                dirX = aimX, 
                dirY = aimY,
                isGolden = true,
                damage = 5,
                sizeMultiplier = 2.5,
            }
        elseif firepower == 6 then
            -- Single golden bullet
            shots[1] = { 
                offsetX = 0, 
                offsetY = 0, 
                dirX = aimX, 
                dirY = aimY,
                isGolden = true,
                damage = 7,
                sizeMultiplier = 3,
            }
        elseif firepower == 7 then
            -- Single golden bullet
            shots[1] = { 
                offsetX = 0, 
                offsetY = 0, 
                dirX = aimX, 
                dirY = aimY,
                isGolden = true,
                damage = 9,
                sizeMultiplier = 3.5,
            }
        else
            -- Firepower 8+: Two golden bullets at original size (2.5x), 6 damage each
            local sideX = -aimY
            local sideY = aimX
            local offset = projectileSize * 0.8
            shots[1] = { 
                offsetX = sideX * offset, 
                offsetY = sideY * offset, 
                dirX = aimX, 
                dirY = aimY,
                isGolden = true,
                damage = 5,
                sizeMultiplier = 2.5,
            }
            shots[2] = { 
                offsetX = -sideX * offset, 
                offsetY = -sideY * offset, 
                dirX = aimX, 
                dirY = aimY,
                isGolden = true,
                damage = 5,
                sizeMultiplier = 2.5,
            }
        end
        return shots
    end

    if firepower == 1 then
        shots[1] = { offsetX = 0, offsetY = 0, dirX = aimX, dirY = aimY }

    elseif firepower == 2 then
        local sideX = -aimY
        local sideY = aimX
        local offset = projectileSize * 0.6
        shots[1] = { offsetX = sideX * offset, offsetY = sideY * offset, dirX = aimX, dirY = aimY }
        shots[2] = { offsetX = -sideX * offset, offsetY = -sideY * offset, dirX = aimX, dirY = aimY }

    elseif firepower == 3 then
        local sideX = -aimY
        local sideY = aimX
        local offset = projectileSize * 0.6
        shots[1] = { offsetX = 0, offsetY = 0, dirX = aimX, dirY = aimY }
        shots[2] = { offsetX = sideX * offset, offsetY = sideY * offset, dirX = aimX, dirY = aimY }
        shots[3] = { offsetX = -sideX * offset, offsetY = -sideY * offset, dirX = aimX, dirY = aimY }

    else
        local halfSpread = math.rad(SPREAD_ANGLE_DEG) / 2
        local step = (2 * halfSpread) / (firepower - 1)

        for i = 0, firepower - 1 do
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
function TriangleShooterAbilities.getShots(firepower, tipX, tipY, aimX, aimY, projectileSize)
    return generateShotPattern(firepower, aimX, aimY, projectileSize)
end

return TriangleShooterAbilities
