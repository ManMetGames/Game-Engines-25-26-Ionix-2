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
	
	if(Input.get_button_down(0, 0)) then -- instanceId, button
		print("Player 1 input")
	end
	
	if(Input.get_button_down(1, 0)) then -- instanceId, button
		print("Player 2 input")
	end
end

return player
