project	"TwoMA"
location	"."
kind	"SharedLib"
language	"C++"
targetdir	"%{P64}"
implibdir	"%{L64}"

files {
	"*.h",
	"*.cpp",
	"*.rc",
	"*.ico",
}

vpaths {
	["Resources"]	=
	{
		"*.rc",
		"*.ico",
		"resource.h"
	},
	["Stdafx"]	= "stdafx.*",
	["TwoMA"]	= "TwoMA.*",
}

pchheader "stdafx.h"
pchsource "stdafx.cpp"

links {
	"qms_ta64",
	"qms",
}
