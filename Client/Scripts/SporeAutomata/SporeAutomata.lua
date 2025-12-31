local spore = {}

function spore:OnStart()    
    -- Initialize grid (example: 32x32)
    self.gridSize = 32
    
    self.EMPTY  = 0
    self.RED    = 1
    self.BLUE   = 2
    self.CYAN   = 3
    self.PURPLE = 4
    self.WHITE  = 5
    
    


    self.grid = {}
    for y = 1, self.gridSize do
        self.grid[y] = {}
        for x = 1, self.gridSize do
            -- 0 = empty, 1 = spore type A, 2 = spore type B, etc.
            self.grid[y][x] = 0
        end
    end
    
    -- Place some initial spores
    local sporeCount = {0, 0, 0, 0, 0}  -- Count each type
    for i = 1, 200 do  -- More spores for larger grid
        local x = math.random(1, self.gridSize)
        local y = math.random(1, self.gridSize)
        if self.grid[y][x] == 0 then  -- Only place if empty
            local sporeType = math.random(self.RED, self.WHITE)  -- Random color type
            self.grid[y][x] = sporeType
            sporeCount[sporeType] = sporeCount[sporeType] + 1
        end
    end
    
    print("[INIT] Spore counts - RED:" .. sporeCount[self.RED] .. " BLUE:" .. sporeCount[self.BLUE] .. " CYAN:" .. sporeCount[self.CYAN] .. " PURPLE:" .. sporeCount[self.PURPLE] .. " WHITE:" .. sporeCount[self.WHITE])

    self.tileSize = 32
    self.tiles = {}
    self.tileSprites = {}

    print("[Spore] Spore automata initialized.")
    local assets = require("Scripts.Assets")

    for y = 1, self.gridSize do
        self.tiles[y] = {}
        self.tileSprites[y] = {}
        for x = 1, self.gridSize do
            local e = Entity.create_entity()
            Entity.set_global_pos(e,(x-1) * self.tileSize,(y-1) * self.tileSize)
            local s = Entity.add_sprite_component(e,assets.textures.Debug, self.tileSize, self.tileSize, 10)
            Sprite.set_playback_mode(s, 4)
            self.tiles[y][x] = e
            self.tileSprites[y][x] = s
            
        end
    
    end

    self.accum = 0
    self.step = 0.1
    
    -- Initialize isolation death system
    self.isolationGrid = {}
    for y = 1, self.gridSize do
        self.isolationGrid[y] = {}
        for x = 1, self.gridSize do
            self.isolationGrid[y][x] = 0  -- Time isolated (0 = not isolated)
        end
    end
    self.isolationDeathTime = 2.0  -- Die after 2 seconds of isolation
 
    self:RenderGrid()
end 


function spore:RenderGrid()
    local colorCounts = {0, 0, 0, 0, 0}  -- Count each color being rendered
    for y = 1, self.gridSize do 
        for x = 1, self.gridSize do
            local type = self.grid[y][x]

            if type == self.RED then
                Sprite.set_color(self.tileSprites[y][x], 255, 0, 0)
                colorCounts[self.RED] = colorCounts[self.RED] + 1
            elseif type == self.BLUE then
                Sprite.set_color(self.tileSprites[y][x], 0, 0, 200)
                colorCounts[self.BLUE] = colorCounts[self.BLUE] + 1
            elseif type == self.CYAN then
                Sprite.set_color(self.tileSprites[y][x], 0, 255, 255)
                colorCounts[self.CYAN] = colorCounts[self.CYAN] + 1
                if colorCounts[self.CYAN] == 1 then
                    print("[RENDER] CYAN spore found at (" .. x .. "," .. y .. ") - setting color to bright cyan")
                end
            elseif type == self.PURPLE then
                Sprite.set_color(self.tileSprites[y][x], 150, 0, 255)
                colorCounts[self.PURPLE] = colorCounts[self.PURPLE] + 1
            elseif type == self.WHITE then
                Sprite.set_color(self.tileSprites[y][x], 255, 255, 255)
                colorCounts[self.WHITE] = colorCounts[self.WHITE] + 1
                if colorCounts[self.WHITE] == 1 then
                    print("[RENDER] WHITE spore found at (" .. x .. "," .. y .. ") - setting color to bright white")
                end
            else
                Sprite.set_color(self.tileSprites[y][x], 0, 0, 0)       -- Black (Empty)
            end
            
        end
    end 
    
    -- Print color counts every few renders to track what's being rendered
    if colorCounts[self.CYAN] > 0 or colorCounts[self.WHITE] > 0 then
        print("[RENDER] Colors on grid - RED:" .. colorCounts[self.RED] .. " BLUE:" .. colorCounts[self.BLUE] .. " CYAN:" .. colorCounts[self.CYAN] .. " PURPLE:" .. colorCounts[self.PURPLE] .. " WHITE:" .. colorCounts[self.WHITE])
    end
end

-- Helper function to check if a spore is part of a small vulnerable colony
function spore:IsSmallColony(x, y, sporeType, maxSize)
    if self.grid[y][x] ~= sporeType then
        return false
    end
    
    -- Use flood fill to count colony size
    local visited = {}
    for i = 1, self.gridSize do
        visited[i] = {}
        for j = 1, self.gridSize do
            visited[i][j] = false
        end
    end
    
    local function floodFill(fx, fy)
        if fx < 1 or fx > self.gridSize or fy < 1 or fy > self.gridSize then
            return 0
        end
        if visited[fy][fx] or self.grid[fy][fx] ~= sporeType then
            return 0
        end
        
        visited[fy][fx] = true
        local count = 1
        
        -- Check 4 directions (not diagonals for colony counting)
        count = count + floodFill(fx + 1, fy)
        count = count + floodFill(fx - 1, fy)
        count = count + floodFill(fx, fy + 1)
        count = count + floodFill(fx, fy - 1)
        
        return count
    end
    
    local colonySize = floodFill(x, y)
    return colonySize <= maxSize
end

-- Helper function to count neighbors of the same type
function spore:CountSameNeighbors(x, y, sporeType)
    local count = 0
    for dy = -1, 1 do
        for dx = -1, 1 do
            if dx ~= 0 or dy ~= 0 then  -- Don't count self
                local nx = x + dx
                local ny = y + dy
                if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                    if self.grid[ny][nx] == sporeType then
                        count = count + 1
                    end
                end
            end
        end
    end
    return count
end

function spore:OnUpdate()

    self.accum = self.accum + Mafs.delta_time()
    if self.accum < self.step then
        return
    end
    self.accum = self.accum - self.step

    -- Update isolation timers and kill isolated spores
    local newIsolationGrid = {}
    for y = 1, self.gridSize do
        newIsolationGrid[y] = {}
        for x = 1, self.gridSize do
            local current = self.grid[y][x]
            if current ~= 0 then
                -- Count neighbors of same type
                local sameNeighbors = self:CountSameNeighbors(x, y, current)
                
                if sameNeighbors < 2 then
                    -- Spore is isolated (less than 2 neighbors)
                    newIsolationGrid[y][x] = self.isolationGrid[y][x] + self.step
                    
                    -- Kill if isolated too long
                    if newIsolationGrid[y][x] >= self.isolationDeathTime then
                        self.grid[y][x] = 0  -- Kill the spore
                        newIsolationGrid[y][x] = 0
                    end
                else
                    -- Spore has enough neighbors, reset isolation timer
                    newIsolationGrid[y][x] = 0
                end
            else
                newIsolationGrid[y][x] = 0
            end
        end
    end
    self.isolationGrid = newIsolationGrid

    -- Simple cellular automata update
    local newGrid = {}
    
    for y = 1, self.gridSize do
        newGrid[y] = {}
        for x = 1, self.gridSize do
            newGrid[y][x] = 0
        end
    end

    for y = 1, self.gridSize do
        for x = 1, self.gridSize do
            local current = self.grid[y][x]
            
            if current == self.RED then
                -- RED spore: moderate spreader
                if math.random() < 0.07 then
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        if self.grid[ny][nx] == self.EMPTY then
                            newGrid[ny][nx] = self.RED
                        end
                    end
                end
                newGrid[y][x] = current
                
            elseif current == self.BLUE then
                -- BLUE spore: slow spreader
                if math.random() < 0.04 then
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        if self.grid[ny][nx] == self.EMPTY then
                            newGrid[ny][nx] = self.BLUE
                        end
                    end
                end
                newGrid[y][x] = current
                
            elseif current == self.CYAN then
                -- CYAN spore: bandit behavior - can invade small colonies
                if math.random() < 0.12 then
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        local target = self.grid[ny][nx]
                        if target == self.EMPTY then
                            -- Normal spreading to empty space
                            newGrid[ny][nx] = self.CYAN
                        elseif target ~= self.CYAN and self:IsSmallColony(nx, ny, target, 25) then
                            -- Bandit behavior: invade small enemy colonies (25 tiles or less)
                            newGrid[ny][nx] = self.CYAN
                        end
                    end
                end
                newGrid[y][x] = current
                
            elseif current == self.PURPLE then
                -- PURPLE spore: moderate spreader
                if math.random() < 0.06 then
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        if self.grid[ny][nx] == self.EMPTY then
                            newGrid[ny][nx] = self.PURPLE
                        end
                    end
                end
                newGrid[y][x] = current
                
            elseif current == self.WHITE then
                -- WHITE spore: bandit behavior - can invade small colonies
                if math.random() < 0.09 then
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        local target = self.grid[ny][nx]
                        if target == self.EMPTY then
                            -- Normal spreading to empty space
                            newGrid[ny][nx] = self.WHITE
                        elseif target ~= self.WHITE and self:IsSmallColony(nx, ny, target, 20) then
                            -- Bandit behavior: invade small enemy colonies (20 tiles or less)
                            newGrid[ny][nx] = self.WHITE
                        end
                    end
                end
                newGrid[y][x] = current
            end
        end
    end
    
    self.grid = newGrid
    self:RenderGrid()
end

return spore
