local SystemShooterPlayerProgress = {}

local SPREAD_ANGLE_DEG = 10

--=====================================================================
--  UPGRADE CONFIGURATION
--  Central definition for all upgrades: caps, labels, descriptions,
--  minimum level requirements, and apply behavior.
--=====================================================================
local UPGRADE_CONFIG = {
    firepower = {
        statKey      = "firepower",
        label        = "upgradetype.firepower",
        desc         = "upgradedesc.firepower",
        minLevel     = 1,
        maxValue     = 8,
        defaultValue = 1,
        weight       = 10,  -- Higher weight = more common
    },
    pierce = {
        statKey      = "pierceCount",
        label        = "upgradetype.pierce",
        desc         = "upgradedesc.pierce",
        minLevel     = 6,
        maxValue     = 2,
        defaultValue = 0,
        weight       = 5,
    },
    bounce = {
        statKey      = "bounceCount",
        label        = "upgradetype.bounce",
        desc         = "upgradedesc.bounce",
        minLevel     = 3,
        maxValue     = 1,
        defaultValue = 0,
        weight       = 5,
    },
    fire_rate = {
        statKey      = "fireRateUpgradeCount",
        label        = "upgradetype.fireinterval",
        desc         = "upgradedesc.fireinterval",
        minLevel     = 1,
        maxValue     = 3,
        defaultValue = 0,
        customApply  = true,
        weight       = 8,
    },
    no_witnesses = {
        statKey      = "lowEnemyDamageStacks",
        label        = "upgradetype.no_witnesses",
        desc         = "upgradedesc.no_witnesses",
        minLevel     = 1,
        maxValue     = 2,
        defaultValue = 0,
        damageMultiplier = 2.0,
        fireIntervalDelta = 0.05,
        weight       = 6,
    },
    max_health = {
        statKey      = "maxHealth",
        label        = "upgradetype.maxhp",
        desc         = "upgradedesc.maxhp",
        minLevel     = 4,
        maxValue     = nil,
        defaultValue = 100,
        increment    = 30,
        weight       = 7,
    },
}

local playerLevel = 1
local xp = 0
local xpToNextLevel = 100

local playerStats = {
    firepower = 1,
    pierceCount = 0,
    bounceCount = 0,
    fireInterval = 0.3,
    fireRateUpgradeCount = 0,
    lowEnemyDamageStacks = 0,
    maxHealth = 100,
}

local timeoutCount = 0
local MAX_TIMEOUTS = 5

--=====================================================================
--  FIREPOWER SHOT PATTERNS (chronological by firepower value)
--=====================================================================
local function generateShotPattern(firepower, aimX, aimY, projectileSize)
    local shots = {}
    local baseAngle = math.atan(aimY, aimX)

    if firepower == 1 then
        shots[1] = { offsetX = 0, offsetY = 0, dirX = aimX, dirY = aimY }

    elseif firepower == 2 then
        local sideX = -aimY
        local sideY = aimX
        local offset = projectileSize * 0.6
        shots[1] = { offsetX = sideX * offset, offsetY = sideY * offset, dirX = aimX, dirY = aimY }
        shots[2] = { offsetX = -sideX * offset, offsetY = -sideY * offset, dirX = aimX, dirY = aimY }

    elseif firepower == 3 then
        local sideX = -aimY
        local sideY = aimX
        local offset = projectileSize * 0.6
        shots[1] = { offsetX = 0, offsetY = 0, dirX = aimX, dirY = aimY }
        shots[2] = { offsetX = sideX * offset, offsetY = sideY * offset, dirX = aimX, dirY = aimY }
        shots[3] = { offsetX = -sideX * offset, offsetY = -sideY * offset, dirX = aimX, dirY = aimY }

    elseif firepower == 4 then
        local halfSpread = math.rad(SPREAD_ANGLE_DEG) / 2
        local step = (2 * halfSpread) / (firepower - 1)
        for i = 0, firepower - 1 do
            local angle = baseAngle - halfSpread + step * i
            local dirX = math.cos(angle)
            local dirY = math.sin(angle)
            shots[#shots + 1] = { offsetX = 0, offsetY = 0, dirX = dirX, dirY = dirY }
        end

    elseif firepower == 5 then
        shots[1] = {
            offsetX = 0,
            offsetY = 0,
            dirX = aimX,
            dirY = aimY,
            isGolden = true,
            damage = 5,
            sizeMultiplier = 2,
        }

    elseif firepower == 6 then
        shots[1] = {
            offsetX = 0,
            offsetY = 0,
            dirX = aimX,
            dirY = aimY,
            isGolden = true,
            damage = 6,
            sizeMultiplier = 2.5,
        }

    elseif firepower == 7 then
        shots[1] = {
            offsetX = 0,
            offsetY = 0,
            dirX = aimX,
            dirY = aimY,
            isGolden = true,
            damage = 7,
            sizeMultiplier = 3,
        }

    else
        local sideX = -aimY
        local sideY = aimX
        local offset = projectileSize * 0.8
        shots[1] = {
            offsetX = sideX * offset,
            offsetY = sideY * offset,
            dirX = aimX,
            dirY = aimY,
            isGolden = true,
            damage = 4,
            sizeMultiplier = 2,
        }
        shots[2] = {
            offsetX = -sideX * offset,
            offsetY = -sideY * offset,
            dirX = aimX,
            dirY = aimY,
            isGolden = true,
            damage = 4,
            sizeMultiplier = 2,
        }
    end

    return shots
end

function SystemShooterPlayerProgress.getShots(firepower, tipX, tipY, aimX, aimY, projectileSize)
    return generateShotPattern(firepower, aimX, aimY, projectileSize)
end

local function GetXpForNextLevel(level)
    local n = level - 1
    local base = 100 + 85 * n + 15 * n * math.max(n - 1, 0)

    -- Late-game slowdown: quadratic bump starting at level 10
    local lateBumpStart = 10
    local lateBumpScale = 60
    if level > lateBumpStart then
        local delta = level - lateBumpStart
        base = base + lateBumpScale * delta * delta
    end

    return math.floor(base)
end

local pendingLevelUp = false

local function OnLevelUp()
    playerLevel = playerLevel + 1
    xpToNextLevel = GetXpForNextLevel(playerLevel)
    pendingLevelUp = true
end

--=====================================================================
--  UPGRADE LOGIC (driven by UPGRADE_CONFIG)
--=====================================================================
function SystemShooterPlayerProgress.canTakeUpgrade(upgradeType)
    local cfg = UPGRADE_CONFIG[upgradeType]
    if not cfg then return true end
    if cfg.maxValue == nil then return true end
    
    local currentValue = playerStats[cfg.statKey] or cfg.defaultValue
    return currentValue < cfg.maxValue
end

function SystemShooterPlayerProgress.applyUpgrade(upgradeType)
    local cfg = UPGRADE_CONFIG[upgradeType]
    if not cfg then return end
    
    if cfg.customApply then
        if upgradeType == "fire_rate" then
            local count = playerStats.fireRateUpgradeCount or 0
            if count < cfg.maxValue then
                local delta = 0.1
                if count == 1 then
                    delta = 0.05
                elseif count == 2 then
                    delta = 0.025
                end
                playerStats.fireInterval = math.max(0.05, playerStats.fireInterval - delta)
                playerStats.fireRateUpgradeCount = count + 1
            end
        end
    else
        local increment = cfg.increment or 1
        playerStats[cfg.statKey] = (playerStats[cfg.statKey] or cfg.defaultValue) + increment
    end
end

function SystemShooterPlayerProgress.getUpgradeConfig()
    return UPGRADE_CONFIG
end

function SystemShooterPlayerProgress.addXp(amount)
    xp = xp + amount
    while xp >= xpToNextLevel do
        xp = xp - xpToNextLevel
        OnLevelUp()
    end
end

function SystemShooterPlayerProgress.hasPendingLevelUp()
    return pendingLevelUp
end

function SystemShooterPlayerProgress.consumePendingLevelUp()
    if pendingLevelUp then
        pendingLevelUp = false
        return true
    end
    return false
end

function SystemShooterPlayerProgress.getProgress()
    return playerLevel, xp, xpToNextLevel
end

function SystemShooterPlayerProgress.getFirepower()
    return playerStats.firepower
end

function SystemShooterPlayerProgress.getPierceCount()
    return playerStats.pierceCount
end

function SystemShooterPlayerProgress.getBounceCount()
    return playerStats.bounceCount
end

function SystemShooterPlayerProgress.getLowEnemyDamageStacks()
    return playerStats.lowEnemyDamageStacks or 0
end

function SystemShooterPlayerProgress.getNoWitnessesDamageMultiplier()
    local cfg = UPGRADE_CONFIG.no_witnesses
    return (cfg and cfg.damageMultiplier) or 1
end

function SystemShooterPlayerProgress.getNoWitnessesFireIntervalDelta()
    local cfg = UPGRADE_CONFIG.no_witnesses
    return (cfg and cfg.fireIntervalDelta) or 0
end

function SystemShooterPlayerProgress.getCurrentFireInterval()
    return playerStats.fireInterval
end

function SystemShooterPlayerProgress.getMaxHealth()
    return playerStats.maxHealth or 100
end

function SystemShooterPlayerProgress.getStats()
    return playerStats
end

function SystemShooterPlayerProgress.getTimeoutCount()
    return timeoutCount
end

function SystemShooterPlayerProgress.getMaxTimeouts()
    return MAX_TIMEOUTS
end

function SystemShooterPlayerProgress.incrementTimeoutCount()
    timeoutCount = timeoutCount + 1
    return timeoutCount
end

function SystemShooterPlayerProgress.reset()
    playerLevel = 1
    xp = 0
    xpToNextLevel = 100
    pendingLevelUp = false
    timeoutCount = 0
    
    playerStats.firepower = 1
    playerStats.pierceCount = 0
    playerStats.bounceCount = 0
    playerStats.fireInterval = 0.3
    playerStats.fireRateUpgradeCount = 0
    playerStats.lowEnemyDamageStacks = 0
    playerStats.maxHealth = 100
end

return SystemShooterPlayerProgress
