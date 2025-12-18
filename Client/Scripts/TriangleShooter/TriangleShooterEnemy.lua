local TriangleShooterEnemy = {}

 --=====================================================================
 --  [MODULE] Dependencies
 --=====================================================================
local assets = require("Scripts.Assets")

 --=====================================================================
 --  [TUNING] ENEMY STATS / TYPES
 --=====================================================================
local DEFAULTS = {
    baseSpeed = 550,
    size = 30,
    sizePerHp = 0.125,
    health = 50,
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
    teleportChargeTime = 0.5,
    teleportCooldown = 1.5,
    beamDuration = 0.3,
}

local ENEMY_TYPE_COLORS = {
    bounce = {100, 255, 100},
    stationary = {100, 150, 255},
    stationary_boss = {255, 100, 100},
    orbit = {200, 100, 255},
    teleporter = nil,
}

TriangleShooterEnemy.DEFAULTS = DEFAULTS

 --=====================================================================
 --  [INTERNAL] Forward Declarations
 --=====================================================================
 local updateBounceMovement
 local updateOrbitMovement
 local updateTeleporterMovement
 local updateSpin
 local updateShootingRotation

 --=====================================================================
 --  [PUBLIC API] Enemy Lifecycle (Create / Clear)
 --=====================================================================
function TriangleShooterEnemy.createEnemy(x, y, config, playerX, playerY, playerSize)
    config = config or {}
    
    local size = config.size or DEFAULTS.size
    local health = config.health or DEFAULTS.health
    local movementType = config.movementType or DEFAULTS.movementType
    local colorKey = movementType
    if colorKey == "stationary" and config.spinWhileShooting then
        colorKey = "stationary_boss"
    end
    local color = config.color or ENEMY_TYPE_COLORS[colorKey] or {255, 255, 255}
    
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

    local healthScaling = config.healthScaling
    if healthScaling == nil then healthScaling = true end
    
    local sizePerHp = config.sizePerHp or DEFAULTS.sizePerHp
    local displaySize = size
    if healthScaling then
        displaySize = size + (health * sizePerHp)
    end

    local enemy = {
        entity = entity,
        sprite = sprite,
        x = x,
        y = y,
        size = size,
        health = health,
        healthScaling = healthScaling,
        sizePerHp = sizePerHp,
        displaySize = displaySize,
        color = color,
        rotation = 0,
        spinVelocity = 0,
        speedMultiplier = config.speed or 1,
        dirX = dirX,
        dirY = dirY,
        shootTimer = 0,
        flashTimer = 0,
        
        movementType = movementType,
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
        
        teleportChargeTime = config.teleportChargeTime or DEFAULTS.teleportChargeTime,
        teleportCooldown = config.teleportCooldown or DEFAULTS.teleportCooldown,
        beamDuration = config.beamDuration or DEFAULTS.beamDuration,
        teleportState = "charging",
        teleportTimer = 0,
        teleportVisible = true,
        beamTargetX = 0,
        beamTargetY = 0,
        rainbowHue = 0,
        baseSize = size,
    }

    Sprite.set_image_width(sprite, math.floor(displaySize))
    Sprite.set_image_height(sprite, math.floor(displaySize))

    return enemy
end

function TriangleShooterEnemy.updateDisplaySize(enemy)
    if not enemy.healthScaling then return end
    
    local hp = enemy.health
    if hp < 0 then hp = 0 end
    
    enemy.displaySize = enemy.size + (hp * enemy.sizePerHp)
end

function TriangleShooterEnemy.clearEnemies(enemies)
    for i = 1, #enemies do
        local enemy = enemies[i]
        if enemy.entity then
            Entity.set_global_pos(enemy.entity, -1000, -1000)
        end
    end
end

function TriangleShooterEnemy.setEnemyDisabled(enemy, disabled, screenW, screenH)
    enemy.disabled = disabled
    if enemy.sprite then
        local r, g, b = enemy.color[1], enemy.color[2], enemy.color[3]
        if disabled then
            -- Half alpha by darkening the color (since we may not have alpha control)
            Sprite.set_color(enemy.sprite, math.floor(r * 0.5), math.floor(g * 0.5), math.floor(b * 0.5))
            
            -- For orbit enemies: calculate and store the orbitAngle that matches current position
            -- This prevents warping when re-enabled
            if enemy.movementType == "orbit" then
                local centerX = enemy.orbitCenter and enemy.orbitCenter[1] or ((screenW or 1920) / 2)
                local centerY = enemy.orbitCenter and enemy.orbitCenter[2] or ((screenH or 1080) / 2)
                local eDisplaySize = enemy.displaySize or enemy.size
                local enemyCenterX = enemy.x + eDisplaySize / 2
                local enemyCenterY = enemy.y + eDisplaySize / 2
                enemy.orbitAngle = math.atan(enemyCenterY - centerY, enemyCenterX - centerX)
            end
        else
            Sprite.set_color(enemy.sprite, r, g, b)
        end
    end
end

function TriangleShooterEnemy.enableAllEnemies(enemies)
    for i = 1, #enemies do
        TriangleShooterEnemy.setEnemyDisabled(enemies[i], false)
    end
end

function TriangleShooterEnemy.disableAllEnemies(enemies, screenW, screenH)
    for i = 1, #enemies do
        TriangleShooterEnemy.setEnemyDisabled(enemies[i], true, screenW, screenH)
    end
end

 --=====================================================================
 --  [PUBLIC API] Per-Frame Update
 --=====================================================================
 function TriangleShooterEnemy.updateEnemyMovement(
     enemies,
     playerX, playerY, playerSize,
     screenW, screenH,
     enemyProjectilesEnabled, enemyShootIntervalSeconds,
     SpawnEnemyProjectile,
     TriggerWallLerp,
     SpawnBeam,
     EmitTeleportBurst,
     EmitBeamCharge
 )
     local dt = Mafs.delta_time()
     local playerCenterX = playerX + playerSize/2
     local playerCenterY = playerY + playerSize/2

     for i = 1, #enemies do
         local enemy = enemies[i]
         
         -- Skip all behavior for disabled enemies (preview state)
         if enemy.disabled then
             goto continue
         end
         
         local movementType = enemy.movementType or "bounce"

         if movementType == "bounce" then
             updateBounceMovement(enemy, dt, playerCenterX, playerCenterY, screenW, screenH, TriggerWallLerp)
         elseif movementType == "orbit" then
             updateOrbitMovement(enemy, dt, screenW, screenH)
         elseif movementType == "stationary" then
             -- No movement
         elseif movementType == "stationary_boss" then
             -- No movement
         elseif movementType == "teleporter" then
             updateTeleporterMovement(enemy, dt, playerCenterX, playerCenterY, screenW, screenH, SpawnBeam, EmitTeleportBurst, EmitBeamCharge)
         end

         if movementType ~= "teleporter" then
             local shootInterval = enemy.shootInterval
             if enemyProjectilesEnabled and shootInterval and shootInterval > 0 then
                 enemy.shootTimer = (enemy.shootTimer or 0) + dt
                 if enemy.shootTimer >= shootInterval then
                     SpawnEnemyProjectile(enemy)
                     enemy.shootTimer = enemy.shootTimer - shootInterval
                 end
             end
         end

         updateShootingRotation(enemy, dt)
         updateSpin(enemy, dt)
         
         Entity.set_global_rot(enemy.entity, enemy.rotation)
         if enemy.teleportVisible ~= false then
             Entity.set_global_pos(enemy.entity, enemy.x, enemy.y)
         end
        
        ::continue::
    end
end

 --=====================================================================
 --  [MOVEMENT] Bounce
 --=====================================================================
 updateBounceMovement = function(enemy, dt, playerCenterX, playerCenterY, screenW, screenH, TriggerWallLerp)
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

 --=====================================================================
 --  [MOVEMENT] Orbit
 --=====================================================================
 updateOrbitMovement = function(enemy, dt, screenW, screenH)
    local centerX = enemy.orbitCenter and enemy.orbitCenter[1] or (screenW / 2)
    local centerY = enemy.orbitCenter and enemy.orbitCenter[2] or (screenH / 2)
    local eDisplaySize = enemy.displaySize or enemy.size
    
    enemy.orbitAngle = enemy.orbitAngle + enemy.orbitSpeed * dt
    enemy.x = centerX + math.cos(enemy.orbitAngle) * enemy.orbitRadius - eDisplaySize/2
    enemy.y = centerY + math.sin(enemy.orbitAngle) * enemy.orbitRadius - eDisplaySize/2
end

 --=====================================================================
 --  [HELPERS] Color / Math
 --=====================================================================
local function hsvToRgb(h, s, v)
    local i = math.floor(h * 6)
    local f = h * 6 - i
    local p = v * (1 - s)
    local q = v * (1 - f * s)
    local t = v * (1 - (1 - f) * s)
    i = i % 6
    if i == 0 then return v, t, p
    elseif i == 1 then return q, v, p
    elseif i == 2 then return p, v, t
    elseif i == 3 then return p, q, v
    elseif i == 4 then return t, p, v
    else return v, p, q
    end
end

 --=====================================================================
 --  [MOVEMENT] Teleporter State Machine
 --=====================================================================
local TELEPORT_SHRINK_DURATION = 0.25
local TELEPORT_GROW_DURATION = 0.25

 updateTeleporterMovement = function(enemy, dt, playerCenterX, playerCenterY, screenW, screenH, SpawnBeam, EmitTeleportBurst, EmitBeamCharge)
    enemy.teleportTimer = enemy.teleportTimer + dt
    
    enemy.rainbowHue = (enemy.rainbowHue + dt * 0.5) % 1.0
    local r, g, b = hsvToRgb(enemy.rainbowHue, 1, 1)
    enemy.color = {math.floor(r * 255), math.floor(g * 255), math.floor(b * 255)}
    if enemy.teleportVisible and enemy.sprite then
        local cr, cg, cb = enemy.color[1], enemy.color[2], enemy.color[3]
        -- Respect disabled state (darken colors)
        if enemy.disabled then
            cr = math.floor(cr * 0.5)
            cg = math.floor(cg * 0.5)
            cb = math.floor(cb * 0.5)
        end
        Sprite.set_color(enemy.sprite, cr, cg, cb)
    end
    
    enemy.rotation = (enemy.rotation or 0) + dt * 120
    
    if enemy.teleportState == "charging" then
        local eDisplaySize = enemy.displaySize or enemy.size
        local enemyCenterX = enemy.x + eDisplaySize/2
        local enemyCenterY = enemy.y + eDisplaySize/2
        
        local lockTime = enemy.teleportChargeTime - 0.2
        if lockTime < 0 then lockTime = 0 end
        if not enemy.beamLocked and enemy.teleportTimer >= lockTime then
            enemy.beamTargetX = playerCenterX
            enemy.beamTargetY = playerCenterY
            enemy.beamLocked = true
        end
        
        local targetX = enemy.beamLocked and enemy.beamTargetX or playerCenterX
        local targetY = enemy.beamLocked and enemy.beamTargetY or playerCenterY
        
        local dx = targetX - enemyCenterX
        local dy = targetY - enemyCenterY
        local dist = math.sqrt(dx * dx + dy * dy)
        local chargeX = enemyCenterX
        local chargeY = enemyCenterY
        if dist > 0 then
            local dirX = dx / dist
            local dirY = dy / dist
            chargeX = enemyCenterX + dirX * (eDisplaySize / 2)
            chargeY = enemyCenterY + dirY * (eDisplaySize / 2)
        end
        if EmitBeamCharge then
            EmitBeamCharge(chargeX, chargeY, targetX, targetY, enemy.color[1], enemy.color[2], enemy.color[3])
        end
        if enemy.teleportTimer >= enemy.teleportChargeTime then
            enemy.teleportState = "shooting"
            enemy.teleportTimer = 0
            enemy.beamLocked = false
        end
    elseif enemy.teleportState == "shooting" then
        local eDisplaySize = enemy.displaySize or enemy.size
        local enemyCenterX = enemy.x + eDisplaySize/2
        local enemyCenterY = enemy.y + eDisplaySize/2
        local dx = enemy.beamTargetX - enemyCenterX
        local dy = enemy.beamTargetY - enemyCenterY
        local dist = math.sqrt(dx * dx + dy * dy)
        local beamStartX = enemyCenterX
        local beamStartY = enemyCenterY
        if dist > 0 then
            local dirX = dx / dist
            local dirY = dy / dist
            beamStartX = enemyCenterX + dirX * (eDisplaySize / 2)
            beamStartY = enemyCenterY + dirY * (eDisplaySize / 2)
        end
        if SpawnBeam then
            SpawnBeam(enemy, beamStartX, beamStartY, enemy.beamTargetX, enemy.beamTargetY)
        end
        if enemy.teleportTimer >= enemy.beamDuration then
            enemy.teleportState = "cooldown"
            enemy.teleportTimer = 0
        end
    elseif enemy.teleportState == "cooldown" then
        if enemy.teleportTimer >= enemy.teleportCooldown then
            enemy.teleportState = "shrinking"
            enemy.teleportTimer = 0
            enemy.baseSize = enemy.baseSize or enemy.size
        end
    elseif enemy.teleportState == "shrinking" then
        local t = enemy.teleportTimer / TELEPORT_SHRINK_DURATION
        if t > 1 then t = 1 end
        local scale = 1 - t
        if scale < 0.01 then scale = 0.01 end
        local scaledSize = math.floor(enemy.baseSize * scale)
        if scaledSize < 1 then scaledSize = 1 end
        Sprite.set_image_width(enemy.sprite, scaledSize)
        Sprite.set_image_height(enemy.sprite, scaledSize)
        if EmitTeleportBurst then
            local cx = enemy.x + enemy.baseSize/2
            local cy = enemy.y + enemy.baseSize/2
            EmitTeleportBurst(cx, cy, enemy.color[1], enemy.color[2], enemy.color[3], true)
        end
        if enemy.teleportTimer >= TELEPORT_SHRINK_DURATION then
            enemy.teleportState = "teleporting"
            enemy.teleportTimer = 0
            enemy.teleportVisible = false
            Entity.set_global_pos(enemy.entity, -1000, -1000)
        end
    elseif enemy.teleportState == "teleporting" then
        local margin = enemy.baseSize + 20
        enemy.x = margin + math.random() * (screenW - 2 * margin)
        enemy.y = margin + math.random() * (screenH - 2 * margin)
        enemy.teleportVisible = true
        enemy.teleportState = "growing"
        enemy.teleportTimer = 0
        Sprite.set_image_width(enemy.sprite, 1)
        Sprite.set_image_height(enemy.sprite, 1)
    elseif enemy.teleportState == "growing" then
        local t = enemy.teleportTimer / TELEPORT_GROW_DURATION
        if t > 1 then t = 1 end
        local scale = t
        if scale < 0.01 then scale = 0.01 end
        local scaledSize = math.floor(enemy.baseSize * scale)
        if scaledSize < 1 then scaledSize = 1 end
        Sprite.set_image_width(enemy.sprite, scaledSize)
        Sprite.set_image_height(enemy.sprite, scaledSize)
        if EmitTeleportBurst then
            local cx = enemy.x + enemy.baseSize/2
            local cy = enemy.y + enemy.baseSize/2
            EmitTeleportBurst(cx, cy, enemy.color[1], enemy.color[2], enemy.color[3], false)
        end
        if enemy.teleportTimer >= TELEPORT_GROW_DURATION then
            enemy.teleportState = "charging"
            enemy.teleportTimer = 0
            Sprite.set_image_width(enemy.sprite, enemy.baseSize)
            Sprite.set_image_height(enemy.sprite, enemy.baseSize)
        end
    end
end

 --=====================================================================
 --  [COMBAT / FEEL] Spin / Shooting Rotation
 --=====================================================================
 updateSpin = function(enemy, dt)
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

 updateShootingRotation = function(enemy, dt)
    if enemy.spinWhileShooting and enemy.projectileCount > 4 then
        local spinSpeed = 2
        enemy.shootAngleOffset = (enemy.shootAngleOffset or 0) + dt * spinSpeed
        enemy.rotation = (enemy.rotation or 0) + dt * spinSpeed * 60
    end
end

return TriangleShooterEnemy
