local NavTest = {}

local agentEntity
local agent

local assets = require("Scripts.Assets")
local enums  = require("Scripts.Enums")
function NavTest:OnStart()
    agentEntity = Entity.create_entity()

    Entity.add_sprite_component(
        agentEntity,
        assets.textures.PacMan,
        64, 64,
        0
    )

    Entity.set_global_pos(agentEntity, 50, 100)

    Entity.add_fysics_component(
        agentEntity,
        enums.bodytype.kinematicBody,
        true
    )

    Fysics.add_sprite_collider(agentEntity, false, 1)

    Nav.load_nav_mef(0, 0, 1920/4, 1080/4, 25/4)
    agent = Nav.create_agent(agentEntity, 4.0)

    print("[NavTest] NavMesh + Agent initialised")
end
function NavTest:OnUpdate()
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
        agent:move_to(wx, wy)
        print("[NavTest] Move request:", wx, wy)
    end
end

return NavTest
