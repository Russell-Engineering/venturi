-- premake5.lua
workspace "venturi"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "venturi"
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "vk/vkExternal.lua"
include "oak/oakExternal.lua"
include "venturi"