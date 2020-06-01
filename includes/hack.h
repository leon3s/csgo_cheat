#pragma once

#include <iostream>

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset);type name;}

struct Vec2 {
  float x;
  float y;
};

struct Vec3 {
  float x;
  float y;
  float z;
};

struct Vec4 {
  float x;
  float y;
  float z;
  float w;
};

class Ent {
public:
	union {
		// isDormant
		DEFINE_MEMBER_N(bool, isDormant, 0xED);
		// iHealth
		DEFINE_MEMBER_N(int, iHealth, 0x100);
		// vecOrigin
		DEFINE_MEMBER_N(Vec3, vecOrigin, 0x138);
		// iTeamNum
		DEFINE_MEMBER_N(int, iTeamNum, 0xF4);
    // clientID
    DEFINE_MEMBER_N(int, clientId, 0x2FC8);
    // armor value
    DEFINE_MEMBER_N(int, iArmor, 0xB368);

    DEFINE_MEMBER_N(int, boneMatrix, 0x26A8);
	};
};

class EntListObj {
public:
  struct Ent *ent;
  char padding[12];
};

class EntList {
public:
  EntListObj ents[32];
};

class Hack {
public:
  uintptr_t dwEntityList = 0x4D4B134;
  uintptr_t dwLocalPlayer = 0xD36B94;
  uintptr_t dwClientState = 0x589DD4;
  uintptr_t dwClientState_GetLocalPlayer = 0x180;
  uintptr_t dwViewMatrix = 0x4D3CA64;
  uintptr_t   engine = 0x0;
  uintptr_t   client = 0x0;
  Ent       *localEnt;
  EntList   *entList;
  float     viewMatrix[16];

  ID3DXLine *LineL;

  void init();
  void update();
  bool checkValidEnt(Ent *ent);
  bool worldToScreen(Vec3 pos, Vec2 &screen);
  Vec3 getBonePos(Ent *ent, int bone);
};
