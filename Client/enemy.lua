local enemy = {
	health = 100
}

function enemy:OnStart()
	print("Enemy OnStart Ran")
end

function enemy:OnUpdate()
	print("Enemy OnUpdate Ran")
end

return enemy