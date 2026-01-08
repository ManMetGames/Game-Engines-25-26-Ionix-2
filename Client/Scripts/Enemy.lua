local enemy = {health = 100}
	
local enemy
local health
local dmgHealth
local maxHealth = 100 
local x =500
local y = 600
local attackDamage = 5
local isDead
	
function enemy:OnStart()
	
	health = maxHealth
	
	Texture.add_texture("./Assets/ball.png", "enemy")
	
	enemy = Entity.create_entity()
	Entity.set_entity_pos(enemy, x, y) 
	enemySprite = Entity.add_sprite_component(enemy, "ball", 75, 75, 0) 
	Sprite.set_playback_mode(enemySprite, 4) 
	
	
	
end

function enemy:death()

if health = 0() then
	isDead = true
	Entity.set_entity_pos(enemy, 10000, 10000) --send the enemy far off screen to compensate for not knowing how to remove entitys 


function enemy:OnUpdate()

end

return enemy