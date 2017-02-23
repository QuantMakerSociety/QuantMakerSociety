#pragma once

extern "C" {;
/**
* DLL Entry point.
* @param hInstance Handle of DLL.
* @param dwReason Reason for calling the DLL.
* @param lpReserved Reserved for use by windows.
* @return 1 on success 0 on failure.
*/
int WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

} // extern "C"
