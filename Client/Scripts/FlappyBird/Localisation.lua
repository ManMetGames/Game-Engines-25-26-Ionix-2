local Localisation = {}

local current = "en"

local STRINGS = {
  en = {
    ["fb.leaderboard"]           = "Leaderboard",
    ["fb.press_space_start"]     = "Press SPACE to start!",
    ["fb.press_space_restart"]   = "Press SPACE to restart",
    ["fb.game_over_try_again"]   = "GAME OVER!! TRY AGAIN",
    ["fb.new_highscore_line1"]   = "New Highscore! Enter your name",
    ["fb.new_highscore_line2"]   = "to be added to the leaderboard:",
  },


  jp = {
    ["fb.leaderboard"] = "ƒ‰ƒ“ƒLƒ“ƒO",
  },
}

function Localisation.set_language(lang)
  if STRINGS[lang] ~= nil then
    current = lang
  else
    current = "en"
  end
end

function Localisation.get_language()
  return current
end

function Localisation.t(key, ...)
  local langTable = STRINGS[current] or STRINGS.en
  local s = (langTable and langTable[key]) or (STRINGS.en and STRINGS.en[key]) or key

  -- Allow string.format style usage: Localisation.t("some.key", arg1, arg2)
  if select("#", ...) > 0 then
    return string.format(s, ...)
  end
  return s
end

return Localisation
