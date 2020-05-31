#include "includes.h"
#include <D3d9.h>
#include <d3d9.h>

int windowHeight = 0;
int windowWidth = 0;
HWND window = 0;

BOOL CALLBACK enumWind(HWND handle, LPARAM lp) {
  DWORD procId;

  GetWindowThreadProcessId(handle, &procId);
  if (GetCurrentProcessId() != procId) {
    return TRUE;
  }
  window = handle;
  return FALSE;
}

HWND GetProcessWindow() {
  RECT size;

  EnumWindows(enumWind, 0);
  GetWindowRect(window, &size);
  windowHeight = size.bottom - size.top;
  windowWidth = size.right - size.left;
  windowHeight -= 29;
  windowWidth -= 5;
  return window;
}

 bool GetD3D9Device(void **pTable, size_t size) {
   if (!pTable) {
     return false;
   }

   IDirect3D9 *pD3D = Direct3DCreate9(D3D_SDK_VERSION);

   if (!pD3D) {
     return false;
   }

   IDirect3DDevice9 *pDummyDevice = nullptr;

   D3DPRESENT_PARAMETERS d3dpp = {};
   d3dpp.Windowed = false;
   d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
   d3dpp.hDeviceWindow = GetProcessWindow();

   HRESULT dummyDevCreated = pD3D->CreateDevice(
     D3DADAPTER_DEFAULT,
     D3DDEVTYPE_HAL,
     d3dpp.hDeviceWindow,
     D3DCREATE_SOFTWARE_VERTEXPROCESSING,
     &d3dpp,
     &pDummyDevice
   );

   if (dummyDevCreated != S_OK) {
     d3dpp.Windowed = !d3dpp.Windowed;
     HRESULT dummyDevCreated = pD3D->CreateDevice(
       D3DADAPTER_DEFAULT,
       D3DDEVTYPE_HAL,
       d3dpp.hDeviceWindow,
       D3DCREATE_SOFTWARE_VERTEXPROCESSING,
       &d3dpp,
       &pDummyDevice
     );
     if (dummyDevCreated != S_OK) {
       pD3D->Release();
       return false;
     }
   }
   memcpy(pTable, *(void **)(pDummyDevice), size);
   pDummyDevice->Release();
   pD3D->Release();
   return true;
 }
