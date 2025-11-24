local player = {}

local playerEntity = nil
local speed = 400
local radius = 32
local screenW, screenH = 0, 0

function player:OnStart()
    local assets = GetModule("Assets")
    screenW = Window.get_width()
    screenH = Window.get_height()

    playerEntity = Entity.create_entity()
    local startX = screenW / 2
    local startY = screenH - 80
    Entity.set_entity_pos(playerEntity, startX, startY)

    local textureName = "player_ship"
    if assets and assets.textures.player_ship then
        textureName = "player_ship"
    end
    Entity.add_sprite_component(playerEntity, textureName, radius * 2, radius * 2, 0)
    print("[Player] Entity created at:", startX, startY)
end

function player:OnUpdate()
    if not playerEntity then return end

    local dt = Mafs.delta_time() or 0.016

    local x, y = Entity.get_entity_pos(playerEntity)
    
    if Input.get_key_held(Keys.ionix_a) then
        x = x - speed * dt
    end
    if Input.get_key_held(Keys.ionix_d) then
        x = x + speed * dt
    end
    x = Mafs.clamp(x, radius, screenW - radius)
    Entity.set_entity_pos(playerEntity, x, y)
end

function Player:OnShutdown()
    if playerEntity then
        Entity.destroy_entity(playerEntity)
        playerEntity = nil
        print("[Player] Entity destroyed on shutdown.")
    end
end

function Player:reset()
    if playerEntity then
        local startX = screenW / 2
        local startY = screenH - 80
        Entity.set_entity_pos(playerEntity, startX, startY)
        print("[Player] Entity reset to starting position.")
    end
end

function Player:get_entity()
    return playerEntity
end

function Player:get_radius()
    return radius
end

return player
