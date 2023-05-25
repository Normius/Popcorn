#pragma once

#include <Windows.h>

#include "Border.h"
#include "Platform.h"
#include "Level.h"

enum EKeyType
{
    LeftArrowKey,
    RightArrowKey,
    SpaceKey
};

enum EGameState
{
    LevelPlaying,
    BallLost,
    RestartLevel
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
    EGameState GameState;

    CBall Ball;
    CLevel Level;
    CPlatform Platform;
    CBorder Border;
};

