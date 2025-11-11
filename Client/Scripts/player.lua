local player = {}
local keyCode = SDL_SCANCODE_SPACE

function player:OnStart()
end


function player:OnUpdate()
	local enemy = GetModule("enemy")
	print ("left stick is X: " + Input.get_left_stick_x() + " Y: " + Input.get_left_stick_Y())

	if Input.get_key_down(44) then
		print(Input.get_mouse_x())
	end

	if Input.get_key_up(44) then
        print(Input.get_mouse_x())
    end

	if Input.get_key_held(44) then
        print(Input.get_mouse_x())
    end


end
return player