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
    terrainEntity = Entity.create_entity()
    Entity.set_global_pos(terrainEntity, 500, 500)
    Entity.add_sprite_component(terrainEntity, assets.textures.Sand, 64, 64, 10)
    Entity.add_fysics_component(terrainEntity, enums.bodytype.staticBody, true)
    Fysics.add_sprite_collider(terrainEntity, false)

    
    Nav.add_obstacle(terrainEntity)

    Nav.load_nav_mef(0, 0, 1920/4, 1080/4, 25/4)
    agent = Nav.create_agent(agentEntity, 4.0)

    print("[NavTest] NavMesh + Agent initialised")

    

end

function NavTest:OnUpdate()
    Nav.draw_nav_grid(0, 0, 1920, 1080, 25)
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
        print("[NavTest] Move request:", wx, wy)
    end
end

return NavTest
