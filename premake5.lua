workspace "Vulkan"
    architecture "x86_64"
    startproject "Game"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include dirs relative to root (solution)
IncludeDir = {}
IncludeDir["GLFW"] = "Vulkan/vendor/GLFW/include"
IncludeDir["Vulkan"] = "$(VULKAN_SDK)/include"


group "Dependencies"
    include "Vulkan/vendor/GLFW"

group ""

project "Vulkan"
    location "Vulkan"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    pchheader "pch.h"
    pchsource "Vulkan/src/pch.cpp"

    files 
    {
        "%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
    }

    defines
	{
        "_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Vulkan}"
    }

    links
    {
        "GLFW",
        "$(VULKAN_SDK)/lib/vulkan-1.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"