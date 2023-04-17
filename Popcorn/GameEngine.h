#pragma once

#include <Windows.h>

enum EKeyType
{
    LeftArrowKey,
    RightArrowKey,
    SpaceKey
};

const unsigned int TimerID = WM_USER + 1;

void InitGameEngine(HWND hwnd);

void DrawFrame(HDC hdc, RECT& paintarea);

int OnKeyDown(EKeyType keyType);

int On_Timer();