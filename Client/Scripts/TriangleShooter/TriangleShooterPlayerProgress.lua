local TriangleShooterPlayerProgress = {}

local playerLevel = 1
local xp = 0
local xpToNextLevel = 100

local playerStats = {
    bulletCount = 1,
    pierceCount = 0,
    bounceCount = 0,
    fireInterval = 0.3,
    fireRateUpgradeCount = 0,
    maxHealth = 100,
}

local function GetXpForNextLevel(level)
    local n = level - 1
    local base = 100 + 85 * n + 20 * n * math.max(n - 1, 0)
    
    if level > 10 then
        local lateN = level - 10
        base = base + (lateN * lateN * lateN * 15)
    end
    
    return math.floor(base)
end

local pendingLevelUp = false

local function OnLevelUp()
    playerLevel = playerLevel + 1
    xpToNextLevel = GetXpForNextLevel(playerLevel)
    pendingLevelUp = true
end

function TriangleShooterPlayerProgress.canTakeUpgrade(upgradeType)
    if upgradeType == "fire_rate" then
        return (playerStats.fireRateUpgradeCount or 0) < 3
    end
    return true
end

function TriangleShooterPlayerProgress.applyUpgrade(upgradeType)
    if upgradeType == "bullet" then
        playerStats.bulletCount = playerStats.bulletCount + 1
    elseif upgradeType == "pierce" then
        playerStats.pierceCount = playerStats.pierceCount + 1
    elseif upgradeType == "bounce" then
        playerStats.bounceCount = playerStats.bounceCount + 1
    elseif upgradeType == "fire_rate" then
        local count = playerStats.fireRateUpgradeCount or 0
        if count < 3 then
            local delta = 0.1
            if count == 1 then
                delta = 0.05
            elseif count == 2 then
                delta = 0.025
            end
            playerStats.fireInterval = math.max(0.05, playerStats.fireInterval - delta)
            playerStats.fireRateUpgradeCount = count + 1
        end
    elseif upgradeType == "max_health" then
        playerStats.maxHealth = (playerStats.maxHealth or 100) + 20
    end
end

function TriangleShooterPlayerProgress.addXp(amount)
    xp = xp + amount
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

function TriangleShooterPlayerProgress.getBulletCount()
    return playerStats.bulletCount
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
    
    playerStats.bulletCount = 1
    playerStats.pierceCount = 0
    playerStats.bounceCount = 0
    playerStats.fireInterval = 0.3
    playerStats.fireRateUpgradeCount = 0
    playerStats.maxHealth = 100
end

return TriangleShooterPlayerProgress
