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
    Fysics.add_sprite_collider(hitbox, true, 1) -- isTrigger
    
    --Store owner and timer
    hitbox.owner = owner
    hitbox.timer = duration or 0.2
    
    --Tracking Hitbox
    table.insert(hitboxes, hitbox)
    
    return hitbox
end

function Hitbox.update()
    for i = #hitboxes, 1, -1 do
        local hb = hitboxes[i]
        if hb.timer then
            hb.timer = hb.timer - Mafs.delta_time()
            
            if hb.timer <= 0 then
                table.remove(hitboxes, i)
            end
        end
    end
end

return Hitbox