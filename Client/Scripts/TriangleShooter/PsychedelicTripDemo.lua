local ParticleSystem = require("Scripts.TriangleShooter.ParticleSystem")

local PsychedelicTripDemo = {}

local t = 0

local bpm = 133
local secondsPerBeat = 60.0 / bpm

local SPIRAL_EMITTER_COUNT = 12
local SPIRAL_INTERVAL = 0.25
local spiralTimer = 0

local RING_POINT_COUNT = 24
local RING_INTERVAL = 1.0
local ringTimer = 0

local WAVE_POINT_COUNT = 14
local WAVE_INTERVAL = 0.25
local waveTimer = 0

local function emitSpiral(centerX, centerY, t)
    local baseRadius = 80
    local breathingAmplitude = 80
    local breathingSpeed = 0.7
    local radius = baseRadius + math.sin(t * breathingSpeed) * breathingAmplitude

    local spiralOffsetSpeed = 20
    local spiralOffset = t * spiralOffsetSpeed

    for i = 1, SPIRAL_EMITTER_COUNT do
        local angle = t * 0.5 + (i / SPIRAL_EMITTER_COUNT) * 2 * math.pi
        local effectiveRadius = radius + spiralOffset
        local x = centerX + math.cos(angle) * effectiveRadius
        local y = centerY + math.sin(angle) * effectiveRadius
        ParticleSystem.emitHitBurst(x, y)
    end
end

local function emitRing(centerX, centerY, t)
    local baseRadius = 140
    local pulseAmplitude = 60
    local pulseSpeed = 1.3
    local radius = baseRadius + math.sin(t * pulseSpeed) * pulseAmplitude

    for i = 1, RING_POINT_COUNT do
        local angle = (i / RING_POINT_COUNT) * 2 * math.pi
        local x = centerX + math.cos(angle) * radius
        local y = centerY + math.sin(angle) * radius
        ParticleSystem.emitHitBurst(x, y)
    end
end

local function emitWaves(w, h, t)
    local centerY = h * 0.5
    local centerX = w * 0.5

    local waveHeight = h * 0.25
    local waveSpeed = 1.7

    for i = 0, WAVE_POINT_COUNT - 1 do
        local u = i / (WAVE_POINT_COUNT - 1)
        local x = u * w
        local phase = t * waveSpeed + u * 4 * math.pi
        local y = centerY + math.sin(phase) * waveHeight
        ParticleSystem.emitHitBurst(x, y)
    end

    for i = 0, WAVE_POINT_COUNT - 1 do
        local u = i / (WAVE_POINT_COUNT - 1)
        local y = u * h
        local phase = t * waveSpeed + u * 4 * math.pi
        local x = centerX + math.cos(phase) * (w * 0.25)
        ParticleSystem.emitHitBurst(x, y)
    end
end

function PsychedelicTripDemo.update(dt, screenW, screenH)
    t = t + dt

    local beatDelta = dt / secondsPerBeat
    spiralTimer = spiralTimer + beatDelta
    ringTimer = ringTimer + beatDelta
    waveTimer = waveTimer + beatDelta

    local w = screenW or 1280
    local h = screenH or 720
    local centerX = w * 0.5
    local centerY = h * 0.5

    if spiralTimer >= SPIRAL_INTERVAL then
        spiralTimer = spiralTimer - SPIRAL_INTERVAL
        emitSpiral(centerX, centerY, t)
    end

    if ringTimer >= RING_INTERVAL then
        ringTimer = ringTimer - RING_INTERVAL
        emitRing(centerX, centerY, t)
    end

    if waveTimer >= WAVE_INTERVAL then
        waveTimer = waveTimer - WAVE_INTERVAL
        emitWaves(w, h, t)
    end
end

return PsychedelicTripDemo
