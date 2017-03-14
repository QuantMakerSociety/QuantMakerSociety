#include "stdafx.h"
//
#include "Globals.h"

extern "C" {;

__declspec(dllexport) HICON __stdcall PluginGetIcon()
{
	return g_hIconSmall;
}

} // extern "C"