local Localisation = {}

local current = "en"

local STRINGS = {
  en = {
    ["fb.leaderboard"]           = "Leaderboard",
    ["fb.coins"]               = "Coins: %d",
    ["fb.press_space_start"]     = "Press SPACE to start!",
    ["fb.press_space_restart"]   = "Press SPACE to restart",
    ["fb.game_over_try_again"]   = "GAME OVER!! TRY AGAIN",
    ["fb.new_highscore_line1"]   = "New Highscore! Enter your name",
    ["fb.new_highscore_line2"]   = "to be added to the leaderboard:",
  },


  ja = {
    ["fb.leaderboard"] = "�����L���O",
    ["fb.coins"]               = "",
    ["fb.press_space_start"]     = "",
    ["fb.press_space_restart"]   = "",
    ["fb.game_over_try_again"]   = "",
    ["fb.new_highscore_line1"]   = "",
    ["fb.new_highscore_line2"]   = "",
  }
}

function Localisation.set_language(lang)
  if STRINGS[lang] then current = lang else current = "en" end
end

function Localisation.get_language()
  return current
end

function Localisation.t(key)
  local v = STRINGS[current] and STRINGS[current][key]
  if v ~= nil then return v end
  local fallback = STRINGS.en and STRINGS.en[key]
  return fallback ~= nil and fallback or key
end

return Localisation

