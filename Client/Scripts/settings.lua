local modules = {}

function Loadmodule(name,path)
	local module = dofile(path)
	modules[name] = module
end

Loadmodule("enemy","Scripts/enemy.lua")
Loadmodule("player","Scripts/player.lua")

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

function GetModle(name)
	return modules[name] 
end
