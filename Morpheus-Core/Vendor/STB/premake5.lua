project "STB"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs {
		"Include/**.**"
	}
	
	libdirs {
		"Lib/**.**"
	}