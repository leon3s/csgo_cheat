#include "includes.h"

// data
void *d3d9Device[119];

BYTE EndSceneBytes[7]{ 0 };

tEndScene oEndScene = nullptr;

LPDIRECT3DDEVICE9 pDevice = nullptr;

Hack *hack;

// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
  Vec2 entPos2D;
  D3DCOLOR color;

  if (!pDevice) {
    pDevice = o_pDevice;
  }
  DrawFilledRect(10, 10, 10, 10, D3DCOLOR_ARGB(255, 255, 255, 1));
  for (int i = 0; i < 32; i++) {
    Ent *curEnt = nullptr;

    curEnt = hack->entList->ents[i].ent;
    if (!hack->checkValidEnt(curEnt))
      continue;
    color = D3DCOLOR_ARGB(0, 0, 0, 0);
    if (curEnt->iTeamNum != hack->localEnt->iTeamNum) {
        color = D3DCOLOR_ARGB(255, 255, 0, 0);
    }
    if (hack->worldToScreen(curEnt->vecOrigin, entPos2D)) {
      DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 1, color);
    }
  }
  oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {
  hack = new Hack();
  hack->init();
  //hook
  if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
    memcpy(EndSceneBytes, (char *)d3d9Device[42], 7);
    oEndScene = (tEndScene)TrampHook((char *)d3d9Device[42], (char *)hkEndScene, 7);
  }
  while (!GetAsyncKeyState(VK_END)) {
    hack->update();
  }
  //unhook
  Patch((BYTE *)d3d9Device[42], EndSceneBytes, 7);
  // uninject
  FreeLibraryAndExitThread(hModule, 0);
}

FILE *dll_init() {
  FILE *f;

  AllocConsole();
  freopen_s(&f, "CONIN$", "r", stdin);
  freopen_s(&f, "CONOUT$", "w", stdout);
  freopen_s(&f, "CONOUT$", "w", stderr);
  return f;
}

void dll_free(FILE *f) {
  fclose(f);
  FreeConsole();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
  FILE *f = nullptr;

  f = dll_init();
  if (reason == DLL_PROCESS_ATTACH)
    CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
  if (reason == DLL_PROCESS_DETACH)
    dll_free(f);
  if (reason == DLL_THREAD_DETACH) {
    dll_free(f);
  }
  return TRUE;
}
