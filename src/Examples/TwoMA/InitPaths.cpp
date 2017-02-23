#include "stdafx.h"
//
#include "Globals.h"

int InitPaths()
{
	::SetLastError(0);
	DWORD len = ::GetModuleFileNameA(g_hInstance, g_dllPath, sizeof(g_dllPath));
	if(!len) {
		return -1;
	}

	// Find the name of the dll.
	const char *dllName;
	for(dllName = g_dllPath + len; dllName > g_dllPath; --dllName) {
		if('\\' == *dllName) {
			++dllName;
			break;
		}
	}

	if(dllName == g_dllPath) {
		return -1;
	}

	// Based on the path, get the working directory.
	char *p = g_workDir;
	for(const char *cp = g_dllPath; cp != dllName; ++cp) {
		*p++ = *cp;
	}
	*p = 0;

	// Copy the name of the dll.
	strcpy(g_dllName, dllName);

	// Take out the extension form the name.
	p = g_dllName;
	while(*p) {
		++p;
	}

	while(p > g_dllName) {
		if('.' == *p) {
			*p = 0;
			break;
		}
		--p;
	}

	// Calculate configuration path.
	sprintf_s(g_dllCfg, sizeof(g_dllCfg), "%s%s.cfg", g_workDir, g_dllName);

	strcpy_s(g_pluginInfo.name, sizeof(g_pluginInfo.name), g_dllName);

	// Calculate log path.
	sprintf_s(g_pluginInfo.logPath, sizeof(g_pluginInfo.logPath), "%s%s.log",
		g_workDir, g_dllName);

	return 0;
}

