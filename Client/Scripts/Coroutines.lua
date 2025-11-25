local coroutines = {}

function coroutines:OnStart()
end

function coroutines:OnUpdate()
    for i,co in ipairs(coroutines) do
        if coroutine.status(co) == "dead" then
            coroutines[i] = nil
        end

        if co ~= nil then
            coroutine.resume(co)
        end
    end
end

function coroutines:AddCoroutine(co)
    local foundPlace = false

    for i,coroute in ipairs(coroutines) do
        if coroute == nil then
            coroutines[i] = co
            foundPlace = true
            break
        end

        if foundPlace == false then
            coroutines[coroutines.length() + 1] = co
        end
    end
end

function coroutines:RemoveCoroutine(co)
end