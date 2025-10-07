local modules = {}

function LoadModule(name, path)
	local module = dofile (path)
	modules[name] = module
end

LoadModule("enemy", "enemy.lua")
LoadModule("player", "player.lua")

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

function GetModule(name)
	return modules[name]
end