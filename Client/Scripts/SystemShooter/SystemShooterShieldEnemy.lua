local SystemShooterShieldEnemy = {}

--=====================================================================
--  [MODULE] Shield Enemy Behavior System
--  A shield enemy that protects allies by positioning itself between
--  the player and its chosen ally, rotating a 180° shield to block shots.
--=====================================================================

--=====================================================================
--  [CONFIG] Tuning Parameters
--=====================================================================
local CONFIG = {
    -- Orbit / positioning
    orbitRadius = 65,              -- Distance to maintain from ally center
    approachSpeed = 280,           -- Speed when moving towards ally (pixels/sec)
    orbitSpeed = 2.8,              -- Speed when repositioning around ally (radians/sec)
    
    -- Shield rotation
    shieldRotationSpeed = 4.5,     -- Max shield rotation speed (radians/sec)
    
    -- Shield VFX
    shieldOuterRadius = 55,        -- VFX outer radius
    shieldInnerRadius = 38,        -- VFX inner radius (thickness)
    
    -- Ally selection
    allySearchRadius = 600,        -- Max distance to search for allies
    
    -- Blocking detection
    blockingAngleTolerance = 0.35, -- Radians tolerance for "blocking" check (~20 degrees)
    repositionThreshold = 0.5,     -- Start repositioning if angle diff exceeds this
}

SystemShooterShieldEnemy.CONFIG = CONFIG

--=====================================================================
--  [STATE] Shield Enemy States
--=====================================================================
local STATE = {
    SELECTING_ALLY = 1,    -- Finding an ally to protect
    APPROACHING = 2,       -- Moving towards chosen ally
    GUARDING = 3,          -- In position, rotating shield to face player
    REPOSITIONING = 4,     -- Orbiting around ally to block clear shots
}

SystemShooterShieldEnemy.STATE = STATE

--=====================================================================
--  [HELPERS] Math Utilities
--=====================================================================

-- Normalize angle to [-π, π]
local function normalizeAngle(angle)
    while angle > math.pi do angle = angle - 2 * math.pi end
    while angle < -math.pi do angle = angle + 2 * math.pi end
    return angle
end

-- Shortest angle difference (signed)
local function angleDiff(from, to)
    return normalizeAngle(to - from)
end

-- Lerp angle with max step
local function lerpAngle(current, target, maxStep)
    local diff = angleDiff(current, target)
    if math.abs(diff) <= maxStep then
        return target
    end
    if diff > 0 then
        return current + maxStep
    else
        return current - maxStep
    end
end

-- Distance between two points
local function distance(x1, y1, x2, y2)
    local dx = x2 - x1
    local dy = y2 - y1
    return math.sqrt(dx * dx + dy * dy)
end

--=====================================================================
--  [PUBLIC API] Initialize Shield Enemy
--  Call this after creating the enemy to set up shield-specific data
--=====================================================================
function SystemShooterShieldEnemy.initShieldEnemy(enemy, screenW, screenH)
    -- Shield enemy specific state
    enemy.shieldState = STATE.SELECTING_ALLY
    enemy.targetAlly = nil
    enemy.targetAllyIndex = nil
    
    -- Position relative to ally
    enemy.orbitAngle = 0           -- Current angle around ally (radians)
    enemy.orbitRadius = CONFIG.orbitRadius
    
    -- Shield facing direction (towards player)
    enemy.shieldRotation = 0       -- Radians, 0 = right, π/2 = down
    enemy.shieldRotationSpeed = CONFIG.shieldRotationSpeed
    
    return enemy
end

--=====================================================================
--  [PUBLIC API] Cleanup Shield Enemy
--  Call this when the enemy is destroyed
--=====================================================================
function SystemShooterShieldEnemy.destroyShieldEnemy(enemy)
    -- Cleanup will be handled when VFX is reimplemented
end

--=====================================================================
--  [ALLY SELECTION] Find best ally to protect
--=====================================================================
function SystemShooterShieldEnemy.selectBestAlly(enemy, enemies, enemyIndex)
    local bestAlly = nil
    local bestAllyIndex = nil
    local bestScore = -math.huge
    
    local enemyCenterX = enemy.x + (enemy.displaySize or enemy.size) / 2
    local enemyCenterY = enemy.y + (enemy.displaySize or enemy.size) / 2
    
    for i = 1, #enemies do
        local ally = enemies[i]
        
        -- Skip self
        if i == enemyIndex then
            goto continue
        end
        
        -- Skip dead or disabled allies
        if ally.isDead or ally.disabled then
            goto continue
        end
        
        -- Skip other shield enemies (don't protect each other)
        if ally.movementType == "shield" then
            goto continue
        end
        
        -- Calculate ally center
        local allyCenterX = ally.x + (ally.displaySize or ally.size) / 2
        local allyCenterY = ally.y + (ally.displaySize or ally.size) / 2
        
        -- Check distance
        local dist = distance(enemyCenterX, enemyCenterY, allyCenterX, allyCenterY)
        if dist > CONFIG.allySearchRadius then
            goto continue
        end
        
        -- Score: prefer closer allies with higher health
        -- Stationary enemies are good targets (can't dodge)
        local score = 1000 - dist
        score = score + ally.health * 2
        
        if ally.movementType == "stationary" or ally.movementType == "stationary_boss" then
            score = score + 300  -- Prefer stationary targets
        end
        
        if score > bestScore then
            bestScore = score
            bestAlly = ally
            bestAllyIndex = i
        end
        
        ::continue::
    end
    
    return bestAlly, bestAllyIndex
end

--=====================================================================
--  [POSITIONING] Check if at orbit distance from ally
--=====================================================================
local function isAtOrbitDistance(enemy, ally, tolerance)
    tolerance = tolerance or 10
    
    local enemyCenterX = enemy.x + (enemy.displaySize or enemy.size) / 2
    local enemyCenterY = enemy.y + (enemy.displaySize or enemy.size) / 2
    local allyCenterX = ally.x + (ally.displaySize or ally.size) / 2
    local allyCenterY = ally.y + (ally.displaySize or ally.size) / 2
    
    local dist = distance(enemyCenterX, enemyCenterY, allyCenterX, allyCenterY)
    return math.abs(dist - enemy.orbitRadius) <= tolerance
end

--=====================================================================
--  [BLOCKING] Calculate angle from ally to player
--=====================================================================
local function getAngleAllyToPlayer(ally, playerX, playerY)
    local allyCenterX = ally.x + (ally.displaySize or ally.size) / 2
    local allyCenterY = ally.y + (ally.displaySize or ally.size) / 2
    return math.atan(playerY - allyCenterY, playerX - allyCenterX)
end

--=====================================================================
--  [BLOCKING] Check if shield is blocking player's shot to ally
--=====================================================================
local function isShieldBlockingPlayer(enemy, ally, playerX, playerY)
    local allyCenterX = ally.x + (ally.displaySize or ally.size) / 2
    local allyCenterY = ally.y + (ally.displaySize or ally.size) / 2
    
    -- Angle from ally center to player
    local angleToPlayer = math.atan(playerY - allyCenterY, playerX - allyCenterX)
    
    -- Shield enemy's current angle relative to ally
    local enemyCenterX = enemy.x + (enemy.displaySize or enemy.size) / 2
    local enemyCenterY = enemy.y + (enemy.displaySize or enemy.size) / 2
    local shieldAngleFromAlly = math.atan(enemyCenterY - allyCenterY, enemyCenterX - allyCenterX)
    
    -- Check if shield enemy is roughly between ally and player
    local angleDifference = math.abs(angleDiff(shieldAngleFromAlly, angleToPlayer))
    
    return angleDifference <= CONFIG.blockingAngleTolerance
end

--=====================================================================
--  [PUBLIC API] Update Shield Enemy
--  Main update function called each frame
--=====================================================================
function SystemShooterShieldEnemy.updateShieldEnemy(enemy, enemies, enemyIndex, playerX, playerY, screenW, screenH, dt)
    if enemy.isDead or enemy.disabled then
        return
    end
    
    local state = enemy.shieldState
    local ally = enemy.targetAlly
    
    -- Check if ally is still valid
    if ally and (ally.isDead or ally.disabled) then
        enemy.targetAlly = nil
        enemy.targetAllyIndex = nil
        enemy.shieldState = STATE.SELECTING_ALLY
        state = STATE.SELECTING_ALLY
    end
    
    --=================================================================
    -- STATE: SELECTING_ALLY
    --=================================================================
    if state == STATE.SELECTING_ALLY then
        local newAlly, newAllyIndex = SystemShooterShieldEnemy.selectBestAlly(enemy, enemies, enemyIndex)
        
        if newAlly then
            enemy.targetAlly = newAlly
            enemy.targetAllyIndex = newAllyIndex
            enemy.shieldState = STATE.APPROACHING
            
            -- Calculate initial orbit angle (direction from ally to shield enemy)
            local allyCenterX = newAlly.x + (newAlly.displaySize or newAlly.size) / 2
            local allyCenterY = newAlly.y + (newAlly.displaySize or newAlly.size) / 2
            local enemyCenterX = enemy.x + (enemy.displaySize or enemy.size) / 2
            local enemyCenterY = enemy.y + (enemy.displaySize or enemy.size) / 2
            enemy.orbitAngle = math.atan(enemyCenterY - allyCenterY, enemyCenterX - allyCenterX)
        end
        -- If no ally found, stay in SELECTING_ALLY state
        
    --=================================================================
    -- STATE: APPROACHING
    --=================================================================
    elseif state == STATE.APPROACHING then
        ally = enemy.targetAlly
        if not ally then
            enemy.shieldState = STATE.SELECTING_ALLY
            return
        end
        
        local allyCenterX = ally.x + (ally.displaySize or ally.size) / 2
        local allyCenterY = ally.y + (ally.displaySize or ally.size) / 2
        local enemyCenterX = enemy.x + (enemy.displaySize or enemy.size) / 2
        local enemyCenterY = enemy.y + (enemy.displaySize or enemy.size) / 2
        
        local dist = distance(enemyCenterX, enemyCenterY, allyCenterX, allyCenterY)
        
        if dist <= enemy.orbitRadius + 5 then
            -- Reached orbit distance
            enemy.shieldState = STATE.GUARDING
            -- Update orbit angle to current position
            enemy.orbitAngle = math.atan(enemyCenterY - allyCenterY, enemyCenterX - allyCenterX)
        else
            -- Move towards ally
            local dx = allyCenterX - enemyCenterX
            local dy = allyCenterY - enemyCenterY
            local dirX = dx / dist
            local dirY = dy / dist
            
            -- Stop at orbit radius
            local moveDistance = math.min(CONFIG.approachSpeed * dt, dist - enemy.orbitRadius)
            enemy.x = enemy.x + dirX * moveDistance
            enemy.y = enemy.y + dirY * moveDistance
        end
        
        -- While approaching, still rotate shield towards player
        local angleToPlayer = math.atan(playerY - enemyCenterY, playerX - enemyCenterX)
        enemy.shieldRotation = lerpAngle(enemy.shieldRotation, angleToPlayer, enemy.shieldRotationSpeed * dt)
        
    --=================================================================
    -- STATE: GUARDING
    --=================================================================
    elseif state == STATE.GUARDING then
        ally = enemy.targetAlly
        if not ally then
            enemy.shieldState = STATE.SELECTING_ALLY
            return
        end
        
        local allyCenterX = ally.x + (ally.displaySize or ally.size) / 2
        local allyCenterY = ally.y + (ally.displaySize or ally.size) / 2
        local enemyCenterX = enemy.x + (enemy.displaySize or enemy.size) / 2
        local enemyCenterY = enemy.y + (enemy.displaySize or enemy.size) / 2
        
        -- Rotate shield to face player
        local angleToPlayer = math.atan(playerY - enemyCenterY, playerX - enemyCenterX)
        enemy.shieldRotation = lerpAngle(enemy.shieldRotation, angleToPlayer, enemy.shieldRotationSpeed * dt)
        
        -- Check if we're blocking the player's shot to ally
        if not isShieldBlockingPlayer(enemy, ally, playerX, playerY) then
            -- Player has moved, check if we need to reposition
            local angleAllyToPlayer = getAngleAllyToPlayer(ally, playerX, playerY)
            local currentAngle = math.atan(enemyCenterY - allyCenterY, enemyCenterX - allyCenterX)
            local diff = math.abs(angleDiff(currentAngle, angleAllyToPlayer))
            
            if diff > CONFIG.repositionThreshold then
                enemy.shieldState = STATE.REPOSITIONING
            end
        end
        
        -- Stay at orbit distance (in case ally moves)
        local dist = distance(enemyCenterX, enemyCenterY, allyCenterX, allyCenterY)
        if math.abs(dist - enemy.orbitRadius) > 5 then
            local dx = enemyCenterX - allyCenterX
            local dy = enemyCenterY - allyCenterY
            if dist > 0 then
                local targetX = allyCenterX + (dx / dist) * enemy.orbitRadius - (enemy.displaySize or enemy.size) / 2
                local targetY = allyCenterY + (dy / dist) * enemy.orbitRadius - (enemy.displaySize or enemy.size) / 2
                enemy.x = enemy.x + (targetX - enemy.x) * 0.1
                enemy.y = enemy.y + (targetY - enemy.y) * 0.1
            end
        end
        
    --=================================================================
    -- STATE: REPOSITIONING
    --=================================================================
    elseif state == STATE.REPOSITIONING then
        ally = enemy.targetAlly
        if not ally then
            enemy.shieldState = STATE.SELECTING_ALLY
            return
        end
        
        local allyCenterX = ally.x + (ally.displaySize or ally.size) / 2
        local allyCenterY = ally.y + (ally.displaySize or ally.size) / 2
        local enemyCenterX = enemy.x + (enemy.displaySize or enemy.size) / 2
        local enemyCenterY = enemy.y + (enemy.displaySize or enemy.size) / 2
        
        -- Target angle: between ally and player
        local angleAllyToPlayer = getAngleAllyToPlayer(ally, playerX, playerY)
        
        -- Current orbit angle
        local currentOrbitAngle = math.atan(enemyCenterY - allyCenterY, enemyCenterX - allyCenterX)
        
        -- Determine which direction to orbit (shortest path)
        local diff = angleDiff(currentOrbitAngle, angleAllyToPlayer)
        local orbitDirection = 1
        if diff < 0 then orbitDirection = -1 end
        
        -- Update orbit angle
        local orbitStep = CONFIG.orbitSpeed * dt
        if math.abs(diff) <= orbitStep then
            enemy.orbitAngle = angleAllyToPlayer
            enemy.shieldState = STATE.GUARDING
        else
            enemy.orbitAngle = enemy.orbitAngle + orbitDirection * orbitStep
        end
        
        -- Update position based on orbit angle
        local halfSize = (enemy.displaySize or enemy.size) / 2
        enemy.x = allyCenterX + math.cos(enemy.orbitAngle) * enemy.orbitRadius - halfSize
        enemy.y = allyCenterY + math.sin(enemy.orbitAngle) * enemy.orbitRadius - halfSize
        
        -- Rotate shield to face player while repositioning
        local newEnemyCenterX = enemy.x + halfSize
        local newEnemyCenterY = enemy.y + halfSize
        local angleToPlayer = math.atan(playerY - newEnemyCenterY, playerX - newEnemyCenterX)
        enemy.shieldRotation = lerpAngle(enemy.shieldRotation, angleToPlayer, enemy.shieldRotationSpeed * dt)
    end

end

--=====================================================================
--  [PUBLIC API] Check if projectile is blocked by shield
--  Returns true if projectile should be blocked
--=====================================================================
function SystemShooterShieldEnemy.isProjectileBlocked(enemy, projX, projY, projDirX, projDirY)
    if enemy.isDead or enemy.disabled then
        return false
    end
    
    local halfSize = (enemy.displaySize or enemy.size) / 2
    local shieldCenterX = enemy.x + halfSize
    local shieldCenterY = enemy.y + halfSize
    
    -- Check if projectile is within shield radius
    local dx = projX - shieldCenterX
    local dy = projY - shieldCenterY
    local dist = math.sqrt(dx * dx + dy * dy)
    
    if dist > CONFIG.shieldOuterRadius or dist < CONFIG.shieldInnerRadius then
        return false
    end
    
    -- Check if projectile is within shield arc (180°)
    local angleToProj = math.atan(dy, dx)
    local shieldFacing = enemy.shieldRotation
    
    -- Shield covers from (facing - 90°) to (facing + 90°)
    local angleDifference = math.abs(angleDiff(angleToProj, shieldFacing))
    
    -- If projectile is within the 180° arc (90° on each side)
    return angleDifference <= math.pi / 2
end

--=====================================================================
--  [PUBLIC API] Destroy shield enemy VFX (called on death)
--=====================================================================
function SystemShooterShieldEnemy.destroyShieldEnemy(enemy)
    if enemy.shieldVfxId and enemy.shieldVfxId >= 0 then
        VFX.destroy_shield(enemy.shieldVfxId)
        enemy.shieldVfxId = -1
    end
    enemy.shieldState = nil
    enemy.protectedAllyIndex = nil
end

--=====================================================================
--  [PUBLIC API] Get shield enemy color
--=====================================================================
function SystemShooterShieldEnemy.getColor()
    return {0, 200, 200}  -- Cyan-ish to match shield
end

return SystemShooterShieldEnemy
