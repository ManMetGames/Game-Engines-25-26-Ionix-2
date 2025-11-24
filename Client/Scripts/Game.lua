local Game = {}

local score = 0
local lives = 3
local isGameOver = false

function Game:OnStart()
    score = 0
    lives = 3
    isGameOver = false

    print("[Game] Meteor Dodge started!")
    print("[Game] Controls: A/D to move.")
    print("[Game] Lives:", lives, "Score:", score)
end

function Game:OnUpdate()
    if isGameOver then
        if Input.get_key_down(Keys.ionix_r) then
            print("[Game] Restarting game...")
            self:OnStart()  -- Restart the game
            local player = GetModule("Player")
            if player and player.reset then
                player:reset()  -- Reset player state if applicable
            end
            local meteorField = GetModule("MeteorField")
            if meteorField and meteorField.reset then
                meteorField:reset()  -- Reset meteor field if applicable
            end
        end
    end
end

function Game.OnShutdown()
    print("[Game] Game shutting down. Final Score:", score)
end

function Game:add_score(amount)
    if isGameOver then return end
    score = score + (amount or 1)
    print("[Game] Score updated:", score)
end

function Game.on_player_hit()
    if isGameOver then return end
    lives = lives - 1
    print("[Game] Player hit! Lives remaining:", lives)
    local audio = GetModule("Audio")
    if audio and audio.play_hit then
        audio:play_hit()  -- Play hit sound if available
    end

    if lives <= 0 then
        isGameOver = true
        print("[Game] Game Over! Final Score:", score)
    end
end

function Game.is_game_over()
    return isGameOver
end

return Game