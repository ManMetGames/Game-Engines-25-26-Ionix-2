-- Scripts/Battleships/Battleships.lua
local Battleships = {}
local Board = require("Scripts.Battleships.Board")
local assets = require("Scripts.Assets")

local originX, originY, cellSize = 100, 100, 32
local enemyOriginX = originX + 400  -- space between boards

local playerGrid = {}
local enemyGrid = {}

local playerBoard
local enemyBoard

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function Battleships:OnStart()
    print("[Battleships] OnStart called")

    -- Create two boards
    playerBoard = Board.new(10, 10)
    enemyBoard  = Board.new(10, 10)

    -- Example player ship
    playerBoard:placeShip(2, 3, 4, true)

    -- Random enemy ships
    enemyBoard:randomizeShips()

    -- Background
    local bg = Entity.create_entity()
    Entity.set_global_pos(bg, 0, 0)
    Entity.add_sprite_component(bg, assets.textures.Ocean, Window.get_width(), Window.get_height(), 0)

    ------------------------------------------------------
    -- Create player grid
    ------------------------------------------------------
    for y=1,playerBoard.height do
        playerGrid[y] = {}
        for x=1,playerBoard.width do
            local cell = Entity.create_entity()
            Entity.set_global_pos(cell, originX+(x-1)*cellSize, originY+(y-1)*cellSize)
            local sprite = Entity.add_sprite_component(cell, assets.textures.Debug, cellSize, cellSize, 1)
            playerGrid[y][x] = {entity=cell, sprite=sprite}
        end
    end

    ------------------------------------------------------
    -- Create enemy grid
    ------------------------------------------------------
    for y=1,enemyBoard.height do
        enemyGrid[y] = {}
        for x=1,enemyBoard.width do
            local cell = Entity.create_entity()
            Entity.set_global_pos(cell, enemyOriginX+(x-1)*cellSize, originY+(y-1)*cellSize)
            local sprite = Entity.add_sprite_component(cell, assets.textures.Debug, cellSize, cellSize, 1)
            enemyGrid[y][x] = {entity=cell, sprite=sprite}
        end
    end
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function Battleships:OnUpdate()
    -- Handle mouse click on ENEMY board only
    if Input.get_mouse_button_down(1) then
        local mx = Input.get_mouse_x()
        local my = Input.get_mouse_y()

        local cellX = math.floor((mx - enemyOriginX) / cellSize) + 1
        local cellY = math.floor((my - originY) / cellSize) + 1

        if cellX >= 1 and cellX <= enemyBoard.width and cellY >= 1 and cellY <= enemyBoard.height then
            if enemyBoard.cells[cellY][cellX] == CellState.Ship then
                enemyBoard:markHit(cellX, cellY)
            elseif enemyBoard.cells[cellY][cellX] == CellState.Empty then
                enemyBoard:markMiss(cellX, cellY)
            end
        end
    end

    ------------------------------------------------------
    -- Update visuals for both boards
    ------------------------------------------------------
    local function updateGrid(board, grid)
        for y=1,board.height do
            for x=1,board.width do
                local state = board.cells[y][x]
                local sprite = grid[y][x].sprite

                if state == CellState.Hit then
                    Sprite.set_color(sprite, 255,0,0,255)
                elseif state == CellState.Miss then
                    Sprite.set_color(sprite, 255,255,255,255)
                elseif state == CellState.Ship then
                    Sprite.set_color(sprite, 128,128,128,255)
                else
                    Sprite.set_color(sprite, 0,0,255,255)
                end
            end
        end
    end

    updateGrid(playerBoard, playerGrid)
    updateGrid(enemyBoard, enemyGrid)
end

return Battleships