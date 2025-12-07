local TriangleShooterLevels = {}

local levels = {
    [1] = {
        timeLimitSeconds = 20,
        enemyCount = 1,
        enemyHealth = 75,
        enemyProjectiles = false,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 800,
        windowHeight = 400,
    },
    [2] = {
        timeLimitSeconds = 20,
        enemyCount = 1,
        enemyHealth = 125,
        enemyProjectiles = true,
        enemyShootIntervalSeconds = 0.5,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 1026,
        windowHeight = 640,  
    },
    [3] = {
        timeLimitSeconds = 20,
        enemyCount = 2,
        enemyHealth = 85,
        enemyProjectiles = true,
        enemyShootIntervalSeconds = 0.75,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 960,
        windowHeight = 460,
    },
    [4] = {
        timeLimitSeconds = 15,
        enemyCount = 1,
        enemyHealth = 130,
        enemyProjectiles = true,
        wallPingPong = true,
        enemyShootIntervalSeconds = 0.25,
        coinPerHit = 1,
        windowWidth = 1280,
        windowHeight = 670,
    },
    [5] = {
        timeLimitSeconds = 25,
        enemyCount = 3,
        enemyHealth = 60,
        enemyProjectiles = true,
        enemyShootIntervalSeconds = 0.75,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 1026,
        windowHeight = 640,
    },
    [6] = {
        timeLimitSeconds = 17,
        enemyCount = 2,
        enemyHealth = 90,
        enemyProjectiles = true,
        enemyShootIntervalSeconds = 0.5,
        wallPingPong = true,
        coinPerHit = 1,
        windowWidth = 1026,
        windowHeight = 640,
    },
}

function TriangleShooterLevels.getLevelConfig(index)
    return levels[index]
end

return TriangleShooterLevels
