local SystemShooterLevels = {}

local PROCEDURAL_START_LEVEL = 6

--=====================================================================
--  [MUSIC] BPM & Beat-Synced Fire Rates
--=====================================================================
local BPM = 133
local BEAT_DURATION = 60 / BPM

local function beatsToSeconds(beats)
    return beats * BEAT_DURATION
end

local FIRE_RATE_WEIGHTS = {
    standard = {
        [1.5] = 1,
        [2]   = 3,
        [2.5] = 2,
        [3]   = 1,
    },
    boss = {
        [2.5]   = 2,
        [3.5]   = 1,
    },
}

local function weightedBeatPick(weights)
    local total = 0
    for _, w in pairs(weights) do total = total + w end
    local roll = math.random() * total
    local cumulative = 0
    for beats, w in pairs(weights) do
        cumulative = cumulative + w
        if roll <= cumulative then
            return beatsToSeconds(beats)
        end
    end
    -- Fallback (shouldn't happen)
    for beats, _ in pairs(weights) do
        return beatsToSeconds(beats)
    end
end

local generatedLevelCache = {}

local HEALTH_BUDGET = {
    base = 60,
    perLevel = 15,
    levelSquaredFactor = 0.7,
    lateLevelThreshold = 15,
    lateSquaredFactor = 1.75,
    lateCubedFactor = 0.045,
}

local TIMER_CONFIG = {
    min = 20,
    max = 35,
    standard = 20,
    healthPerSecond = 12,
}

local WINDOW_CONFIG = {
    minWidth = 400,
    minHeight = 400,
    maxWidth = 1400,
    maxHeight = 850,
    maxArea = 950000,
}

local ENEMY_TEMPLATES = {
    bounce = {
        minLevel = 1,
        healthMin = 25,
        healthMax = 70,
        budgetPercentCap = 0.35,
        budgetCapMinLevel = 20,
        maxPerLevel = 3,
        spaceRequirement = 0,
        weight = 3,
        baseSize = 32,
        generate = function(health, level, windowW, windowH)
            local speedBase = 1.0 + (level - 6) * 0.05
            local speed = speedBase + math.random() * 0.3
            if speed > 1.6 then speed = 1.6 end
            return {
                movementType = "bounce",
                health = health,
                speed = speed,
            }
        end,
    },
    stationary = {
        minLevel = 1,
        healthMin = 30,
        healthMax = 120,
        maxPerLevel = 3,
        spaceRequirement = 1,
        weight = 5,
        baseSize = 32,
        generate = function(health, level, windowW, windowH)
            local margin = 100
            local x = margin + math.random() * (windowW - 2 * margin)
            local y = margin + math.random() * (windowH - 2 * margin)
            return {
                movementType = "stationary",
                x = x,
                y = y,
                health = health,
                shootPattern = "cone",
                projectileCount = 2 + math.random(0, 2),
                shootInterval = weightedBeatPick(FIRE_RATE_WEIGHTS.standard),
            }
        end,
    },
    orbit = {
        minLevel = 7,
        healthMin = 40,
        healthMax = 70,
        maxPerLevel = 4,
        spaceRequirement = 2,
        weight = 8,
        baseSize = 32,
        generate = function(health, level, windowW, windowH)
            local centerX = windowW / 2
            local centerY = windowH / 2
            local radius = 80 + math.random() * 80
            local speed = 0.8 + math.random() * 0.8
            if math.random() > 0.5 then speed = -speed end
            return {
                movementType = "orbit",
                orbitCenter = {centerX, centerY},
                orbitRadius = radius,
                orbitSpeed = speed,
                health = health,
                shootPattern = "cone",
                projectileCount = math.random(1, 2),
                shootInterval = weightedBeatPick(FIRE_RATE_WEIGHTS.standard),
            }
        end,
    },
    stationary_boss = {
        minLevel = 20,
        healthMin = 150,
        healthMax = 300,
        maxPerLevel = 1,
        spaceRequirement = 2,
        weight = 6,
        baseSize = 45,
        generate = function(health, level, windowW, windowH)
            local x = windowW / 2
            local y = windowH / 2
            return {
                movementType = "stationary",
                x = x,
                y = y,
                health = health,
                shootPattern = "circle",
                projectileCount = 6 + math.random(0, 4),
                shootInterval = weightedBeatPick(FIRE_RATE_WEIGHTS.boss),
                spinWhileShooting = true,
            }
        end,
    },
    teleporter = {
        minLevel = 35,
        healthMin = 180,
        healthMax = 350,
        maxPerLevel = 1,
        spaceRequirement = 2,
        weight = 2,
        baseSize = 40,
        healthScaling = false,
        generate = function(health, level, windowW, windowH)
            local margin = 100
            local x = margin + math.random() * (windowW - 2 * margin)
            local y = margin + math.random() * (windowH - 2 * margin)
            return {
                movementType = "teleporter",
                x = x,
                y = y,
                health = health,
                healthScaling = false,
                size = 40,
                teleportChargeTime = beatsToSeconds(4),      -- 4 beats to charge
                beamDuration = beatsToSeconds(0.5),          -- 0.5 beats beam
                teleportCooldown = beatsToSeconds(5.5),      -- 5.5 beats cooldown
            }
        end,
    },
    shielder = {
        minLevel = 10,
        healthMin = 40,
        healthMax = 80,
        maxPerLevel = 2,
        spaceRequirement = 1,
        weight = 3,
        baseSize = 28,
        generate = function(health, level, windowW, windowH)
            -- Shielder spawns at a random position, will find ally to protect
            local margin = 80
            local x = margin + math.random() * (windowW - 2 * margin)
            local y = margin + math.random() * (windowH - 2 * margin)
            local speedBase = 400 + (level - 10) * 5
            local speedFast = 650 + (level - 10) * 8
            return {
                movementType = "shielder",
                x = x,
                y = y,
                health = health,
                shielderSpeed = speedBase,
                shielderSpeedFast = speedFast,
                shielderSpeedSlow = 200,
                shielderRotationSpeed = 2.25,  -- Rotation speed around ally (radians/sec)
                -- Orbit radius from center-to-center (should be at least sum of half-sizes to prevent overlap)
                shielderOrbitRadius = 90,
                -- Lightning shield config for the shielder itself (ALWAYS enabled for shielder)
                lightningShield = {
                    enabled = true,
                    radius = 90,
                    spreadAngle = math.rad(100),
                    facingAngle = -math.pi / 2,
                    maxRotationSpeed = 1.75,
                    thickness = 6.5,
                    jaggedness = 0.04,
                    segments = 6,
                    color = { r = 50, g = 200, b = 200, a = 255 },
                    flickerSpeed = 0.12,
                    corruption = {
                        enabled = true,
                        targetColor = { r = 255, g = 255, b = 255 },
                        decayRate = 0.075,
                        spreadRate = 0.25,
                        hitAmount = 0.20,
                        breakThreshold = 0.925,
                    },
                }
            }
        end,
    },
}
local levels = {
    [1] = {
        timeLimitSeconds = 20,
        windowWidth = 640,
        windowHeight = 800,
        enemies = {
            { 
                movementType = "stationary", 
                x = 400, 
                y = 500, 
                health = 45, 
                shootPattern = "cone", 
                projectileCount = 2, 
                shootInterval = beatsToSeconds(2),
            },
        },
    },
    [2] = {
        timeLimitSeconds = 20,
        windowWidth = 800,
        windowHeight = 400,
        enemies = {
            { movementType = "orbit", x = 506, y = 186, health = 35, orbitCenter = {400, 200}, orbitRadius = 120, orbitSpeed = 1.0, shootPattern = "cone", projectileCount = 1, shootInterval = beatsToSeconds(2) },
            
        },
    },
    [3] = {
        timeLimitSeconds = 20,
        windowWidth = 1026,
        windowHeight = 640,
        enemies = {
            { movementType = "stationary", x = 500, y = 300, health = 30, shootPattern = "cone", projectileCount = 2, shootInterval = beatsToSeconds(2) },
            { movementType = "orbit", x = 659, y = 306, health = 30, orbitCenter = {513, 320}, orbitRadius = 160, orbitSpeed = -1.0, shootPattern = "cone", projectileCount = 1, shootInterval = beatsToSeconds(2) },
        },
    },
    [4] = {
        timeLimitSeconds = 25,
        windowWidth = 800,
        windowHeight = 800,
        enemies = {
            { movementType = "stationary", x = 400, y = 260, health = 30, shootPattern = "cone", projectileCount = 2, shootInterval = beatsToSeconds(2) },
            { movementType = "bounce", x = 400, y = 540, health = 30 },
        },
    },
    [5] = {
        timeLimitSeconds = 20,
        windowWidth = 600,
        windowHeight = 800,
        enemies = {
            { movementType = "stationary", x = 500, y = 300, health = 35, shootPattern = "cone", projectileCount = 1, shootInterval = beatsToSeconds(2) },
            { movementType = "stationary", x = 350, y = 200, health = 50, shootPattern = "cone", projectileCount = 2, shootInterval = beatsToSeconds(2.5) },
        },
    },
}
local function calculateHealthBudget(level)
    local n = level - PROCEDURAL_START_LEVEL
    local budget = HEALTH_BUDGET.base + (n * HEALTH_BUDGET.perLevel) + (n * n * HEALTH_BUDGET.levelSquaredFactor)
    
    if level > HEALTH_BUDGET.lateLevelThreshold then
        local lateN = level - HEALTH_BUDGET.lateLevelThreshold
        budget = budget + (lateN * lateN * HEALTH_BUDGET.lateSquaredFactor)
        budget = budget + (lateN * lateN * lateN * HEALTH_BUDGET.lateCubedFactor)
    end
    
    return budget
end

local function getScaledHealthRange(template, level, healthBudget)
    local n = level - PROCEDURAL_START_LEVEL
    local scaleFactor = 1 + (n * 0.08)
    local scaledMin = math.floor(template.healthMin * scaleFactor)
    local scaledMax = math.floor(template.healthMax * scaleFactor)
    
    if template.budgetPercentCap and template.budgetCapMinLevel and healthBudget then
        if level >= template.budgetCapMinLevel then
            local percentCap = math.floor(healthBudget * template.budgetPercentCap)
            if scaledMax > percentCap then
                scaledMax = percentCap
            end
        end
    end
    
    if scaledMax < scaledMin then
        scaledMax = scaledMin
    end
    
    return scaledMin, scaledMax
end

local function calculateTimer(totalHealth)
    local timer = totalHealth / TIMER_CONFIG.healthPerSecond
    timer = math.max(TIMER_CONFIG.min, math.min(TIMER_CONFIG.max, timer))
    return timer
end

local function getAvailableTemplates(level)
    local available = {}
    for name, template in pairs(ENEMY_TEMPLATES) do
        if level >= template.minLevel then
            table.insert(available, {name = name, template = template})
        end
    end
    return available
end

local MAX_ENEMIES_CONFIG = {
    { stage = 35, max = 7 },
    { stage = 30, max = 6 },
    { stage = 22, max = 5 },
    { stage = 16, max = 4 },
    { stage = 8,  max = 3 },
    { stage = 1,  max = 2 }, -- Default/fallback
}

local function getMaxEnemiesForStage(stage)
    for _, cfg in ipairs(MAX_ENEMIES_CONFIG) do
        if stage >= cfg.stage then
            return cfg.max
        end
    end
    return 2
end

local function weightedRandomPick(available, counts, totalEnemies, stage)
    local maxEnemies = getMaxEnemiesForStage(stage)
    if totalEnemies >= maxEnemies then return nil end

    local totalWeight = 0
    for _, entry in ipairs(available) do
        local name = entry.name
        local template = entry.template
        local currentCount = counts[name] or 0
        if currentCount < template.maxPerLevel then
            totalWeight = totalWeight + template.weight
        end
    end
    if totalWeight <= 0 then return nil end
    
    local roll = math.random() * totalWeight
    local cumulative = 0
    for _, entry in ipairs(available) do
        local name = entry.name
        local template = entry.template
        local currentCount = counts[name] or 0
        if currentCount < template.maxPerLevel then
            cumulative = cumulative + template.weight
            if roll <= cumulative then
                return entry
            end
        end
    end
    return available[1]
end

local function calculateWindowSize(enemies)
    local spaceScore = 0
    local enemyCount = #enemies
    
    for _, enemy in ipairs(enemies) do
        local templateName = enemy.movementType
        if templateName == "stationary" and enemy.spinWhileShooting then
            templateName = "stationary_boss"
        end
        local template = ENEMY_TEMPLATES[templateName]
        if template then
            spaceScore = spaceScore + template.spaceRequirement
        end
    end
    
    spaceScore = spaceScore + enemyCount * 0.5
    
    local widthRatio = 0.5 + math.random() * 0.5
    local baseArea = 350000 + spaceScore * 80000
    if baseArea > WINDOW_CONFIG.maxArea then baseArea = WINDOW_CONFIG.maxArea end
    
    local width, height
    if math.random() > 0.7 then
        height = math.sqrt(baseArea * widthRatio)
        width = baseArea / height
    else
        width = math.sqrt(baseArea / widthRatio)
        height = baseArea / width
    end
    
    width = math.max(WINDOW_CONFIG.minWidth, math.min(WINDOW_CONFIG.maxWidth, math.floor(width)))
    height = math.max(WINDOW_CONFIG.minHeight, math.min(WINDOW_CONFIG.maxHeight, math.floor(height)))
    
    return width, height
end

local function repositionEnemies(enemies, windowW, windowH)
    local centerX = windowW / 2
    local centerY = windowH / 2
    
    for _, enemy in ipairs(enemies) do
        if enemy.movementType == "stationary" or enemy.movementType == "teleporter" then
            local margin = (enemy.size or 48) + 50
            if enemy.x then
                enemy.x = math.max(margin, math.min(windowW - margin, enemy.x))
            end
            if enemy.y then
                enemy.y = math.max(margin, math.min(windowH - margin, enemy.y))
            end
        elseif enemy.movementType == "orbit" then
            enemy.orbitCenter = {centerX, centerY}
            local maxRadius = math.min(windowW, windowH) / 2 - 80
            if enemy.orbitRadius > maxRadius then
                enemy.orbitRadius = maxRadius
            end
        end
    end
end

local function generateProceduralLevel(levelIndex)
    math.randomseed(os.time() + levelIndex * 1000 + math.random(1, 10000))
    
    local healthBudget = calculateHealthBudget(levelIndex)
    local available = getAvailableTemplates(levelIndex)
    local enemies = {}
    local counts = {}
    local remainingBudget = healthBudget
    
    local minHealth = 999999
    for _, entry in ipairs(available) do
        local scaledMin, _ = getScaledHealthRange(entry.template, levelIndex, healthBudget)
        if scaledMin < minHealth then
            minHealth = scaledMin
        end
    end
    
    local minEnemies = 2
    
    local attempts = 0
    local maxAttempts = 20
    
    while attempts < maxAttempts do
        attempts = attempts + 1
        
        local picked = weightedRandomPick(available, counts, #enemies, levelIndex)
        if not picked then break end
        
        local template = picked.template
        local name = picked.name
        
        local scaledMin, scaledMax = getScaledHealthRange(template, levelIndex, healthBudget)
        
        if #enemies < minEnemies then
            local maxHealthForMinEnemies = remainingBudget - (minEnemies - #enemies - 1) * minHealth
            local healthMax = math.min(scaledMax, maxHealthForMinEnemies)
            if healthMax < scaledMin then
                goto continue
            end
            local targetHealth = scaledMin + math.random() * (healthMax - scaledMin)
            local health = math.floor(targetHealth)
            local enemy = template.generate(health, levelIndex, 1000, 600)
            enemy.size = template.baseSize
            enemy.healthScaling = template.healthScaling
            table.insert(enemies, enemy)
            counts[name] = (counts[name] or 0) + 1
            remainingBudget = remainingBudget - health
        else
            if remainingBudget < minHealth then break end
            
            local healthMax = math.min(scaledMax, remainingBudget)
            if healthMax < scaledMin then
                goto continue
            end
            
            local health
            if remainingBudget <= scaledMax * 1.2 then
                health = remainingBudget
            else
                local targetHealth = scaledMin + math.random() * (healthMax - scaledMin)
                health = math.floor(targetHealth)
            end
            
            local enemy = template.generate(health, levelIndex, 1000, 600)
            enemy.size = template.baseSize
            enemy.healthScaling = template.healthScaling
            table.insert(enemies, enemy)
            counts[name] = (counts[name] or 0) + 1
            remainingBudget = remainingBudget - health
        end
        
        ::continue::
    end
    
    if remainingBudget > 0 and #enemies > 0 then
        for i = #enemies, 1, -1 do
            if remainingBudget <= 0 then break end
            
            local enemy = enemies[i]
            local templateName = enemy.movementType
            local template = ENEMY_TEMPLATES[templateName]
            if template then
                local _, scaledMax = getScaledHealthRange(template, levelIndex, healthBudget)
                local headroom = scaledMax - enemy.health
                if headroom > 0 then
                    local toAdd = math.min(headroom, remainingBudget)
                    enemy.health = enemy.health + toAdd
                    remainingBudget = remainingBudget - toAdd
                end
            end
        end
    end
    
    local windowW, windowH = calculateWindowSize(enemies)
    repositionEnemies(enemies, windowW, windowH)
    
    local totalHealth = healthBudget - remainingBudget
    local timer = calculateTimer(totalHealth)
    
    return {
        timeLimitSeconds = timer,
        windowWidth = windowW,
        windowHeight = windowH,
        enemies = enemies,
        _generated = true,
        _healthBudget = healthBudget,
        _actualHealth = totalHealth,
    }
end

function SystemShooterLevels.getLevelConfig(index)
    if index < PROCEDURAL_START_LEVEL then
        return levels[index]
    end
    if not generatedLevelCache[index] then
        generatedLevelCache[index] = generateProceduralLevel(index)
    end
    return generatedLevelCache[index]
end

function SystemShooterLevels.clearLevelCache(index)
    if index then
        generatedLevelCache[index] = nil
    else
        generatedLevelCache = {}
    end
end

function SystemShooterLevels.regenerateLevel(index)
    if index >= PROCEDURAL_START_LEVEL then
        generatedLevelCache[index] = generateProceduralLevel(index)
    end
    return SystemShooterLevels.getLevelConfig(index)
end

function SystemShooterLevels.getHealthBudgetConfig()
    return HEALTH_BUDGET
end

function SystemShooterLevels.getTimerConfig()
    return TIMER_CONFIG
end

function SystemShooterLevels.getWindowConfig()
    return WINDOW_CONFIG
end

function SystemShooterLevels.getEnemyTemplates()
    return ENEMY_TEMPLATES
end

function SystemShooterLevels.getBPM()
    return BPM
end

function SystemShooterLevels.getBeatDuration()
    return BEAT_DURATION
end

function SystemShooterLevels.getFireRateWeights()
    return FIRE_RATE_WEIGHTS
end

return SystemShooterLevels
