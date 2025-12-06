local TriangleShooterEnemy = {}

local assets = require("Scripts.Assets")

local enemySpinMaxVelocity = 36
local enemySpinBoost = 1
local enemySpinDecayBase = 0.0005
local enemySpinDecayExtra = 0.01

local dashSpeed = 5
local enemyBounceSteer = 0.4
local enemySteerStrength = 0.0

function TriangleShooterEnemy.createEnemy(x, y, health, enemySize, playerX, playerY, playerSize)
    local entity = Entity.create_entity()
    Entity.set_global_pos(entity, x, y)
    local sprite = Entity.add_sprite_component(entity, assets.textures.Cube, enemySize, enemySize, 5)
    Sprite.set_columns(sprite, 1)

    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    local enemyCenterX = x + enemySize/2
    local enemyCenterY = y + enemySize/2
    local dx = playerCenterX - enemyCenterX
    local dy = playerCenterY - enemyCenterY
    local dist = math.sqrt(dx * dx + dy * dy)
    local dashX, dashY = 0, 0
    if dist > 0 then
        dashX = dx / dist
        dashY = dy / dist
    end

    local enemy = {
        entity = entity,
        sprite = sprite,
        x = x,
        y = y,
        size = enemySize,
        health = health,
        rotation = 0,
        spinVelocity = 0,
        dashDirX = dashX,
        dashDirY = dashY,
        shootCooldown = 0,
        flashTimer = 0,
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

function TriangleShooterEnemy.updateEnemyDash(
    enemies,
    playerX, playerY, playerSize,
    screenW, screenH,
    originalWindowWidth, originalWindowHeight,
    enemyProjectilesEnabled, enemyShootIntervalSeconds,
    SpawnEnemyProjectile,
    TriggerWallLerp
)
    local dt = Mafs.delta_time()
    local minX = 0
    local minY = 0

    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2

    for i = 1, #enemies do
        local enemy = enemies[i]
        local enemySize = enemy.size or 48
        local maxX = screenW - enemySize
        local maxY = screenH - enemySize

        local enemyCenterX = enemy.x + enemySize/2
        local enemyCenterY = enemy.y + enemySize/2
        local dx = playerCenterX - enemyCenterX
        local dy = playerCenterY - enemyCenterY

        if enemySteerStrength > 0 then
            local distToPlayer = math.sqrt(dx * dx + dy * dy)
            if distToPlayer > 0 then
                local toPlayerDirX = dx / distToPlayer
                local toPlayerDirY = dy / distToPlayer
                local steer = enemySteerStrength
                local newDirX = enemy.dashDirX * (1 - steer) + toPlayerDirX * steer
                local newDirY = enemy.dashDirY * (1 - steer) + toPlayerDirY * steer
                local newLen = math.sqrt(newDirX * newDirX + newDirY * newDirY)
                if newLen > 0 then
                    enemy.dashDirX = newDirX / newLen
                    enemy.dashDirY = newDirY / newLen
                end
            end
        end

        local scaleX = screenW / originalWindowWidth
        local scaleY = screenH / originalWindowHeight
        local scale = (scaleX + scaleY) / 2
        local currentSpeed = dashSpeed * scale

        enemy.x = enemy.x + enemy.dashDirX * currentSpeed
        enemy.y = enemy.y + enemy.dashDirY * currentSpeed

        if enemyProjectilesEnabled and enemyShootIntervalSeconds and enemyShootIntervalSeconds > 0 then
            enemy.shootTimer = (enemy.shootTimer or 0) + dt
            if enemy.shootTimer >= enemyShootIntervalSeconds then
                SpawnEnemyProjectile(enemy)
                enemy.shootTimer = enemy.shootTimer - enemyShootIntervalSeconds
            end
        end

        local hitLeft = false
        local hitRight = false
        local hitTop = false
        local hitBottom = false

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
            if hitLeft or hitRight then
                enemy.dashDirX = -enemy.dashDirX
            end
            if hitTop or hitBottom then
                enemy.dashDirY = -enemy.dashDirY
            end
            local len = math.sqrt(enemy.dashDirX * enemy.dashDirX + enemy.dashDirY * enemy.dashDirY)
            if len > 0 then
                enemy.dashDirX = enemy.dashDirX / len
                enemy.dashDirY = enemy.dashDirY / len
            end
            local enemyCenterX2 = enemy.x + enemySize/2
            local enemyCenterY2 = enemy.y + enemySize/2
            local toPlayerX = playerCenterX - enemyCenterX2
            local toPlayerY = playerCenterY - enemyCenterY2
            local dist = math.sqrt(toPlayerX * toPlayerX + toPlayerY * toPlayerY)
            if dist > 0 then
                local toPlayerDirX = toPlayerX / dist
                local toPlayerDirY = toPlayerY / dist
                local steer = enemyBounceSteer
                local newDirX = enemy.dashDirX * (1 - steer) + toPlayerDirX * steer
                local newDirY = enemy.dashDirY * (1 - steer) + toPlayerDirY * steer
                local newLen = math.sqrt(newDirX * newDirX + newDirY * newDirY)
                if newLen > 0 then
                    enemy.dashDirX = newDirX / newLen
                    enemy.dashDirY = newDirY / newLen
                end
            end

            if hitLeft then TriggerWallLerp("left") end
            if hitRight then TriggerWallLerp("right") end
            if hitTop then TriggerWallLerp("top") end
            if hitBottom then TriggerWallLerp("bottom") end

            enemy.spinVelocity = (enemy.spinVelocity or 0) + enemySpinBoost
            if enemy.spinVelocity > enemySpinMaxVelocity then
                enemy.spinVelocity = enemySpinMaxVelocity
            end
        end

        local speed = math.abs(enemy.spinVelocity or 0)
        local t = speed / enemySpinMaxVelocity
        if t > 1 then t = 1 end
        local decay = enemySpinDecayBase + enemySpinDecayExtra * t
        enemy.spinVelocity = (enemy.spinVelocity or 0) - (enemy.spinVelocity or 0) * decay
        if math.abs(enemy.spinVelocity or 0) < 0.01 then
            enemy.spinVelocity = 0
        end
        enemy.rotation = (enemy.rotation or 0) + enemy.spinVelocity
        Entity.set_global_rot(enemy.entity, enemy.rotation)

        Entity.set_global_pos(enemy.entity, enemy.x, enemy.y)
    end
end

return TriangleShooterEnemy
