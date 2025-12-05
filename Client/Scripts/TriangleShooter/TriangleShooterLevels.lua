local TriangleShooterLevels = {}

local levels = {
    [1] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 1,
        enemyHealth = 75,
        enemyProjectiles = false,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 1060,
        windowHeight = 640,
    },
    [2] = {
        timeLimitFrames = 60 * 60,
        enemyCount = 1,
        enemyHealth = 125,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 800,
        windowHeight = 400,  
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
        timeLimitFrames = 60 * 30,
        enemyCount = 1,
        enemyHealth = 160,
        enemyProjectiles = true,
        wallPingPong = true,
        coinPerHit = 1,
        windowWidth = 1920,
        windowHeight = 1080,
    },
    [5] = {
        timeLimitFrames = 60 * 50,
        enemyCount = 3,
        enemyHealth = 60,
        enemyProjectiles = true,
        wallPingPong = false,
        coinPerHit = 1,
        windowWidth = 1024,
        windowHeight = 768,
    },
    [6] = {
        timeLimitFrames = 60 * 35,
        enemyCount = 2,
        enemyHealth = 90,
        enemyProjectiles = true,
        wallPingPong = true,
        coinPerHit = 1,
        windowWidth = 1000,
        windowHeight = 600,
    },
}

function TriangleShooterLevels.getLevelConfig(index)
    return levels[index]
end

return TriangleShooterLevels
