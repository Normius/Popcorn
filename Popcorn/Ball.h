#pragma once

#include "Config.h"
#include "Level.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBall
class CBall
{
public:
    CBall();

    void Init();

    void Draw(HDC hdc, RECT& painArea, HPEN bgBlackPen, HBRUSH bgBlackBrush);
    void Move(HWND hWnd, CLevel* level, int platformPos_X, int platformWidth);

private:
    int BallPos_X;
    int BallPos_Y;
    float BallSpeed;
    float BallDirection;

    HPEN ballWhitePen;
    HBRUSH ballWhiteBrush;
    RECT BallRect, OldBallRect;
};
