local TriangleShooterLevels = {}

local levels = {
    [1] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 1,
        enemyHealth = 75,
        enemyProjectiles = false,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 1280,
        windowHeight = 720,
    },
    [2] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 1,
        enemyHealth = 125,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 1280,
        windowHeight = 720,  
    },
    [3] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 2,
        enemyHealth = 85,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 1280,
        windowHeight = 720,
    },
    [4] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 1,
        enemyHealth = 120,
        enemyProjectiles = true,
        wallPingPong = true,
        coinPerHit = 1,
        windowWidth = 1280,
        windowHeight = 720,
    },
    [5] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 2,
        enemyHealth = 90,
        enemyProjectiles = true,
        wallPingPong = true,
        coinPerHit = 1,
        windowWidth = 1280,
        windowHeight = 720,
    },
}

function TriangleShooterLevels.getLevelConfig(index)
    return levels[index]
end

return TriangleShooterLevels
