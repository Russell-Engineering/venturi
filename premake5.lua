-- premake5.lua

workspace "Venturi"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
VULKAN_SDK = os.getenv("VULKAN_SDK")

include "wiExternal.lua"
include "Venturi"