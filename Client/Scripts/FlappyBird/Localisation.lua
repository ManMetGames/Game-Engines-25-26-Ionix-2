local Localisation = {}

local current = "en"

local STRINGS = {
  en = {
    ["fb.title"] = "Flappy Bird",
    ["fb.play"] = "PLAY",
    ["fb.paused"] = "Paused",
    ["fb.leaderboard"]           = "Leaderboard",
    ["fb.best_score"]          = "Best Score: ",
    ["fb.coins"]               = "Coins: ",
    ["fb.score_label"]         = "Score: ",
    ["fb.press_space_start"]     = "Press SPACE to start!",
    ["fb.press_space_restart"]   = "Press SPACE to restart",
    ["fb.game_over_try_again"]   = "GAME OVER!! TRY AGAIN",
    ["fb.new_highscore_line1"]   = "New Highscore! Enter your name",
    ["fb.new_highscore_line2"]   = "to be added to the leaderboard:",
    ["fb.final_score"] = "Final Score: ",
    ["fb.coins_collected"] = "Coins Collected: ",
    ["fb.highscore"] = "Highscore: ",
    ["fb.customise"]   = "Customise",
        ["customise.customise"] = "CUSTOMISE",
        ["customise.coins"] = "Coins: ",
        ["customise.equipped"] = "(Equipped)",
        ["customise.owned"] = "(Owned)",
        ["customise.price"] = " coins",
        ["customise.buy"] = "Buy",
        ["customise.equip"] = "Equip",
        ["customise.selected"] = "Selected",
        ["customise.backgrounds"] = "Backgrounds",
            ["backgrounds.classic"] = "Classic Sky",
            ["backgrounds.classicnight"] = "Night sky",
            ["backgrounds.plains"] = "Open Plains",
        ["customise.birds"] = "Birds",
            ["birds.classic"] = "Classic Bird",
            ["birds.purple"] = "Purple Bird",
            ["birds.gold"] = "Gold Bird",
            ["birds.sandy"] = "Sandy Bird",
            ["birds.ice"] = "Ice Bird",
            ["birds.hologram"] = "Hologram Bird",
            ["bird.gui"] = "Gui Bird",
            ["birds.glitchy"] = "Glitchy Bird",
            ["birds.space"] = "Space Bird",
        ["customise.back"] = "Back",
    ["fb.settings"] = "Settings",
        ["settings.settings"] = "SETTINGS",
        ["settings.audio"] = "Audio settings",
        ["settings.musiccb"] = "Music",
        ["settings.musicvol"] = "Music Volume",
        ["settings.sfxcb"] = "SFX",
        ["settings.sfxvol"] = "SFX Volume",
        ["settings.back"] = "Back",
        ["settings.mastervol"] = "Master volume",
        ["settings.language"] = "Language",
    ["fb.exit"] = "Exit",
    ["fb.resume"] = "Resume",
    ["fb.restart"] = "Restart",
    ["fb.returntomainmenu"] = "Main Menu",
  },

  ja = {
    ["fb.title"] = "フラッピーバード",
    ["fb.play"] = "プレイ",
    ["fb.paused"] = "一時停止",
    ["fb.leaderboard"] = "ランキング",
    ["fb.best_score"] = "ベストスコア: ",
    ["fb.coins"] = "コイン: ",
    ["fb.score_label"] = "スコア: ",
    ["fb.press_space_start"] = "スペースキーで開始",
    ["fb.press_space_restart"] = "スペースキーで再スタート",
    ["fb.game_over_try_again"] = "ゲームオーバー！もう一度挑戦しよう",
    ["fb.new_highscore_line1"] = "新しいハイスコア！名前を入力してください",
    ["fb.new_highscore_line2"] = "ランキングに追加されます：",
    ["fb.final_score"] = "最終スコア: ",
    ["fb.coins_collected"] = "獲得コイン: ",
    ["fb.highscore"] = "ハイスコア: ",
    ["fb.customise"] = "カスタマイズ",
        ["customise.customise"] = "カスタマイズ",
        ["customise.coins"] = "コイン: ",
        ["customise.equipped"] = "（装備中）",
        ["customise.owned"] = "（所持）",
        ["customise.price"] = " コイン",
        ["customise.buy"] = "購入",
        ["customise.equip"] = "装備",
        ["customise.selected"] = "選択済み",
        ["customise.backgrounds"] = "背景",
            ["backgrounds.classic"] = "クラシック",
            ["backgrounds.classicnight"] = "夜空",
            ["backgrounds.plains"] = "大草原",
        ["customise.birds"] = "鳥",
            ["birds.classic"] = "クラシックバード",
            ["birds.purple"] = "パープルバード",
            ["birds.gold"] = "ゴールドバード",
            ["birds.sandy"] = "サンディバード",
            ["birds.ice"] = "アイスバード",
            ["birds.hologram"] = "ホログラムバード",
            ["birds.gui"] = "スライムバード",
            ["birds.glitchy"] = "グリッチバード",
            ["birds.space"] = "スペースバード",
        ["customise.back"] = "戻る",
    ["fb.settings"] = "設定",
        ["settings.settings"] = "設定",
        ["settings.audio"] = "オーディオ設定",
        ["settings.musiccb"] = "音楽",
        ["settings.musicvol"] = "音楽の音量",
        ["settings.sfxcb"] = "効果音",
        ["settings.sfxvol"] = "効果音の音量",
        ["settings.back"] = "戻る",
        ["settings.mastervol"] = "マスター音量",
        ["settings.language"] = "言語",
    ["fb.exit"] = "終了",
    ["fb.resume"] = "再開",
    ["fb.restart"] = "再スタート",
    ["fb.returntomainmenu"] = "メインメニューへ",
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