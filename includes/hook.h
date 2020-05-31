#pragma once

#ifndef HOOK_H
#define HOOK_H

void Patch(BYTE *dst, BYTE *src, unsigned int size);

bool Hook(char *src, char *dst, int len);

char *TrampHook(char *src, char *dst, unsigned int len);

#endif
