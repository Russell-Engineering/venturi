workspace "oakApp"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "oakApp"
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "oakExternal.lua"
include "oak"