local player = {}

function player:OnStart()
end

function player:OnUpdate()
    local leftX = Input.get_left_stick_x()
    local leftY = Input.get_left_stick_y()
    local rightX = Input.get_right_stick_x()
    local rightY = Input.get_right_stick_y()

    local leftTrigger = Input.get_left_trigger()
    local rightTrigger = Input.get_right_trigger()
end

return player
