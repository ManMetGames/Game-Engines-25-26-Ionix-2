# UpdateHealthBudget.ps1
# Auto-updates HEALTH_BUDGET values in SystemShooterLevels.lua from health_budget_config.json

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$jsonPath = Join-Path $scriptDir "health_budget_config.json"
$luaPath = Join-Path $scriptDir "SystemShooterLevels.lua"

# Check if JSON file exists
if (-not (Test-Path $jsonPath)) {
    Write-Host "ERROR: health_budget_config.json not found!" -ForegroundColor Red
    Write-Host "Please export the JSON from HealthBudgetCalculator.html first." -ForegroundColor Yellow
    exit 1
}

# Check if Lua file exists
if (-not (Test-Path $luaPath)) {
    Write-Host "ERROR: SystemShooterLevels.lua not found at expected location!" -ForegroundColor Red
    exit 1
}

# Read JSON config
Write-Host "Reading configuration from health_budget_config.json..." -ForegroundColor Cyan
$config = Get-Content $jsonPath -Raw | ConvertFrom-Json

# Validate required fields
$requiredFields = @('base', 'perLevel', 'levelSquaredFactor', 'lateLevelThreshold', 'lateSquaredFactor', 'lateCubedFactor')
foreach ($field in $requiredFields) {
    if ($null -eq $config.$field) {
        Write-Host "ERROR: Missing required field '$field' in JSON!" -ForegroundColor Red
        exit 1
    }
}

# Read current Lua file
$luaContent = Get-Content $luaPath -Raw

# Build new HEALTH_BUDGET table
$newHealthBudget = @"
local HEALTH_BUDGET = {
    base = $($config.base),
    perLevel = $($config.perLevel),
    levelSquaredFactor = $($config.levelSquaredFactor),
    lateLevelThreshold = $($config.lateLevelThreshold),
    lateSquaredFactor = $($config.lateSquaredFactor),
    lateCubedFactor = $($config.lateCubedFactor),
}
"@

# Use regex to find and replace HEALTH_BUDGET table
$pattern = '(?s)local HEALTH_BUDGET = \{[^}]*\}'
if ($luaContent -match $pattern) {
    $updatedContent = $luaContent -replace $pattern, $newHealthBudget
    
    # Backup original file
    $backupPath = "$luaPath.backup"
    Copy-Item $luaPath $backupPath -Force
    Write-Host "Created backup: $backupPath" -ForegroundColor Green
    
    # Write updated content
    $updatedContent | Set-Content $luaPath -NoNewline
    
    Write-Host ""
    Write-Host "SUCCESS! Updated SystemShooterLevels.lua with new values:" -ForegroundColor Green
    Write-Host "  base:                 $($config.base)" -ForegroundColor White
    Write-Host "  perLevel:             $($config.perLevel)" -ForegroundColor White
    Write-Host "  levelSquaredFactor:   $($config.levelSquaredFactor)" -ForegroundColor White
    Write-Host "  lateLevelThreshold:   $($config.lateLevelThreshold)" -ForegroundColor White
    Write-Host "  lateSquaredFactor:    $($config.lateSquaredFactor)" -ForegroundColor White
    Write-Host "  lateCubedFactor:      $($config.lateCubedFactor)" -ForegroundColor White
    Write-Host ""
    Write-Host "Original file backed up to: $backupPath" -ForegroundColor Yellow
} else {
    Write-Host "ERROR: Could not find HEALTH_BUDGET table in Lua file!" -ForegroundColor Red
    Write-Host "Pattern expected: local HEALTH_BUDGET = { ... }" -ForegroundColor Yellow
    exit 1
}
