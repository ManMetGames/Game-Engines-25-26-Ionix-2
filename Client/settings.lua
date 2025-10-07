local modules = {}

function LoadModule(name, path)
	local module = dofile(path)
	modules[name] = module
end

LoadModule("player", "player.lua")
LoadModule("enemy", "enemy.lua")

function OnStart()
	for name, module in pairs(modules) do
		if module.OnStart then
			module:OnStart()
		end
	end
end

function OnUpdate()
	for name, module in pairs(modules) do
		if module.OnUpdate then
			module:OnUpdate()
		end
	end
end