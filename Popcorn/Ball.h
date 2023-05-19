#pragma once

#include "Config.h"
#include "Level.h"

enum EBallState
{
    BallNormal,
    BallMissing,
    BallOnPlatform
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBall
class CBall
{
public:
    CBall();

    void Init();
    void Draw(HDC hdc, RECT& painArea);
    void Move(CLevel* level, int platformPos_X, int platformWidth);
    void SetState(EBallState newballstate, int pos_x);
    EBallState GetState();

private:
    void ReDraw();

    float ballPos_X;
    float ballPos_Y;
    float ballSpeed;
    float ballDirection;

    EBallState BallState;

    HPEN ballWhitePen;
    HBRUSH ballWhiteBrush;
    RECT BallRect, OldBallRect;

    static const float StartBallPos_Y;
};
