local enemy = {health = 100}
	
function enemy:OnStart()
	print("enemy start ran.")
end


function enemy:OnUpdate()
	print("enemy update ran.")
end
return enemy