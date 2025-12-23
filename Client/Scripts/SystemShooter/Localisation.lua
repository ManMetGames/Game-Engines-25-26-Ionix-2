local Localisation = {}

local current = "en"

local STRINGS = {
  en = {
    ["menu.play"] = "PLAY",
        ["play.difficulty"] = "SELECT DIFFICULTY",
            ["difficulty.easy"] = "EASY",
            ["difficulty.medium"] = "MEDIUM",
            ["difficulty.hard"] = "HARD",
        ["play.prompttxt"] = "ENTER YOUR NAME",
        ["play.promptdesc"] = "Register yourself on the Leaderboard",
        ["play.continue"] = "CONTINUE",
        ["play.error"] = "Please enter a name.",
    ["menu.starting"] = "Starting...",
    ["menu.settings"] = "SETTINGS",
    ["menu.leaderboard"] = "LEADERBOARD",
            ["leaderboard.description"] = "Top 10",
            ["leaderboard.stage"] = "Stage %d",
    ["menu.quit"] = "QUIT",
    ["menu.back"] = "BACK",
    ["menu.basiccontrols"] = "Mouse to move | Hold LMB to shoot",
    ["menu.beststage"] = "Best Stage: ",
    ["gameplay.stage"] = "Stage: ",
    ["gameplay.level"] = "Lv. ",
    ["gameplay.exp"] = "  XP ",
    ["gameplay.leveluptxt"] = "LEVEL UP!",
    ["gameplay.upgradetxt"] = "Choose an upgrade",
    ["gameplay.upgradechoose"] = "Choose",
    ["gameplay.upgradeselected"] = "Selected",
    ["gameplay.upgradeconfirm"] = "CONFIRM CHOICE",
    ["gameplay.resuming"] = "Starting...",
        ["upgradetype.firepower"] = "+1 Firepower",
            ["upgradedesc.firepower"] = "Increase your firepower.",
        ["upgradetype.pierce"] = "+1 Pierce",
            ["upgradedesc.pierce"] = "Bullets pierce +1 enemy.",
        ["upgradetype.bounce"] = "+ Window Bounce",
            ["upgradedesc.bounce"] = "Bullets bounce off the window edges.",
        ["upgradetype.fireinterval"] = "+ Fire Rate",
            ["upgradedesc.fireinterval"] = "Shoot faster (lower cooldown).",
        ["upgradetype.no_witnesses"] = "No Witnesses",
            ["upgradedesc.no_witnesses"] = "Deal +50% damage when there are 1 (Rank 1) or 2 or less (Rank 2) enemies left.",
        ["upgradetype.maxhp"] = "+ Max Health",
            ["upgradedesc.maxhp"] = "Increase your max health.",
        ["upgradetype.healingorbspawn"] = "+ Healing Orb Spawn Rate",
            ["upgradedesc.healingorbspawn"] = "Increases the chance for enemies to drop healing orbs.",

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
    ["play.difficulty"] = "難易度を選択",
            ["difficulty.easy"] = "イージー",
            ["difficulty.medium"] = "ノーマル",
            ["difficulty.hard"] = "ハード",
        ["play.prompttxt"] = "名前を入力してください",
        ["play.promptdesc"] = "ランキングに登録します",
        ["play.continue"] = "続く",
        ["play.error"] = "名前が未入力です。",
    ["menu.starting"] = "開始中...",
    ["menu.settings"] = "設定",
    ["menu.leaderboard"] = "ランキング",
        ["leaderboard.description"] = "上位10位",
        ["leaderboard.stage"] = "ステージ %d",
    ["menu.quit"] = "終了",
    ["menu.back"] = "戻る",
    ["menu.basiccontrols"] = "マウスで移動 ｜ 左ボタン長押しで射撃",
    ["menu.beststage"] = "最高ステージ: ",

    ["gameplay.stage"] = "ステージ： ",
    ["gameplay.level"] = "Lv. ",
    ["gameplay.exp"] = "  XP ",
    ["gameplay.leveluptxt"] = "レベルアップ!",
    ["gameplay.upgradetxt"] = "アップグレードを選択",
    ["gameplay.upgradechoose"] = "選ぶ",
    ["gameplay.upgradeselected"] = "選択中",
    ["gameplay.upgradeconfirm"] = "決定",
    ["gameplay.resuming"] = "再開中．．．",
        ["upgradetype.firepower"] = "火力＋1",
            ["upgradedesc.firepower"] = "弾＋1",
        ["upgradetype.pierce"] = "貫通＋1",
            ["upgradedesc.pierce"] = "弾が敵を1体追加で貫通する",
        ["upgradetype.bounce"] = "跳弾＋",
            ["upgradedesc.bounce"] = "画面端での跳弾回数が上がる。",
        ["upgradetype.fireinterval"] = "発射速度＋",
            ["upgradedesc.fireinterval"] = "発射速度が上がる。",
        ["upgradetype.no_witnesses"] = "口封じ",
            ["upgradedesc.no_witnesses"] = "敵が（ランク1：1体以下／ランク2：2体以下）のとき、ダメージが50%増加する。",
        ["upgradetype.maxhp"] = "最大HP＋",
            ["upgradedesc.maxhp"] = "最大HPが増える。",
            ["upgradetype.healingorbspawn"] = "回復オーブ出現率＋",
            ["upgradedesc.healingorbspawn"] = "敵が回復オーブをドロップする確率が上がる。",
    ["pause.title"] = "一時停止",
    ["pause.resume"] = "再開",
    ["pause.quit"] = "メインメニューへ",

    ["gameover.title"] = "ゲームオーバー",
    ["gameover.retry"] = "リトライ",
    ["gameover.back"] = "メインメニューへ",
    ["gameover.summary"] = "リザルト",
        ["summary.overall"] = "全体",
        ["summary.stage"] = "到達ステージ",
        ["summary.level"] = "Lv.",
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
