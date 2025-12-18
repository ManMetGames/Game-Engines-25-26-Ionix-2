local TriangleShooterUI = {}

local TriangleShooterPlayerProgress = require("Scripts.TriangleShooter.TriangleShooterPlayerProgress")

local isUpgradeMenuOpen = false
local upgradeOptions = {}      -- { type, label, desc }
local selectedIndex = 0        -- 0 = none

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

local function splitLines(s)
  local t = {}
  if not s or s == "" then return t end
  for line in string.gmatch(s, "([^\n]+)") do
    table.insert(t, line)
  end
  return t
end

function TriangleShooterUI.getRandomUpgradeOptions(count, playerLevel)
  count = count or 2
  playerLevel = playerLevel or 1

  local upgradeConfig = TriangleShooterPlayerProgress.getUpgradeConfig()
  local available = {}
  for upgradeType, cfg in pairs(upgradeConfig) do
    local minLevel = cfg.minLevel or 1
    if playerLevel >= minLevel and TriangleShooterPlayerProgress.canTakeUpgrade(upgradeType) then
      table.insert(available, { type = upgradeType, label = cfg.label, desc = cfg.desc })
    end
  end

  local chosen = {}
  for _ = 1, count do
    if #available == 0 then break end
    local idx = math.random(1, #available)
    table.insert(chosen, table.remove(available, idx))
  end
  return chosen
end

function TriangleShooterUI.showUpgradeMenu(options, playerLevel)
  isUpgradeMenuOpen = true
  upgradeOptions = options or TriangleShooterUI.getRandomUpgradeOptions(2, playerLevel)
  selectedIndex = 0

  confirmPending = false
  confirmTimer = 0

  Input.set_relative_mouse_mode(false)
end

function TriangleShooterUI.hideUpgradeMenu()
  isUpgradeMenuOpen = false
  upgradeOptions = {}
  selectedIndex = 0

  confirmPending = false
  confirmTimer = 0

  Input.set_relative_mouse_mode(true)
end

function TriangleShooterUI.isMenuOpen()
  return isUpgradeMenuOpen
end

function TriangleShooterUI.getSelectedUpgrade()
  if selectedIndex > 0 and selectedIndex <= #upgradeOptions then
    return upgradeOptions[selectedIndex]
  end
  return nil
end

function TriangleShooterUI.draw(screenW, screenH)
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

  UI.add_centered_label(screenW/2, 30, "LEVEL UP!", "ImGuiDefaultBold", 2.0)
  UI.add_centered_label(screenW/2, 55, "Choose an upgrade", "", 1.4)


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

      UI.add_centered_label(cx, 40, opt.label, "ImGuiDefaultBold", 1.7)

      local lines = splitLines(opt.desc or "")
      local baseY = 95
      local lineGap = 22
      for li, txt in ipairs(lines) do
        UI.add_centered_label(cx, baseY + (li-1)*lineGap, txt, "", 1.3)
      end

      local chooseW, chooseH = math.min(180, math.floor(cardW * 0.55)), 44
      local chooseX = (cardW - chooseW) / 2
      local chooseY = cardH - chooseH - 25

      local chooseText = selected and "Selected" or "Choose"
      UI.add_button(
        chooseX, chooseY, chooseW, chooseH,
        chooseText, "upgrade_choose_"..tostring(i),
        "ImGuiDefaultBold", 1.0,
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

  local confirmLabel = confirmPending and "Starting..." or "CONFIRM CHOICE"
  UI.add_button(
    confirmX, confirmY, confirmW, confirmH,
    confirmLabel, "upgrade_confirm",
    "ImGuiDefaultBold", 1.1,
    BTN_ROUND, true,
    cr, cg, cb, 0.90
  )
end

function TriangleShooterUI.handleInput()
  if not isUpgradeMenuOpen then return nil end


  -- Choose buttons
  for i=1,#upgradeOptions do
    if UI.was_button_pressed("upgrade_choose_"..tostring(i)) then
      selectedIndex = i
    end
  end

  -- Confirm - immediately close menu and return selection
  if selectedIndex > 0 and UI.was_button_pressed("upgrade_confirm") then
    local selected = TriangleShooterUI.getSelectedUpgrade()
    TriangleShooterUI.hideUpgradeMenu()
    return selected
  end

  return nil
end

return TriangleShooterUI
