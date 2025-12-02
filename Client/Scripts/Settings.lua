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
LoadModule("Assets", "Scripts/Assets.lua")
LoadModule("Enums", "Scripts/Enums.lua")
LoadModule("FlappyBird", "Scripts/FlappyBird/FlappyBird.lua")
--LoadModule("ExampleScript", "Scripts/ExampleScript.lua")

-- Lifecycle hooks
function OnStart()
    for name, module in pairs(modules) do
        if module.OnStart then
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

function OnFixedUpdate()
    for name, module in pairs(modules) do
        if module.OnFixedUpdate then
            module:OnFixedUpdate()
        end
    end
end

function OnShutdown()
    for name, module in pairs(modules) do
        if module.OnShutdown then
            module:OnShutdown()
        end
    end
end



function OnCollisionEnter(collision1, collision2)
    for name, module in pairs(modules) do
        if module.OnCollisionEnter then
            --print("OnCollisionEnter")
            module:OnCollisionEnter(collision1, collision2)
        end
    end
end

function OnTriggerEnter(collision1, collision2)
    for name, module in pairs(modules) do
        if module.OnTriggerEnter then
            --print("OnCollisionEnter")
            module:OnTriggerEnter(collision1, collision2)
        end
    end
end

-- Retrieve module by name (for cross-module communication)
function GetModule(name)
    return modules[name]
end

-- Load System Modules
LoadModule("Coroutines", "Scripts/Coroutines.lua")

return {
    OnStart = OnStart,
    OnFixedUpdate = OnFixedUpdate,
    OnUpdate = OnUpdate,
    OnShutdown = OnShutdown,
    GetModule = GetModule,
    OnCollisionEnter = OnCollisionEnter,
    OnTriggerEnter = OnTriggerEnter
}