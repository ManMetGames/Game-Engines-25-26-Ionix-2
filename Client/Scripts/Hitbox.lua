local Hitbox = {}
local hitboxes = {} --Tracks active hitBoxes

-- spawn a hitbox
function Hitbox.spawn(owner, x, y, width, height, duration, assets, enums)
    local hitbox = Entity.create_entity()
    
   --Position
    Entity.set_global_pos(hitbox, x, y)
    
    --Visual aid (can delete after testing)
    local hbSprite = Entity.add_sprite_component(hitbox, assets.textures.Debug, width, height, 1)
    Sprite.set_columns(hbSprite, 1)
    
    --Physics
    Entity.add_fysics_component(hitbox, enums.bodytype.kinematicBody, false)
    Fysics.add_sprite_collider(hitbox, false, 1)
    
    --Store owner and timer
    hitbox.owner = owner
    hitbox.timer = duration or 0.2
    
    --Tracking Hitbox
    table.insert(hitboxes, hitbox)
    
    return hitbox
end

return Hitbox