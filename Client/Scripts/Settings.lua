local modules = {}

-- Loads a Lua script and stores it as a named module
function LoadModule(name, path)
    local module = dofile(path)
    modules[name] = module
end

-- Load all game modules here
LoadModule("Assets", "Scripts/Assets.lua")
--LoadModule("Audio", "Scripts/Audio.lua")
--LoadModule("Enemy", "Scripts/Enemy.lua")
--LoadModule("Player", "Scripts/Player.lua")
--LoadModule("EntityPlayer", "Scripts/EntityPlayer.lua")
--LoadModule("Ball", "Scripts/Ball.lua")
LoadModule("FlappyBird", "Scripts/FlappyBird/FlappyBird.lua")
--LoadModule("ExampleScript", "Scripts/ExampleScript.lua")

-- Lifecycle hooks
function OnStart()
    for name, module in pairs(modules) do
        if module.OnStart then
            print("[Lua] Starting module:", name)
            module:OnStart()
        end
    end
end

-- Called each frame
function OnUpdate()
    for name, module in pairs(modules) do
        if module.OnUpdate then
            module:OnUpdate()
        end
    end
end
funtion OnFixedUpdate()
for name,module in pairs(modules) do
    if modules.OnFixedUpdate then
        module.OnFixedUpdate()
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