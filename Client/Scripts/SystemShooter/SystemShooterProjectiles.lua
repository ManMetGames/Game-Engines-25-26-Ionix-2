local SystemShooterProjectiles = {}

--=====================================================================
--  [MODULE] Imports / Dependencies
--=====================================================================
local assets = require("Scripts.Assets")
local SystemShooterDifficulty = require("Scripts.SystemShooter.SystemShooterDifficulty")

--=====================================================================
--  [CONFIG] Player Projectile Settings
--=====================================================================
SystemShooterProjectiles.PlayerConfig = {
    size = 24,
    speed = 1120,           -- pixels per second
    lifetimeSeconds = 4,    -- base lifetime before auto-despawn
    bounceLifetimeBonus = 2.5, -- extra lifetime per bounce count
    texture = nil,          -- set on init
    defaultColor = {255, 255, 255},
    goldenColor = {255, 215, 0},
    noWitnessesColor = {255, 0, 0},
    layer = 4,              -- render layer (under enemies at 5)
}

--=====================================================================
--  [CONFIG] Enemy Projectile Settings
--  Note: speed and damage are now driven by difficulty settings.
--=====================================================================
SystemShooterProjectiles.EnemyConfig = {
    size = 24,
    speed = 325,            -- Base value (overridden by difficulty)
    lifetimeSeconds = 6,    -- separate lifetime for enemy projectiles
    texture = nil,          -- set on init
    color = {128, 0, 255},  -- purple
    layer = 4,              -- render layer
    damage = 4,             -- Base value (overridden by difficulty)
}

-- Get effective enemy projectile speed (from difficulty)
local function getEnemySpeed()
    return SystemShooterDifficulty.getEnemyProjectileSpeed()
end

-- Get effective enemy projectile damage (from difficulty)
local function getEnemyDamage()
    return SystemShooterDifficulty.getEnemyProjectileDamage()
end

--=====================================================================
--  [STATE] Active Projectiles & Pools
--=====================================================================
local playerProjectiles = {}
local playerProjectilePool = {}

local enemyProjectiles = {}
local enemyProjectilePool = {}

--=====================================================================
--  [INTERNAL] Screen bounds (must be set by main module)
--=====================================================================
local screenW = 1920
local screenH = 1080

--=====================================================================
--  [INTERNAL] Callbacks (set by main module)
--=====================================================================
local callbacks = {
    getEnemies = nil,           -- returns enemies table
    getPlayerPos = nil,         -- returns playerX, playerY, playerSize
    isNoWitnessesActive = nil,  -- returns bool
    getNoWitnessesDamageMultiplier = nil,
    getLowEnemyDamageStacks = nil,
    getActiveEnemyCount = nil,
    onEnemyHit = nil,           -- called when enemy is hit
    onEnemyKilled = nil,        -- called when enemy dies
    onPlayerHit = nil,          -- called when player is hit by enemy projectile
    addXp = nil,                -- add xp to player
}

--=====================================================================
--  [INIT] Initialize module
--=====================================================================
function SystemShooterProjectiles.init(config)
    config = config or {}
    
    -- Set textures
    SystemShooterProjectiles.PlayerConfig.texture = assets.textures.Ghast_Tear
    SystemShooterProjectiles.EnemyConfig.texture = assets.textures.Ghast_Tear
    
    -- Override configs if provided
    if config.playerConfig then
        for k, v in pairs(config.playerConfig) do
            SystemShooterProjectiles.PlayerConfig[k] = v
        end
    end
    if config.enemyConfig then
        for k, v in pairs(config.enemyConfig) do
            SystemShooterProjectiles.EnemyConfig[k] = v
        end
    end
    
    -- Set callbacks
    if config.callbacks then
        for k, v in pairs(config.callbacks) do
            callbacks[k] = v
        end
    end
end

--=====================================================================
--  [API] Set screen bounds
--=====================================================================
function SystemShooterProjectiles.setScreenBounds(w, h)
    screenW = w
    screenH = h
end

--=====================================================================
--  [API] Get projectile tables (for external access like rewind)
--=====================================================================
function SystemShooterProjectiles.getPlayerProjectiles()
    return playerProjectiles
end

function SystemShooterProjectiles.getEnemyProjectiles()
    return enemyProjectiles
end

--=====================================================================
--  [PLAYER PROJECTILES] Clear all
--=====================================================================
function SystemShooterProjectiles.clearAllPlayerProjectiles()
    for i = #playerProjectiles, 1, -1 do
        local proj = playerProjectiles[i]
        Entity.set_global_pos(proj.entity, -1000, -1000)
        table.insert(playerProjectilePool, table.remove(playerProjectiles, i))
    end
end

--=====================================================================
--  [ENEMY PROJECTILES] Clear all
--=====================================================================
function SystemShooterProjectiles.clearAllEnemyProjectiles()
    for i = #enemyProjectiles, 1, -1 do
        local proj = enemyProjectiles[i]
        Entity.set_global_pos(proj.entity, -1000, -1000)
        table.insert(enemyProjectilePool, table.remove(enemyProjectiles, i))
    end
end

--=====================================================================
--  [API] Clear all projectiles
--=====================================================================
function SystemShooterProjectiles.clearAll()
    SystemShooterProjectiles.clearAllPlayerProjectiles()
    SystemShooterProjectiles.clearAllEnemyProjectiles()
end

--=====================================================================
--  [PLAYER PROJECTILES] Spawn single projectile
--=====================================================================
function SystemShooterProjectiles.spawnPlayerProjectile(spawnX, spawnY, dirX, dirY, pierceCount, bounceCount, shotData)
    local projData
    shotData = shotData or {}
    
    local cfg = SystemShooterProjectiles.PlayerConfig
    local isGolden = shotData.isGolden or false
    local damage = shotData.damage or 1
    local sizeMultiplier = shotData.sizeMultiplier or 1
    local actualSize = cfg.size * sizeMultiplier

    -- Determine projectile color
    local noWitnessesActive = callbacks.isNoWitnessesActive and callbacks.isNoWitnessesActive() or false
    local r, g, b = cfg.defaultColor[1], cfg.defaultColor[2], cfg.defaultColor[3]
    if isGolden then
        r, g, b = cfg.goldenColor[1], cfg.goldenColor[2], cfg.goldenColor[3]
    elseif noWitnessesActive then
        r, g, b = cfg.noWitnessesColor[1], cfg.noWitnessesColor[2], cfg.noWitnessesColor[3]
    end

    -- Try to reuse a pooled projectile
    if #playerProjectilePool > 0 then
        projData = table.remove(playerProjectilePool)
        local sprite = Entity.get_sprite_component(projData.entity)
        if sprite then
            Sprite.set_image_width(sprite, math.floor(actualSize))
            Sprite.set_image_height(sprite, math.floor(actualSize))
            Sprite.set_color(sprite, r, g, b)
        end
    else
        local proj = Entity.create_entity()
        local sprite = Entity.add_sprite_component(proj, cfg.texture, math.floor(actualSize), math.floor(actualSize), cfg.layer)
        if sprite then
            Sprite.set_color(sprite, r, g, b)
        end
        projData = { entity = proj }
    end

    -- Adjust spawn position for larger projectiles
    local adjustedSpawnX = spawnX - (actualSize - cfg.size) / 2
    local adjustedSpawnY = spawnY - (actualSize - cfg.size) / 2

    -- Set position and rotation
    Entity.set_global_pos(projData.entity, adjustedSpawnX, adjustedSpawnY)
    local projAngle = math.deg(math.atan(dirY, dirX)) + 90
    Entity.set_global_rot(projData.entity, projAngle)

    -- Initialize projectile data
    projData.x = adjustedSpawnX
    projData.y = adjustedSpawnY
    projData.vx = dirX * cfg.speed
    projData.vy = dirY * cfg.speed
    projData.age = 0
    projData.pierceRemaining = pierceCount or 0
    projData.bounceRemaining = bounceCount or 0
    projData.maxLifetime = cfg.lifetimeSeconds + (bounceCount * cfg.bounceLifetimeBonus)
    projData.hitEnemies = {}
    projData.hasHit = false
    projData.damage = damage
    projData.isGolden = isGolden
    projData.size = actualSize

    table.insert(playerProjectiles, projData)
end

--=====================================================================
--  [PLAYER PROJECTILES] Find closest enemy (for bounce targeting)
--=====================================================================
local function findClosestEnemy(fromX, fromY)
    if not callbacks.getEnemies then return nil end
    local enemies = callbacks.getEnemies()
    
    local closestEnemy = nil
    local closestDistSq = math.huge
    for j = 1, #enemies do
        local enemy = enemies[j]
        if not enemy.disabled and not enemy.isDead and (enemy.teleportVisible == nil or enemy.teleportVisible) then
            local eDisplaySize = enemy.displaySize or enemy.size or 48
            local enemyCenterX = enemy.x + eDisplaySize/2
            local enemyCenterY = enemy.y + eDisplaySize/2
            local dx = enemyCenterX - fromX
            local dy = enemyCenterY - fromY
            local distSq = dx * dx + dy * dy
            if distSq < closestDistSq then
                closestDistSq = distSq
                closestEnemy = enemy
            end
        end
    end
    return closestEnemy
end

--=====================================================================
--  [PLAYER PROJECTILES] Return to pool
--=====================================================================
local function returnPlayerProjectileToPool(proj, index)
    Entity.set_global_pos(proj.entity, -1000, -1000)
    table.insert(playerProjectilePool, table.remove(playerProjectiles, index))
end

--=====================================================================
--  [PLAYER PROJECTILES] Update
--=====================================================================
function SystemShooterProjectiles.updatePlayerProjectiles(dt, runStats)
    if not callbacks.getEnemies then return end
    
    local cfg = SystemShooterProjectiles.PlayerConfig
    local enemies = callbacks.getEnemies()
    local noWitnessesActive = callbacks.isNoWitnessesActive and callbacks.isNoWitnessesActive() or false
    
    for i = #playerProjectiles, 1, -1 do
        local proj = playerProjectiles[i]
        
        -- Update projectile color based on no-witnesses state
        local sprite = Entity.get_sprite_component(proj.entity)
        if sprite then
            if noWitnessesActive then
                Sprite.set_color(sprite, cfg.noWitnessesColor[1], cfg.noWitnessesColor[2], cfg.noWitnessesColor[3])
            elseif proj.isGolden then
                Sprite.set_color(sprite, cfg.goldenColor[1], cfg.goldenColor[2], cfg.goldenColor[3])
            else
                Sprite.set_color(sprite, cfg.defaultColor[1], cfg.defaultColor[2], cfg.defaultColor[3])
            end
        end
        
        -- Move projectile
        proj.x = proj.x + proj.vx * dt
        proj.y = proj.y + proj.vy * dt
        Entity.set_global_pos(proj.entity, proj.x, proj.y)
        
        -- Check collision with enemies
        local projSize = proj.size or cfg.size
        local projCenterX = proj.x + projSize/2
        local projCenterY = proj.y + projSize/2
        local hitEnemyIndex = nil

        for j = #enemies, 1, -1 do
            local enemy = enemies[j]
            if enemy.disabled or enemy.isDead then
                goto continue_proj_collision
            end
            local isVisible = enemy.teleportVisible == nil or enemy.teleportVisible
            if isVisible and not proj.hitEnemies[enemy] then
                local eDisplaySize = enemy.displaySize or enemy.size or 48
                local enemyCenterX = enemy.x + eDisplaySize/2
                local enemyCenterY = enemy.y + eDisplaySize/2
                local enemyHitRadius = eDisplaySize/2 + projSize/2
                local dx = projCenterX - enemyCenterX
                local dy = projCenterY - enemyCenterY
                local distSq = dx * dx + dy * dy
                if distSq < enemyHitRadius * enemyHitRadius then
                    hitEnemyIndex = j
                    break
                end
            end
            ::continue_proj_collision::
        end

        local shouldRemove = false

        if hitEnemyIndex ~= nil then
            local enemy = enemies[hitEnemyIndex]
            local damage = proj.damage or 1
            
            -- Apply no-witnesses damage multiplier (only when 0 < activeCount <= threshold)
            if callbacks.isNoWitnessesActive and callbacks.isNoWitnessesActive() then
                if callbacks.getNoWitnessesDamageMultiplier then
                    damage = damage * callbacks.getNoWitnessesDamageMultiplier()
                end
            end
            
            -- Track hit stats
            if not proj.hasHit then
                proj.hasHit = true
                if runStats then runStats.shotsHit = (runStats.shotsHit or 0) + 1 end
            end
            if runStats then runStats.damageDealt = (runStats.damageDealt or 0) + damage end
            
            -- Apply damage to enemy
            enemy.health = (enemy.health or 0) - damage
            
            -- Callback for enemy hit
            if callbacks.onEnemyHit then
                callbacks.onEnemyHit(enemy, damage, proj)
            end
            
            -- Add XP
            if callbacks.addXp then
                callbacks.addXp(damage)
            end

            -- Check if enemy died
            if enemy.health <= 0 and not enemy.isDead then
                if callbacks.onEnemyKilled then
                    callbacks.onEnemyKilled(enemy, enemies)
                end
            end

            -- Handle pierce
            if proj.pierceRemaining > 0 then
                proj.pierceRemaining = proj.pierceRemaining - 1
                proj.hitEnemies[enemy] = true
            else
                shouldRemove = true
            end
        end

        if not shouldRemove then
            proj.age = proj.age + dt
            local maxLife = proj.maxLifetime or cfg.lifetimeSeconds

            if proj.age > maxLife then
                shouldRemove = true
            else
                local hitEdge = false
                local edgeX, edgeY = nil, nil

                if proj.x < 0 then
                    hitEdge = true
                    edgeX = "left"
                    proj.x = 0
                elseif proj.x + projSize > screenW then
                    hitEdge = true
                    edgeX = "right"
                    proj.x = screenW - projSize
                end

                if proj.y < 0 then
                    hitEdge = true
                    edgeY = "top"
                    proj.y = 0
                elseif proj.y + projSize > screenH then
                    hitEdge = true
                    edgeY = "bottom"
                    proj.y = screenH - projSize
                end

                if hitEdge then
                    if proj.bounceRemaining > 0 then
                        proj.bounceRemaining = proj.bounceRemaining - 1
                        proj.hitEnemies = {}

                        local newProjCenterX = proj.x + projSize/2
                        local newProjCenterY = proj.y + projSize/2
                        local closestEnemy = findClosestEnemy(newProjCenterX, newProjCenterY)

                        if closestEnemy then
                            local ceDisplaySize = closestEnemy.displaySize or closestEnemy.size or 48
                            local targetX = closestEnemy.x + ceDisplaySize/2
                            local targetY = closestEnemy.y + ceDisplaySize/2
                            local dx = targetX - newProjCenterX
                            local dy = targetY - newProjCenterY
                            local len = math.sqrt(dx*dx + dy*dy)
                            if len > 0 then
                                dx = dx / len
                                dy = dy / len
                            end
                            proj.vx = dx * cfg.speed
                            proj.vy = dy * cfg.speed
                        else
                            if edgeX then proj.vx = -proj.vx end
                            if edgeY then proj.vy = -proj.vy end
                        end

                        local projAngle = math.deg(math.atan(proj.vy, proj.vx)) + 90
                        Entity.set_global_rot(proj.entity, projAngle)
                        Entity.set_global_pos(proj.entity, proj.x, proj.y)
                    else
                        shouldRemove = true
                    end
                end
            end
        end

        if shouldRemove then
            returnPlayerProjectileToPool(proj, i)
        end
    end
end

--=====================================================================
--  [ENEMY PROJECTILES] Spawn single projectile
--=====================================================================
local function spawnEnemySingleProjectile(enemy, dirX, dirY)
    local projData
    local cfg = SystemShooterProjectiles.EnemyConfig
    local speed = getEnemySpeed()
    
    if #enemyProjectilePool > 0 then
        projData = table.remove(enemyProjectilePool)
        Sprite.set_color(projData.sprite, cfg.color[1], cfg.color[2], cfg.color[3])
        Sprite.set_zed_order(projData.sprite, cfg.layer)
    else
        local proj = Entity.create_entity()
        local sprite = Entity.add_sprite_component(proj, cfg.texture, cfg.size, cfg.size, cfg.layer)
        Sprite.set_color(sprite, cfg.color[1], cfg.color[2], cfg.color[3])
        projData = { entity = proj, sprite = sprite }
    end
    
    local eSize = enemy.displaySize or enemy.size or 48
    local enemyCenterX = enemy.x + eSize/2
    local enemyCenterY = enemy.y + eSize/2
    local spawnX = enemyCenterX - cfg.size/2
    local spawnY = enemyCenterY - cfg.size/2
    
    Entity.set_global_pos(projData.entity, spawnX, spawnY)
    local projAngle = math.deg(math.atan(dirY, dirX)) + 90
    Entity.set_global_rot(projData.entity, projAngle)
    
    projData.x = spawnX
    projData.y = spawnY
    projData.vx = dirX * speed
    projData.vy = dirY * speed
    projData.age = 0
    projData.sourceEnemy = enemy
    
    table.insert(enemyProjectiles, projData)
end

--=====================================================================
--  [ENEMY PROJECTILES] Spawn projectile (handles patterns)
--=====================================================================
function SystemShooterProjectiles.spawnEnemyProjectile(enemy)
    if not callbacks.getPlayerPos then return end
    
    local playerX, playerY, playerSize = callbacks.getPlayerPos()
    
    local eSize = enemy.displaySize or enemy.size or 48
    local enemyCenterX = enemy.x + eSize/2
    local enemyCenterY = enemy.y + eSize/2
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    local dx = playerCenterX - enemyCenterX
    local dy = playerCenterY - enemyCenterY
    local dist = math.sqrt(dx * dx + dy * dy)
    
    local baseDirX, baseDirY = 0, -1
    if dist > 0 then
        baseDirX = dx / dist
        baseDirY = dy / dist
    end
    
    local shootPattern = enemy.shootPattern or "single"
    local projectileCount = enemy.projectileCount or 1
    
    if shootPattern == "single" or projectileCount <= 1 then
        spawnEnemySingleProjectile(enemy, baseDirX, baseDirY)
        
    elseif shootPattern == "cone" and projectileCount <= 4 then
        local spreadAngle = math.rad(15)
        local baseAngle = math.atan(baseDirY, baseDirX)
        
        -- For even counts (2, 4), ensure one bullet goes straight at player
        if projectileCount == 2 or projectileCount == 4 then
            -- Spawn one bullet straight at player
            spawnEnemySingleProjectile(enemy, baseDirX, baseDirY)
            
            -- Spawn remaining bullets spread around
            local remaining = projectileCount - 1
            local halfSpread = spreadAngle * remaining / 2
            local step = spreadAngle
            
            for i = 1, remaining do
                local offset = -halfSpread + step * (i - 1)
                -- Skip the center (already spawned)
                if math.abs(offset) > 0.01 then
                    local angle = baseAngle + offset
                    local dirX = math.cos(angle)
                    local dirY = math.sin(angle)
                    spawnEnemySingleProjectile(enemy, dirX, dirY)
                end
            end
        else
            -- Odd counts: use original spread (center bullet is already aimed)
            local startAngle = baseAngle - spreadAngle * (projectileCount - 1) / 2
            for i = 1, projectileCount do
                local angle = startAngle + spreadAngle * (i - 1)
                local dirX = math.cos(angle)
                local dirY = math.sin(angle)
                spawnEnemySingleProjectile(enemy, dirX, dirY)
            end
        end
        
    else
        local angleOffset = enemy.shootAngleOffset or 0
        for i = 1, projectileCount do
            local angle = angleOffset + (2 * math.pi * (i - 1)) / projectileCount
            local dirX = math.cos(angle)
            local dirY = math.sin(angle)
            spawnEnemySingleProjectile(enemy, dirX, dirY)
        end
    end
end

--=====================================================================
--  [ENEMY PROJECTILES] Update
--=====================================================================
function SystemShooterProjectiles.updateEnemyProjectiles(dt, damageCooldown, runStats)
    if not callbacks.getPlayerPos then return end
    
    -- Check if antivirus is active
    local isInvulnerable = false
    if callbacks.isAntivirusActive then
        isInvulnerable = callbacks.isAntivirusActive()
    end
    
    local cfg = SystemShooterProjectiles.EnemyConfig
    local damage = getEnemyDamage()
    local playerX, playerY, playerSize = callbacks.getPlayerPos()
    
    for i = #enemyProjectiles, 1, -1 do
        local proj = enemyProjectiles[i]
        
        -- Move projectile
        proj.x = proj.x + proj.vx * dt
        proj.y = proj.y + proj.vy * dt
        Entity.set_global_pos(proj.entity, proj.x, proj.y)
        
        -- Check collision with player
        local projCenterX = proj.x + cfg.size/2
        local projCenterY = proj.y + cfg.size/2
        local playerCenterX = playerX + playerSize/2
        local playerCenterY = playerY + playerSize/2
        
        local dx = projCenterX - playerCenterX
        local dy = projCenterY - playerCenterY
        local distSq = dx * dx + dy * dy
        local hitRadius = playerSize/2 + cfg.size/2
        
        if distSq < hitRadius * hitRadius and damageCooldown <= 0 and not isInvulnerable then
            -- Hit player
            if callbacks.onPlayerHit then
                callbacks.onPlayerHit(damage)
            end
            if runStats then runStats.damageTaken = (runStats.damageTaken or 0) + damage end
            Entity.set_global_pos(proj.entity, -1000, -1000)
            table.insert(enemyProjectilePool, table.remove(enemyProjectiles, i))
        else
            -- Age and remove if expired or off screen
            proj.age = proj.age + dt
            if proj.age > cfg.lifetimeSeconds or proj.y < -50 or proj.y > screenH + 50 or proj.x < -50 or proj.x > screenW + 50 then
                Entity.set_global_pos(proj.entity, -1000, -1000)
                table.insert(enemyProjectilePool, table.remove(enemyProjectiles, i))
            end
        end
    end
end

--=====================================================================
--  [API] Get projectile counts (for debugging/UI)
--=====================================================================
function SystemShooterProjectiles.getPlayerProjectileCount()
    return #playerProjectiles
end

function SystemShooterProjectiles.getEnemyProjectileCount()
    return #enemyProjectiles
end

function SystemShooterProjectiles.getPoolSizes()
    return #playerProjectilePool, #enemyProjectilePool
end

return SystemShooterProjectiles
