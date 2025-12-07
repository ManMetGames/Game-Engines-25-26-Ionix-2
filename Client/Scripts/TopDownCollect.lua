-- TopDownCollect.lua
local TopDownCollect = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

-- Entities
local background
local player
local enemy
local coins = {}
local coinHidden = {}
local coinCount = 8

