workspace "Morpheus-Engine"
	architecture "x86_64"
	startproject "Morpheus-Sandbox"

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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Morpheus-Core/Vendor/GLFW/include"
IncludeDir["VULKAN"] = "Morpheus-Core/Vendor/VULKAN/Include"
IncludeDir["IMGUI"] = "Morpheus-Core/Vendor/IMGUI"
IncludeDir["GLM"] = "Morpheus-Core/Vendor/GLM"
IncludeDir["STB"] = "Morpheus-Core/Vendor/STB"

group "Dependencies"
	include "Morpheus-Core/Vendor/GLFW"
	include "Morpheus-Core/Vendor/VULKAN"
	include "Morpheus-Core/Vendor/IMGUI"
	include "Morpheus-Core/Vendor/GLM"
	include "Morpheus-Core/Vendor/STB"

group ""

project "Morpheus-Core"
	location "Morpheus-Core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("Morpheus-Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Morpheus-Bin-Int/" .. outputdir .. "/%{prj.name}")

	pchheader "Morppch.h"
	pchsource "Morpheus-Core/Source/Morppch.cpp"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Vendor/STB/**.h",
		"%{prj.name}/Vendor/STB/**.cpp",
		"%{prj.name}/Vendor/GLM/glm/**.hpp",
		"%{prj.name}/Vendor/GLM/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

	includedirs
	{ 
		"%{prj.name}/Source",

		"%{IncludeDir.GLFW}",
		"%{IncludeDir.VULKAN}",
		"%{IncludeDir.IMGUI}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STB}"
	}

	links
	{
		"GLFW",
		"VULKAN",
		"IMGUI"
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"MORP_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"GLFW_INCLUDE_VULKAN"
		}

	filter "configurations:Debug"
		defines "MORP_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MORP_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MORP_DIST"
		runtime "Release"
		optimize "on"

project "Morpheus-Sandbox"
	location "Morpheus-Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("Morpheus-Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Morpheus-Bin-Int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/MorpheusApp.cpp",
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{ 
		"Morpheus-Core/Source",
		"Morpheus-Core/Vendor",
		"%{IncludeDir.VULKAN}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.IMGUI}"
	}

	links
	{
		"Morpheus-Core",
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"MORP_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MORP_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MORP_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MORP_DIST"
		runtime "Release"
		optimize "on"