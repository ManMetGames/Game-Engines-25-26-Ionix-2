local TriangleShooterPlayerProgress = {}

local playerLevel = 1
local xp = 0
local xpToNextLevel = 100
local currentShootAbility = "basic"
local currentFireIntervalSeconds = 0.3

local function GetXpForNextLevel(level)
    local n = level - 1
    return math.floor(100 + 60 * n + 20 * n * math.max(n - 1, 0))
end

local function OnLevelUp()
    playerLevel = playerLevel + 1
    xpToNextLevel = GetXpForNextLevel(playerLevel)

    if playerLevel == 2 then
        currentShootAbility = "dual"
        currentFireIntervalSeconds = 0.3
    elseif playerLevel == 3 then
        currentShootAbility = "dual"
        currentFireIntervalSeconds = 0.2
    elseif playerLevel == 4 then
        currentShootAbility = "triple"
        currentFireIntervalSeconds = 0.2
    elseif playerLevel == 5 then
        currentShootAbility = "triple"
        currentFireIntervalSeconds = 0.1
    elseif playerLevel == 6 then
        currentShootAbility = "wide"
        currentFireIntervalSeconds = 0.1
    elseif playerLevel == 7 then
        currentShootAbility = "wide"
        currentFireIntervalSeconds = 0.05
    end

    local minInterval = 0.0015
    currentFireIntervalSeconds = math.max(minInterval, currentFireIntervalSeconds)
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

function TriangleShooterPlayerProgress.getCurrentShootAbility()
    return currentShootAbility
end

function TriangleShooterPlayerProgress.getCurrentFireInterval()
    return currentFireIntervalSeconds
end

return TriangleShooterPlayerProgress
