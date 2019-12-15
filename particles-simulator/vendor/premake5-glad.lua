project "Glad"
	location "Glad"
    kind "StaticLib"
    language "C"
    
	targetdir ("%{wks.location}/bin/" .. outputDirName .. "/%{prj.name}")
	objdir ("%{wks.location}/bin/" .. outputDirName .. "/%{prj.name}/obj")

	files {
        "Glad/include/glad/glad.h",
		"Glad/src/glad.c"
    }
	
	includedirs {
		"Glad/include"
	}
    
	filter "system:windows"
        --buildoptions { "-std=c11", "-lgdi32" }
        systemversion "latest"
        staticruntime "On"
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"