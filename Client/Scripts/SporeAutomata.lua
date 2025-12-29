local spore = {}

function spore:OnStart()
    print("[Spore] Spore automata initialized.")
    
    -- Initialize grid (example: 64x64)
    self.gridSize = 64
    self.grid = {}
    for y = 1, self.gridSize do
        self.grid[y] = {}
        for x = 1, self.gridSize do
            -- 0 = empty, 1 = spore type A, 2 = spore type B, etc.
            self.grid[y][x] = 0
        end
    end
    
    -- Place some initial spores
    for i = 1, 10 do
        local x = math.random(1, self.gridSize)
        local y = math.random(1, self.gridSize)
        self.grid[y][x] = 1  -- Type A spore
    end
end

function spore:OnUpdate()
    -- Simple cellular automata update
    local newGrid = {}
    
    for y = 1, self.gridSize do
        newGrid[y] = {}
        for x = 1, self.gridSize do
            local current = self.grid[y][x]
            
            if current == 0 then
                -- Empty cell: check if spores shoot into it
                newGrid[y][x] = 0
            elseif current == 1 then
                -- Spore type A: try to shoot/spread
                if math.random() < 0.1 then  -- 10% chance to shoot
                    -- Pick random direction
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    -- Check bounds
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        if self.grid[ny][nx] == 0 then
                            newGrid[ny][nx] = 1  -- Shoot new spore
                        end
                    end
                end
                newGrid[y][x] = current  -- Original stays alive
            else
                newGrid[y][x] = current
            end
        end
    end
    
    self.grid = newGrid
end

return spore
