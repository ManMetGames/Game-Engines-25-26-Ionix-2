local ParticleSystem = {}

local assets = require("Scripts.Assets")

local particles = {}
local pool = {}

local HIT_PARTICLE_SIZE = 16
local HIT_PARTICLE_LIFETIME = 0.25
local HIT_PARTICLE_COUNT = 8
local HIT_PARTICLE_MIN_SPEED = 240
local HIT_PARTICLE_MAX_SPEED = 520

local OFFSCREEN_X = -1000
local OFFSCREEN_Y = -1000

local function allocateParticle()
    local p
    if #pool > 0 then
        p = table.remove(pool)
    else
        local entity = Entity.create_entity()
        local sprite = Entity.add_sprite_component(entity, assets.textures.balls, HIT_PARTICLE_SIZE, HIT_PARTICLE_SIZE, 7)
        Sprite.set_columns(sprite, 1)
        p = {
            entity = entity,
            sprite = sprite,
            x = OFFSCREEN_X,
            y = OFFSCREEN_Y,
            vx = 0,
            vy = 0,
            age = 0,
            lifetime = HIT_PARTICLE_LIFETIME,
        }
    end

    p.age = 0
    p.lifetime = HIT_PARTICLE_LIFETIME
    p.vx = 0
    p.vy = 0
    p.x = OFFSCREEN_X
    p.y = OFFSCREEN_Y

    return p
end

local function recycleParticle(index)
    local p = table.remove(particles, index)
    if p and p.entity then
        Entity.set_global_pos(p.entity, OFFSCREEN_X, OFFSCREEN_Y)
        table.insert(pool, p)
    end
end

function ParticleSystem.emitHitBurst(x, y)
    for i = 1, HIT_PARTICLE_COUNT do
        local p = allocateParticle()
        local angle = math.random() * 2 * math.pi
        local speed = math.random(HIT_PARTICLE_MIN_SPEED, HIT_PARTICLE_MAX_SPEED)
        p.vx = math.cos(angle) * speed
        p.vy = math.sin(angle) * speed
        p.x = x - HIT_PARTICLE_SIZE / 2
        p.y = y - HIT_PARTICLE_SIZE / 2
        Entity.set_global_pos(p.entity, p.x, p.y)
        Sprite.set_color(p.sprite, 255, 255, 255)
        table.insert(particles, p)
    end
end

function ParticleSystem.update(dt)
    if #particles == 0 then
        return
    end

    for i = #particles, 1, -1 do
        local p = particles[i]
        p.age = p.age + dt
        if p.age >= p.lifetime then
            recycleParticle(i)
        else
            p.x = p.x + p.vx * dt
            p.y = p.y + p.vy * dt
            Entity.set_global_pos(p.entity, p.x, p.y)

            local t = p.age / p.lifetime
            if t < 0 then t = 0 end
            if t > 1 then t = 1 end

            local size = HIT_PARTICLE_SIZE * (1.0 - t * 0.75)
            local s = math.floor(size + 0.5)
            Sprite.set_image_width(p.sprite, s)
            Sprite.set_image_height(p.sprite, s)
        end
    end
end

return ParticleSystem
