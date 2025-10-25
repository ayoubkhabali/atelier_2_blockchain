# ========================================
# Run Compiled Executable (PowerShell)
# ========================================
# This script can be run from anywhere or double-clicked

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  BLOCKCHAIN CA-HASH - TEST RESULTS" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Get the directory where this script is located
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Check if executable exists
$exePath = $null

if (Test-Path "$scriptDir\build-vs\Release\atelier2_part1.exe") {
    $exePath = "$scriptDir\build-vs\Release\atelier2_part1.exe"
    Write-Host "Running tests from build-vs\Release..." -ForegroundColor Green
} elseif (Test-Path "$scriptDir\build\atelier2_part1.exe") {
    $exePath = "$scriptDir\build\atelier2_part1.exe"
    Write-Host "Running tests from build..." -ForegroundColor Green
} else {
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "ERROR: Executable not found!" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please build the project first:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Option 1 - Visual Studio:" -ForegroundColor White
    Write-Host '  Open "x64 Native Tools Command Prompt for VS 2019"' -ForegroundColor White
    Write-Host "  Run: run_tests_vs.bat" -ForegroundColor White
    Write-Host ""
    Write-Host "Option 2 - MinGW (if compatible):" -ForegroundColor White
    Write-Host "  Run: run_tests.bat" -ForegroundColor White
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host ""

# Run the executable
& $exePath

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Execution completed!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

Read-Host "Press Enter to exit"
