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
        "../wi/wi/src",
        "../wi/wi/include",
        "../vk/vk/src",
        "../vk/vk/include",
        "../oak/oak/src",
        "../oak/oak/include",
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
        "wi",
        "vk",
        "oak",
    }
    
    defines
    {
        "_USE_MATH_DEFINES",
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE",
    }


    filter "system:windows"
        systemversion "latest"
        defines "WI_PLATFORM_WINDOWS"

    filter  "configurations:Debug" 
        defines { "WI_DEBUG" }
        symbols "On"

    filter  "configurations:Release"
        defines { "WI_RELEASE" }
        optimize "On"

    filter  "configurations:Dist"
        defines { "WI_DIST" }
        optimize "On"