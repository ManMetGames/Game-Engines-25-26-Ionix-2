local SystemShooterPlayerProgress = {}

local SPREAD_ANGLE_DEG = 10

--=====================================================================
--  [MUSIC] BPM & Beat-Synced Timing
--=====================================================================
local BPM = 133
local BEAT_DURATION = 60 / BPM

local function beatsToSeconds(beats)
    return beats * BEAT_DURATION
end

--=====================================================================
--  UPGRADE CONFIGURATION
--  Central definition for all upgrades: caps, labels, descriptions,
--  minimum level requirements, and apply behavior.
--=====================================================================

--=====================================================================
--  TIME BONUS XP CONFIGURATION
--=====================================================================
local TIME_BONUS_CONFIG = {
    -- Multiplier applied to level XP based on remaining time percentage
    bonusMultiplier = 0.50,
    
    -- If true, only award bonus XP on non-mutated levels
    requireNonMutated = true,
}

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
        minLevel     = 1,
        maxValue     = 2,
        defaultValue = 0,
        weight       = 4,
    },
    bounce = {
        statKey      = "bounceCount",
        label        = "upgradetype.bounce",
        desc         = "upgradedesc.bounce",
        minLevel     = 1,
        maxValue     = 1,
        defaultValue = 0,
        weight       = 3,
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
        -- Delta values for each upgrade level
        levelDeltas  = { 0.1, 0.05, 0.025 },  -- [1] = first upgrade, [2] = second, etc.
    },
    no_witnesses = {
        statKey      = "lowEnemyDamageStacks",
        label        = "upgradetype.no_witnesses",
        desc         = "upgradedesc.no_witnesses",
        minLevel     = 1,
        maxValue     = 2,
        defaultValue = 0,
        damageMultiplier = 2.15,
        fireIntervalDelta = 0.05,
        weight       = 4,
    },
    max_health = {
        statKey      = "maxHealth",
        label        = "upgradetype.maxhp",
        desc         = "upgradedesc.maxhp",
        minLevel     = 10,
        maxValue     = 2,
        defaultValue = 100,
        increment    = 50,
        weight       = 4,
    },
    healing_orb_spawn = {
        statKey      = "healingOrbSpawnUpgrade",
        label        = "upgradetype.healingorbspawn",
        desc         = "upgradedesc.healingorbspawn",
        minLevel     = 6,
        maxValue     = 2,
        defaultValue = 0,
        customApply  = true,
        weight       = 6,
        -- Drop chance values for each upgrade level
        dropChances  = { 0.25, 0.35 },  -- [1] = first upgrade, [2] = second upgrade
    },
    antivirus = {
        statKey      = "antivirusUpgrade",
        label        = "upgradetype.antivirus",
        desc         = "upgradedesc.antivirus",
        minLevel     = 12,
        maxValue     = 1,
        defaultValue = 0,
        customApply  = true,
        weight       = 4,
        cycleDuration = 7.2,  -- seconds (total cycle time)
        activeDuration = 1.8,  -- seconds (active immunity time)
    },
    overhealth = {
        statKey      = "overhealthUpgrade",
        label        = "upgradetype.overhealth",
        desc         = "upgradedesc.overhealth",
        minLevel     = 8,
        maxValue     = 1,
        defaultValue = 0,
        customApply  = true,
        weight       = 5,
        -- Configuration for overhealth system
        decayRate    = 0.65,      -- HP lost per second
        damageRadius = 300,    -- Circle radius in pixels
        -- Lightning zap configuration (replaces continuous DPS)
        zapInterval  = beatsToSeconds(2),    -- Every 2 beats
        zapDamagePercent = 0.075,  -- Percentage of enemy max HP dealt as damage 
        zapMinDamage = 10,        -- Minimum damage dealt by zap regardless of percentage
        maxZapTargets = 3,     -- Maximum enemies to zap at once
        lightningLifetime = 0.35,  -- Lightning VFX duration
        lightningColor = { r = 100, g = 200, b = 255, a = 255 },  -- Electric blue
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
    healingOrbSpawnUpgrade = 0,
    antivirusUpgrade = 0,
    overhealthUpgrade = 0,
}

-- Overhealth tracking (runtime state, not saved)
local currentOverhealth = 0
local overhealthEnabled = false
local overhealthZapTimer = 0  -- Timer for periodic lightning zaps

local timeoutCount = 0

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

    -- Late-game slowdown: mixed quadratic/cubic formula starting at level 10
    -- Keeps early levels similar, but ramps up more aggressively at higher levels
    local lateBumpStart = 10
    if level > lateBumpStart then
        local delta = level - lateBumpStart
        base = base + 40 * delta * delta + 10 * delta * delta * delta
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
    
    local currentValue = playerStats[cfg.statKey]
    if currentValue == nil then currentValue = cfg.defaultValue end

    -- If this upgrade uses an increment (e.g. maxHealth increases by X per upgrade),
    -- compute how many times it has been applied and compare against maxValue.
    if cfg.increment then
        local default = cfg.defaultValue or 0
        local inc = cfg.increment or 1
        local applied = math.floor((currentValue - default) / inc)
        if applied < 0 then applied = 0 end
        return applied < cfg.maxValue
    end

    return currentValue < cfg.maxValue
end

function SystemShooterPlayerProgress.applyUpgrade(upgradeType)
    local cfg = UPGRADE_CONFIG[upgradeType]
    if not cfg then return end
    
    if cfg.customApply then
        if upgradeType == "fire_rate" then
            local count = playerStats.fireRateUpgradeCount or 0
            if count < cfg.maxValue then
                -- Get delta from config based on current count (next upgrade level)
                local delta = cfg.levelDeltas and cfg.levelDeltas[count + 1] or 0.1
                playerStats.fireInterval = math.max(0.05, playerStats.fireInterval - delta)
                playerStats.fireRateUpgradeCount = count + 1
            end
        elseif upgradeType == "healing_orb_spawn" then
            local count = playerStats.healingOrbSpawnUpgrade or 0
            if count < cfg.maxValue then
                playerStats.healingOrbSpawnUpgrade = count + 1
                -- Update the pickup module's spawn chance using config value
                local SystemShooterPickups = require("Scripts.SystemShooter.SystemShooterPickups")
                local dropChance = cfg.dropChances and cfg.dropChances[count + 1] or 1.0
                SystemShooterPickups.CONFIG.HEALING_ORB_DROP_CHANCE = dropChance
            end
        elseif upgradeType == "antivirus" then
            local count = playerStats.antivirusUpgrade or 0
            if count < cfg.maxValue then
                playerStats.antivirusUpgrade = count + 1
                -- Enable antivirus system in player module with config values
                local SystemShooterPlayer = require("Scripts.SystemShooter.SystemShooterPlayer")
                SystemShooterPlayer.enableAntivirus(cfg.cycleDuration, cfg.activeDuration)
            end
        elseif upgradeType == "overhealth" then
            local count = playerStats.overhealthUpgrade or 0
            if count < cfg.maxValue then
                playerStats.overhealthUpgrade = count + 1
                overhealthEnabled = true
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

function SystemShooterPlayerProgress.getTimeBonusConfig()
    return TIME_BONUS_CONFIG
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
    local SystemShooterDifficulty = require("Scripts.SystemShooter.SystemShooterDifficulty")
    return SystemShooterDifficulty.getPlayerTimeoutCount()
end

function SystemShooterPlayerProgress.incrementTimeoutCount()
    timeoutCount = timeoutCount + 1
    return timeoutCount
end

--=====================================================================
--  OVERHEALTH SYSTEM
--=====================================================================
function SystemShooterPlayerProgress.isOverhealthEnabled()
    return overhealthEnabled
end

function SystemShooterPlayerProgress.getOverhealth()
    return currentOverhealth
end

function SystemShooterPlayerProgress.setOverhealth(value)
    if not overhealthEnabled then
        currentOverhealth = 0
        return
    end
    local maxHealth = playerStats.maxHealth or 100
    currentOverhealth = math.max(0, math.min(value, maxHealth))
end

function SystemShooterPlayerProgress.addOverhealth(amount)
    if not overhealthEnabled then return end
    local maxHealth = playerStats.maxHealth or 100
    currentOverhealth = math.min(currentOverhealth + amount, maxHealth)
end

function SystemShooterPlayerProgress.getOverhealthConfig()
    return UPGRADE_CONFIG.overhealth
end

-- Called each frame to decay overhealth and return zap info
-- Returns: isActive (bool), damageRadius (float), shouldZap (bool), zapDamage (float), maxTargets (int)
function SystemShooterPlayerProgress.updateOverhealth(dt)
    if not overhealthEnabled or currentOverhealth <= 0 then
        overhealthZapTimer = 0
        return false, 0, false, 0, 0
    end
    
    local cfg = UPGRADE_CONFIG.overhealth
    
    -- Decay overhealth over time
    local decayAmount = (cfg.decayRate or 3) * dt
    currentOverhealth = math.max(0, currentOverhealth - decayAmount)
    
    -- If still active, check zap timer
    if currentOverhealth > 0 then
        overhealthZapTimer = overhealthZapTimer + dt
        local zapInterval = cfg.zapInterval or 0.5
        
        if overhealthZapTimer >= zapInterval then
            overhealthZapTimer = overhealthZapTimer - zapInterval
            -- Time to zap!
            return true, cfg.damageRadius or 200, true, cfg.zapDamage or 25, cfg.maxZapTargets or 2
        else
            -- Active but not zapping this frame
            return true, cfg.damageRadius or 200, false, 0, 0
        end
    end
    
    overhealthZapTimer = 0
    return false, 0, false, 0, 0
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
    playerStats.healingOrbSpawnUpgrade = 0
    playerStats.antivirusUpgrade = 0
    playerStats.overhealthUpgrade = 0
    
    -- Reset overhealth
    currentOverhealth = 0
    overhealthEnabled = false
    overhealthZapTimer = 0
    
    -- Reset healing orb spawn chance to base value from config
    local SystemShooterPickups = require("Scripts.SystemShooter.SystemShooterPickups")
    SystemShooterPickups.CONFIG.HEALING_ORB_DROP_CHANCE = SystemShooterPickups.CONFIG.BASE_HEALING_ORB_DROP_CHANCE
    
    -- Reset antivirus
    local SystemShooterPlayer = require("Scripts.SystemShooter.SystemShooterPlayer")
    SystemShooterPlayer.disableAntivirus()
end

return SystemShooterPlayerProgress
