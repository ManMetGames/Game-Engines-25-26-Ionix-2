local TriangleShooterPlayerProgress = {}

local playerLevel = 1
local xp = 0
local xpToNextLevel = 100
local currentShootAbility = "basic"

local function GetXpForNextLevel(level)
    local n = level - 1
    return math.floor(100 + 140 * n + 23 * n * math.max(n - 1, 0))
end

local function OnLevelUp()
    playerLevel = playerLevel + 1
    xpToNextLevel = GetXpForNextLevel(playerLevel)

    if playerLevel == 2 then
        currentShootAbility = "dual"
    elseif playerLevel == 3 then
        currentShootAbility = "triple"
    elseif playerLevel == 4 then
        currentShootAbility = "wide"
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

function TriangleShooterPlayerProgress.getCurrentShootAbility()
    return currentShootAbility
end

return TriangleShooterPlayerProgress
