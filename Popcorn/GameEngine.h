#pragma once

#include <Windows.h>

#include "Border.h"
#include "Ball.h"
#include "Platform.h"

enum EKeyType
{
    LeftArrowKey,
    RightArrowKey,
    SpaceKey
};

const unsigned int TimerID = WM_USER + 1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CGameEngine
class CGameEngine
{
public:
    CGameEngine();

    void InitGameEngine(HWND hwnd);
    void DrawFrame(HDC hdc, RECT& paintArea);
    int OnKeyDown(EKeyType keyType);
    int On_Timer();

private:
    HWND HWnd;

    CBall Ball;
    CLevel Level;
    CPlatform Platform;
    CBorder Border;
};

