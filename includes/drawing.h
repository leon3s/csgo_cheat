#pragma once

#include "hack.h"

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col);
void DrawLine(int x1, int y1, int x2, int y2, int tickness, D3DCOLOR color);
void DrawLine(Vec2 src, Vec2 dst, int tickness, D3DCOLOR color);
void DrawCrosshair(Vec2 crossHair2D, int crossHairSize);
void DrawEspBox2D(Vec2 top, Vec2 bot, int tickness, int ratio, D3DCOLOR color);
void DrawProgressBar(Vec2 entPos2D, Vec2 entHead2D, int value, int height, int dX, int offset, int tickness, D3DCOLOR color);
void DrawEnnemy(Ent* curEnt, Vec2 entPos2D, Vec2 entHead2D);
