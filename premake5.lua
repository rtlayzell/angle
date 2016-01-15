solution "math"
	configurations { "debug", "release" }

	configuration "gmake"
		buildoptions { "-std=c++11", "-Wall", "-Wextra" }

	-- visual studio stuff here.

	project "math-test"
		kind "ConsoleApp"
		language "C++"
		targetdir "build/%{cfg.buildcfg}"

		includedirs { "math/" }

		files { "*.hpp", "*.cpp" }

		filter "configurations:debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		filter "configurations:release"
			defines { "NDEBUG" }
			optimize "On"
