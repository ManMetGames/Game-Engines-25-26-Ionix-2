local SystemShooterUI = {}

local SystemShooterPlayerProgress = require("Scripts.SystemShooter.SystemShooterPlayerProgress")

local isUpgradeMenuOpen = false
local upgradeOptions = {}      -- { type, label, desc }
local selectedIndex = 0        -- 0 = none

-- Window resize for upgrade menu
local preUpgradeWindowW = nil
local preUpgradeWindowH = nil
local MIN_UPGRADE_W = 1000
local MIN_UPGRADE_H = 700

-- Confirm countdown (keeps menu open briefly so game doesn't resume instantly)
local confirmPending = false
local confirmTimer = 0
local confirmDuration = 1.0    -- tweak: 0.5-0.8 feels good

-- Theme
local OVERLAY_ALPHA = 0.55

local CARD_ROUND = 12
local CARD_BG_ALPHA = 0.92
local CARD_BG_R, CARD_BG_G, CARD_BG_B = 25, 25, 25

local BORDER_SIZE = 3.0
local BORDER_RED   = {170, 25, 25, 1.0}
local BORDER_GREEN = {0, 170, 110, 1.0}

-- Slightly lighter Select option button collors
--local BORDER_RED   = {200, 40, 40, 1.0}
--local BORDER_GREEN = {0, 200, 130, 1.0}

local BTN_ROUND = 12

-- For language translation
local Localisation = require("Scripts.SystemShooter.Localisation")
local function T(key) return Localisation.t(key) end

local function splitLines(s)
  local t = {}
  if not s or s == "" then return t end
  for line in string.gmatch(s, "([^\n]+)") do
    table.insert(t, line)
  end
  return t
end

function SystemShooterUI.getRandomUpgradeOptions(count, playerLevel)
  count = count or 2
  playerLevel = playerLevel or 1

  local upgradeConfig = SystemShooterPlayerProgress.getUpgradeConfig()
  local stats = SystemShooterPlayerProgress.getStats()
  local available = {}
  for upgradeType, cfg in pairs(upgradeConfig) do
    local minLevel = cfg.minLevel or 1
    if playerLevel >= minLevel and SystemShooterPlayerProgress.canTakeUpgrade(upgradeType) then
      local currentValue = stats[cfg.statKey] or cfg.defaultValue or 0
      table.insert(available, { 
        type = upgradeType, 
        label = cfg.label, 
        desc = cfg.desc, 
        cfg = cfg, 
        currentValue = currentValue,
        weight = cfg.weight or 1
      })
    end
  end

  -- Weighted random selection
  local chosen = {}
  for _ = 1, count do
    if #available == 0 then break end
    
    -- Calculate total weight
    local totalWeight = 0
    for _, opt in ipairs(available) do
      totalWeight = totalWeight + opt.weight
    end
    
    -- Pick weighted random
    local roll = math.random() * totalWeight
    local cumulative = 0
    local selectedIdx = 1
    for i, opt in ipairs(available) do
      cumulative = cumulative + opt.weight
      if roll <= cumulative then
        selectedIdx = i
        break
      end
    end
    
    table.insert(chosen, table.remove(available, selectedIdx))
  end
  return chosen
end

function SystemShooterUI.showUpgradeMenu(options, playerLevel)
  -- Check if window is too small for upgrade menu
  local currentW = Window.get_width()
  local currentH = Window.get_height()
  
  if currentW < MIN_UPGRADE_W or currentH < MIN_UPGRADE_H then
    -- Store current size
    preUpgradeWindowW = currentW
    preUpgradeWindowH = currentH
    
    -- Resize to comfortable size
    local targetW = math.max(currentW, MIN_UPGRADE_W)
    local targetH = math.max(currentH, MIN_UPGRADE_H)
    local displayW = Window.get_display_width()
    local displayH = Window.get_display_height()
    local x = math.floor((displayW - targetW) * 0.5)
    local y = math.floor((displayH - targetH) * 0.5)
    Window.set_pos(x, y)
    Window.set_size(targetW, targetH)
  end
  
  isUpgradeMenuOpen = true
  upgradeOptions = options or SystemShooterUI.getRandomUpgradeOptions(2, playerLevel)
  selectedIndex = 0

  confirmPending = false
  confirmTimer = 0

  Input.set_relative_mouse_mode(false)
end

function SystemShooterUI.hideUpgradeMenu()
  -- Restore original window size if it was resized for the menu
  if preUpgradeWindowW and preUpgradeWindowH then
    local displayW = Window.get_display_width()
    local displayH = Window.get_display_height()
    local x = math.floor((displayW - preUpgradeWindowW) * 0.5)
    local y = math.floor((displayH - preUpgradeWindowH) * 0.5)
    Window.set_pos(x, y)
    Window.set_size(preUpgradeWindowW, preUpgradeWindowH)
    preUpgradeWindowW = nil
    preUpgradeWindowH = nil
  end
  
  isUpgradeMenuOpen = false
  upgradeOptions = {}
  selectedIndex = 0

  confirmPending = false
  confirmTimer = 0

  Input.set_relative_mouse_mode(true)
end

function SystemShooterUI.isMenuOpen()
  return isUpgradeMenuOpen
end

function SystemShooterUI.getSelectedUpgrade()
  if selectedIndex > 0 and selectedIndex <= #upgradeOptions then
    return upgradeOptions[selectedIndex]
  end
  return nil
end

function SystemShooterUI.draw(screenW, screenH)
if not isUpgradeMenuOpen then return end


  -- Dim background
  UI.add_panel(0, 0, screenW, screenH, OVERLAY_ALPHA, 0, 0, 0, 0)


  -- Responsive layout based on window size
  local marginX = math.max(20, math.floor(screenW * 0.05))
  local marginY = math.max(20, math.floor(screenH * 0.08))

  local gap = math.floor(screenW * 0.08)
  gap = math.max(30, math.min(gap, 140))

  -- card width: try to fit 2 cards + gap inside the window with margins
  local cardW = math.floor((screenW - marginX*2 - gap) / 2)
  cardW = math.max(260, math.min(cardW, 520))

  -- card height: a fraction of height, clamped
  local cardH = math.floor(screenH * 0.42)
  cardH = math.max(220, math.min(cardH, 360))

  local totalW = cardW * 2 + gap
  local startX = math.floor((screenW - totalW) / 2)
  local startY = math.floor((screenH - cardH) / 2) - math.floor(screenH * 0.08)

  local headerH = 70  -- reserved space for "LEVEL UP" + subtitle
  local minStartY = headerH + 20
  if startY < minStartY then
    startY = minStartY
  end

  UI.add_centered_label(screenW/2, 15, T("gameplay.leveluptxt"), TS_UI.FONT_TITLE, 1)
  UI.add_centered_label(screenW/2, 50, T("gameplay.upgradetxt"), TS_UI.FONT_SUB, 1)


  for i, opt in ipairs(upgradeOptions) do
    local cardX = startX + (i-1) * (cardW + gap)
    local cardY = startY

    local selected = (i == selectedIndex)
    local border = selected and BORDER_GREEN or BORDER_RED
    local br, bg, bb, ba = border[1], border[2], border[3], border[4]

    -- Card as child panel (bg + border + thickness + border color)
    UI.begin_child(
      cardX, cardY, cardW, cardH, "upgrade_card_"..tostring(i),
      true, 0,
      true, CARD_BG_ALPHA, CARD_ROUND, CARD_BG_R, CARD_BG_G, CARD_BG_B,
      BORDER_SIZE,
      false, 0.85,
      br, bg, bb, ba
    )

      local cx = cardW / 2

      local function getDisplayLabel(option)
        if option.cfg then
          local base = T(option.cfg.label) or option.cfg.label or ""
          if option.cfg.maxValue then
            local currentVal = option.currentValue or 0
            return string.format("%s %d/%d", base, currentVal, option.cfg.maxValue)
          end
          return base
        end
        return T(option.label) or option.label
      end

      UI.add_centered_label(cx, 30, getDisplayLabel(opt), TS_UI.FONT_TITLE, 0.8)

      local descText = T(opt.desc) or ""
      if opt.type == "no_witnesses" then
        -- Custom description per rank
        local currentRank = opt.currentValue or 0
        if currentRank == 0 then
            descText = T("upgradedesc.no_witnesses_rank1")
        elseif currentRank >= 1 then
            descText = T("upgradedesc.no_witnesses_rank2")
        end
      end

      local pad = 20
      local wrapW = cardW - pad*2
      local descY = 95

      UI.add_centered_label(cx, descY, descText, TS_UI.FONT_SUB, 1.0, 9000)

      local chooseW, chooseH = math.min(180, math.floor(cardW * 0.55)), 44
      local chooseX = (cardW - chooseW) / 2
      local chooseY = cardH - chooseH - 25

      local chooseText = selected and T("gameplay.upgradeselected") or T("gameplay.upgradechoose")
      UI.add_button(
        chooseX, chooseY, chooseW, chooseH,
        chooseText, "upgrade_choose_"..tostring(i),
        TS_UI.FONT_SUB, 1.0,
        BTN_ROUND, true,
        br, bg, bb, 1.0
      )

    UI.end_child()
  end

  -- Confirm button (responsive width)
  local confirmW = math.floor(screenW * 0.55)
  confirmW = math.max(320, math.min(confirmW, 640))
  local confirmH = 55
  local confirmX = math.floor((screenW - confirmW) / 2)
  local confirmY = startY + cardH + math.floor(screenH * 0.07)

  local enabled = (selectedIndex > 0) and not confirmPending
  local cr, cg, cb = 74, 12, 255
  if not enabled then cr, cg, cb = 80, 80, 80 end

  local confirmLabel = confirmPending and T("gameplay.resuming") or T("gameplay.upgradeconfirm")
  UI.add_button(
    confirmX, confirmY, confirmW, confirmH,
    confirmLabel, "upgrade_confirm",
    TS_UI.FONT_SUB, 1.2,
    BTN_ROUND, true,
    cr, cg, cb, 0.90
  )
end

function SystemShooterUI.handleInput()
  if not isUpgradeMenuOpen then return nil end


  -- Choose buttons
  for i=1,#upgradeOptions do
    if UI.was_button_pressed("upgrade_choose_"..tostring(i)) then
      selectedIndex = i
    end
  end

  -- Confirm - immediately close menu and return selection
  if selectedIndex > 0 and UI.was_button_pressed("upgrade_confirm") then
    local selected = SystemShooterUI.getSelectedUpgrade()
    SystemShooterUI.hideUpgradeMenu()
    return selected
  end

  return nil
end

return SystemShooterUI
