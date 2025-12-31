local spore = {}

function spore:OnStart()    
    -- Initialize grid (example: 16x16)
    self.gridSize = 16
    
    self.EMPTY  = 0
    self.RED    = 1
    self.BLUE   = 2
    self.GREEN  = 3
    self.PURPLE = 4
    self.YELLOW = 5
    
    self.grid = {}
    for y = 1, self.gridSize do
        self.grid[y] = {}
        for x = 1, self.gridSize do
            -- 0 = empty, 1 = spore type A, 2 = spore type B, etc.
            self.grid[y][x] = 0
        end
    end
    
    -- Place some initial spores
    for i = 1, 50 do  -- More spores to fill the space better
        local x = math.random(1, self.gridSize)
        local y = math.random(1, self.gridSize)
        if self.grid[y][x] == 0 then  -- Only place if empty
            local sporeType = math.random(1, 5)  -- Random type 1-5
            self.grid[y][x] = sporeType
        end
    end

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
 
    self:RenderGrid()
end 


function spore:RenderGrid()
    for y = 1, self.gridSize do 
        for x = 1, self.gridSize do
            local type = self.grid[y][x]

            if type == self.RED then
                Sprite.set_color(self.tileSprites[y][x], 255, 0, 0)     -- Red
            elseif type == self.BLUE then
                Sprite.set_color(self.tileSprites[y][x], 0, 100, 255)   -- Blue
            elseif type == self.GREEN then
                Sprite.set_color(self.tileSprites[y][x], 0, 255, 0)     -- Green
            elseif type == self.PURPLE then
                Sprite.set_color(self.tileSprites[y][x], 150, 0, 255)   -- Purple
            elseif type == self.YELLOW then
                Sprite.set_color(self.tileSprites[y][x], 255, 255, 0)   -- Yellow
            else
                Sprite.set_color(self.tileSprites[y][x], 0, 0, 0)       -- Black (Empty)
            end
            
        end
    end 
end



function spore:OnUpdate()

    self.accum = self.accum + Mafs.delta_time()
    if self.accum < self.step then
        return
    end
    self.accum = self.accum - self.step

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
            
            if current == 1 then
                -- RED spore: aggressive spreader
                if math.random() < 0.15 then  -- 15% chance to shoot
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        if self.grid[ny][nx] == 0 then
                            newGrid[ny][nx] = 1
                        end
                    end
                end
                newGrid[y][x] = current
                
            elseif current == 2 then
                -- BLUE spore: slow spreader
                if math.random() < 0.05 then  -- 5% chance to shoot
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        if self.grid[ny][nx] == 0 then
                            newGrid[ny][nx] = 2
                        end
                    end
                end
                newGrid[y][x] = current
                
            elseif current == 3 then
                -- GREEN spore: moderate spreader
                if math.random() < 0.08 then  -- 8% chance to shoot
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        if self.grid[ny][nx] == 0 then
                            newGrid[ny][nx] = 3
                        end
                    end
                end
                newGrid[y][x] = current
                
            elseif current == 4 then
                -- PURPLE spore: fast spreader
                if math.random() < 0.12 then  -- 12% chance to shoot
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        if self.grid[ny][nx] == 0 then
                            newGrid[ny][nx] = 4
                        end
                    end
                end
                newGrid[y][x] = current
                
            elseif current == 5 then
                -- YELLOW spore: very slow spreader
                if math.random() < 0.03 then  -- 3% chance to shoot
                    local dx = math.random(-1, 1)
                    local dy = math.random(-1, 1)
                    local nx = x + dx
                    local ny = y + dy
                    
                    if nx >= 1 and nx <= self.gridSize and ny >= 1 and ny <= self.gridSize then
                        if self.grid[ny][nx] == 0 then
                            newGrid[ny][nx] = 5
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
