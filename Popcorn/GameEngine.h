#pragma once

#include <Windows.h>

enum EKeyType
{
    LeftArrowKey,
    RightArrowKey,
    SpaceKey
};

void InitGameEngine(HWND hwnd);

void DrawFrame(HDC hdc, RECT& paintarea);

int OnKeyDown(EKeyType keyType);