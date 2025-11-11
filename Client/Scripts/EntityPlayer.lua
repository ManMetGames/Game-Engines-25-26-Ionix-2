local EntityPlayer = {}
local floor_entity
local spawn_height = 600

function EntityPlayer:OnStart()
    floor_entity = Entity.create_entity()
    Texture.add_texture("./Assets/square.png", "aur")
    Entity.add_sprite_component(floor_entity, "aur", 5, 32, 32, 1, 1)
    Entity.set_entity_pos(floor_entity, 640, 700)
    Fysics.add_rigidbody_component(floor_entity, "floor", 0)
    Fysics.add_collider(floor_entity, 0, 0.1, false)
end

function EntityPlayer:OnUpdate()
    if Input.get_key_down(Keys.ionix_d) then
        local box = Entity.create_entity()
        Entity.add_sprite_component(box, "aur", 5, 32, 32, 1, 1)

        Entity.set_entity_pos(box, 640, spawn_height)

        Fysics.add_rigidbody_component(box, "box" .. tostring(box), 2)
        Fysics.add_collider(box, 0, 0.1, false)

        local random_x = (math.random() - 0.5) * 2
        local upward_y = -2 - math.random() * 2

        Fysics.add_force(box, random_x, upward_y, 0, 0)

        print("Spawned box with force: " .. random_x .. ", " .. upward_y)
    end
end
return EntityPlayer