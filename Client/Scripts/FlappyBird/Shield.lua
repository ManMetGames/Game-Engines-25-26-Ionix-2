local assets = require("Scripts.Assets")

local Shield = {}

----------------------------------------------------------
-- CONFIG
----------------------------------------------------------
Shield.maxMeter         = 3.0
Shield.drainRate        = 1.2
Shield.regenRate        = 0.8
Shield.breakCooldown    = 1.2
Shield.triggerThreshold = 0.25

Shield.size    = 48
Shield.xOffset = 15
Shield.yOffset = -10

----------------------------------------------------------
-- STATE
----------------------------------------------------------
local playerEnt = { [1]=nil, [2]=nil }

local shieldEnt    = { [1]=nil, [2]=nil }
local shieldSprite = { [1]=nil, [2]=nil }

local meter      = { [1]=Shield.maxMeter, [2]=Shield.maxMeter }
local breakTimer = { [1]=0, [2]=0 }

local held    = { [1]=false, [2]=false }
local active  = { [1]=false, [2]=false }
local latched = { [1]=false, [2]=false }

----------------------------------------------------------
-- INIT
----------------------------------------------------------
function Shield.Init(p1, p2)
    playerEnt[1] = p1
    playerEnt[2] = p2

    for i = 1, 2 do
        shieldEnt[i] = Entity.create_entity()
        shieldSprite[i] = Entity.add_sprite_component(
            shieldEnt[i],
            assets.textures.Sand,
            Shield.size,
            Shield.size,
            50
        )

        Sprite.set_color(shieldSprite[i], 100, 180, 255)
        Entity.set_global_pos(shieldEnt[i], -1000, -1000)
    end
end

----------------------------------------------------------
-- INPUT
----------------------------------------------------------
function Shield.SetHeld(index, isHeld)
    held[index] = isHeld
end

----------------------------------------------------------
-- HELPERS
----------------------------------------------------------
function Shield.CanDeploy(index)
    return meter[index] >= Shield.maxMeter
           and breakTimer[index] <= 0
end

function Shield.IsActive(index)
    return active[index]
end

function Shield.Reset(index)
    meter[index] = Shield.maxMeter
    breakTimer[index] = 0
    active[index] = false
    latched[index] = false
    Entity.set_global_pos(shieldEnt[index], -1000, -1000)
end

----------------------------------------------------------
-- UPDATE
----------------------------------------------------------
function Shield.Update(dt)
    for i = 1, 2 do
        if breakTimer[i] > 0 then
            breakTimer[i] = breakTimer[i] - dt
            if breakTimer[i] < 0 then breakTimer[i] = 0 end
        end

        if held[i] and not latched[i] and Shield.CanDeploy(i) then
            latched[i] = true
            active[i]  = true
        end

        if not held[i] then
            latched[i] = false
            active[i]  = false
        end

        if active[i] then
            meter[i] = meter[i] - Shield.drainRate * dt
            if meter[i] <= 0 then
                meter[i] = 0
                active[i] = false
                latched[i] = false
                breakTimer[i] = Shield.breakCooldown
            end
        else
            if breakTimer[i] <= 0 then
                meter[i] = math.min(
                    meter[i] + Shield.regenRate * dt,
                    Shield.maxMeter
                )
            end
        end

        -- visual positioning
        if active[i] then
            local p = Entity.get_global_pos(playerEnt[i])
            local px = Mafs.get_vec_x(p)
            local py = Mafs.get_vec_y(p)

            Entity.set_global_pos(
                shieldEnt[i],
                px - (Shield.size * 0.5) + Shield.xOffset,
                py + Shield.yOffset
            )
        else
            Entity.set_global_pos(shieldEnt[i], -1000, -1000)
        end
    end
end

----------------------------------------------------------
-- DAMAGE
----------------------------------------------------------
function Shield.AbsorbDamage(index, dmg)
    if not active[index] then return false end

    meter[index] = meter[index] - dmg

    if meter[index] <= 0 then
        meter[index] = 0
        active[index] = false
        latched[index] = false
        breakTimer[index] = Shield.breakCooldown
    end

    return true
end

return Shield
