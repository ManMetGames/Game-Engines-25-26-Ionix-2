local TriangleShooterLevels = {}

local levels = {
    [1] = {
        timeLimitFrames = 60 * 15,
        enemyCount = 1,
        enemyHealth = 50,
        enemyProjectiles = false,
        wallPingPong = false,
        coinPerHit = 1,
    },
    [2] = {
        timeLimitFrames = 60 * 20,
        enemyCount = 1,
        enemyHealth = 60,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
    },
    [3] = {
        timeLimitFrames = 60 * 20,
        enemyCount = 2,
        enemyHealth = 70,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
    },
    [4] = {
        timeLimitFrames = 60 * 25,
        enemyCount = 1,
        enemyHealth = 80,
        enemyProjectiles = true,
        wallPingPong = true,
        coinPerHit = 1,
    },
    [5] = {
        timeLimitFrames = 60 * 25,
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
