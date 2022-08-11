-- wiExternal.lua
group "dependencies"
    include "vendor/glfw"

    project "ImPlot"
       location "vendor/implot"
       kind "StaticLib"
       language "C++"
       staticruntime "off"

        targetdir ("vendor/implot/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("vendor/implot/bin-int/" .. outputdir .. "/%{prj.name}")

        includedirs
        {
            "vendor/imgui",
            "vendor/implot"
        }
        files
        {
            "vendor/imgui/imconfig.h",
            "vendor/imgui/imgui.h",
            "vendor/imgui/imgui.cpp",
            "vendor/imgui/imgui_draw.cpp",
            "vendor/imgui/imgui_internal.h",
            "vendor/imgui/imgui_tables.cpp",
            "vendor/imgui/imgui_widgets.cpp",
            "vendor/imgui/imstb_rectpack.h",
            "vendor/imgui/imstb_textedit.h",
            "vendor/imgui/imstb_truetype.h",
            "vendor/imgui/imgui_demo.cpp", 

            "vendor/implot/implot.h",
            "vendor/implot/implot.cpp",
            "vendor/implot/implot_demo.cpp", 
            "vendor/implot/implot_internal.h", 
            "vendor/implot/implot_items.cpp", 
        }

        filter "system:windows"
            systemversion "latest"
            cppdialect "C++17"

        filter "system:linux"
            pic "On"
            systemversion "latest"
            cppdialect "C++17"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            runtime "Release"
            optimize "on"
            symbols "off"

group ""

