project "venturi"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
   
   
    files 
    { 
        "src/**.h", 
        "src/**.cpp",
    }
    
    includedirs
    {  
        "../oak/oak/src",
        "../oak/vendor/imgui",
        "../oak/vendor/implot",
        "../oak/vendor/glad/include",
        "../oak/vendor/glfw/include",
        "../oak/vendor/glm",
        "../oak/vendor/spdlog/include",
    
        -- "%{IncludeDir.VulkanSDK}",
    }

    -- todo:: find a way to have oak include all these other libraries
    links
    {
        "glad",
        "GLFW",
        "gdi32", 
        "Dwmapi",
        "opengl32",
        "ImPlot",
        
        -- "C:/VulkanSDK/1.3.216.0/Lib/vulkan-1",

        "oak"
    }
    -- defines
    -- {
    --     "GLFW_INCLUDE_NONE"
    -- }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines "OAK_PLATFORM_WINDOWS"

    filter  "configurations:Debug" 
        defines { "DEBUG" }
        symbols "On"

    filter  "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        
    filter  "configurations:Release"
        defines { "WL_DIST" }
        optimize "On"