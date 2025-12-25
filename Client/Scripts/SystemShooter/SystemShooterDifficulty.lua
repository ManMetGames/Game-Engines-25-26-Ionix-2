local SystemShooterDifficulty = {}

--=====================================================================
--  DIFFICULTY CONFIGURATION
--  Central definition for all difficulty-related settings.
--  Medium difficulty represents the baseline (current game balance).
--=====================================================================

local DIFFICULTY_PRESETS = {
    easy = {
        name = "Easy",
        -- Enemy projectile settings
        enemyProjectileSpeed = 260,          
        enemyProjectileDamage = 3,          
        
        -- Player survivability
        playerInvulnerabilityTime = 0.75,   
        
        -- Bonus XP percentage (applied to level XP based on time remaining)
        bonusXpPercent = 0.625,               
        
        -- Bounce enemy base speed (pixels per second)
        bounceEnemyBaseSpeed = 300,
        
        -- Number of times player can timeout
        playerTimeoutCount = 5,
    },
    
    medium = {
        name = "Medium",
        -- Enemy projectile settings (baseline values)
        enemyProjectileSpeed = 325,
        enemyProjectileDamage = 4,
        
        -- Player survivability
        playerInvulnerabilityTime = 0.5,
        
        -- Bonus XP percentage (baseline)
        bonusXpPercent = 0.45,  
        
        -- Bounce enemy base speed (pixels per second)
        bounceEnemyBaseSpeed = 375,
        
        -- Number of times player can timeout
        playerTimeoutCount = 4,
    },
    
    hard = {
        name = "Hard",
        -- Enemy projectile settings
        enemyProjectileSpeed = 425, 
        enemyProjectileDamage = 6.5,   
        
        -- Player survivability
        playerInvulnerabilityTime = 0.25,    
        
        -- Bonus XP percentage (reduced for hard mode)
        bonusXpPercent = 0.35,     
        
        -- Bounce enemy base speed (pixels per second)
        bounceEnemyBaseSpeed = 425,
        
        -- Number of times player can timeout
        playerTimeoutCount = 3,
    },
}

--=====================================================================
--  STATE
--=====================================================================
local currentDifficulty = "medium"

--=====================================================================
--  API
--=====================================================================

--- Set the current difficulty
---@param difficulty string "easy", "medium", or "hard"
function SystemShooterDifficulty.setDifficulty(difficulty)
    if DIFFICULTY_PRESETS[difficulty] then
        currentDifficulty = difficulty
    else
        print("[Difficulty] Warning: Unknown difficulty '" .. tostring(difficulty) .. "', defaulting to medium")
        currentDifficulty = "medium"
    end
end

--- Get the current difficulty key
---@return string
function SystemShooterDifficulty.getDifficulty()
    return currentDifficulty
end

--- Get the current difficulty config table
---@return table
function SystemShooterDifficulty.getConfig()
    return DIFFICULTY_PRESETS[currentDifficulty]
end

--- Get all difficulty presets (for UI or debugging)
---@return table
function SystemShooterDifficulty.getAllPresets()
    return DIFFICULTY_PRESETS
end

--=====================================================================
--  CONVENIENCE GETTERS
--  Use these to fetch individual values without needing to access
--  the full config table.
--=====================================================================

function SystemShooterDifficulty.getEnemyProjectileSpeed()
    return DIFFICULTY_PRESETS[currentDifficulty].enemyProjectileSpeed
end

function SystemShooterDifficulty.getEnemyProjectileDamage()
    return DIFFICULTY_PRESETS[currentDifficulty].enemyProjectileDamage
end

function SystemShooterDifficulty.getPlayerInvulnerabilityTime()
    return DIFFICULTY_PRESETS[currentDifficulty].playerInvulnerabilityTime
end

function SystemShooterDifficulty.getBonusXpPercent()
    return DIFFICULTY_PRESETS[currentDifficulty].bonusXpPercent
end

function SystemShooterDifficulty.getBounceEnemyBaseSpeed()
    return DIFFICULTY_PRESETS[currentDifficulty].bounceEnemyBaseSpeed
end

function SystemShooterDifficulty.getPlayerTimeoutCount()
    return DIFFICULTY_PRESETS[currentDifficulty].playerTimeoutCount
end

--=====================================================================
--  RESET
--=====================================================================
function SystemShooterDifficulty.reset()
    currentDifficulty = "medium"
end

return SystemShooterDifficulty
