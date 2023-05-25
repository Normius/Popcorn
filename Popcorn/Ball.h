#pragma once

#include "Config.h"

enum EBallState
{
    BallNormal,
    BallMissing,
    BallOnPlatform
};

class CBall;
class CHitChecker
{
public:
    virtual bool CheckHit(float nextBallPos_X, float nextBallPos_Y, CBall* ball) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBall
class CBall
{
public:
    CBall();

    void Init();
    void Draw(HDC hdc, RECT& painArea);
    void Move(CHitChecker* levelhitchecker, CHitChecker* borderhitChecker, CHitChecker* platformhitChecker);
    void SetState(EBallState newballstate, float pos_x);

    EBallState GetState();
    
    static void AddHitChecker(CHitChecker *hitchecker);

    float ballDirection;
    

    static const float Radius;

private:
    void ReDraw();

    float ballCenterPos_X;
    float ballCenterPos_Y;
    float ballSpeed;
    float restDistance; //ball can move more than 1 pxl per timer, but we need to account crossing and collision with other object every pxl. Otherwise ball can slip through something. So we divide his movement into min parts (1 pxl)

    EBallState BallState;

    HPEN ballWhitePen;
    HBRUSH ballWhiteBrush;
    RECT BallRect, OldBallRect;

    static const float StartBallPos_Y;

    static int hitCheckersCount;
    static CHitChecker* HitCheckers[3];
};
