local TriangleShooterLevels = {}

local PROCEDURAL_START_LEVEL = 6

local generatedLevelCache = {}

local HEALTH_BUDGET = {
    base = 70,
    perLevel = 18,
    levelSquaredFactor = 0.5,
    lateLevelThreshold = 12,
    lateSquaredFactor = 2.5,
    lateCubedFactor = 0.3,
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
    maxHeight = 900,
    maxArea = 950000,
}

local ENEMY_TEMPLATES = {
    bounce = {
        minLevel = 1,
        healthMin = 25,
        healthMax = 70,
        maxPerLevel = 4,
        spaceRequirement = 0,
        weight = 10,
        baseSize = 32,
        generate = function(health, level, windowW, windowH)
            local speedBase = 1.0 + (level - 6) * 0.05
            local speed = speedBase + math.random() * 0.3
            if speed > 1.6 then speed = 1.6 end
            return {
                movementType = "bounce",
                health = health,
                speed = speed,
                shootInterval = 0.4 + math.random() * 0.3,
            }
        end,
    },
    stationary = {
        minLevel = 1,
        healthMin = 30,
        healthMax = 120,
        maxPerLevel = 2,
        spaceRequirement = 1,
        weight = 6,
        baseSize = 36,
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
                shootInterval = 0.5 + math.random() * 0.4,
            }
        end,
    },
    orbit = {
        minLevel = 8,
        healthMin = 40,
        healthMax = 70,
        maxPerLevel = 4,
        spaceRequirement = 2,
        weight = 5,
        baseSize = 28,
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
                color = {200, 100, 255},
                shootInterval = 0.5 + math.random() * 0.4,
            }
        end,
    },
    stationary_boss = {
        minLevel = 12,
        healthMin = 150,
        healthMax = 300,
        maxPerLevel = 1,
        spaceRequirement = 2,
        weight = 3,
        baseSize = 40,
        generate = function(health, level, windowW, windowH)
            local x = windowW / 2
            local y = windowH / 2
            return {
                movementType = "stationary",
                x = x,
                y = y,
                health = health,
                color = {255, 100, 100},
                shootPattern = "circle",
                projectileCount = 6 + math.random(0, 4),
                shootInterval = 0.3 + math.random() * 0.3,
                spinWhileShooting = true,
            }
        end,
    },
    teleporter = {
        minLevel = 15,
        healthMin = 180,
        healthMax = 350,
        maxPerLevel = 1,
        spaceRequirement = 2,
        weight = 6,
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
                teleportChargeTime = 0.7 + math.random() * 0.3,
                beamDuration = 0.25 + math.random() * 0.15,
                teleportCooldown = 1.2 + math.random() * 0.6,
            }
        end,
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

local function getScaledHealthRange(template, level)
    local n = level - PROCEDURAL_START_LEVEL
    local scaleFactor = 1 + (n * 0.08)
    local scaledMin = math.floor(template.healthMin * scaleFactor)
    local scaledMax = math.floor(template.healthMax * scaleFactor)
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

local function weightedRandomPick(available, counts)
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
        local scaledMin, _ = getScaledHealthRange(entry.template, levelIndex)
        if scaledMin < minHealth then
            minHealth = scaledMin
        end
    end
    
    local maxEnemies = 6
    local minEnemies = 2
    
    local attempts = 0
    local maxAttempts = 20
    
    while #enemies < maxEnemies and attempts < maxAttempts do
        attempts = attempts + 1
        
        local picked = weightedRandomPick(available, counts)
        if not picked then break end
        
        local template = picked.template
        local name = picked.name
        
        local scaledMin, scaledMax = getScaledHealthRange(template, levelIndex)
        
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
        enemies[#enemies].health = enemies[#enemies].health + remainingBudget
        remainingBudget = 0
    end
    
    local windowW, windowH = calculateWindowSize(enemies)
    repositionEnemies(enemies, windowW, windowH)
    
    local totalHealth = healthBudget - remainingBudget
    local timer = calculateTimer(totalHealth)
    
    return {
        timeLimitSeconds = timer,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = windowW,
        windowHeight = windowH,
        enemies = enemies,
        _generated = true,
        _healthBudget = healthBudget,
        _actualHealth = totalHealth,
    }
end

local levels = {
    [1] = {
        timeLimitSeconds = 20,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 800,
        windowHeight = 400,
        enemies = {
            { movementType = "bounce", x = 900, y = 400, health = 30},
        },
    },
    [2] = {
        timeLimitSeconds = 20,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 640,
        windowHeight = 800,
        enemies = {
            { movementType = "stationary", x = 400, y = 500, health = 50, shootPattern = "cone", projectileCount = 2, shootInterval = 0.3 },
        },
    },
    [3] = {
        timeLimitSeconds = 20,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 1026,
        windowHeight = 640,
        enemies = {
            { movementType = "stationary", x = 500, y = 300, health = 30, shootPattern = "cone", projectileCount = 2, shootInterval = 0.3 },
            { movementType = "bounce", x = 900, y = 550, health = 30, shootPattern = "cone", projectileCount = 0, shootInterval = 0},
        },
    },
    [4] = {
        timeLimitSeconds = 20,
        enemyCount = 2,
        enemyHealth = 40,
        enemyProjectiles = false,
        enemyShootIntervalSeconds = 0.5,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 800,
        windowHeight = 800,
    },
    [5] = {
        timeLimitSeconds = 20,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 600,
        windowHeight = 800,
        enemies = {
            { movementType = "stationary", x = 500, y = 300, health = 40, shootPattern = "cone", projectileCount = 1, shootInterval = 0.4 },
            { movementType = "stationary", x = 350, y = 200, health = 40, shootPattern = "cone", projectileCount = 2,  shootInterval = 0.6 },
        },
    },
}

function TriangleShooterLevels.getLevelConfig(index)
    if index < PROCEDURAL_START_LEVEL then
        return levels[index]
    end
    if not generatedLevelCache[index] then
        generatedLevelCache[index] = generateProceduralLevel(index)
    end
    return generatedLevelCache[index]
end

function TriangleShooterLevels.clearLevelCache(index)
    if index then
        generatedLevelCache[index] = nil
    else
        generatedLevelCache = {}
    end
end

function TriangleShooterLevels.regenerateLevel(index)
    if index >= PROCEDURAL_START_LEVEL then
        generatedLevelCache[index] = generateProceduralLevel(index)
    end
    return TriangleShooterLevels.getLevelConfig(index)
end

function TriangleShooterLevels.getHealthBudgetConfig()
    return HEALTH_BUDGET
end

function TriangleShooterLevels.getTimerConfig()
    return TIMER_CONFIG
end

function TriangleShooterLevels.getWindowConfig()
    return WINDOW_CONFIG
end

function TriangleShooterLevels.getEnemyTemplates()
    return ENEMY_TEMPLATES
end

return TriangleShooterLevels
