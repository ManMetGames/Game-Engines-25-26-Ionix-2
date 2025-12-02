local MeteorField = {}
local meteors = {}
local spawnInterval = 0.8
local spawnTimer = 0
local minSpeed = 150
local maxSpeed = 300
local screenW, screenH = 0, 0
local PlayerModule
local GameModule
local AssetsModule
local meteorRadius = 24

local function SpawnMeteor()
    local entity = Entity.create_entity()
    local x = math.random(meteorRadius, screenW - meteorRadius)
    local y = -meteorRadius

    Entity.set_entity_pos(entity, x, y)

    local textureName = "meteor"
    Entity.add_sprite_component(entity, textureName, meteorRadius * 2, meteorRadius * 2, 0)

    local speed = math.random(minSpeed, maxSpeed)
    local meteor = {
        entity = entity,
        speed = speed,
        radius = meteorRadius,
        active = true,
    }

    table.insert(meteors, meteor)
    print(string.format("[MeteorField] Spawned meteor at (%.2f, %.2f) with speed %.2f", x, y, speed))
end 

local function CheckCollisionWithPlayer(meteor)
    if not meteor.active then return end
    if not PlayerModule then return end
    if not meteor.entity then
        meteor.active = false
        return
    end

    local playerEntity = PlayerModule:get_entity()
    if not playerEntity then return end

    local px, py = Entity.get_entity_pos(playerEntity)
    local mx, my = Entity.get_entity_pos(meteor.entity)

    if not mx or not my then
        meteor.active = false
        meteor.entity = nil
        return
    end

    local dx = px - mx
    local dy = py - my
    local distSq = dx * dx + dy * dy
    local minDist = meteor.radius + PlayerModule:get_radius()

    if distSq < minDist * minDist then
        print("[MeteorField] Collision detected between meteor and player.")
        meteor.active = false
        if GameModule then
            GameModule:on_player_hit()
        end

        if Entity.destroy_entity then
            Entity.destroy_entity(meteor.entity)
            meteor.entity = nil
        end
    end
end

local function ResetAllMeteors()
    for _, meteor in ipairs(meteors) do
        if meteor.entity and Entity.destroy_entity then
            Entity.destroy_entity(meteor.entity)
        end
    end
    meteors = {}
    spawnTimer = 0
    print("[MeteorField] All meteors reset.")
end

function MeteorField:OnStart()
    screenW = Window.get_width()
    screenH = Window.get_height()

    PlayerModule = GetModule("Player")
    GameModule = GetModule("Game")
    AssetsModule = GetModule("Assets")

    print("[MeteorField] Meteor field initialized.")
end

function MeteorField:OnUpdate()
    if GameModule and GameModule:is_game_over() then
        return
    end

    local dt = Mafs.delta_time() or 0.016

    spawnTimer = spawnTimer + dt
    if spawnTimer >= spawnInterval then
        SpawnMeteor()
        spawnTimer = spawnTimer - spawnInterval
    end

    for _, meteor in ipairs(meteors) do
        if meteor.active then
            if not meteor.entity then
                meteor.active = false
            else
                local x, y = Entity.get_entity_pos(meteor.entity)
                if not x or not y then
                    meteor.active = false
                    meteor.entity = nil
                else
                    y = y + meteor.speed * dt
                    Entity.set_entity_pos(meteor.entity, x, y)

                    CheckCollisionWithPlayer(meteor)

                    if y - meteor.radius > screenH then
                        meteor.active = false
                        if Entity.destroy_entity then
                            Entity.destroy_entity(meteor.entity)
                            meteor.entity = nil
                        end
                        print("[MeteorField] Meteor went off-screen and was removed.")
                    end
                end
            end
        end
    end
end

function MeteorField:OnShutdown()
    ResetAllMeteors()
    print("[MeteorField] Meteor field shutting down.")
end

function MeteorField:reset()
    ResetAllMeteors()
end

return MeteorField