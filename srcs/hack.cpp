#include "includes.h"

void Hack::init() {
  this->client = (uintptr_t)GetModuleHandle("client_panorama.dll");
  this->engine = (uintptr_t)GetModuleHandle("engine.dll");
  this->localEnt = *(Ent **)(this->client + this->dwLocalPlayer);
  this->entList = (EntList *)(this->client + this->dwEntityList);
}

void Hack::update() {
  this->localEnt = *(Ent **)(this->client + this->dwLocalPlayer);
  this->entList = (EntList *)(this->client + this->dwEntityList);
  int playerIndex = *(int *)(this->engine + this->dwClientState_GetLocalPlayer);

  std::cout << "local player index : " << playerIndex << std::endl;
  std::cout << " hack init function ! localEnt iHealth | iArmor " << this->localEnt->iHealth << " " << this->localEnt->iArmor << std::endl;
  memcpy(&this->viewMatrix, (PBYTE *)(this->client + this->dwViewMatrix), sizeof(this->viewMatrix));
}

bool Hack::checkValidEnt(Ent* ent) {
  if (ent == nullptr) {
    return false;
  }
  if (ent->iHealth <= 0) {
    return false;
  }
  if (ent->isDormant) {
    return false;
  }
  return true;
}

bool Hack::worldToScreen(Vec3 pos, Vec2& screen) {
	Vec4 clipCoords;

	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];
	if (clipCoords.w < 0.1f)
		return false;
	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}
