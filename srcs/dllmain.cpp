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
  Vec2 entHead2D;
  D3DCOLOR color;
  if (!pDevice) {
    pDevice = o_pDevice;
  }

  for (int i = 0; i < 32; i++) {
    Ent *curEnt = nullptr;

    curEnt = hack->entList->ents[i].ent;
    if (!hack->checkValidEnt(curEnt))
      continue;
    color = D3DCOLOR_ARGB(0, 0, 0, 0);
    if (curEnt->iTeamNum != hack->localEnt->iTeamNum) {
      color = D3DCOLOR_ARGB(255, 255, 0, 255);
    }
    Vec3 entHead3D = hack->getBonePos(curEnt, 8);
    entHead3D.z += 8;
    if (hack->worldToScreen(curEnt->vecOrigin, entPos2D)) {
      // Draw line to enemy
      // DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 1, color);
      if (hack->worldToScreen(entHead3D, entHead2D)) {
        DrawEspBox2D(entPos2D, entHead2D, 1, color);
        int height = ABS(entPos2D.y - entHead2D.y);
        int dX = (entPos2D.x - entHead2D.x);
        float healthPerc = curEnt->iHealth / 100.f;
        float armorPerc = curEnt->iArmor / 100.f;
        Vec2 botHealth, topHealth, botArmor, topArmor;
        int healthHeight = height * healthPerc;
        int armorHeight = height * armorPerc;
        botHealth.y = botArmor.y = entPos2D.y;
        botHealth.x = entPos2D.x - (height / 4) - 2;
        botArmor.x = entPos2D.x + (height / 4) + 2;
        topHealth.y = entHead2D.y + height - healthHeight;
        topArmor.y = entHead2D.y + height - armorHeight;
        topHealth.x = entPos2D.x - (height / 4) - 2 - (dX * healthPerc);
        topArmor.x = entPos2D.x + (height / 4) + 2 - (dX * armorPerc);
        DrawLine(botHealth, topHealth, 4, D3DCOLOR_ARGB(255, 46, 139, 87));
        DrawLine(botArmor, topArmor, 4, D3DCOLOR_ARGB(255, 30, 144, 255));
      }
    }
  }

  hack->crossHair2D.x = windowWidth / 2;
  hack->crossHair2D.y = windowHeight / 2;
  Vec2 l, r, t, b;
  l = r = t = b = hack->crossHair2D;
  l.x -= hack->crossHairSize;
  r.x += hack->crossHairSize;
  b.y += hack->crossHairSize;
  t.y -= hack->crossHairSize;

  DrawLine(l, r, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
  DrawLine(t, b, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
  // call native function //
  oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {
  //hook
  if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
    // dll_init();
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
  delete hack;
  // std::cout << "Exiting" << std::endl;
  // dll_eject();
  FreeLibraryAndExitThread(hModule, true);
  return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpr) {
  if (reason == DLL_PROCESS_ATTACH) {
    CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
  }
  return TRUE;
}
