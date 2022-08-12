-- premake5.lua

workspace "venturi"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "venturi"
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
VULKAN_SDK = os.getenv("VULKAN_SDK")

include "oak/oakExternal.lua"
include "venturi"