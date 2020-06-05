#include "includes.h"

const int generalRatio = 12;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
  D3DRECT rect = {x, y, x + w, y + h};
  pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, int tickness, D3DCOLOR color) {
  D3DXVECTOR2 Line[2];

  if (!hack->LineL)
    D3DXCreateLine(pDevice, &hack->LineL);

  Line[0] = D3DXVECTOR2(x1, y1);
  Line[1] = D3DXVECTOR2(x2, y2);
  hack->LineL->SetWidth(tickness);
  hack->LineL->Draw(Line, 2, color);
}

void DrawLine(Vec2 src, Vec2 dst, int tickness, D3DCOLOR color) {
  DrawLine(src.x, src.y, dst.x, dst.y, tickness, color);
}

void DrawEspBox2D(Vec2 top, Vec2 bot, int tickness, int ratio, D3DCOLOR color) {
  int height = ABS(top.y - bot.y);

  Vec2 tl, tr;
  tl.x = top.x - height / ratio;
  tr.x = top.x + height / ratio;
  tl.y = tr.y = top.y;

  Vec2 bl, br;
  bl.x = bot.x - height / ratio;
  br.x = bot.x + height / ratio;
  bl.y = br.y = bot.y;

  DrawLine(tl, tr, tickness, color);
  DrawLine(bl, br, tickness, color);
  DrawLine(tl, bl, tickness, color);
  DrawLine(tr, br, tickness, color);
}

void DrawProgressBar(Vec2 entPos2D, Vec2 entHead2D, int value, int height, int dX, int offset, int tickness, D3DCOLOR color) {
  Vec2 bot, top;
  float perc = value / 100.f;
  int barHeight = height * perc;
  bot.y = entPos2D.y;
  bot.x = entPos2D.x - (height / generalRatio);
  bot.x = bot.x + offset;
  top.y = entHead2D.y + height - barHeight;
  top.x = entPos2D.x - (height / generalRatio);
  top.x = top.x + offset - (dX * perc);
  DrawLine(bot, top, tickness, color);
}

void DrawCrosshair(Vec2 crossHair2D, int crossHairSize) {
  Vec2 l, r, t, b;
  l = r = t = b = crossHair2D;
  l.x -= crossHairSize;
  r.x += crossHairSize;
  b.y += crossHairSize;
  t.y -= crossHairSize;

  DrawLine(l, r, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
  DrawLine(t, b, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void DrawEnnemy(Ent* curEnt, Vec2 entPos2D, Vec2 entHead2D) {
  int height = ABS(entPos2D.y - entHead2D.y);
  int dX = (entPos2D.x - entHead2D.x);
  DrawEspBox2D(entPos2D, entHead2D, 2, generalRatio, D3DCOLOR_ARGB(255, 255, 0, 255));
  DrawProgressBar(
    entPos2D,
    entHead2D,
    curEnt->iHealth,
    height,
    dX,
    -6,
    4,
    D3DCOLOR_ARGB(255, 46, 139, 87)
  );
  DrawProgressBar(
    entPos2D,
    entHead2D,
    curEnt->iArmor,
    height,
    dX,
    -12,
    4,
    D3DCOLOR_ARGB(255, 30, 144, 255)
  );
}
