local TriangleShooterLevels = {}

local levels = {
    [1] = {
        timeLimitFrames = 60 * 65,
        enemyCount = 1,
        enemyHealth = 75,
        enemyProjectiles = false,
        wallPingPong = false,
        coinPerHit = 1,
    },
    [2] = {
        timeLimitFrames = 60 * 85,
        enemyCount = 1,
        enemyHealth = 125,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
    },
    [3] = {
        timeLimitFrames = 60 * 120,
        enemyCount = 2,
        enemyHealth = 70,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
    },
    [4] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 1,
        enemyHealth = 70,
        enemyProjectiles = true,
        wallPingPong = true,
        coinPerHit = 1,
    },
    [5] = {
        timeLimitFrames = 60 * 140,
        enemyCount = 2,
        enemyHealth = 90,
        enemyProjectiles = true,
        wallPingPong = true,
        coinPerHit = 1,
    },
}

function TriangleShooterLevels.getLevelConfig(index)
    return levels[index]
end

return TriangleShooterLevels
