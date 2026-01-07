local SystemShooterRewind = {}

--=====================================================================
--  [REWIND SYSTEM] Level Timeout Rewind Mechanic
--  Phases: nil (inactive), "slowing", "stopped", "rewinding", "done"
--=====================================================================

local state = {
    phase = nil,
    slowdownDuration = 2.0,      -- seconds for enemies to slow to a stop
    stopPauseDuration = 0.3,     -- brief pause at full stop before rewinding
    duration = 4.5,              -- seconds for enemies to rewind to spawn positions
    timer = 0,                   -- current phase timer
    timeScale = 1.0,             -- current time scale (1.0 = normal, 0.0 = stopped)
    enemyStartPositions = {},    -- stores {enemy -> {x, y}} for lerp start (keyed by enemy ref)
    enemyStartRotations = {},    -- stores {enemy -> rotation} for spin slowdown
    enemyOriginalSpeedMults = {},-- stores {enemy -> speedMultiplier} for restoring
    enemyOriginalSpinVelocities = {}, -- stores {enemy -> spinVelocity} for spin slowdown
    enemyStartHealth = {},       -- stores {enemy -> health} at rewind start for HP lerp
    enemyTargetHealth = {},      -- stores {enemy -> health} target HP (from level config)
    enemyWasDead = {},           -- stores {enemy -> bool} whether enemy was dead at rewind start
    enemyProjStartPositions = {},-- stores {projIndex -> {x, y}} for enemy projectile rewind
    levelTimerStart = 0,         -- timer value at start of rewind phase
    levelTimerTarget = 0,        -- timer value to rewind to (full level time)
    healthStart = 0,             -- health at start of rewind
    healthTarget = 0,            -- health to rewind to (80% of max)
}

--=====================================================================
--  [PUBLIC API] State Queries
--=====================================================================
function SystemShooterRewind.isActive()
    return state.phase ~= nil
end

function SystemShooterRewind.getPhase()
    return state.phase
end

function SystemShooterRewind.getTimeScale()
    return state.timeScale
end

--=====================================================================
--  [PUBLIC API] Store Spawn Positions (called when level loads)    
--=====================================================================
function SystemShooterRewind.setSpawnPosition(enemyIndex, x, y)
    -- No-op: spawn positions now stored on enemy objects directly
end

function SystemShooterRewind.clearSpawnPositions()
    -- No-op: spawn positions now stored on enemy objects directly
end

--=====================================================================
--  [PUBLIC API] Reset State (for menu/game over)
--=====================================================================
function SystemShooterRewind.reset()
    state.phase = nil
    state.timer = 0
    state.timeScale = 1.0
    state.enemyStartPositions = {}
    state.enemyStartRotations = {}
    state.enemyOriginalSpeedMults = {}
    state.enemyOriginalSpinVelocities = {}
    state.enemyStartHealth = {}
    state.enemyTargetHealth = {}
    state.enemyWasDead = {}
    state.enemyProjStartPositions = {}
end

--=====================================================================
--  [PUBLIC API] Start Rewind Sequence
--=====================================================================
function SystemShooterRewind.start(enemies, playerHealth, maxHealth, levelTimeLimitSeconds, enemyTargetHealthList)
    state.phase = "slowing"
    state.timer = state.slowdownDuration
    state.timeScale = 1.0
    
    -- Capture current enemy positions, rotations, speed multipliers, spin velocities, health, and dead state
    state.enemyStartPositions = {}
    state.enemyStartRotations = {}
    state.enemyOriginalSpeedMults = {}
    state.enemyOriginalSpinVelocities = {}
    state.enemyStartHealth = {}
    state.enemyTargetHealth = {}
    state.enemyWasDead = {}
    for i, enemy in ipairs(enemies) do
        -- For dead enemies, use their spawn position as start position
        if enemy.isDead then
            state.enemyStartPositions[enemy] = { x = enemy.spawnX, y = enemy.spawnY }
            state.enemyWasDead[enemy] = true
        else
            state.enemyStartPositions[enemy] = { x = enemy.x, y = enemy.y }
            state.enemyWasDead[enemy] = false
        end
        state.enemyStartRotations[enemy] = enemy.rotation or 0
        state.enemyOriginalSpeedMults[enemy] = enemy.speedMultiplier or 1.0
        state.enemyOriginalSpinVelocities[enemy] = enemy.spinVelocity or 0
        state.enemyStartHealth[enemy] = enemy.health or 0
        state.enemyTargetHealth[enemy] = enemyTargetHealthList and enemyTargetHealthList[i] or enemy.health or 0
    end
    
    -- Capture level timer target (full level time)
    state.levelTimerStart = 0  -- timer is at 0 when timeout happens
    state.levelTimerTarget = levelTimeLimitSeconds or 0
    
    -- Capture health values for animation
    state.healthStart = playerHealth
    state.healthTarget = math.floor(maxHealth * 0.8)  -- 80% of max
end

--=====================================================================
--  [PUBLIC API] Update Rewind System
--  Returns: table with results or nil if not active
--  {
--      phase = current phase,
--      done = true if rewind just completed,
--      levelTimer = current level timer value,
--      playerHealth = current player health value,
--  }
--=====================================================================
function SystemShooterRewind.update(dt, enemies, projectiles, enemyProjectiles, updateEnemyMovementFn, clearProjectilesFn)
    if not state.phase then
        return nil
    end
    
    local result = {
        phase = state.phase,
        done = false,
        levelTimer = nil,
        playerHealth = nil,
    }
    
    if state.phase == "slowing" then
        -- Gradually slow down everything with accelerating deceleration (ease-in)
        state.timer = state.timer - dt
        local progress = 1.0 - (state.timer / state.slowdownDuration)
        if progress > 1.0 then progress = 1.0 end
        
        -- Ease-in curve (accelerating deceleration): starts slow, ends fast
        local easedProgress = progress * progress
        state.timeScale = 1.0 - easedProgress  -- Goes from 1.0 to 0.0 with ease-in
        
        local scaledDt = dt * state.timeScale
        
        -- Player projectiles slow down with same easing
        for _, proj in ipairs(projectiles) do
            proj.x = proj.x + proj.vx * scaledDt
            proj.y = proj.y + proj.vy * scaledDt
            Entity.set_global_pos(proj.entity, proj.x, proj.y)
        end
        
        -- Enemy projectiles slow down with enemies
        for _, proj in ipairs(enemyProjectiles) do
            proj.x = proj.x + proj.vx * scaledDt
            proj.y = proj.y + proj.vy * scaledDt
            Entity.set_global_pos(proj.entity, proj.x, proj.y)
        end
        
        -- Scale enemy speed multipliers AND spin velocities for gradual slowdown (alive enemies only)
        for i, enemy in ipairs(enemies) do
            if not enemy.isDead then
                local originalMult = state.enemyOriginalSpeedMults[enemy] or 1.0
                local originalSpin = state.enemyOriginalSpinVelocities[enemy] or 0
                enemy.speedMultiplier = originalMult * state.timeScale
                enemy.spinVelocity = originalSpin * state.timeScale
                
                -- Apply transparency during slowdown (lerp to 50% brightness)
                if enemy.sprite then
                    local c = enemy.color or {255, 255, 255}
                    local alpha = 1.0 - (easedProgress * 0.5)  -- Goes from 1.0 to 0.5
                    Sprite.set_color(enemy.sprite, 
                        math.floor(c[1] * alpha), 
                        math.floor(c[2] * alpha), 
                        math.floor(c[3] * alpha))
                end
            end
        end
        
        -- Run normal enemy movement (will use scaled speedMultiplier and spinVelocity)
        if updateEnemyMovementFn then
            updateEnemyMovementFn()
        end
        
        if state.timer <= 0 then
            -- Capture final positions/rotations after slowdown for rewind lerp
            for i, enemy in ipairs(enemies) do
                if not enemy.isDead then
                    state.enemyStartPositions[enemy] = { x = enemy.x, y = enemy.y }
                    state.enemyStartRotations[enemy] = enemy.rotation or 0
                    -- Restore original speed multiplier (spin stays at 0)
                    enemy.speedMultiplier = state.enemyOriginalSpeedMults[enemy] or 1.0
                    enemy.spinVelocity = 0
                end
            end
            
            state.phase = "stopped"
            state.timer = state.stopPauseDuration
            state.timeScale = 0.0
        end
        
    elseif state.phase == "stopped" then
        -- Brief pause at full stop, then capture projectile positions for rewind
        state.timer = state.timer - dt
        if state.timer <= 0 then
            -- Destroy player projectiles only
            if clearProjectilesFn then
                clearProjectilesFn(true)  -- Pass true to only clear player projectiles
            end
            
            -- Capture enemy projectile positions for rewind lerp
            state.enemyProjStartPositions = {}
            for i, proj in ipairs(enemyProjectiles) do
                state.enemyProjStartPositions[i] = { x = proj.x, y = proj.y }
            end
            
            -- Respawn dead enemies at their spawnpoints with 1hp
            for i, enemy in ipairs(enemies) do
                if state.enemyWasDead[enemy] then
                    -- Position at spawnpoint
                    enemy.x = enemy.spawnX
                    enemy.y = enemy.spawnY
                    Entity.set_global_pos(enemy.entity, enemy.spawnX, enemy.spawnY)
                    
                    -- Set to 1hp (will lerp to target during rewind)
                    enemy.health = 1
                    state.enemyStartHealth[enemy] = 1
                    
                    -- Update display size for 1hp
                    if enemy.healthScaling then
                        enemy.displaySize = enemy.size + (1 * (enemy.sizePerHp or 0.125))
                        if enemy.sprite then
                            Sprite.set_image_width(enemy.sprite, math.floor(enemy.displaySize))
                            Sprite.set_image_height(enemy.sprite, math.floor(enemy.displaySize))
                        end
                    end
                    
                    -- Mark as alive again
                    enemy.isDead = false
                    
                    -- Apply 50% transparency to match alive enemies
                    if enemy.sprite then
                        local c = enemy.color or {255, 255, 255}
                        Sprite.set_color(enemy.sprite, 
                            math.floor(c[1] * 0.5), 
                            math.floor(c[2] * 0.5), 
                            math.floor(c[3] * 0.5))
                    end
                    
                    -- Reset rotation
                    enemy.rotation = 0
                    Entity.set_global_rot(enemy.entity, 0)
                end
            end
            
            -- Transition to rewinding phase
            state.phase = "rewinding"
            state.timer = state.duration
        end
        
    elseif state.phase == "rewinding" then
        -- Lerp enemies back to spawn positions with reverse spin
        state.timer = state.timer - dt
        local progress = 1.0 - (state.timer / state.duration)
        if progress > 1.0 then progress = 1.0 end
        
        -- Smooth easing (ease-in-out)
        local easedProgress = progress < 0.5 
            and 2 * progress * progress 
            or 1 - math.pow(-2 * progress + 2, 2) / 2
        
        -- Calculate reverse spin speed (peaks in middle, slows at ends)
        -- Use a bell curve: sin(progress * pi) peaks at 0.5
        local spinCurve = math.sin(progress * math.pi)
        local reverseSpinSpeed = -120 * spinCurve  -- Negative = reverse direction
        
        -- Lerp enemy positions and apply reverse spin toward rotation 0
        for i, enemy in ipairs(enemies) do
            local startPos = state.enemyStartPositions[enemy]
            -- Target is always the enemy's own spawnpoint
            local targetX = enemy.spawnX
            local targetY = enemy.spawnY
            if startPos then
                local newX = startPos.x + (targetX - startPos.x) * easedProgress
                local newY = startPos.y + (targetY - startPos.y) * easedProgress
                enemy.x = newX
                enemy.y = newY
                Entity.set_global_pos(enemy.entity, newX, newY)
            end
            
            -- Apply reverse spin during rewind, lerping toward rotation 0
            -- Normalize start rotation to [-180, 180] range
            local startRot = state.enemyStartRotations[enemy] or 0
            startRot = ((startRot + 180) % 360) - 180
            
            -- Base lerp toward 0
            local baseRotation = startRot + (0 - startRot) * easedProgress
            -- Add reverse spin overlay (diminishes as we approach the end)
            local spinOverlay = reverseSpinSpeed * dt * (1.0 - easedProgress)
            enemy.rotation = baseRotation + spinOverlay
            Entity.set_global_rot(enemy.entity, enemy.rotation)
            
            -- Keep enemies at 50% transparency during rewind (peace timer will handle them)
            -- No lerp needed here
            
            -- Lerp enemy health back to target (for size scaling)
            local startHp = state.enemyStartHealth[enemy] or 0
            local targetHp = state.enemyTargetHealth[enemy] or startHp
            enemy.health = math.floor(startHp + (targetHp - startHp) * easedProgress)
            
            -- Update display size based on new health
            if enemy.healthScaling then
                enemy.displaySize = enemy.size + (enemy.health * (enemy.sizePerHp or 0.125))
                if enemy.sprite then
                    Sprite.set_image_width(enemy.sprite, math.floor(enemy.displaySize))
                    Sprite.set_image_height(enemy.sprite, math.floor(enemy.displaySize))
                end
            end
        end
        
        -- Lerp enemy projectiles back to their source enemies
        for i, proj in ipairs(enemyProjectiles) do
            local startPos = state.enemyProjStartPositions[i]
            local sourceEnemy = proj.sourceEnemy
            if startPos and sourceEnemy then
                -- Target is the center of the source enemy
                local eSize = sourceEnemy.displaySize or sourceEnemy.size or 48
                local targetX = sourceEnemy.x + eSize/2 - 12  -- Adjust for projectile size (24/2)
                local targetY = sourceEnemy.y + eSize/2 - 12
                
                local newX = startPos.x + (targetX - startPos.x) * easedProgress
                local newY = startPos.y + (targetY - startPos.y) * easedProgress
                proj.x = newX
                proj.y = newY
                Entity.set_global_pos(proj.entity, newX, newY)
            end
        end
        
        -- Lerp level timer bar (refill)
        result.levelTimer = state.levelTimerStart + (state.levelTimerTarget - state.levelTimerStart) * easedProgress
        
        -- Lerp health bar (refill to 80%)
        result.playerHealth = state.healthStart + (state.healthTarget - state.healthStart) * easedProgress
        
        if state.timer <= 0 then
            state.phase = "done"
            state.timer = 0
        end
        
    elseif state.phase == "done" then
        -- Rewind complete
        result.done = true
        result.levelTimer = state.levelTimerTarget
        result.playerHealth = state.healthTarget
        result.clearEnemyProjectiles = true  -- Signal to clear enemy projectiles
        
        -- Reset state
        state.phase = nil
        state.timeScale = 1.0
    end
    
    result.phase = state.phase
    return result
end

--=====================================================================
--  [PUBLIC API] Restore Enemy Colors (for game over during rewind)
--=====================================================================
function SystemShooterRewind.restoreEnemyColors(enemies)
    for _, enemy in ipairs(enemies) do
        if enemy.sprite then
            local c = enemy.color or {255, 255, 255}
            Sprite.set_color(enemy.sprite, c[1], c[2], c[3])
        end
    end
end

return SystemShooterRewind
