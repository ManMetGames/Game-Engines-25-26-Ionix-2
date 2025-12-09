local modules = {}

-- Loads a Lua script and stores it as a named module
function LoadModule(name, path)
    local module = dofile(path)
    modules[name] = module
end

-- Retrieve module by name (for cross-module communication)
function GetModule(name)
    return modules[name]
end

-- Load System Modules
LoadModule("CoModusOperandi", "Scripts/Autism.lua")

-- Load all game modules here
--LoadModule("Audio", "Scripts/Audio.lua")
--LoadModule("Enemy", "Scripts/Enemy.lua")
--LoadModule("Player", "Scripts/Player.lua")
--LoadModule("EntityPlayer", "Scripts/EntityPlayer.lua")
--LoadModule("Assets", "Scripts/Assets.lua")
LoadModule("Enums", "Scripts/Enums.lua")
LoadModule("FlappyBird", "Scripts/FlappyBird/FlappyBird.lua")
--LoadModule("JointsTest", "Scripts/JointsTest.lua")
--LoadModule("CorutineTest", "Scripts/TestingCoroutine/CoroutineTest.lua")
--LoadModule("ExampleScript", "Scripts/ExampleScript.lua")
--LoadModule("Ball", "Scripts/Ball.lua")

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
            print("[Lua] Shutting down module:", name)
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

function OnCollisionExit(collision1, collision2)
    for name, module in pairs(modules) do
        if module.OnCollisionExit then
            --print("OnCollisionEnter")
            module:OnCollisionExit(collision1, collision2)
        end
    end
end

function OnTriggerExit(collision1, collision2)
    for name, module in pairs(modules) do
        if module.OnTriggerExit then
            --print("OnCollisionEnter")
            module:OnTriggerExit(collision1, collision2)
        end
    end
end

return {
    OnStart = OnStart,
    OnUpdate = OnUpdate,
    OnFixedUpdate = OnFixedUpdate,
    OnShutdown = OnShutdown,
    GetModule = GetModule,
    OnCollisionEnter = OnCollisionEnter,
    OnTriggerEnter = OnTriggerEnter,
    OnCollisionExit = OnCollisionExit,
    OnTriggerExit = OnTriggerExit
}