workspace "particles-simulator"
	startproject "particles-simulator"	
	
	architecture "x64"		
	configurations {
		"Debug",
		"Release"
	}


-- Pattern for output directory name with executable
outputDirName = "%{cfg.system}-%{cfg.buildcfg}-%{cfg.architecture}"
includePaths = {}


-- Include other project's (dependencies) configurations
include "particles-simulator/vendor/premake5-glfw.lua"
includePaths["GLFW"] = "%{wks.name}/vendor/GLFW/include"

include "particles-simulator/vendor/premake5-glad.lua"
includePaths["Glad"] = "%{wks.name}/vendor/Glad/include"


-- main project
project "particles-simulator"
	location "particles-simulator"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	-- Precompiler defines
	defines {
        "PS_PROJ_NAME=particles-simulator",
		"GLFW_INCLUDE_NONE",
		"_USE_MATH_DEFINES"
	}

	-- Output directories for executables and intermediates objs.
	targetdir ("bin/" .. outputDirName .. "/%{prj.name}")
	objdir ("bin/" .. outputDirName .. "/%{prj.name}/obj")
	
	-- Working directory (for relative paths etc.) = executable directory
	debugdir ("%{cfg.targetdir}")

	-- Precompiled headers
	pchsource "%{prj.name}/src/pch.cpp"

	-- Source code files in the project
	files {
		"%{prj.name}/src/*",
		"%{prj.name}/src/Events/*",
		"%{prj.name}/src/OpenGL/*",
		"%{prj.name}/src/Game/*",
		"%{prj.name}/res/Shaders/*",
	}

	-- Include file search paths for the compiler.
	includedirs {
		"%{prj.name}/src",
		"%{includePaths.GLFW}",
		"%{includePaths.Glad}",
		"%{prj.name}/vendor/glm"
	}

	-- List of libraries and projects to link against.
	links {
		"GLFW",
		"opengl32.lib",
		"Glad"
	}

	filter "system:windows"		
		systemversion "latest"
		pchheader "pch.h"
		staticruntime "On"
		defines {
			"PS_SYSTEM_WINDOWS"
		}
		
	filter "system:linux"
		pchheader "%{prj.name}/src/pch.h"
		defines {
			"PS_SYSTEM_LINUX"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		defines {
			"PS_DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		defines {
			"PS_RELEASE"
		}


