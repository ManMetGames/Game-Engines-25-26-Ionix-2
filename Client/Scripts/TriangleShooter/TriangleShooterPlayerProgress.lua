local TriangleShooterPlayerProgress = {}

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
    },
    pierce = {
        statKey      = "pierceCount",
        label        = "upgradetype.pierce",
        desc         = "upgradedesc.pierce",
        minLevel     = 6,
        maxValue     = 2,
        defaultValue = 0,
    },
    bounce = {
        statKey      = "bounceCount",
        label        = "upgradetype.bounce",
        desc         = "upgradedesc.bounce",
        minLevel     = 3,
        maxValue     = 1,
        defaultValue = 0,
    },
    fire_rate = {
        statKey      = "fireRateUpgradeCount",
        label        = "upgradetype.fireinterval",
        desc         = "upgradedesc.fireinterval",
        minLevel     = 1,
        maxValue     = 3,
        defaultValue = 0,
        customApply  = true,
    },
    max_health = {
        statKey      = "maxHealth",
        label        = "upgradetype.maxhp",
        desc         = "upgradedesc.maxhp",
        minLevel     = 4,
        maxValue     = nil,
        defaultValue = 100,
        increment    = 30,
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
    maxHealth = 100,
}

local function GetXpForNextLevel(level)
    local n = level - 1
    local base = 100 + 85 * n + 15 * n * math.max(n - 1, 0)

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
function TriangleShooterPlayerProgress.canTakeUpgrade(upgradeType)
    local cfg = UPGRADE_CONFIG[upgradeType]
    if not cfg then return true end
    if cfg.maxValue == nil then return true end
    
    local currentValue = playerStats[cfg.statKey] or cfg.defaultValue
    return currentValue < cfg.maxValue
end

function TriangleShooterPlayerProgress.applyUpgrade(upgradeType)
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

function TriangleShooterPlayerProgress.getUpgradeConfig()
    return UPGRADE_CONFIG
end

function TriangleShooterPlayerProgress.addXp(amount)
    xp = xp + 100
    while xp >= xpToNextLevel do
        xp = xp - xpToNextLevel
        OnLevelUp()
    end
end

function TriangleShooterPlayerProgress.hasPendingLevelUp()
    return pendingLevelUp
end

function TriangleShooterPlayerProgress.consumePendingLevelUp()
    if pendingLevelUp then
        pendingLevelUp = false
        return true
    end
    return false
end

function TriangleShooterPlayerProgress.getProgress()
    return playerLevel, xp, xpToNextLevel
end

function TriangleShooterPlayerProgress.getFirepower()
    return playerStats.firepower
end

function TriangleShooterPlayerProgress.getPierceCount()
    return playerStats.pierceCount
end

function TriangleShooterPlayerProgress.getBounceCount()
    return playerStats.bounceCount
end

function TriangleShooterPlayerProgress.getCurrentFireInterval()
    return playerStats.fireInterval
end

function TriangleShooterPlayerProgress.getMaxHealth()
    return playerStats.maxHealth or 100
end

function TriangleShooterPlayerProgress.getStats()
    return playerStats
end

function TriangleShooterPlayerProgress.reset()
    playerLevel = 1
    xp = 0
    xpToNextLevel = 100
    pendingLevelUp = false
    
    playerStats.firepower = 1
    playerStats.pierceCount = 0
    playerStats.bounceCount = 0
    playerStats.fireInterval = 0.3
    playerStats.fireRateUpgradeCount = 0
    playerStats.maxHealth = 100
end

return TriangleShooterPlayerProgress
