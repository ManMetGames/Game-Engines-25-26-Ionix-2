local enemy = { 
	health = 100
}

function enemy:OnStart()
	print("Enemy OnStart Run")
end

function enemy:OnUpdate()
	print("Enemy OnUpdate Run")
end

return enemy