@echo off

if not exist .\Client\AssetBuilder.exe (
    cd .\Client
    echo [ASSETS] Building executable
    if defined VSINSTALLDIR (
        echo Visual Studio environment detected.
    ) else (
        echo Visual Studio environment NOT detected - entering...
        "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
    )
    cl.exe /Fe:AssetBuilder.exe AssetBuilder.cpp /std:c++17 /EHsc
    echo [ASSETS] Built executable
    del *.obj
    echo [ASSETS] Rebuilding asset files
    AssetBuilder.exe
    echo [ASSETS] Rebuilt asset files
    cd ..
    pause
)

cd .\Client
dir
echo [ASSETS] Rebuilding asset files
AssetBuilder.exe
echo [ASSETS] Rebuilt asset files
cd ..
pause
