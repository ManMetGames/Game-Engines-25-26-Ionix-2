project "ImGui"
    kind "StaticLib"
    language "C++"
       
    files {
        "imgui.h",
        "imconfig.h",
        "backends/imgui_impl_sdl2.h",
        "backends/imgui_impl_opengl3.h",
        "imgui_internal.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "imstb_rectpack.h",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_widgets.cpp",
        "imgui_demo.cpp",
        "imgui_tables.cpp",
        "backends/imgui_impl_opengl3.cpp",
        "backends/imgui_impl_sdl2.cpp"
    }


    libdirs {
        "../SDL/SDL2-2.30.6/lib/x64"
    }

    includedirs {
        "../SDL/SDL2-2.30.6/include"
    }

    links {
        "SDL2"
    }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")


    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"
        
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"
