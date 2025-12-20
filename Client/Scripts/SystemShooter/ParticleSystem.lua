local ParticleSystem = {}

local assets = require("Scripts.Assets")
local HIT_PARTICLE_SIZE = 12
local HIT_PARTICLE_LIFETIME = 0.225
local HIT_PARTICLE_COUNT = 12
local HIT_PARTICLE_MIN_SPEED = 340
local HIT_PARTICLE_MAX_SPEED = 600

local RENDER_LAYER = 0

function ParticleSystem.emitHitBurst(x, y, r, g, b)
	r = r or 255
	g = g or 255
	b = b or 255
	
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
			HIT_PARTICLE_SIZE * 0.25,
			false,
			r, g, b, 255,
			r, g, b, 0
		)
	end
end

local BEAM_PARTICLE_SIZE = 8
local BEAM_PARTICLE_LIFETIME = 0.18
local BEAM_PARTICLE_RING_COUNT = 8
local BEAM_CYLINDER_RADIUS = 16

function ParticleSystem.emitBeamParticle(x, y, r, g, b, beamDirX, beamDirY)
	r = r or 255
	g = g or 255
	b = b or 255
	beamDirX = beamDirX or 0
	beamDirY = beamDirY or 1
	
	local perpX = -beamDirY
	local perpY = beamDirX
	
	for i = 1, BEAM_PARTICLE_RING_COUNT do
		local angle = (i / BEAM_PARTICLE_RING_COUNT) * 2 * math.pi
		local radiusJitter = BEAM_CYLINDER_RADIUS * (0.85 + math.random() * 0.3)
		
		local offsetX = perpX * math.cos(angle) * radiusJitter
		local offsetY = perpY * math.cos(angle) * radiusJitter
		offsetY = offsetY + math.sin(angle) * radiusJitter * 0.3
		
		local spawnX = x + offsetX - BEAM_PARTICLE_SIZE / 2
		local spawnY = y + offsetY - BEAM_PARTICLE_SIZE / 2
		
		local vx = (math.random() - 0.5) * 20
		local vy = (math.random() - 0.5) * 20

		Particles.emit(
			assets.textures.balls,
			RENDER_LAYER,
			spawnX,
			spawnY,
			vx,
			vy,
			BEAM_PARTICLE_LIFETIME,
			BEAM_PARTICLE_SIZE,
			BEAM_PARTICLE_SIZE * 0.4,
			false,
			r, g, b, 220,
			r, g, b, 100
		)
	end
end

local CHARGE_PARTICLE_SIZE = 4
local CHARGE_PARTICLE_LIFETIME = 0.2
local CHARGE_PARTICLE_COUNT = 4
local CHARGE_SPAWN_RADIUS = 30

function ParticleSystem.emitBeamCharge(fromX, fromY, toX, toY, r, g, b)
	r = r or 255
	g = g or 255
	b = b or 255
	
	local dx = toX - fromX
	local dy = toY - fromY
	local dist = math.sqrt(dx * dx + dy * dy)
	if dist < 1 then return end
	local dirX = dx / dist
	local dirY = dy / dist
	
	for i = 1, CHARGE_PARTICLE_COUNT do
		local angle = math.random() * 2 * math.pi
		local spawnDist = CHARGE_SPAWN_RADIUS * (0.5 + math.random() * 0.5)
		
		local spawnX = fromX + math.cos(angle) * spawnDist - CHARGE_PARTICLE_SIZE / 2
		local spawnY = fromY + math.sin(angle) * spawnDist - CHARGE_PARTICLE_SIZE / 2
		
		local speed = 180 + math.random() * 80
		local vx = -math.cos(angle) * speed + dirX * 40
		local vy = -math.sin(angle) * speed + dirY * 40

		Particles.emit(
			assets.textures.balls,
			RENDER_LAYER,
			spawnX,
			spawnY,
			vx,
			vy,
			CHARGE_PARTICLE_LIFETIME,
			CHARGE_PARTICLE_SIZE,
			CHARGE_PARTICLE_SIZE * 0.3,
			false,
			r, g, b, 200,
			r, g, b, 0
		)
	end
end

local TELEPORT_PARTICLE_SIZE = 8
local TELEPORT_PARTICLE_LIFETIME = 0.3
local TELEPORT_PARTICLE_COUNT = 5

function ParticleSystem.emitTeleportBurst(x, y, r, g, b, inward)
	r = r or 255
	g = g or 255
	b = b or 255
	
	for i = 1, TELEPORT_PARTICLE_COUNT do
		local angle = math.random() * 2 * math.pi
		local baseSpeed = 150 + math.random() * 100
		local startDist = inward and 60 or 0
		
		local spawnX, spawnY, vx, vy
		if inward then
			spawnX = x + math.cos(angle) * startDist - TELEPORT_PARTICLE_SIZE / 2
			spawnY = y + math.sin(angle) * startDist - TELEPORT_PARTICLE_SIZE / 2
			vx = -math.cos(angle) * baseSpeed
			vy = -math.sin(angle) * baseSpeed
		else
			spawnX = x - TELEPORT_PARTICLE_SIZE / 2
			spawnY = y - TELEPORT_PARTICLE_SIZE / 2
			vx = math.cos(angle) * baseSpeed
			vy = math.sin(angle) * baseSpeed
		end

		Particles.emit(
			assets.textures.balls,
			RENDER_LAYER,
			spawnX,
			spawnY,
			vx,
			vy,
			TELEPORT_PARTICLE_LIFETIME,
			TELEPORT_PARTICLE_SIZE,
			TELEPORT_PARTICLE_SIZE * 0.3,
			false,
			r, g, b, 255,
			r, g, b, 0
		)
	end
end

function ParticleSystem.update(dt)
	-- No-op: C++ ParticleSystem handles update and render.
end

return ParticleSystem
