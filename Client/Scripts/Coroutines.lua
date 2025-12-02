local coroutines = {}
local methods = {}

function coroutines:OnStart()
    --print("Start")
end

function coroutines:OnUpdate()
    for i,co in ipairs(methods) do
        if coroutine.status(methods[i][1]) == "dead" then
            methods[i] = {}
        end

        if methods[i][1] ~= nil and methods[i][3] - os.time() <= 0 then
            coroutine.resume(methods[i][1])
            methods[i][3] = methods[i][2] + methods[i][3]
        end

    end
end

function coroutines:AddCoroutine(co, time)
    local foundPlace = false
    if time == nil then
        time = 0
    end

    for i,coroute in ipairs(methods) do
        if coroute == nil then
            methods[i] = {}
            methods[i][1] = co
            methods[i][2] = time
            methods[i][3] = os.time()
            foundPlace = true
            break
        end
    end

    if foundPlace == false then
        local index = # methods + 1
        methods[index] = {}
        methods[index][1] = co
        methods[index][2] = time
        methods[index][3] = os.time()
    end
end

function coroutines:RemoveCoroutine(co)
end

function coroutines:Yield(co)
    coroutine.yield(co)
end

function coroutines:YieldTime(co)
end

return coroutines