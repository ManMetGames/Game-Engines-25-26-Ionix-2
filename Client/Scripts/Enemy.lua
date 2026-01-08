local enemy = {health = 100}
	
local enemy
local maxHealth = 100 
local x =500
local y = 600
	
	
function enemy:OnStart()
	
	Texture.add_texture("./Assets/ball.png", "enemy")
	
	enemy = Entity.create_entity()
	Entity.set_entity_pos(enemy, x, y) 
	enemySprite = Entity.add_sprite_component(enemy, "ball", 75, 75, 0) 
	
end

function enemy:OnUpdate()

end

return enemy