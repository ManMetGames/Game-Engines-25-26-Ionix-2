local TriangleShooterUI = {}

local isUpgradeMenuOpen = false
local upgradeOptions = {}  -- Array of {type, label} for the current choices
local selectedUpgradeIndex = 0  -- 0 = none selected yet

local RADIO_GROUP_ID = "upgrade_selection"

local upgradePool = {
    { type = "pierce",     label = "+1 Pierce" },
    { type = "bullet",     label = "+1 Bullet" },
    { type = "fire_rate",  label = "+ Fire Rate" },
    { type = "bounce",     label = "+ Window Bounce" },
    { type = "max_health", label = "+ Max Health" },
    { type = "orb_heal",   label = "+ Orb Healing" },
}

function TriangleShooterUI.getRandomUpgradeOptions(count)
    count = count or 2
    local available = {}
    for i, upgrade in ipairs(upgradePool) do
        table.insert(available, { type = upgrade.type, label = upgrade.label })
    end

    local selected = {}
    for i = 1, count do
        if #available == 0 then break end
        local idx = math.random(1, #available)
        table.insert(selected, table.remove(available, idx))
    end
    return selected
end

function TriangleShooterUI.showUpgradeMenu(options)
    isUpgradeMenuOpen = true
    upgradeOptions = options or TriangleShooterUI.getRandomUpgradeOptions(2)
    selectedUpgradeIndex = 0
    Input.set_relative_mouse_mode(false)
end

function TriangleShooterUI.hideUpgradeMenu()
    isUpgradeMenuOpen = false
    upgradeOptions = {}
    selectedUpgradeIndex = 0
    Input.set_relative_mouse_mode(true)
end

function TriangleShooterUI.isMenuOpen()
    return isUpgradeMenuOpen
end

function TriangleShooterUI.getSelectedUpgrade()
    if selectedUpgradeIndex > 0 and selectedUpgradeIndex <= #upgradeOptions then
        return upgradeOptions[selectedUpgradeIndex]
    end
    return nil
end

function TriangleShooterUI.draw(screenW, screenH)
    if not isUpgradeMenuOpen then return end

    -- TODO: style this section
    -- Draw title/header
    local titleY = screenH / 2 - 100
    UI.add_centered_label(screenW / 2, titleY, "LEVEL UP! Choose an upgrade:", "")

    -- Draw radio buttons for each option
    local radioX = screenW / 2 - 100
    local radioY = titleY + 40
    local radioW = 200
    local radioH = 30
    local spacing = 40

    for i, option in ipairs(upgradeOptions) do
        local y = radioY + (i - 1) * spacing
        UI.add_radio(radioX, y, radioW, radioH, option.label, RADIO_GROUP_ID, i, 0)
    end

    -- Draw confirm button area (placeholder position)
    local confirmY = radioY + #upgradeOptions * spacing + 20
    UI.add_centered_label(screenW / 2, confirmY, "[Click radio to select, then confirm]", "")
end

function TriangleShooterUI.handleInput()
    if not isUpgradeMenuOpen then return nil end

    -- Check if radio selection changed
    if UI.was_radio_changed(RADIO_GROUP_ID) then
        selectedUpgradeIndex = UI.get_radio(RADIO_GROUP_ID)
    end

    -- For now, auto-confirm on selection (immediate resume)
    -- TODO: can add a confirm button if desired
    if selectedUpgradeIndex > 0 then
        local selected = TriangleShooterUI.getSelectedUpgrade()
        TriangleShooterUI.hideUpgradeMenu()
        return selected
    end

    return nil
end

return TriangleShooterUI
