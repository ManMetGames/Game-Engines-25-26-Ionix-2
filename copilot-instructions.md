# Copilot instructions (Ionix / GameEngines25-26)

## Big picture
- This repo is a C++17 game engine split into:
  - **Engine**: shared library (`Engine.dll`) in [Engine/](Engine/)
  - **Client**: executable that links the engine in [Client/](Client/)
- Program entrypoint is fixed in [Engine/src/Architecture/main.h](Engine/src/Architecture/main.h). The Client provides `ClientApp::CreateApplication()` in [Client/src/Client/Client.cpp](Client/src/Client/Client.cpp).
- `IonixEngine::Application` owns the main loop + layer stack in [Engine/src/Architecture/Application.cpp](Engine/src/Architecture/Application.cpp):
  - Creates layers (`LayerEditor`, `LayerInput`, `LayerFysics`, `LayerSound`, `LayerUI`, `LayerTexture`, `LayerGraphics`, `LayerScene`).
  - Runs **fixed update** at 60 Hz (`OnFixedUpdate`) and variable update (`OnUpdate`).
  - Calls Lua hooks: `OnStart`, `OnFixedUpdate`, `OnUpdate`.

## Build / run (Windows)
- Generate Visual Studio project files via Premake:
  - Run `Build.bat` (calls `dependencies/bin/premake/premake5.exe vs2022`).
  - Open `GameEngines25-26.sln` and build **Debug | x64**.
- Output binaries land under `bin/Debug-x86_64-windows/Client/` (Premake also copies `Engine.dll` + required runtime DLLs there).
- If the game can’t find `Scripts/` or `Assets/`, fix the **working directory** in the VS debugger settings to the repo root or the expected runtime folder.

## Asset pipeline (IMPORTANT)
- Assets are **generated**, not hand-maintained:
  - Run `Assets.bat` to build and execute `Client/AssetBuilder.cpp`.
  - AssetBuilder scans `Client/Assets/` and rewrites:
    - [Engine/src/Architecture/Assets.hpp](Engine/src/Architecture/Assets.hpp) (C++ hash + path map)
    - [Client/Scripts/Assets.lua](Client/Scripts/Assets.lua) (Lua table of the same hashes)
- When adding/removing/renaming files in `Client/Assets/`, rerun `Assets.bat` rather than editing generated outputs.
- Naming rules come from AssetBuilder: stems are sanitized (`-`, space, `()`, `,` → `_`), leading digits become `_NN`, collisions get suffixes.

## Conventions & “gotchas”
- Physics is consistently spelled **Fysics** in folders/types (e.g. [Engine/src/Fysics/](Engine/src/Fysics/)); keep this naming for new code.
- Do not edit [Engine/src/Architecture/main.h](Engine/src/Architecture/main.h) (comment says it should not need touching).
- Prefer integrating new gameplay via:
  - New Lua scripts in [Client/Scripts/](Client/Scripts/) (hooks are called from `Application`).
  - New layers under [Engine/src/LayerSystem/Layers/](Engine/src/LayerSystem/Layers/).

## Lua scripting integration
- Lua is hosted via **sol2** (`sol::state`) in [Engine/src/Scripting/Scripting.cpp](Engine/src/Scripting/Scripting.cpp).
- Engine API is exposed by modular binding classes under [Engine/src/Scripting/](Engine/src/Scripting/) and registered in `Scripting::RegisterEngineBindings()`.
- Startup loads `Scripts/Settings.lua` (relative path) from `Application`.

## Audio
- Sound effects use SDL_mixer (see `SoundManager`/`AudioPlayer`).
- Music uses **SoLoud** (separate system) per [MUSIC_SYSTEM_GUIDE.md](MUSIC_SYSTEM_GUIDE.md):
  - [Engine/src/Architecture/AudioSystem/MusicManager.cpp](Engine/src/Architecture/AudioSystem/MusicManager.cpp)
  - [Engine/src/Audio/MusicComponent.cpp](Engine/src/Audio/MusicComponent.cpp)

## External integrations
- Firebase leaderboard lives in [Engine/src/Firebase/firebaseLeaderboard.cpp](Engine/src/Firebase/firebaseLeaderboard.cpp) and is initialized from `Application`.

## When changing code
- Keep changes scoped to Engine vs Client correctly (Engine provides systems/bindings; Client provides the concrete `Application` subclass and game content).
- If a change affects runtime resources, verify paths relative to the working directory and rerun AssetBuilder when needed.
