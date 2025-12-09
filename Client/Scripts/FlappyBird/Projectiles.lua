-- Scripts/FlappyBird/Projectiles.lua
local assets = require("Scripts.Assets")

local Projectiles = {}

local projectileSpeed     = 600
local projectileLifetime  = 2.0
local defaultSize         = 16
local minProjectileSize   = 16
local knockbackBaseDuration = 0.25
local knockbackBaseSpeed    = 2.5

-- state
local projectiles = {}

local knockbackP1Time = 0
local knockbackP2Time = 0
local knockbackP1Dir  = 0
local knockbackP2Dir  = 0

----------------------------------------------------------
-- SPAWN
----------------------------------------------------------
function Projectiles.SpawnProjectile(spawnX, spawnY, dirX, dirY, size, owner)
    size = size or defaultSize

    -- normalise dir
    if dirX == 0 and dirY == 0 then
        dirX = 1
        dirY = 0
    else
        local len = math.sqrt(dirX * dirX + dirY * dirY)
        if len > 0 then
            dirX = dirX / len
            dirY = dirY / len
        else
            dirX = 1
            dirY = 0
        end
    end

    local proj = Entity.create_entity()
    local sprite = Entity.add_sprite_component(
        proj,
        assets.textures.Ghast_Tear,
        size,
        size,
        5
    )

    Entity.set_global_pos(proj, spawnX, spawnY)

    local projData = {
        entity = proj,
        sprite = sprite,
        x = spawnX,
        y = spawnY,
        vx = dirX * projectileSpeed,
        vy = dirY * projectileSpeed,
        age = 0,
        size = size,
        owner = owner
    }

    table.insert(projectiles, projData)
end

----------------------------------------------------------
-- UPDATE + HIT DETECTION 
----------------------------------------------------------
function Projectiles.Update(dt, player1, player2)
    for i = #projectiles, 1, -1 do
        local p = projectiles[i]

        p.x = p.x + p.vx * dt
        p.y = p.y + p.vy * dt
        Entity.set_global_pos(p.entity, p.x, p.y)

        local hit = false
        local size = p.size or defaultSize
        local radiusProj   = size * 0.5
        local radiusPlayer = 16

        local isTapShot = size <= (minProjectileSize + 0.5)
        local sizeScale  = math.min(size / minProjectileSize, 2.0)

        -- hit player1
        if player1 and p.owner ~= 1 then
            local pos = Entity.get_global_pos(player1)
            local px  = Mafs.get_vec_x(pos)
            local py  = Mafs.get_vec_y(pos)
            local dx  = px - p.x
            local dy  = py - p.y
            local r   = radiusProj + radiusPlayer

            if dx * dx + dy * dy <= r * r then
                if not isTapShot then
                    local dirX = (p.vx >= 0) and 1 or -1
                    knockbackP1Time = knockbackBaseDuration * sizeScale
                    knockbackP1Dir  = dirX * sizeScale
                end
                hit = true
            end
        end

        -- hit player2
        if not hit and player2 and p.owner ~= 2 then
            local pos = Entity.get_global_pos(player2)
            local px  = Mafs.get_vec_x(pos)
            local py  = Mafs.get_vec_y(pos)
            local dx  = px - p.x
            local dy  = py - p.y
            local r   = radiusProj + radiusPlayer

            if dx * dx + dy * dy <= r * r then
                if not isTapShot then
                    local dirX = (p.vx >= 0) and 1 or -1
                    knockbackP2Time = knockbackBaseDuration * sizeScale
                    knockbackP2Dir  = dirX * sizeScale
                end
                hit = true
            end
        end

        p.age = p.age + dt
        if hit or p.age > projectileLifetime then
            Entity.destroy_entity(p.entity)
            table.remove(projectiles, i)
        end
    end
end

----------------------------------------------------------
-- APPLY KNOCKBACK TO HORIZONTAL VELOCITY
----------------------------------------------------------
function Projectiles.ApplyKnockback(dt, vx1, vx2)
    if knockbackP1Time > 0 then
        local t = knockbackP1Time / knockbackBaseDuration
        vx1 = knockbackP1Dir * knockbackBaseSpeed * t
        knockbackP1Time = knockbackP1Time - dt
        if knockbackP1Time < 0 then knockbackP1Time = 0 end
    end

    if knockbackP2Time > 0 then
        local t = knockbackP2Time / knockbackBaseDuration
        vx2 = knockbackP2Dir * knockbackBaseSpeed * t
        knockbackP2Time = knockbackP2Time - dt
        if knockbackP2Time < 0 then knockbackP2Time = 0 end
    end

    return vx1, vx2
end

return Projectiles
