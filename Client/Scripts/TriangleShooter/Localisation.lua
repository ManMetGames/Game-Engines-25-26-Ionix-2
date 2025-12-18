local Localisation = {}

local current = "en"

local STRINGS = {
  en = {
    ["menu.play"] = "PLAY",
    ["menu.starting"] = "Starting...",
    ["menu.settings"] = "SETTINGS",
    ["menu.leaderboard"] = "LEADERBOARD",
    ["menu.quit"] = "QUIT",
    ["menu.back"] = "BACK",

    ["pause.title"] = "PAUSED",
    ["pause.resume"] = "RESUME",
    ["pause.quit"] = "BACK TO MAIN MENU",

    ["gameover.title"] = "GAME OVER",
    ["gameover.retry"] = "RETRY",
    ["gameover.back"] = "BACK TO MAIN MENU",
    ["gameover.summary"] = "RUN SUMMARY",

    ["settings.title"] = "SETTINGS",
    ["settings.audio"] = "Audio",
    ["settings.controls"] = "Controls",
    ["settings.language"] = "Language",
    ["controls.sensitivity"] = "Sensitivity",
  },

  ja = {
    ["menu.play"] = "プレイ",
    ["menu.starting"] = "開始中...",
    ["menu.settings"] = "設定",
    ["menu.leaderboard"] = "ランキング",
    ["menu.quit"] = "終了",
    ["menu.back"] = "戻る",

    ["pause.title"] = "一時停止",
    ["pause.resume"] = "再開",
    ["pause.quit"] = "メインメニューへ",

    ["gameover.title"] = "ゲームオーバー",
    ["gameover.retry"] = "リトライ",
    ["gameover.back"] = "メインメニューへ",
    ["gameover.summary"] = "リザルト",

    ["settings.title"] = "設定",
    ["settings.audio"] = "オーディオ",
    ["settings.controls"] = "操作",
    ["settings.language"] = "言語",
    ["controls.sensitivity"] = "感度",
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
