#include "includes.h"

void Hack::init() {
  this->client = (uintptr_t)GetModuleHandle("client_panorama.dll");
  this->engine = (uintptr_t)GetModuleHandle("engine.dll");
  this->entList = (EntList *)(this->client + this->dwEntityList);
  this->localEnt = *(Ent **)(this->client + this->dwLocalPlayer);
}

void Hack::update() {
  memcpy(&this->viewMatrix, (PBYTE *)(this->client + this->dwViewMatrix), sizeof(this->viewMatrix));
}

bool Hack::checkValidEnt(Ent* ent) {
  if (ent == nullptr) {
    return false;
  }
  if (ent == this->localEnt) {
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

Vec3 Hack::getBonePos(Ent *ent, int bone) {
  uintptr_t bonePtr = ent->boneMatrix;

  Vec3 bonePos;
  bonePos.x = *(float *)(bonePtr + 0x30 * bone + 0x0c);
  bonePos.y = *(float *)(bonePtr + 0x30 * bone + 0x1c);
  bonePos.z = *(float *)(bonePtr + 0x30 * bone + 0x2c);
  return bonePos;
}
