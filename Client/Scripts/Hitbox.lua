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
    
    --Store data 
    local hb = {
        entity = hitbox, 
        owner = owner, 
        timer = duration or 0.2,
        hasHit = false,
        active = true,
        dirX = 1,
        dirY = 0
    }
    
    --Tracking Hitbox
    table.insert(hitboxes, hb)
    
    return hb
end

function Hitbox.update(player1, player2)
    for i = #hitboxes, 1, -1 do
        local hb = hitboxes[i]
        
        --Check for hit 
        if hb.active and not hb.hasHit then 
            if hb.owner == player1 and Fysics.col(hb.entity, player2) then 
                Hitbox.onHit(hb, player2)
            elseif hb.owner == player2 and Fysics.col(hb.entity, player1) then 
                Hitbox.onHit(hb, player1)
            end 
        end    
        
        --Timer 
        hb.timer = hb.timer - Mafs.delta_time()
        if hb.timer <= 0 then 
            table.remove(hitboxes, i)
            
        end
    end
end

--What happens on Hit 
function Hitbox.onHit(hb, target)
    hb.hasHit = true
    
    --Dmg
    target.health = (target.health or 20) - 1
    print("Hit! playerhealth: ", target.health)
    
    --Knockback when hit 
    local kb = 8
    local dirX = hb.dirX or 1
    local dirY = hb.dirY or 0
    
    local impulse = Mafs.vector2(dirX * kb, dirY * kb)
    Fysics.add_impulse_to_center_v(target, impulse)
end 

return Hitbox