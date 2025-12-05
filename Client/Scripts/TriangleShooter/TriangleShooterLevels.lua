local TriangleShooterLevels = {}

local levels = {
    [1] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 1,
        enemyHealth = 75,
        enemyProjectiles = false,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 800,
        windowHeight = 400,
    },
    [2] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 1,
        enemyHealth = 125,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 1026,
        windowHeight = 640,  
    },
    [3] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 2,
        enemyHealth = 85,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 960,
        windowHeight = 460,
    },
    [4] = {
        timeLimitFrames = 60 * 30,
        enemyCount = 1,
        enemyHealth = 160,
        enemyProjectiles = true,
        wallPingPong = true,
        coinPerHit = 1,
        windowWidth = 1280,
        windowHeight = 670,
    },
    [5] = {
        timeLimitFrames = 60 * 50,
        enemyCount = 3,
        enemyHealth = 60,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 1026,
        windowHeight = 640,
    },
    [6] = {
        timeLimitFrames = 60 * 35,
        enemyCount = 2,
        enemyHealth = 90,
        enemyProjectiles = true,
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
