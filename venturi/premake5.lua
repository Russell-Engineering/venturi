project "Venturi"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    files 
    { 
        "src/venturi/**.h", 
        "src/venturi/**.cpp",
        "src/core/**.h", 
        "src/core/**.cpp", 
        "src/imgui/**.cpp"
    }
    
    includedirs
    {  
        "src",
        "src/core",
        "src/venturi",
        "oak/vendor/imgui",
        "oak/vendor/implot",
        "oak/vendor/glfw/include",
        "oak/vendor/glm",
        "oak/vendor/stb",
        "oak/vendor/spdlog/include",
        "C:/VulkanSDK/1.3.216.0/Include",
    }
    links
    {
        -- "GLFW",
        -- "gdi32",
        -- "Dwmapi",

        -- "ImPlot",
        "Oak",
        "C:/VulkanSDK/1.3.216.0/Lib/vulkan-1",
    }
    
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines "WL_PLATFORM_WINDOWS"

    filter  "configurations:Debug" 
        defines { "DEBUG" }
        symbols "On"

    filter  "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        
    filter  "configurations:Release"
        defines { "WL_DIST" }
        optimize "On"