local SystemShooterPlayer = {}

local SystemShooterPlayerProgress = require("Scripts.SystemShooter.SystemShooterPlayerProgress")
local SystemShooterProjectiles = require("Scripts.SystemShooter.SystemShooterProjectiles")

 --=====================================================================
 --  [STATE] Player (Triangle)
 --=====================================================================
local player
local playerSprite
local playerSize = 48
local playerX = 400
local playerY = 300
local playerSpeed = 0.5  -- Mouse sensitivity multiplier
local playerHealth = 100

-- PLAYER FLASH EFFECT
local playerFlashTimer = 0
local playerFlashDuration = 0.2  -- seconds

-- DAMAGE COOLDOWN
local damageCooldown = 0
local damageCooldownDuration = 0.5  -- seconds

-- PROJECTILE SETTINGS
local fireCooldownTimer = 0
local isFiring = false
local currentFireInterval = 0.5

-- RECOIL
local recoilOffset = 0
local recoilMaxOffset = 16
local recoilLerpSpeed = 12

-- Current aim direction (updated each frame)
local aimDirX = 0
local aimDirY = -1  -- Default: pointing up

-- KNOCKBACK
local knockbackTimer = 0
local knockbackDuration = 0.2
local knockbackBaseSpeed = 1200
local knockbackDirX = 0
local knockbackDirY = 0

-- Screen bounds (set externally)
local screenW = 1920
local screenH = 1080

-- Skip first mouse delta after resume (prevents teleport on unpause)
local skipNextMouseDelta = false

-- ANTIVIRUS SYSTEM
local antivirusEnabled = false
local antivirusActive = false
local antivirusCycleTimer = 0
local antivirusCycleDuration = 10  -- seconds
local antivirusActiveDuration = 2  -- seconds

-- Callbacks (set via init)
local callbacks = {
    getEnemies = nil,
    getEnemySize = nil,
    isNoWitnessesActive = nil,
    playGunshotSfx = nil,
    onShotFired = nil,
}

 --=====================================================================
 --  [INIT] Create Player Entity
 --=====================================================================
function SystemShooterPlayer.init(config)
    local assets = config.assets
    screenW = config.screenW or 1920
    screenH = config.screenH or 1080
    
    if config.callbacks then
        callbacks = config.callbacks
    end
    
    -- Create player triangle
    player = Entity.create_entity()
    
    -- Start at center of screen
    playerX = screenW / 2 - playerSize / 2
    playerY = screenH / 2 - playerSize / 2
    Entity.set_global_pos(player, playerX, playerY)
    
    -- Add sprite component 
    playerSprite = Entity.add_sprite_component(player, assets.textures.Triangle, playerSize, playerSize, 10)
    Sprite.set_columns(playerSprite, 1)
    
    playerHealth = SystemShooterPlayerProgress.getMaxHealth()
end

 --=====================================================================
 --  [GETTERS / SETTERS]
 --=====================================================================
function SystemShooterPlayer.getEntity()
    return player
end

function SystemShooterPlayer.getSprite()
    return playerSprite
end

function SystemShooterPlayer.getPosition()
    return playerX, playerY
end

function SystemShooterPlayer.setPosition(x, y)
    playerX = x
    playerY = y
    Entity.set_global_pos(player, playerX, playerY)
end

function SystemShooterPlayer.getSize()
    return playerSize
end

function SystemShooterPlayer.getHealth()
    return playerHealth
end

function SystemShooterPlayer.setHealth(health)
    playerHealth = health
end

function SystemShooterPlayer.getAimDirection()
    return aimDirX, aimDirY
end

function SystemShooterPlayer.getDamageCooldown()
    return damageCooldown
end

function SystemShooterPlayer.setDamageCooldown(cd)
    damageCooldown = cd
end

function SystemShooterPlayer.getDamageCooldownDuration()
    return damageCooldownDuration
end

function SystemShooterPlayer.enableAntivirus()
    antivirusEnabled = true
    antivirusCycleTimer = 0
    antivirusActive = false
end

function SystemShooterPlayer.disableAntivirus()
    antivirusEnabled = false
    antivirusActive = false
    antivirusCycleTimer = 0
    -- Reset sprite color to white
    if playerSprite then
        Sprite.set_color(playerSprite, 255, 255, 255)
    end
end

function SystemShooterPlayer.isAntivirusActive()
    return antivirusActive
end

function SystemShooterPlayer.isFiring()
    return isFiring
end

function SystemShooterPlayer.setFiring(firing)
    isFiring = firing
end

function SystemShooterPlayer.getFireCooldownTimer()
    return fireCooldownTimer
end

function SystemShooterPlayer.setFireCooldownTimer(timer)
    fireCooldownTimer = timer
end

function SystemShooterPlayer.setScreenBounds(w, h)
    screenW = w
    screenH = h
end

function SystemShooterPlayer.setPosition(x, y)
    playerX = x
    playerY = y
    Entity.set_global_pos(player, playerX, playerY)
end

function SystemShooterPlayer.skipNextDelta()
    skipNextMouseDelta = true
end

 --=====================================================================
 --  [KNOCKBACK]
 --=====================================================================
function SystemShooterPlayer.applyKnockback(dirX, dirY)
    knockbackDirX = dirX
    knockbackDirY = dirY
    knockbackTimer = knockbackDuration
end

 --=====================================================================
 --  [FLASH] Player damage flash
 --=====================================================================
function SystemShooterPlayer.flash()
    -- Don't flash if antivirus is active
    if antivirusActive then
        return
    end
    
    Sprite.set_color(playerSprite, 255, 0, 0)
    playerFlashTimer = playerFlashDuration
    if playerHealth <= 0 then
        Entity.set_global_pos(player, -1000, -1000)
    end
end

function SystemShooterPlayer.updateFlash(dt)
    if playerFlashDuration > 0 and playerFlashTimer > 0 then
        playerFlashTimer = playerFlashTimer - dt
        if playerFlashTimer < 0 then playerFlashTimer = 0 end

        local t = playerFlashTimer / playerFlashDuration
        if t < 0 then t = 0 end
        if t > 1 then t = 1 end

        local r = 255
        local g = math.floor(255 * (1.0 - t) + 0.5)
        local b = math.floor(255 * (1.0 - t) + 0.5)
        
        -- Don't override antivirus yellow color
        if not antivirusActive then
            Sprite.set_color(playerSprite, r, g, b)
        end
    end
    
    -- Damage cooldown
    if damageCooldown > 0 then
        damageCooldown = damageCooldown - dt
    end
    
    -- Antivirus system
    if antivirusEnabled then
        antivirusCycleTimer = antivirusCycleTimer + dt
        
        if antivirusCycleTimer >= antivirusCycleDuration then
            antivirusCycleTimer = 0
        end
        
        -- Check if we should be in active phase
        local wasActive = antivirusActive
        antivirusActive = (antivirusCycleTimer < antivirusActiveDuration)
        
        -- Handle state changes
        if antivirusActive and not wasActive then
            -- Just activated - set yellow sprite
            Sprite.set_color(playerSprite, 255, 255, 0)
        elseif not antivirusActive and wasActive then
            -- Just deactivated - set white sprite
            Sprite.set_color(playerSprite, 255, 255, 255)
        end
    end
end

 --=====================================================================
 --  [RECOIL] Visual recoil effect
 --=====================================================================
function SystemShooterPlayer.updateRecoil(dt)
    local target = 0

    local interval = currentFireInterval or 0.5
    if interval < 0.001 then
        interval = 0.5
    end

    if fireCooldownTimer >= 0 then
        local remaining = fireCooldownTimer
        if remaining < 0 then remaining = 0 end
        if remaining > interval then remaining = interval end

        local elapsed = interval - remaining
        if elapsed < 0 then elapsed = 0 end
        if elapsed > interval then elapsed = interval end

        local tNorm = 0
        if interval > 0 then
            tNorm = elapsed / interval
            if tNorm < 0 then tNorm = 0 end
            if tNorm > 1 then tNorm = 1 end
        end

        local tri = 0
        if tNorm <= 0.5 then
            tri = tNorm / 0.5
        else
            tri = (1.0 - tNorm) / 0.5
        end

        target = recoilMaxOffset * tri
    end

    recoilOffset = target

    local rx = 0
    local ry = 0
    if recoilOffset > 0 then
        rx = -aimDirX * recoilOffset
        ry = -aimDirY * recoilOffset
    end

    Entity.set_global_pos(player, playerX + rx, playerY + ry)
end

 --=====================================================================
 --  [MOVEMENT] Mouse-based movement with knockback
 --=====================================================================
function SystemShooterPlayer.updateMovement(dt, sensitivity)
    -- Get mouse delta (relative movement)
    local delta = Input.get_mouse_delta()
    local deltaX = 0
    local deltaY = 0
    
    -- Skip first delta after resume to prevent teleport
    if skipNextMouseDelta then
        skipNextMouseDelta = false
    elseif knockbackTimer <= 0 then
        deltaX = delta.x
        deltaY = delta.y
    end
    
    -- Move player by delta (allows knockback since not snapping to cursor)
    local sens = sensitivity or 1.0
    playerX = playerX + deltaX * playerSpeed * sens
    playerY = playerY + deltaY * playerSpeed * sens
    
    if knockbackTimer > 0 then
        local tNorm = 1.0 - (knockbackTimer / knockbackDuration)
        if tNorm < 0 then tNorm = 0 end
        if tNorm > 1 then tNorm = 1 end
        local factor = 1.0 - (tNorm * tNorm)
        local speed = knockbackBaseSpeed * factor * dt
        playerX = playerX + knockbackDirX * speed
        playerY = playerY + knockbackDirY * speed
        
        knockbackTimer = knockbackTimer - dt
    end
    
    -- Clamp to screen bounds
    playerX = math.max(0, math.min(screenW - playerSize, playerX))
    playerY = math.max(0, math.min(screenH - playerSize, playerY))
    
    -- Update entity position (without recoil offset)
    Entity.set_global_pos(player, playerX, playerY)
end

 --=====================================================================
 --  [AIMING] Auto-aim at nearest enemy
 --=====================================================================
function SystemShooterPlayer.updateAiming(enemies, enemySize)
    local closestEnemy = nil
    local closestDistSq = nil
    local playerCenterX = playerX + playerSize/2
    local playerCenterY = playerY + playerSize/2
    
    for i = 1, #enemies do
        local e = enemies[i]
        -- Skip dead enemies for auto-aim (but allow disabled enemies for pre-fire)
        if not e.isDead then
            local eSize = e.size or enemySize
            local enemyCenterX = e.x + eSize/2
            local enemyCenterY = e.y + eSize/2
            local dx = enemyCenterX - playerCenterX
            local dy = enemyCenterY - playerCenterY
            local distSq = dx * dx + dy * dy
            if closestDistSq == nil or distSq < closestDistSq then
                closestDistSq = distSq
                closestEnemy = e
            end
        end
    end

    if closestEnemy ~= nil then
        local eSize = closestEnemy.size or enemySize
        local enemyCenterX = closestEnemy.x + eSize/2
        local enemyCenterY = closestEnemy.y + eSize/2
        local dx = enemyCenterX - playerCenterX
        local dy = enemyCenterY - playerCenterY
        local angleRadians = math.atan(dy, dx)
        local angleDegrees = math.deg(angleRadians) + 90  -- +90 because triangle points up by default
        Entity.set_global_rot(player, angleDegrees)

        local dist = math.sqrt(dx * dx + dy * dy)
        if dist > 0 then
            aimDirX = dx / dist
            aimDirY = dy / dist
        end
    end
end

 --=====================================================================
 --  [SHOOTING] Spawn projectiles
 --=====================================================================
local function spawnProjectile(onShotFired)
    -- Spawn at tip of triangle (offset in aim direction)
    local centerX = playerX + playerSize/2
    local centerY = playerY + playerSize/2
    local tipX = centerX + aimDirX * (playerSize/2)
    local tipY = centerY + aimDirY * (playerSize/2)

    local firepower = SystemShooterPlayerProgress.getFirepower()
    local pierceCount = SystemShooterPlayerProgress.getPierceCount()
    local bounceCount = SystemShooterPlayerProgress.getBounceCount()

    local projectileSize = SystemShooterProjectiles.PlayerConfig.size
    local shots = SystemShooterPlayerProgress.getShots(firepower, tipX, tipY, aimDirX, aimDirY, projectileSize)
    if not shots then
        return
    end
    
    if onShotFired then
        onShotFired(#shots)
    end

    for i = 1, #shots do
        local s = shots[i]
        local offsetX = s.offsetX or 0
        local offsetY = s.offsetY or 0
        local dirX = s.dirX or aimDirX
        local dirY = s.dirY or aimDirY

        local spawnX = tipX + offsetX - projectileSize/2
        local spawnY = tipY + offsetY - projectileSize/2
        SystemShooterProjectiles.spawnPlayerProjectile(spawnX, spawnY, dirX, dirY, pierceCount, bounceCount, s)
    end
end

function SystemShooterPlayer.updateShooting(dt, isNoWitnessesActive, playGunshotSfx, onShotFired)
    -- Update fire cooldown
    if fireCooldownTimer > 0 then
        fireCooldownTimer = fireCooldownTimer - dt
    end
    
    if Input.get_mouse_button_down(1) then
        isFiring = true
        if fireCooldownTimer <= 0 then
            spawnProjectile(onShotFired)
            if playGunshotSfx then
                playGunshotSfx()
            end
            local interval = SystemShooterPlayerProgress.getCurrentFireInterval()
            if not interval or interval <= 0 then
                interval = 0.5
            end
            if isNoWitnessesActive and isNoWitnessesActive() then
                local delta = SystemShooterPlayerProgress.getNoWitnessesFireIntervalDelta()
                interval = math.max(0.05, interval - (delta or 0))
            end
            currentFireInterval = interval
            fireCooldownTimer = interval
        end
    end

    if Input.get_mouse_button_up(1) then
        isFiring = false
    end

    if isFiring and fireCooldownTimer <= 0 then
        spawnProjectile(onShotFired)
        if playGunshotSfx then
            playGunshotSfx()
        end
        local interval = SystemShooterPlayerProgress.getCurrentFireInterval()
        if not interval or interval <= 0 then
            interval = 0.5
        end
        if isNoWitnessesActive and isNoWitnessesActive() then
            local delta = SystemShooterPlayerProgress.getNoWitnessesFireIntervalDelta()
            interval = math.max(0.05, interval - (delta or 0))
        end
        currentFireInterval = interval
        fireCooldownTimer = interval
    end
end

 --=====================================================================
 --  [RESET] Reset player state for new level/run
 --=====================================================================
function SystemShooterPlayer.resetForLevel(resetHealth)
    playerX = screenW / 2 - playerSize / 2
    playerY = screenH / 2 - playerSize / 2
    Entity.set_global_pos(player, playerX, playerY)
    
    -- Only reset sprite color if antivirus isn't currently active
    if not antivirusActive then
        Sprite.set_color(playerSprite, 255, 255, 255)
    end
    
    playerFlashTimer = 0
    damageCooldown = 0
    
    if resetHealth then
        playerHealth = SystemShooterPlayerProgress.getMaxHealth()
    end
end

function SystemShooterPlayer.resetForRun()
    fireCooldownTimer = 0
    damageCooldown = 0
    isFiring = false
    knockbackTimer = 0
    playerFlashTimer = 0
    playerHealth = SystemShooterPlayerProgress.getMaxHealth()
    playerX = screenW / 2 - playerSize / 2
    playerY = screenH / 2 - playerSize / 2
    Entity.set_global_pos(player, playerX, playerY)
    
    -- Only reset sprite color if antivirus isn't currently active
    if not antivirusActive then
        Sprite.set_color(playerSprite, 255, 255, 255)
    end
end

function SystemShooterPlayer.stopFiring()
    isFiring = false
end

return SystemShooterPlayer
