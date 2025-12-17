local TriangleShooterPickups = {}

local assets = require("Scripts.Assets")

--=====================================================================
--  [TUNING] Easy-to-adjust values for fine-tuning
--=====================================================================
local CONFIG = {
    -- Drop chance (0.0 to 1.0, where 0.15 = 15%)
    HEALING_ORB_DROP_CHANCE = 0.20,
    
    -- Healing amount (0.0 to 1.0, where 0.20 = 20% of max health)
    HEALING_ORB_HEAL_PERCENT = 0.15,
    
    -- Visual settings
    ORB_SIZE = 16,
    ORB_COLOR = {50, 255, 100},  -- Green
    
    -- Beat/bop animation
    BOP_SCALE = 0.3,             -- How much the orb scales up on beat (0.3 = 30%)
    
    -- Lifetime before despawn (seconds)
    ORB_LIFETIME = 16.0,
    
    -- Pickup radius
    PICKUP_RADIUS = 24,
    
    -- Render layer
    RENDER_LAYER = 5,
}

TriangleShooterPickups.CONFIG = CONFIG

--=====================================================================
--  [STATE] Active pickups
--=====================================================================
local activePickups = {}
local pickupPool = {}

--=====================================================================
--  [PUBLIC API] Spawn healing orb at position
--=====================================================================
function TriangleShooterPickups.trySpawnHealingOrb(x, y)
    local roll = math.random()
    if roll > CONFIG.HEALING_ORB_DROP_CHANCE then
        return false
    end
    
    local pickup
    if #pickupPool > 0 then
        pickup = table.remove(pickupPool)
    else
        local entity = Entity.create_entity()
        local sprite = Entity.add_sprite_component(
            entity,
            assets.textures.balls,
            CONFIG.ORB_SIZE,
            CONFIG.ORB_SIZE,
            CONFIG.RENDER_LAYER
        )
        pickup = {
            entity = entity,
            sprite = sprite,
        }
    end
    
    pickup.x = x - CONFIG.ORB_SIZE / 2
    pickup.y = y - CONFIG.ORB_SIZE / 2
    pickup.age = 0
    pickup.pickupType = "healing"
    pickup.baseSize = CONFIG.ORB_SIZE
    
    Entity.set_global_pos(pickup.entity, pickup.x, pickup.y)
    Sprite.set_color(pickup.sprite, CONFIG.ORB_COLOR[1], CONFIG.ORB_COLOR[2], CONFIG.ORB_COLOR[3])
    Sprite.set_image_width(pickup.sprite, CONFIG.ORB_SIZE)
    Sprite.set_image_height(pickup.sprite, CONFIG.ORB_SIZE)
    
    table.insert(activePickups, pickup)
    return true
end

--=====================================================================
--  [PUBLIC API] Update all pickups (call each frame)
--=====================================================================
function TriangleShooterPickups.update(dt)
    for i = #activePickups, 1, -1 do
        local pickup = activePickups[i]
        pickup.age = pickup.age + dt
        
        if pickup.age >= CONFIG.ORB_LIFETIME then
            Entity.set_global_pos(pickup.entity, -1000, -1000)
            table.insert(pickupPool, table.remove(activePickups, i))
        end
    end
end

--=====================================================================
--  [PUBLIC API] Apply beat/bop animation to all pickups
--=====================================================================
function TriangleShooterPickups.applyBeatBop(bopT)
    local scale = 1.0 + CONFIG.BOP_SCALE * bopT
    
    for i = 1, #activePickups do
        local pickup = activePickups[i]
        if pickup.sprite then
            local baseSize = pickup.baseSize or CONFIG.ORB_SIZE
            local scaledSize = math.floor(baseSize * scale)
            Sprite.set_image_width(pickup.sprite, scaledSize)
            Sprite.set_image_height(pickup.sprite, scaledSize)
            
            local offset = (scaledSize - baseSize) / 2
            Entity.set_global_pos(pickup.entity, pickup.x - offset, pickup.y - offset)
        end
    end
end

--=====================================================================
--  [PUBLIC API] Reset bop (when bop timer ends)
--=====================================================================
function TriangleShooterPickups.resetBop()
    for i = 1, #activePickups do
        local pickup = activePickups[i]
        if pickup.sprite then
            local baseSize = pickup.baseSize or CONFIG.ORB_SIZE
            Sprite.set_image_width(pickup.sprite, baseSize)
            Sprite.set_image_height(pickup.sprite, baseSize)
            Entity.set_global_pos(pickup.entity, pickup.x, pickup.y)
        end
    end
end

--=====================================================================
--  [PUBLIC API] Check collision with player, returns heal amount or nil
--=====================================================================
function TriangleShooterPickups.checkPlayerCollision(playerX, playerY, playerSize, maxHealth)
    local playerCenterX = playerX + playerSize / 2
    local playerCenterY = playerY + playerSize / 2
    local playerRadius = playerSize / 2
    
    for i = #activePickups, 1, -1 do
        local pickup = activePickups[i]
        local pickupCenterX = pickup.x + CONFIG.ORB_SIZE / 2
        local pickupCenterY = pickup.y + CONFIG.ORB_SIZE / 2
        
        local dx = playerCenterX - pickupCenterX
        local dy = playerCenterY - pickupCenterY
        local distSq = dx * dx + dy * dy
        local hitRadius = playerRadius + CONFIG.PICKUP_RADIUS
        
        if distSq < hitRadius * hitRadius then
            Entity.set_global_pos(pickup.entity, -1000, -1000)
            table.insert(pickupPool, table.remove(activePickups, i))
            
            if pickup.pickupType == "healing" then
                local healAmount = math.floor(maxHealth * CONFIG.HEALING_ORB_HEAL_PERCENT)
                return healAmount
            end
        end
    end
    
    return nil
end

--=====================================================================
--  [PUBLIC API] Clear all pickups (on level reset, game over, etc.)
--=====================================================================
function TriangleShooterPickups.clearAll()
    for i = #activePickups, 1, -1 do
        local pickup = activePickups[i]
        Entity.set_global_pos(pickup.entity, -1000, -1000)
        table.insert(pickupPool, table.remove(activePickups, i))
    end
end

--=====================================================================
--  [PUBLIC API] Get active pickup count (for debugging)
--=====================================================================
function TriangleShooterPickups.getActiveCount()
    return #activePickups
end

return TriangleShooterPickups
