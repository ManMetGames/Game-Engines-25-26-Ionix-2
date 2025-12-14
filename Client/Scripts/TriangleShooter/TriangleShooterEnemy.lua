local TriangleShooterEnemy = {}

local assets = require("Scripts.Assets")

local DEFAULTS = {
    baseSpeed = 550,
    size = 48,
    health = 50,
    color = {255, 255, 255},
    movementType = "bounce",
    shootPattern = "single",
    projectileCount = 1,
    shootInterval = 0.5,
    spinWhileShooting = false,
    bounceSteer = 0.4,
    steerStrength = 0.0,
    orbitRadius = 100,
    orbitSpeed = 1.0,
    spinMaxVelocity = 42,
    spinBoost = 1,
    spinDecayBase = 0.0004,
    spinDecayExtra = 0.01,
}

TriangleShooterEnemy.DEFAULTS = DEFAULTS

function TriangleShooterEnemy.createEnemy(x, y, config, playerX, playerY, playerSize)
    config = config or {}
    
    local size = config.size or DEFAULTS.size
    local health = config.health or DEFAULTS.health
    local color = config.color or DEFAULTS.color
    
    local entity = Entity.create_entity()
    Entity.set_global_pos(entity, x, y)
    local sprite = Entity.add_sprite_component(entity, assets.textures.Cube, size, size, 5)
    Sprite.set_columns(sprite, 1)
    Sprite.set_color(sprite, color[1], color[2], color[3])

    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    local enemyCenterX = x + size/2
    local enemyCenterY = y + size/2
    local dx = playerCenterX - enemyCenterX
    local dy = playerCenterY - enemyCenterY
    local dist = math.sqrt(dx * dx + dy * dy)
    local dirX, dirY = 0, 0
    if dist > 0 then
        dirX = dx / dist
        dirY = dy / dist
    end

    local enemy = {
        entity = entity,
        sprite = sprite,
        x = x,
        y = y,
        size = size,
        health = health,
        color = color,
        rotation = 0,
        spinVelocity = 0,
        speedMultiplier = config.speed or 1,
        dirX = dirX,
        dirY = dirY,
        shootTimer = 0,
        flashTimer = 0,
        
        movementType = config.movementType or DEFAULTS.movementType,
        baseSpeed = config.baseSpeed or DEFAULTS.baseSpeed,
        bounceSteer = config.bounceSteer or DEFAULTS.bounceSteer,
        steerStrength = config.steerStrength or DEFAULTS.steerStrength,
        
        shootPattern = config.shootPattern or DEFAULTS.shootPattern,
        projectileCount = config.projectileCount or DEFAULTS.projectileCount,
        shootInterval = config.shootInterval or DEFAULTS.shootInterval,
        spinWhileShooting = config.spinWhileShooting or DEFAULTS.spinWhileShooting,
        shootAngleOffset = 0,
        
        orbitCenter = config.orbitCenter,
        orbitRadius = config.orbitRadius or DEFAULTS.orbitRadius,
        orbitSpeed = config.orbitSpeed or DEFAULTS.orbitSpeed,
        orbitAngle = 0,
        
        spinMaxVelocity = config.spinMaxVelocity or DEFAULTS.spinMaxVelocity,
        spinBoost = config.spinBoost or DEFAULTS.spinBoost,
        spinDecayBase = config.spinDecayBase or DEFAULTS.spinDecayBase,
        spinDecayExtra = config.spinDecayExtra or DEFAULTS.spinDecayExtra,
    }

    return enemy
end

function TriangleShooterEnemy.clearEnemies(enemies)
    for i = 1, #enemies do
        local enemy = enemies[i]
        if enemy.entity then
            Entity.set_global_pos(enemy.entity, -1000, -1000)
        end
    end
end

local function updateBounceMovement(enemy, dt, playerCenterX, playerCenterY, screenW, screenH, TriggerWallLerp)
    local enemySize = enemy.size
    local minX, minY = 0, 0
    local maxX = screenW - enemySize
    local maxY = screenH - enemySize

    if enemy.steerStrength > 0 then
        local enemyCenterX = enemy.x + enemySize/2
        local enemyCenterY = enemy.y + enemySize/2
        local dx = playerCenterX - enemyCenterX
        local dy = playerCenterY - enemyCenterY
        local distToPlayer = math.sqrt(dx * dx + dy * dy)
        if distToPlayer > 0 then
            local toPlayerDirX = dx / distToPlayer
            local toPlayerDirY = dy / distToPlayer
            local steer = enemy.steerStrength
            local newDirX = enemy.dirX * (1 - steer) + toPlayerDirX * steer
            local newDirY = enemy.dirY * (1 - steer) + toPlayerDirY * steer
            local newLen = math.sqrt(newDirX * newDirX + newDirY * newDirY)
            if newLen > 0 then
                enemy.dirX = newDirX / newLen
                enemy.dirY = newDirY / newLen
            end
        end
    end

    local currentSpeed = enemy.baseSpeed * enemy.speedMultiplier
    enemy.x = enemy.x + enemy.dirX * currentSpeed * dt
    enemy.y = enemy.y + enemy.dirY * currentSpeed * dt

    local hitLeft, hitRight, hitTop, hitBottom = false, false, false, false

    if enemy.x <= minX then
        enemy.x = minX
        hitLeft = true
    elseif enemy.x >= maxX then
        enemy.x = maxX
        hitRight = true
    end
    if enemy.y <= minY then
        enemy.y = minY
        hitTop = true
    elseif enemy.y >= maxY then
        enemy.y = maxY
        hitBottom = true
    end

    if hitLeft or hitRight or hitTop or hitBottom then
        if hitLeft or hitRight then enemy.dirX = -enemy.dirX end
        if hitTop or hitBottom then enemy.dirY = -enemy.dirY end
        
        local len = math.sqrt(enemy.dirX * enemy.dirX + enemy.dirY * enemy.dirY)
        if len > 0 then
            enemy.dirX = enemy.dirX / len
            enemy.dirY = enemy.dirY / len
        end
        
        local enemyCenterX2 = enemy.x + enemySize/2
        local enemyCenterY2 = enemy.y + enemySize/2
        local toPlayerX = playerCenterX - enemyCenterX2
        local toPlayerY = playerCenterY - enemyCenterY2
        local dist = math.sqrt(toPlayerX * toPlayerX + toPlayerY * toPlayerY)
        if dist > 0 then
            local toPlayerDirX = toPlayerX / dist
            local toPlayerDirY = toPlayerY / dist
            local steer = enemy.bounceSteer
            local newDirX = enemy.dirX * (1 - steer) + toPlayerDirX * steer
            local newDirY = enemy.dirY * (1 - steer) + toPlayerDirY * steer
            local newLen = math.sqrt(newDirX * newDirX + newDirY * newDirY)
            if newLen > 0 then
                enemy.dirX = newDirX / newLen
                enemy.dirY = newDirY / newLen
            end
        end

        if TriggerWallLerp then
            if hitLeft then TriggerWallLerp("left") end
            if hitRight then TriggerWallLerp("right") end
            if hitTop then TriggerWallLerp("top") end
            if hitBottom then TriggerWallLerp("bottom") end
        end

        enemy.spinVelocity = (enemy.spinVelocity or 0) + enemy.spinBoost
        if enemy.spinVelocity > enemy.spinMaxVelocity then
            enemy.spinVelocity = enemy.spinMaxVelocity
        end
        enemy.speedMultiplier = enemy.speedMultiplier * 1.002
    end
end

local function updateOrbitMovement(enemy, dt, screenW, screenH)
    local centerX = enemy.orbitCenter and enemy.orbitCenter[1] or (screenW / 2)
    local centerY = enemy.orbitCenter and enemy.orbitCenter[2] or (screenH / 2)
    
    enemy.orbitAngle = enemy.orbitAngle + enemy.orbitSpeed * dt
    enemy.x = centerX + math.cos(enemy.orbitAngle) * enemy.orbitRadius - enemy.size/2
    enemy.y = centerY + math.sin(enemy.orbitAngle) * enemy.orbitRadius - enemy.size/2
end

local function updateSpin(enemy, dt)
    local speed = math.abs(enemy.spinVelocity or 0)
    local t = speed / enemy.spinMaxVelocity
    if t > 1 then t = 1 end
    local decay = enemy.spinDecayBase + enemy.spinDecayExtra * t
    enemy.spinVelocity = (enemy.spinVelocity or 0) - (enemy.spinVelocity or 0) * decay
    if math.abs(enemy.spinVelocity or 0) < 0.01 then
        enemy.spinVelocity = 0
    end
    enemy.rotation = (enemy.rotation or 0) + enemy.spinVelocity
end

local function updateShootingRotation(enemy, dt)
    if enemy.spinWhileShooting and enemy.projectileCount > 4 then
        enemy.shootAngleOffset = (enemy.shootAngleOffset or 0) + dt * 2
    end
end

function TriangleShooterEnemy.updateEnemyMovement(
    enemies,
    playerX, playerY, playerSize,
    screenW, screenH,
    enemyProjectilesEnabled, enemyShootIntervalSeconds,
    SpawnEnemyProjectile,
    TriggerWallLerp
)
    local dt = Mafs.delta_time()
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2

    for i = 1, #enemies do
        local enemy = enemies[i]
        local movementType = enemy.movementType or "bounce"

        if movementType == "bounce" then
            updateBounceMovement(enemy, dt, playerCenterX, playerCenterY, screenW, screenH, TriggerWallLerp)
        elseif movementType == "orbit" then
            updateOrbitMovement(enemy, dt, screenW, screenH)
        elseif movementType == "stationary" then
            -- No movement
        end

        local shootInterval = enemy.shootInterval
        if enemyProjectilesEnabled and shootInterval and shootInterval > 0 then
            enemy.shootTimer = (enemy.shootTimer or 0) + dt
            if enemy.shootTimer >= shootInterval then
                SpawnEnemyProjectile(enemy)
                enemy.shootTimer = enemy.shootTimer - shootInterval
            end
        end

        updateShootingRotation(enemy, dt)
        updateSpin(enemy, dt)
        
        Entity.set_global_rot(enemy.entity, enemy.rotation)
        Entity.set_global_pos(enemy.entity, enemy.x, enemy.y)
    end
end

return TriangleShooterEnemy
