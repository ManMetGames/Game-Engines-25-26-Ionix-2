local player = {}
local keyCode = SDL_SCANCODE_SPACE

function player:OnStart()
end


function player:OnUpdate()
	local enemy = GetModule("enemy")
	if Input.get_key_down(Keys.ionix_equals) then
		print(Mafs.clamp(25,10,20))
                print(Mafs.abs(25))
                print(Mafs.min(25,10))
                print(Mafs.max(25,10))
                print(Mafs.Round(25))
	end
end
return player