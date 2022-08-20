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
        "../oak/oak/src",
        "../oak/vendor",
        "../oak/vendor/implot",
        "../oak/vendor/imgui",
        "../oak/vendor/spdlog/include",
        "../oak/vendor/glad/include",
        "../oak/vendor/glfw/include",
        "../oak/vendor/glm",
    }

    links
    {
        "oak"
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
        defines "OAK_DIST"
        optimize "On"