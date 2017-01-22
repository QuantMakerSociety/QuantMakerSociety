project	"qms"
location	"."
kind	"StaticLib"
language	"C++"
targetdir	"%{L64}"

files {
	"%{INC}/qms/*.h",
	"*.h",
	"*.cpp"
}

vpaths {
	["Stdafx"]	= "stdafx.*",
	["include/qms"]	= "%{INC}/qms/*.h",
	["Headers"]	= "*.h",
	["Source"]	= "*.cpp",
}

pchheader "stdafx.h"
pchsource "stdafx.cpp"

