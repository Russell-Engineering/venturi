-- premake5.lua

workspace "Venturi"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "wiExternal.lua"
include "Venturi"