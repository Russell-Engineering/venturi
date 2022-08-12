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
        "../oak/vendor/imgui",
        "../oak/vendor/glfw/include",
        "../oak/oak/src",

        "%{IncludeDir.VulkanSDK}",
    }
    links
    {
        "oak",
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