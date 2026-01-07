local NavTest = {}

local agentEntity
local terrainEntity
local assets = require("Scripts.Assets")
local enums  = require("Scripts.Enums")
function NavTest:OnStart()
    Window.set_size_centered(960, 600)

    agentEntity = Entity.create_entity()
    Entity.set_global_pos(agentEntity, 200, 300)
    Entity.add_sprite_component(agentEntity, assets.textures.PacMan, 64, 64, 10)
    Entity.add_fysics_component(agentEntity, enums.bodytype.dynamicBody, true) -- dynamic body
    Fysics.add_sprite_collider(agentEntity,false)
    Fysics.set_gravity_scale(agentEntity, 0)

    --------Terrain-----------
    -- terrainEntity = Entity.create_entity()
    -- Entity.set_global_pos(terrainEntity, 500, 500)
    -- Entity.add_sprite_component(terrainEntity, assets.textures.Sand, 64, 64, 10)
    -- Entity.add_fysics_component(terrainEntity, enums.bodytype.staticBody, true)
    -- Fysics.add_sprite_collider(terrainEntity, false)

    


    Nav.load_nav_mef(0, 0, 960 / 100, 600 / 100, 25 / 100)
    Nav.add_obstacle(400, 400, 500, 500)

    agent = Nav.create_agent(agentEntity, 4.0)

    --print("[NavTest] NavMesh + Agent initialised")

    

end

function NavTest:OnUpdate()
    Nav.draw_nav_grid(0, 0, 960, 600, 25)
    local function screen_to_world(x, y)
        local ppm = 4.0 
        return x / ppm, y / ppm
    end
    if agent == nil then
        return
    end
    if Input.get_mouse_button_down(1) then
        local mx, my = Input.get_mouse_x(), Input.get_mouse_y()
        local wx, wy = screen_to_world(mx, my)
        agent:move_to(mx / 100, my / 100)
        --print("[NavTest] Move request:", mx / 100, my / 100)
    end

        if Input.get_key_down(Keys.ionix_space) then
            print("rightMouse")
            local mx, my = Input.get_mouse_x(), Input.get_mouse_y()
                Nav.add_obstacle(mx, my, mx + 64, my + 64)

    end
end

return NavTest
