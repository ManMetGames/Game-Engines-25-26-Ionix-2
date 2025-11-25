local coroutines = {}
local methods = {}

function coroutines:OnStart()
    --print("Start")
end

function coroutines:OnUpdate()
    for i,co in ipairs(methods) do
        if coroutine.status(co) == "dead" then
            methods[i] = nil
        end

        if co ~= nil then
            coroutine.resume(co)
        end
    end
end

function coroutines:AddCoroutine(co)
    local foundPlace = false

    for i,coroute in ipairs(methods) do
        if coroute == nil then
            methods[i] = co
            foundPlace = true
            break
        end
    end

    if foundPlace == false then
        methods[# methods + 1] = co
    end
end

function coroutines:RemoveCoroutine(co)
end

return coroutines