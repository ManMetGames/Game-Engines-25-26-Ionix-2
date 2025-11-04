@echo off
setlocal EnableDelayedExpansion

:: Set some environment variables for easy reuse
set VENDOR_DIR=dependencies\bin\
set PREMAKE_CMD=%VENDOR_DIR%\premake\premake5.exe
set TARGET="vs2022"

:: Check if Premake executable exists
if not exist "%PREMAKE_CMD%" (
    echo.
    echo [ERROR] Premake executable not found in %VENDOR_DIR%\premake\.
    echo [ERROR] Make sure you've installed all necessary dependencies.
    echo.
    pause
    exit /b 1
)

:: Call Premake to generate Visual Studio project files
echo.
echo [INFO] Generating Visual Studio project files...
echo.

"%PREMAKE_CMD%" %TARGET%
if errorlevel 1 (
    echo.
    echo [ERROR] Premake command failed! Please check your setup and try again.
    echo.
    pause
    exit /b 1
)

:: Provide some feedback and pause the script
echo.
echo [SUCCESS] Visual Studio project files generated successfully!
echo [INFO] Target: %TARGET%
echo.
pause
