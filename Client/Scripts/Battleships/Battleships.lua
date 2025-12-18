-- Scripts/Battleships/Battleships.lua
local Battleships = {}
local Board = require("Scripts.Battleships.Board")
local assets = require("Scripts.Assets")

local originX, originY, cellSize = 100, 100, 32
local gridEntities = {}

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function Battleships:OnStart()
    print("[Battleships] OnStart called")
    Board:init()
    Board:placeShip(2, 3, 4, true) -- example ship

    -- Ocean background
    local bg = Entity.create_entity()
    Entity.set_global_pos(bg, 0, 0)
    Entity.add_sprite_component(bg, assets.textures.Ocean, Window.get_width(), Window.get_height(), 0)

    -- Grid entities
    for y=1,Board.height do
        gridEntities[y] = {}
        for x=1,Board.width do
            local cell = Entity.create_entity()
            Entity.set_global_pos(cell, originX+(x-1)*cellSize, originY+(y-1)*cellSize)
            local sprite = Entity.add_sprite_component(cell, assets.textures.Debug, cellSize, cellSize, 1)
            gridEntities[y][x] = {entity=cell, sprite=sprite}
        end
    end
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function Battleships:OnUpdate()
    -- Handle mouse click
    if Input.get_mouse_button_down(1) then
        local mx = Input.get_mouse_x()
        local my = Input.get_mouse_y()

        local cellX = math.floor((mx - originX) / cellSize) + 1
        local cellY = math.floor((my - originY) / cellSize) + 1

        if cellX >= 1 and cellX <= Board.width and cellY >= 1 and cellY <= Board.height then
            if Board.cells[cellY][cellX] == CellState.Ship then
                Board:markHit(cellX, cellY)
            elseif Board.cells[cellY][cellX] == CellState.Empty then
                Board:markMiss(cellX, cellY)
            end
        end
    end

    -- Update visuals
    for y=1,Board.height do
        for x=1,Board.width do
            local state = Board.cells[y][x]
            local sprite = gridEntities[y][x].sprite
            if state == CellState.Hit then
                Sprite.set_color(sprite, 255,0,0,255) -- red
            elseif state == CellState.Miss then
                Sprite.set_color(sprite, 255,255,255,255) -- white
            elseif state == CellState.Ship then
                Sprite.set_color(sprite, 128,128,128,255) -- gray
            else
                Sprite.set_color(sprite, 0,0,255,255) -- blue water
            end
        end
    end
end

----------------------------------------------------------
-- OnFixedUpdate
----------------------------------------------------------
function Battleships:OnFixedUpdate()
    -- No physics needed yet
end

return Battleships