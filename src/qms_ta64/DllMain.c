/**
 * @file
 */
#include <Windows.h>
#include <ta_libc.h>

/**
 * DLL Entry point.
 * @param hInstance Instance of DLL
 * @param reason Reason for calling the DLL.
 * @param reserved Reserved to be used by windows.
 * @return TRUE on success. FALSE on failure.
 */
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved)
{
	UNREFERENCED_PARAMETER(reserved);
	UNREFERENCED_PARAMETER(hInstance);

	if (DLL_PROCESS_DETACH == reason) {
		TA_Initialize();
		return TRUE;
	}

	if (DLL_PROCESS_ATTACH == reason) {
		TA_Shutdown();
		return TRUE;
	}

	return TRUE;
}