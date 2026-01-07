local NavTest = {}

local agentEntity
local terrainEntity
local showNavMesh
local assets = require("Scripts.Assets")
local enums  = require("Scripts.Enums")
function NavTest:OnStart()
    Window.set_size_centered(960, 600)
    showNavMesh = false

    agentEntity = Entity.create_entity()
    Entity.set_global_pos(agentEntity, 200, 300)
    Entity.add_sprite_component(agentEntity, assets.textures.FlappyBird, 32, 32, 10)
    Entity.add_fysics_component(agentEntity, enums.bodytype.dynamicBody, true) -- dynamic body
    Fysics.add_sprite_collider(agentEntity,false)
    Fysics.set_gravity_scale(agentEntity, 0)

    --------Terrain-----------
    -- terrainEntity = Entity.create_entity()
    -- Entity.set_global_pos(terrainEntity, 500, 500)
    -- Entity.add_sprite_component(terrainEntity, assets.textures.Sand, 64, 64, 10)
    -- Entity.add_fysics_component(terrainEntity, enums.bodytype.staticBody, true)
    -- Fysics.add_sprite_collider(terrainEntity, false)

    

    Nav.load_nav_mef(0, 0, 960 / 100, 600 / 100, 10 / 100)
    agent = Nav.create_agent(agentEntity, 4.0)
    --Nav.add_obstacle(400, 400, 500, 500)

    print("[NavTest] NavMesh + Agent initialised")

    

end

function NavTest:OnUpdate()

    if showNavMesh == true then
        Nav.draw_nav_grid(0, 0, 960, 600, 10)
    end

    if Input.get_key_down(Keys.ionix_e) then
    showNavMesh = not showNavMesh
end
    
    if agent == nil then
        return
    end
    if Input.get_mouse_button_down(1) then
        local mx, my = Input.get_mouse_x(), Input.get_mouse_y()
        Nav.agent_move_to(agent, mx / 100, my / 100)
        print("[NavTest] Move request:", mx / 100, my / 100)
    end

        if Input.get_key_down(Keys.ionix_space) then
        local mx, my = Input.get_mouse_x(), Input.get_mouse_y()
        Nav.add_obstacle(mx, my, mx + 120, my + 120)
        terrainEntity = Entity.create_entity()
        Entity.set_global_pos(terrainEntity, mx - 2, my - 2)
        Entity.add_sprite_component(terrainEntity, assets.textures.Sand, 110, 110, 10)
    end
end

return NavTest
