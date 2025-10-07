local enemy = { 
	health = 100}

function enemy:OnStart()
print("enemy OnStart Ran.")
end

function enemy:OnUpdate()
	print("Player OnUpdate Run.")
end

return enemy