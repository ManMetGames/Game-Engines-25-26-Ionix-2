local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")
local player1
local x = 245
local y = Window.get_height()/5 - 50
local tileGapFactor = 0.85


--PACMAN MOVEMENTS
local upBuffer
local upBufferTime = 0
local downBuffer
local downBufferTime = 0
local leftBuffer
local leftBufferTime = 0
local rightBuffer
local rightBufferTime = 0

local upDelay = 0
local downDelay = 0
local leftDelay = 0
local rightDelay = 0

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()
    local tileSize = 128
    local YPos = Window.get_height()/5

    	------------------------------------------------------
		-- makes map by looping through and making a 3x3 map grid, can be easily adjustable
		------------------------------------------------------
	for i = 1, 3 do
        for j = 1, 3 do
		local tile = Entity.create_entity()
        local xPos = (j * (tileSize * tileGapFactor)) + 192
		------------------------------------------------------
		-- place sprite
		------------------------------------------------------
		Entity.set_global_pos(tile, xPos, YPos)
		local s = Entity.add_sprite_component(tile, assets.textures.MapBox, tileSize, tileSize, 1)
		------------------------------------------------------
		-- add physics body + collider
		------------------------------------------------------
		Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)  -- static
		Fysics.add_sprite_collider(tile, false, 0.5)
        end
        YPos = YPos + (tileSize * tileGapFactor)
	end
      
         ------------------------------------------------------
		-- adds all score dots pacman can eat to gain score
		------------------------------------------------------
    local initialX = 245
    local initialY = 166
    for i = 0, 6 do
        for j = 0, 6 do
            local score = Entity.create_entity()
            local scoreXPos = initialX + ((j * 55)) 
            Entity.set_global_pos(score, scoreXPos, initialY)
            local s = Entity.add_sprite_component(score, assets.textures.PacManScore, 8, 8, 2)

            --Entity.add_fysics_component(score, enums,bodytype.staticBody, false)
            --Fysics.add_sprite_collider(tile, false, 1)

        end
        initialY = initialY + 52
    end
    	------------------------------------------------------
		-- creates map border and adds colliders to it
		------------------------------------------------------
        local tile1 = Entity.create_entity() --UP
        local tile2 = Entity.create_entity() --DOWN
        local tile3 = Entity.create_entity() --LEFT
        local tile4 = Entity.create_entity() --RIGHT

        local up = Entity.add_sprite_component(tile1, assets.textures.MapEdge1, 368, 4, 1)
        local left = Entity.add_sprite_component(tile3, assets.textures.MapEdge2, 4, 368, 1)
        local down = Entity.add_sprite_component(tile2, assets.textures.MapEdge1, 368, 4, 1)
        local right = Entity.add_sprite_component(tile4, assets.textures.MapEdge2, 4, 368, 1)

        Entity.set_global_pos(tile1, 410,140) -- Up
        Entity.set_global_pos(tile3, 220,325) -- Left
        Entity.set_global_pos(tile2, 410,510) -- Down
        Entity.set_global_pos(tile4, 600,325) -- Right

        Entity.add_fysics_component(tile1, enums.bodytype.staticBody, false)  -- static
        Entity.add_fysics_component(tile3, enums.bodytype.staticBody, false)  -- static
        Entity.add_fysics_component(tile2, enums.bodytype.staticBody, false)  -- static
        Entity.add_fysics_component(tile4, enums.bodytype.staticBody, false)  -- static

        Fysics.add_sprite_collider(tile1, false, 1)
        Fysics.add_sprite_collider(tile3, false, 1)
        Fysics.add_sprite_collider(tile2, false, 1)
        Fysics.add_sprite_collider(tile4, false, 1)


        ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()

    Entity.set_global_pos(player1, x, y)
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.PacMan, 32, 32, 10) --Pac man sprite render + animation
    Sprite.set_columns(playerSprite1, 2)
    Sprite.rows(playerSprite1, 2)
    Sprite.set_height(playerSprite1, 32)
    Sprite.set_width(playerSprite1, 32)
    Sprite.set_playback_mode(playerSprite1, 3)

    -- PLAYER 1 PHYSICS

    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true) -- dynamic body, last value is rotation lock
    Fysics.add_sprite_collider(player1,false,1)
    Fysics.set_gravity_scale(player1, 0)

end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    --Get player positions
    local pos = Entity.get_global_pos(player1)
    local playerX = Mafs.get_vec_x(pos)
    local playerY = Mafs.get_vec_y(pos)

    -- Raycast length
    local rayLength = 50

    -- Calculate end positions for all 4 directions using Mafs.vec_2
    local upEndPos = Mafs.vec_2(playerX, playerY - rayLength)
    local downEndPos = Mafs.vec_2(playerX, playerY + rayLength)
    local leftEndPos = Mafs.vec_2(playerX - rayLength, playerY)
    local rightEndPos = Mafs.vec_2(playerX + rayLength, playerY)

    -- Perform raycasts in all 4 directions
    local hitUp, infoUp = Fysics.raycast(pos, upEndPos)
    local hitDown, infoDown = Fysics.raycast(pos, downEndPos)
    local hitLeft, infoLeft = Fysics.raycast(pos, leftEndPos)
    local hitRight, infoRight = Fysics.raycast(pos, rightEndPos)

    -- Visualise raycasts (true = hit, false = no hit for color)
    Fysics.draw_raycast(pos, upEndPos, hitUp)
    Fysics.draw_raycast(pos, downEndPos, hitDown)
    Fysics.draw_raycast(pos, leftEndPos, hitLeft)
    Fysics.draw_raycast(pos, rightEndPos, hitRight)


    -- get current velocity
    -- Constant rightward movement
    local vel1 = Fysics.get_linear_velocity(player1)
    local vx = Mafs.get_vec_x(vel1);
    local vy1 = Mafs.get_vec_y(vel1)
    
	if Input.get_key_down(Keys.ionix_w) and hitUp == false then -- move up
        vy1 = -1
        vx = 0
	end
    if Input.get_key_down(Keys.ionix_s) and hitDown == false then -- move down
        vy1 = 1
        vx = 0
    end
    if Input.get_key_down(Keys.ionix_a) and hitLeft == false then -- move left
        vx = -1
        vy1 = 0
    end
    if Input.get_key_down(Keys.ionix_d) and hitRight == false then -- move right
        vx = 1
        vy1 = 0

    end

    if upBuffer then
        upBufferTime = upBufferTime + 1
        if upBufferTime > 100 then upBufferTime = 0 upBuffer = false end
    end

    if downBuffer then
        downBufferTime = downBufferTime + 1
        if downBufferTime > 100 then downBufferTime = 0 downBuffer = false end
    end

    if leftBuffer then
        leftBufferTime = leftBufferTime + 1
        if leftBufferTime > 100 then leftBufferTime = 0 leftBuffer = false end
    end

    if rightBuffer then
        rightBufferTime = rightBufferTime + 1
        if rightBufferTime > 100 then rightBufferTime = 0 rightBuffer = false end
    end

    Fysics.set_linear_velocity(player1, vx, vy1)
end

return ExampleScript