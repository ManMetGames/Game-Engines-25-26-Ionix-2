local enemy = { 
    health = 100
}

   
function enemy:OnStart()
print("enemy OnStart")
end

function enemy:OnUpdate()
print("enemy onUpdate")
end

return enemy