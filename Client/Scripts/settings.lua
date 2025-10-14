
local modules = {}

-- Loads a Lua script and stores it as a named module
function LoadModule(name, path)
    local module = dofile(path)
    modules[name] = module
end

-- Load all game modules here
LoadModule("audio", "Scripts/audio.lua")
LoadModule("enemy", "Scripts/enemy.lua")
LoadModule("player", "Scripts/player.lua")

-- Lifecycle hooks
function OnStart()
    for name, module in pairs(modules) do
        if module.OnStart then
            print("[Lua] Starting module:", name)
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

function OnShutdown()
    for name, module in pairs(modules) do
        if module.OnShutdown then
            print("[Lua] Shutting down module:", name)
            module:OnShutdown()
        end
    end
end

-- Retrieve module by name (for cross-module communication)
function GetModule(name)
    return modules[name]
end

return {
    OnStart = OnStart,
    OnUpdate = OnUpdate,
    OnShutdown = OnShutdown,
    GetModule = GetModule
}