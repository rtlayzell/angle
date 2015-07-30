solution "angle"
	configurations { "debug", "release" }
	
	configuration "gmake"
		buildoptions { "-std=c++11", "-Wall", "-Wextra" }
		
	-- visual studio stuff here.
	
	project "angle-test"
		kind "ConsoleApp"
		language "C++"
		targetdir "build/%{cfg.buildcfg}"
		
		includedirs { "include/" }
		
		files { "*.hpp", "*.cpp" }
		
		filter "configurations:debug"
			defines { "DEBUG" }
			flags { "Symbols" }
			
		filter "configurations:release"
			defines { "NDEBUG" }
			optimize "On"
		