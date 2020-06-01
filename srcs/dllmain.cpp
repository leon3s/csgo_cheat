#include "includes.h"

// data
void *d3d9Device[119];

BYTE EndSceneBytes[7]{ 0 };

tEndScene oEndScene = nullptr;

LPDIRECT3DDEVICE9 pDevice = nullptr;

Hack *hack;
FILE *f;

void dll_init() {
  AllocConsole();
  freopen_s(&f, "CONIN$", "r", stdin);
  freopen_s(&f, "CONOUT$", "w", stdout);
  freopen_s(&f, "CONOUT$", "w", stderr);
}

void dll_eject() {
  FreeConsole();
  fclose(f);
}

// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
  Vec2 entPos2D;
  D3DCOLOR color;

  if (!pDevice) {
    pDevice = o_pDevice;
  }
  // crosshair
  DrawFilledRect(
    (windowWidth / 2) - 1,
    (windowHeight / 2),
    2,
    2,
    D3DCOLOR_ARGB(255, 255, 255, 255)
  );
  DrawFilledRect(0, 0, 10, 10, D3DCOLOR_ARGB(255, 255, 255, 1));
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
  //hook
  if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
    memcpy(EndSceneBytes, (char *)d3d9Device[42], 7);
    oEndScene = (tEndScene)TrampHook((char *)d3d9Device[42], (char *)hkEndScene, 7);
  }
  dll_init();
  hack = new Hack();
  hack->init();
  while (!GetAsyncKeyState(VK_END)) {
    hack->update();
  }
  //unhook
  Patch((BYTE *)d3d9Device[42], EndSceneBytes, 7);
  delete hack;
  std::cout << "Exiting" << std::endl;
  dll_eject();
  FreeLibraryAndExitThread(hModule, true);
  return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpr) {
  if (reason == DLL_PROCESS_ATTACH) {
    CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
  }
  return TRUE;
}
