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
  freopen_s(&f, "CONOUT$", "w", stdout);
}

void dll_eject() {
  FreeConsole();
  fclose(f);
}

// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
  Vec2 entPos2D;
  Vec2 entHead2D;
  if (!pDevice) {
    pDevice = o_pDevice;
  }
  // Draw crosshair
  hack->crossHair2D.x = windowWidth / 2;
  hack->crossHair2D.y = windowHeight / 2;
  DrawCrosshair(hack->crossHair2D, hack->crossHairSize);
  for (int i = 0; i < 32; i++) {
    Ent *curEnt = nullptr;
    curEnt = hack->entList->ents[i].ent;
    if (!hack->checkValidEnt(curEnt))
      continue;
    Vec3 entHead3D = hack->getBonePos(curEnt, 8);
    entHead3D.z += 8;
    if (hack->worldToScreen(curEnt->vecOrigin, entPos2D)) {
      // Draw line to enemy
      // DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 1, color);
      if (hack->worldToScreen(entHead3D, entHead2D)) {
        DrawEnnemy(curEnt, entPos2D, entHead2D);
      }
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
  hack = new Hack();
  hack->init();
  while (!GetAsyncKeyState(VK_END)) {
    hack->update();
    // Vec3 pAng = hack->localEnt->aimPunchAngle;
    // hack->crossHair2D.x = windowWidth / 2 - (windowWidth / 90 * pAng.y);
    // hack->crossHair2D.y = windowHeight / 2 - (windowHeight / 90 * pAng.x);
  }
  //unhook
  Patch((BYTE *)d3d9Device[42], EndSceneBytes, 7);
  hack->LineL->Release();
  delete hack;
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
