project "venturi"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
   
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "src/**.h", 
        "src/**.cpp",
    }
    
    includedirs
    {  
        "src", 
        "../vk/vk/src",
        "../oak/oak/src",
        "../oak/vendor",
        "../oak/vendor/implot",
        "../oak/vendor/imgui",
        "../oak/vendor/imnodes",
        "../oak/vendor/spdlog/include",
        "../oak/vendor/glad/include",
        "../oak/vendor/glfw/include",
        "../oak/vendor/glm",
        "../oak/vendor/yaml-cpp/include"
    }

    links
    {
        "vk",
        "oak",
    }
    
    defines
    {
        "_USE_MATH_DEFINES"
    }


    filter "system:windows"
        systemversion "latest"
        defines "OAK_PLATFORM_WINDOWS"

    filter  "configurations:Debug" 
        defines "OAK_DEBUG"
        symbols "On"

    filter  "configurations:Release"
        defines "OAK_RELEASE"
        optimize "On"
        
    filter  "configurations:Dist"
        kind "WindowedApp"    
        defines "OAK_DIST"
        optimize "On"