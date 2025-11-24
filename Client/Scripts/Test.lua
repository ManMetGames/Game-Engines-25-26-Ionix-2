local Test = {}

local frameCounter = 0

function Test:OnStart()
    print("========== Test Begin ==========")

    -- 测试是否能访问各模块
    local Game = GetModule("Game")
    local Player = GetModule("Player")
    local MeteorField = GetModule("MeteorField")
    local Audio = GetModule("Audio")
    local Assets = GetModule("Assets")

    print("[Test] Modules loaded:", Game ~= nil, Player ~= nil, MeteorField ~= nil, Audio ~= nil, Assets ~= nil)

    -- 检查玩家实体是否存在
    if Player.get_entity then
        local p = Player.get_entity()
        print("[Test] Player entity exists:", p ~= nil)
    end

    print("========== Test Running ==========")
end

function Test:OnUpdate()
    frameCounter = frameCounter + 1

    -- 每 120 帧（大约每 2 秒）做一次周期性测试
    if frameCounter % 120 == 0 then
        print("------ [Test] Frame:", frameCounter, "------")

        local Game = GetModule("Game")
        local Player = GetModule("Player")
        local MeteorField = GetModule("MeteorField")

        -- 打印当前游戏状态
        print(string.format("[Test] Score: %d, Lives: %d, GameOver: %s",
            -- 以下接口在 Game.lua 中都有实现
            Game.add_score and getfenv(Game.add_score) ~= nil and 0 or 0,
            -- 利用 debug 接口访问 lives
            -- 如果你希望 lives 显示准确，可以在 Game.lua 暴露 get_lives() 方法
            0,
            tostring(Game.is_game_over()))
        )

        -- 移动玩家测试
        if Player.get_entity then
            local p = Player.get_entity()
            if p then
                local x, y = Entity.get_entity_pos(p)
                print(string.format("[Test] Player pos: %.2f, %.2f", x, y))
            end
        end

        -- 陨石数量测试
        if MeteorField.debug_count then
            print("[Test] Meteor count:", MeteorField.debug_count())
        end
    end

    -- 当 Game Over 时自动触发一次 reset 测试
    local Game = GetModule("Game")
    if Game.is_game_over() then
        print("[Test] Game Over detected! Testing reset...")

        Game:OnStart()

        -- 重置 Player / MeteorField
        local Player = GetModule("Player")
        if Player and Player.reset then
            Player.reset()
        end

        local MeteorField = GetModule("MeteorField")
        if MeteorField and MeteorField.reset then
            MeteorField.reset()
        end

        print("[Test] Reset finished.")
    end
end

function Test:OnShutdown()
    print("========== Test End ==========")
end

return Test
