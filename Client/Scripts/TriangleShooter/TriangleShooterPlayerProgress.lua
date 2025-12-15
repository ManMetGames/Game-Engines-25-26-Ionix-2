local TriangleShooterPlayerProgress = {}

local playerLevel = 1
local xp = 0
local xpToNextLevel = 100

local playerStats = {
    bulletCount = 1,
    pierceCount = 0,
    bounceCount = 0,
    fireInterval = 0.3,
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

local levelUpgrades = {
    [2]  = { type = "bullet" },
    [3]  = { type = "fire_rate", amount = 0.1 },
    [4]  = { type = "bullet" },
    [5]  = { type = "bounce" },
    [6]  = { type = "pierce" },
    [7]  = { type = "bullet" },
    [8]  = { type = "fire_rate", amount = 0.05 },
    [9]  = { type = "bounce" },
    [10] = { type = "pierce" },
    [11] = { type = "bullet" },
}

local function ApplyLevelUpgrade(level)
    local upgrade = levelUpgrades[level]
    if not upgrade then return end

    if upgrade.type == "bullet" then
        playerStats.bulletCount = playerStats.bulletCount + 1
    elseif upgrade.type == "pierce" then
        playerStats.pierceCount = playerStats.pierceCount + 1
    elseif upgrade.type == "bounce" then
        playerStats.bounceCount = playerStats.bounceCount + 1
    elseif upgrade.type == "fire_rate" then
        local reduction = upgrade.amount or 0.05
        playerStats.fireInterval = math.max(0.05, playerStats.fireInterval - reduction)
    end
end

local function OnLevelUp()
    playerLevel = playerLevel + 1
    xpToNextLevel = GetXpForNextLevel(playerLevel)
    ApplyLevelUpgrade(playerLevel)
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
    end
end

function TriangleShooterPlayerProgress.addXp(amount)
    xp = xp + amount
    while xp >= xpToNextLevel do
        xp = xp - xpToNextLevel
        OnLevelUp()
    end
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

function TriangleShooterPlayerProgress.getStats()
    return playerStats
end

return TriangleShooterPlayerProgress
