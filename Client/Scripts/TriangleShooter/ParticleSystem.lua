local ParticleSystem = {}

local assets = require("Scripts.Assets")
local HIT_PARTICLE_SIZE = 16
local HIT_PARTICLE_LIFETIME = 0.25
local HIT_PARTICLE_COUNT = 8
local HIT_PARTICLE_MIN_SPEED = 240
local HIT_PARTICLE_MAX_SPEED = 520

local RENDER_LAYER = 0

function ParticleSystem.emitHitBurst(x, y)
	for i = 1, HIT_PARTICLE_COUNT do
		local angle = math.random() * 2 * math.pi
		local speed = math.random(HIT_PARTICLE_MIN_SPEED, HIT_PARTICLE_MAX_SPEED)
		local vx = math.cos(angle) * speed
		local vy = math.sin(angle) * speed

		local spawnX = x - HIT_PARTICLE_SIZE / 2
		local spawnY = y - HIT_PARTICLE_SIZE / 2

		Particles.emit(
			assets.textures.balls,
			RENDER_LAYER,
			spawnX,
			spawnY,
			vx,
			vy,
			HIT_PARTICLE_LIFETIME,
			HIT_PARTICLE_SIZE,
			HIT_PARTICLE_SIZE * 0.25
		)
	end
end

function ParticleSystem.update(dt)
	-- No-op: C++ ParticleSystem handles update and render.
end

return ParticleSystem
