local GraphicsTesting = {}
local sprite1

function GraphicsTesting:OnStart()
    sprite1 = Sprite.create_sprite("./Assets/Test.png")

    
end


function GraphicsTesting:OnUpdate()
if  Input.get_key_down(Keys.ionix_w) then
   Sprite.draw_sprite(sprite1, 100, 100, 50, 200)
end
    
end
return GraphicsTesting