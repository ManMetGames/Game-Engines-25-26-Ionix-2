local player = {}
function player:OnStart()
print("player OnStart")
end

function player:OnUpdate()
    local enemy = GetModule("enemy");
    if enemy then
        print("Enemy Health: " .. enemy.health)
    end
    print("player onUpdate")
end

return player