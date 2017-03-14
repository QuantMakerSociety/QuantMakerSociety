#include "stdafx.h"
//
#include "Globals.h"
#include "resource.h"

extern "C" {

/**
 * DLL Entry point.
 *
 * @param[in]  hInstance DLL Instance.
 * @param[in]  dwReason Reason for calling.
 * @param[in]  lpReserved Reserved for use by window.s
 *
 * @return TRUE on success. FALSE on failure.
 */
int WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  UNREFERENCED_PARAMETER(lpReserved);
  switch (dwReason) {
  case DLL_PROCESS_DETACH: {
    CloseLog();
  } break;

  case DLL_PROCESS_ATTACH: {
    g_hInstance  = hInstance;
    g_hIconSmall = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(MAIN_ICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

    if (0 > InitPaths()) {
      return FALSE;
    }

    StartLog();
    g_pluginInfo.cfg.Clear();
    for (int i = 0; i < eCfgItem_count; i++) {
      g_pluginInfo.cfg.AddPtr(&g_cfg[i]);
    }
  } break;

  case DLL_THREAD_ATTACH: {
  } break;

  case DLL_THREAD_DETACH: {
  } break;

  default: {
  } break;
  }
  return TRUE;
}

} // extern "C"
