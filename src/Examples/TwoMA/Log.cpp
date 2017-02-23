#include "stdafx.h"
//
#include "Globals.h"

FILE *g_log = NULL;


void StartLog()
{
	if(0 == (g_log = _fsopen(g_pluginInfo.logPath, "a+", _SH_DENYWR))) {
		return;
	}

	Log("Started.\n");
}

void CloseLog()
{
	if(!g_log) {
		return;
	}
	Log("Stopped.\n");
	(void)fclose(g_log);
	g_log = 0;
}


void Log(const char *fmt, ...)
{
	if(!g_log) {
		return;
	}
	SYSTEMTIME st;
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	if(FileTimeToSystemTime(&ft, &st)) {
		DWORD uSec = ft.dwLowDateTime;
		uSec %= 10000;
		uSec /= 10;
		const char *fmt1 = "%d%02d%02d %02d:%02d:%02d.%03d.%03d\t";
		fprintf_s(g_log, fmt1, (int)st.wYear, (int)st.wMonth, (int)st.wDay,
			(int)st.wHour, (int)st.wMinute, (int)st.wSecond,
			(int)st.wMilliseconds, (int)uSec);
	}

	va_list va;
	va_start(va, fmt);
	_vfprintf_p(g_log, fmt, va);
	va_end(va);
	fflush(g_log);
}
