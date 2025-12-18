-- Scripts/Battleships/Board.lua
CellState = { Empty=0, Ship=1, Hit=2, Miss=3 }

local Board = {}
Board.width = 10
Board.height = 10
Board.cells = {}

function Board:init()
    for y=1,self.height do
        self.cells[y] = {}
        for x=1,self.width do
            self.cells[y][x] = CellState.Empty
        end
    end
end

function Board:placeShip(x, y, length, horizontal)
    for i=0,length-1 do
        local cx = horizontal and (x+i) or x
        local cy = horizontal and y or (y+i)
        if cx >= 1 and cx <= self.width and cy >= 1 and cy <= self.height then
            self.cells[cy][cx] = CellState.Ship
        end
    end
end

function Board:markHit(x, y)
    self.cells[y][x] = CellState.Hit
end

function Board:markMiss(x, y)
    self.cells[y][x] = CellState.Miss
end

return Board