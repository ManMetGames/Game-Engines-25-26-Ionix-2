local Localisation = {}

local current = "en"

local STRINGS = {
  en = {
    ["menu.play"] = "PLAY",
    ["menu.starting"] = "Starting...",
    ["menu.settings"] = "SETTINGS",
    ["menu.leaderboard"] = "LEADERBOARD",
            ["leaderboard.description"] = "Top 10",
    ["menu.quit"] = "QUIT",
    ["menu.back"] = "BACK",
    ["menu.basiccontrols"] = "Mouse to move | Hold LMB to shoot",
    ["menu.beststage"] = "Best Stage: ",

    ["pause.title"] = "PAUSED",
    ["pause.resume"] = "RESUME",
    ["pause.quit"] = "BACK TO MAIN MENU",

    ["gameover.title"] = "GAME OVER",
    ["gameover.retry"] = "RETRY",
    ["gameover.back"] = "BACK TO MAIN MENU",
    ["gameover.summary"] = "RUN SUMMARY",
        ["summary.overall"] = "Overall",
        ["summary.stage"] = "Stage reached",
        ["summary.level"] = "Player level",
        ["summary.duration"] = "Time survived",
        ["summary.totalkilled"] = "Enemies killed",
        ["summary.combat"] = "Combat",
        ["summary.shotsfired"] = "Shots fired",
        ["summary.accuracy"] = "Accuracy",
        ["summary.dmgdealt"] = "Damage dealt",
        ["summary.dmgtaken"] = "Damage taken",
        ["summary.hphealed"] = "Health Healed",
        ["summary.recap"] = "Build Recap",
        ["summary.firepower"] = "Firepower: ",
        ["summary.pierce"] = "Pierce: ",
        ["summary.bounce"] = "Bounce: ",
        ["summary.fireinterval"] = "Fire interval: %.2fs",
        ["summary.maxhp"] = "Max HP: ",

    ["settings.title"] = "SETTINGS",
    ["settings.audio"] = "Audio",
        ["audio.master"] = "Master",
            ["audio.music"] = "Music",
                ["audio.sfx"] = "SFX",
    ["settings.controls"] = "Controls",
        ["controls.sensitivity"] = "Sensitivity",
    ["settings.language"] = "Language",

  },

  ja = {
    ["menu.play"] = "プレイ",
    ["menu.starting"] = "開始中...",
    ["menu.settings"] = "設定",
    ["menu.leaderboard"] = "ランキング",
        ["leaderboard.description"] = "上位10位",
    ["menu.quit"] = "終了",
    ["menu.back"] = "戻る",
    ["menu.basiccontrols"] = "マウスで移動 ｜ 左ボタン長押しで射撃",
    ["menu.beststage"] = "最高ステージ: ",

    ["pause.title"] = "一時停止",
    ["pause.resume"] = "再開",
    ["pause.quit"] = "メインメニューへ",

    ["gameover.title"] = "ゲームオーバー",
    ["gameover.retry"] = "リトライ",
    ["gameover.back"] = "メインメニューへ",
    ["gameover.summary"] = "リザルト",
        ["summary.overall"] = "全体",
        ["summary.stage"] = "到達ステージ",
        ["summary.level"] = "レベル",
        ["summary.duration"] = "生存時間",
        ["summary.totalkilled"] = "撃破数",
        ["summary.combat"] = "戦闘",
        ["summary.shotsfired"] = "発射数",
        ["summary.accuracy"] = "命中率",
        ["summary.dmgdealt"] = "与ダメージ",
        ["summary.dmgtaken"] = "被ダメージ",
        ["summary.hphealed"] = "回復したHP",
        ["summary.recap"] = "ビルド概要",
        ["summary.firepower"] = "火力: ",
        ["summary.pierce"] = "貫通: ",
        ["summary.bounce"] = "跳弾: ",
        ["summary.fireinterval"] = "発射間隔: %.2fs",
        ["summary.maxhp"] = "最大HP: ",

    ["settings.title"] = "設定",
    ["settings.audio"] = "オーディオ",
            ["audio.master"] = "マスター",
            ["audio.music"] = "音楽",
                ["audio.sfx"] = "効果",
    ["settings.controls"] = "操作",
        ["controls.sensitivity"] = "感度",
    ["settings.language"] = "言語",

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
