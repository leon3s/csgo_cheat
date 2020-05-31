#pragma once
#ifndef DX_H
#define DX_H

extern HWND window;

typedef HRESULT(APIENTRY *tEndScene)(LPDIRECT3DDEVICE9 pDevice);

bool GetD3D9Device(void **pTable, size_t size);
HWND GetProcessWindow();

extern int windowHeight, windowWidth;
extern LPDIRECT3DDEVICE9 pDevice;

#endif
