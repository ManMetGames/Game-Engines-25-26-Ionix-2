workspace "GameEngines24-25"
    architecture "x64"
    configurations { "Debug" }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}-%{cfg.system}"

include "IonixDependencies/dependencies/bin/ImGui"

project "Engine"
    location "Engine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    vpaths {
        ["Source Files/*"] = "%{prj.name}/src/**.cpp",
        ["Header Files/*"] = "%{prj.name}/src/**.h"
    }

    includedirs {
        "%{prj.name}/src",
        "IonixDependencies/dependencies/bin/SDL/SDL2-2.30.6/include",
        "IonixDependencies/dependencies/bin/SDL/SDL2_image/include",
        "IonixDependencies/dependencies/bin/SDL/SDL_TTF/include",
        "IonixDependencies/dependencies/bin/SDL/SDL2_mixer-2.8.0/include",
        "IonixDependencies/dependencies/bin/lua/include",
        "IonixDependencies/dependencies/bin/ImGui",
        "IonixDependencies/dependencies/bin/box2d/include/box2d",
		"IonixDependencies/dependencies/bin/sol2/include"
    }

    libdirs {
        "IonixDependencies/dependencies/bin/SDL/SDL2-2.30.6/lib/x64",
        "IonixDependencies/dependencies/bin/SDL/SDL2_image/lib/",
        "IonixDependencies/dependencies/bin/SDL/SDL_TTF/lib/",
        "IonixDependencies/dependencies/bin/SDL/SDL2_mixer-2.8.0/lib/x64",
        "IonixDependencies/dependencies/bin/lua",
        "IonixDependencies/dependencies/bin/box2d/lib",        
    }

    links {
        "SDL2",
        "SDL2_image",
        "SDL2_mixer",
        "SDL2_ttf",
        "lua54",
        "ImGui",
        "box2d.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "ENGINE_EXPORTS",
            "ENGINE_BUILD_DLL"
        }

    postbuildcommands {
        '{COPY} "%{cfg.buildtarget.relpath}" "%{wks.location}/bin/' .. outputdir .. '/Client/"'
    }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        buildoptions "/MD"
        symbols "On"

project "Client"
    location "Client"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    vpaths {
        ["Source Files/*"] = "%{prj.name}/src/**.cpp",
        ["Header Files/*"] = "%{prj.name}/src/**.h"
    }

    includedirs {
        "Engine/src",
        "IonixDependencies/dependencies/bin/SDL/SDL2-2.30.6/include",
        "IonixDependencies/dependencies/bin/SDL/SDL2_image/include",
        "IonixDependencies/dependencies/bin/SDL/SDL_TTF/include",
        "IonixDependencies/dependencies/bin/lua/include",
        "IonixDependencies/dependencies/bin/ImGui",
        "IonixDependencies/dependencies/bin/box2d/include/box2d",
		"IonixDependencies/dependencies/bin/sol2/include"
    }

    libdirs {
        "bin/" .. outputdir .. "/Engine",
        "IonixDependencies/dependencies/bin/SDL/SDL2-2.30.6/lib/x64",
        "IonixDependencies/dependencies/bin/SDL/SDL2_image/lib",
        "IonixDependencies/dependencies/bin/SDL/SDL_TTF/lib",
        "IonixDependencies/dependencies/bin/lua",
        "IonixDependencies/dependencies/bin/box2d/lib"
    }

    links {
        "Engine",
        "SDL2",
        "SDL2_image",
        "SDL2_ttf",
        "lua54",
        "box2d.lib"
    }

    postbuildcommands {
        '{COPY} "%{cfg.buildtarget.relpath}" "%{wks.location}/bin/' .. outputdir .. '/Client/"',
        '{COPY} "%{wks.location}/IonixDependencies/dependencies/bin/SDL/SDL2_Image/lib/SDL2_image.dll" "%{wks.location}/bin/' .. outputdir .. '/Client/"',
        '{COPY} "%{wks.location}/IonixDependencies/dependencies/bin/SDL/SDL2_mixer-2.8.0/lib/x64/SDL2_mixer.dll" "%{wks.location}/bin/' .. outputdir .. '/Client/"',
        '{COPY} "%{wks.location}/IonixDependencies/dependencies/bin/SDL/SDL2-2.30.6/lib/x64/SDL2.dll" "%{wks.location}/bin/' .. outputdir .. '/Client/"',
        '{COPY} "%{wks.location}/IonixDependencies/dependencies/bin/lua/lua54.dll" "%{wks.location}/bin/' .. outputdir .. '/Client/"',
        '{COPY} "%{wks.location}/IonixDependencies/dependencies/bin/SDL/SDL_TTF/lib/SDL2_ttf.dll" "%{wks.location}/bin/' .. outputdir .. '/Client/"'
    }


    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "ENGINE_EXPORTS"
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        buildoptions "/MD"
        symbols "On"