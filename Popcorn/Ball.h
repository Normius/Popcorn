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

    void Draw(HDC hdc, RECT& painArea);
    void Move(CLevel* level, int platformPos_X, int platformWidth);

private:
    int ballPos_X;
    int ballPos_Y;
    float ballSpeed;
    float ballDirection;

    HPEN ballWhitePen;
    HBRUSH ballWhiteBrush;
    RECT BallRect, OldBallRect;
};
