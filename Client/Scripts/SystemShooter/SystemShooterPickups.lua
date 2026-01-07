local SystemShooterPickups = {}

local assets = require("Scripts.Assets")

--=====================================================================
--  [TUNING] Easy-to-adjust values for fine-tuning
--=====================================================================
local CONFIG = {

    BASE_HEALING_ORB_DROP_CHANCE = 0.15,  -- Base spawn chance (never modified)
    HEALING_ORB_DROP_CHANCE = 0.15,       -- Active spawn chance (gets modified by upgrades)
    
    HEALING_ORB_HEAL_PERCENT = 0.25,
    
    ORB_SIZE = 16,
    ORB_COLOR = {50, 255, 100},  -- Green
    
    -- Beat/bop animation
    BOP_SCALE = 0.3,             
    
    ORB_LIFETIME = 32.0,
    
    PICKUP_RADIUS = 24,
    
    RENDER_LAYER = 5,
}

SystemShooterPickups.CONFIG = CONFIG

--=====================================================================
--  [STATE] Active pickups
--=====================================================================
local activePickups = {}
local pickupPool = {}

--=====================================================================
--  [PUBLIC API] Spawn healing orb at position
--  NOTE: Health orbs are stationary - they spawn at a fixed position
--  and never move. The x,y coordinates are stored as simple values,
--  not references to enemy positions.
--=====================================================================
function SystemShooterPickups.trySpawnHealingOrb(x, y)
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
    
    -- Store spawn position as local values (not references)
    -- These coordinates never change after spawning
    local spawnX = x - CONFIG.ORB_SIZE / 2
    local spawnY = y - CONFIG.ORB_SIZE / 2
    pickup.x = spawnX
    pickup.y = spawnY
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
function SystemShooterPickups.update(dt)
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
function SystemShooterPickups.applyBeatBop(bopT)
    local scale = 1.0 + CONFIG.BOP_SCALE * bopT
    
    for i = 1, #activePickups do
        local pickup = activePickups[i]
        if pickup.sprite then
            local baseSize = pickup.baseSize or CONFIG.ORB_SIZE
            local scaledSize = math.floor(baseSize * scale)
            Sprite.set_image_width(pickup.sprite, scaledSize)
            Sprite.set_image_height(pickup.sprite, scaledSize)
            -- No position adjustment needed - sprite scaling is centered automatically
        end
    end
end

--=====================================================================
--  [PUBLIC API] Reset bop (when bop timer ends)
--=====================================================================
function SystemShooterPickups.resetBop()
    for i = 1, #activePickups do
        local pickup = activePickups[i]
        if pickup.sprite then
            local baseSize = pickup.baseSize or CONFIG.ORB_SIZE
            Sprite.set_image_width(pickup.sprite, baseSize)
            Sprite.set_image_height(pickup.sprite, baseSize)
            -- No position reset needed - position should remain unchanged
        end
    end
end

--=====================================================================
--  [PUBLIC API] Check collision with player, returns heal amount or nil
--=====================================================================
function SystemShooterPickups.checkPlayerCollision(playerX, playerY, playerSize, maxHealth)
    local playerCenterX = playerX
    local playerCenterY = playerY
    local playerRadius = playerSize / 2
    
    for i = #activePickups, 1, -1 do
        local pickup = activePickups[i]
        local pickupCenterX = pickup.x
        local pickupCenterY = pickup.y
        
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
function SystemShooterPickups.clearAll()
    for i = #activePickups, 1, -1 do
        local pickup = activePickups[i]
        Entity.set_global_pos(pickup.entity, -1000, -1000)
        table.insert(pickupPool, table.remove(activePickups, i))
    end
end

--=====================================================================
--  [PUBLIC API] Get active pickup count (for debugging)
--=====================================================================
function SystemShooterPickups.getActiveCount()
    return #activePickups
end

--=====================================================================
--  [PUBLIC API] Constrain all pickups to screen bounds
--=====================================================================
function SystemShooterPickups.constrainToScreen(screenW, screenH)
    for i = 1, #activePickups do
        local pickup = activePickups[i]
        local orbSize = CONFIG.ORB_SIZE
        local halfSize = orbSize / 2
        
        -- Clamp pickup center position to stay within screen bounds with margin
        if pickup.x - halfSize < 0 then
            pickup.x = halfSize
        elseif pickup.x + halfSize > screenW then
            pickup.x = screenW - halfSize
        end
        
        if pickup.y - halfSize < 0 then
            pickup.y = halfSize
        elseif pickup.y + halfSize > screenH then
            pickup.y = screenH - halfSize
        end
        
        -- Update entity position
        Entity.set_global_pos(pickup.entity, pickup.x, pickup.y)
    end
end

return SystemShooterPickups
