#include "includes.h"

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
  // hack->LineL->Release();
}

void DrawLine(Vec2 src, Vec2 dst, int tickness, D3DCOLOR color) {
  DrawLine(src.x, src.y, dst.x, dst.y, tickness, color);
}

void DrawEspBox2D(Vec2 top, Vec2 bot, int tickness, D3DCOLOR color) {
  int height = ABS(top.y - bot.y);

  Vec2 tl, tr;
  tl.x = top.x - height / 4;
  tr.x = top.x + height / 4;
  tl.y = tr.y = top.y;

  Vec2 bl, br;
  bl.x = bot.x - height / 4;
  br.x = bot.x + height / 4;
  bl.y = br.y = bot.y;

  DrawLine(tl, tr, tickness, color);
  DrawLine(bl, br, tickness, color);
  DrawLine(tl, bl, tickness, color);
  DrawLine(tr, br, tickness, color);
}
