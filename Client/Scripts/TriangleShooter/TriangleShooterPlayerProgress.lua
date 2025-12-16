local TriangleShooterPlayerProgress = {}

local playerLevel = 1
local xp = 0
local xpToNextLevel = 100

local playerStats = {
    bulletCount = 1,
    pierceCount = 0,
    bounceCount = 0,
    fireInterval = 0.3,
    maxHealth = 100,
}

local function GetXpForNextLevel(level)
    local n = level - 1
    local base = 100 + 60 * n + 20 * n * math.max(n - 1, 0)
    
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

function TriangleShooterPlayerProgress.applyUpgrade(upgradeType)
    if upgradeType == "bullet" then
        playerStats.bulletCount = playerStats.bulletCount + 1
    elseif upgradeType == "pierce" then
        playerStats.pierceCount = playerStats.pierceCount + 1
    elseif upgradeType == "bounce" then
        playerStats.bounceCount = playerStats.bounceCount + 1
    elseif upgradeType == "fire_rate" then
        playerStats.fireInterval = math.max(0.05, playerStats.fireInterval - 0.05)
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

return TriangleShooterPlayerProgress
