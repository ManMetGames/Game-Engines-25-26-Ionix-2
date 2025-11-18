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

   --  print(string.format(
       -- "[Controller] Left Stick (X=%.2f, Y=%.2f) | Right Stick (X=%.2f, Y=%.2f) | Triggers (L=%.2f, R=%.2f)",
        -- leftX, leftY, rightX, rightY, leftTrigger, rightTrigger
  --  ))
-- end

return player
