local spore = {}

function spore:OnStart()    
    -- Initialize grid (example: 64x64)
    
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
    for i = 1, 10 do
        local x = math.random(1, self.gridSize)
        local y = math.random(1, self.gridSize)
        self.grid[y][x] = 1  -- Type A spore
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
            if self.grid[y][x] == 1 then
                Sprite.set_color(self.tileSprites[y][x], 255, 0, 0) -- red spore
            else
                Sprite.set_color(self.tileSprites[y][x], 25, 0, 0) -- Black tile
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
            elseif current ~= 0 then
                newGrid[y][x] = current
            end
        end
    end
    
    self.grid = newGrid
    self:RenderGrid()
end

return spore
