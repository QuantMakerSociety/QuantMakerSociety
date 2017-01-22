project	"qms_ta64"
location	"."
kind	"SharedLib"
language	"C"
targetdir	"%{B64}"
implibdir	"%{L64}"

defines {
	"TA_LIB_API=__declspec(dllexport)"
}

files {
	"%{INC}/qms_ta64/*.h",
	"*.h",
	"*.c"
}

includedirs {
	"%{INC}/qms_ta64"
}

vpaths {
	["include/qms_ta64"]	= "%{INC}/qms_ta64/*.h",
	["Headers"]	= "*.h",
	["Source/ta"]	= "ta_*.c",
	["Source/table"]	= "table*.c",
	["Source/DllMain"]	= "DllMain.c",
}


