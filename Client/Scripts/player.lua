local player = {}
local keyCode = SDL_SCANCODE_SPACE

function player:OnStart()
end


function player:OnUpdate()
	local enemy = GetModule("enemy")
	if Input.get_key_down(Keys.ionix_equals) then
		print(Mafs.clamp(25,10,20))
	end

	if Input.get_key_up(44) then
        print("key up")
    end

	if Input.get_key_held(44) then
        print("key held")
    end


end
return player