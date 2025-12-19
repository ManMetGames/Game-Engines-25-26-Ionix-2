-- Scripts/Battleships/Board.lua
CellState = { Empty=0, Ship=1, Hit=2, Miss=3 }

local Board = {}
Board.__index = Board

----------------------------------------------------------
-- Constructor
----------------------------------------------------------
function Board.new(width, height)
    local self = setmetatable({}, Board) -- Class creates board instance
    self.width  = width or 10
    self.height = height or 10
    self.cells  = {}
    self:init()
    return self
end

----------------------------------------------------------
-- Init board
----------------------------------------------------------
function Board:init()
    for y=1,self.height do
        self.cells[y] = {}
        for x=1,self.width do
            self.cells[y][x] = CellState.Empty
        end
    end
end

----------------------------------------------------------
-- Ship placement logic
----------------------------------------------------------
function Board:canPlaceShip(x, y, length, horizontal)
    for i=0,length-1 do
        local cx = horizontal and (x+i) or x
        local cy = horizontal and y or (y+i)

        if cx < 1 or cx > self.width or cy < 1 or cy > self.height then
            return false
        end
        if self.cells[cy][cx] ~= CellState.Empty then
            return false
        end
    end
    return true
end

function Board:placeShip(x, y, length, horizontal)
    for i=0,length-1 do
        local cx = horizontal and (x+i) or x
        local cy = horizontal and y or (y+i)
        self.cells[cy][cx] = CellState.Ship
    end
end

----------------------------------------------------------
-- Random ship placement
----------------------------------------------------------
function Board:randomizeShips()
    local ships = { 5, 4, 3, 3, 2 }

    for _, length in ipairs(ships) do
        local placed = false
        while not placed do
            local x = math.random(1, self.width)
            local y = math.random(1, self.height)
            local horizontal = (math.random(0,1) == 1)

            if self:canPlaceShip(x, y, length, horizontal) then
                self:placeShip(x, y, length, horizontal)
                placed = true
            end
        end
    end
end

----------------------------------------------------------
-- Hit / Miss
----------------------------------------------------------
function Board:markHit(x, y)
    self.cells[y][x] = CellState.Hit
end

function Board:markMiss(x, y)
    self.cells[y][x] = CellState.Miss
end

return Board