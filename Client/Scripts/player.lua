local player = {}

function player:OnStart()
    print("[Lua] Controller Debug Initialized.")
end

function player:OnUpdate()
    local leftX = Input.get_left_stick_x()
    local leftY = Input.get_left_stick_y()
    local rightX = Input.get_right_stick_x()
    local rightY = Input.get_right_stick_y()

    local leftTrigger = Input.get_left_trigger()
    local rightTrigger = Input.get_right_trigger()
	
	if(Input.get_button_down(0, Buttons.ionix_a)) then -- instanceId, button
		print("Player 1 down input")
	end

    if(Input.get_button_held(0, 0)) then -- instanceId, button
		print("Player 1 down input held")
	end

    if(Input.get_button_up(0, 0)) then -- instanceId, button
		print("Player 1 up input")
	end
	
	if(Input.get_button_down(1, 0)) then -- instanceId, button
		print("Player 2 down input")
	end

    if(Input.get_button_held(1, 0)) then -- instanceId, button
		print("Player 2 down input held")
	end

    if(Input.get_button_up(1, 0)) then -- instanceId, button
		print("Player 2 up input")
	end

    print("Left X:", Input.get_left_stick_x(0));
    print("Left Y:", Input.get_left_stick_y(0));
    print("Right X:", Input.get_right_stick_x(0));
    print("Right y:", Input.get_right_stick_y(0));
end

return player
