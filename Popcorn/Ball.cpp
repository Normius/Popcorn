#include "Ball.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBall

const float CBall::StartBallPos_Y = 181.0f;
const float CBall::Radius = 2.0f;

//Ctor
CBall::CBall()
    :BallState(EBallState::BallNormal), ballCenterPos_X(0.0f), ballCenterPos_Y(StartBallPos_Y), ballSpeed(0.0f), restDistance(0.0f), ballDirection(0), ballWhitePen(0), ballWhiteBrush(0), BallRect{}, OldBallRect{}
{
    SetState(EBallState::BallNormal, 0);
}

//StartInit for ball
void CBall::Init()
{
    CConfig::CreatePenAndBrush(255, 255, 255, ballWhitePen, ballWhiteBrush);
}

// Draw a ball
void CBall::Draw(HDC hdc, RECT& paintArea)
{
    RECT intersectionRect;

    //Check for intersection levelRECT with paint RECT
    if (IntersectRect(&intersectionRect, &paintArea, &OldBallRect))
    {
        //At first draw a rectangle with platform size and background color before drawing platform itself to clear after redraw
        SelectObject(hdc, CConfig::backgroundPen);
        SelectObject(hdc, CConfig::backgroundBrush);

        Ellipse(hdc, OldBallRect.left, OldBallRect.top, OldBallRect.right - 1, OldBallRect.bottom - 1);
    }
    
    if (IntersectRect(&intersectionRect, &paintArea, &BallRect))
    {
        //Draw a ball
        SelectObject(hdc, ballWhitePen);
        SelectObject(hdc, ballWhiteBrush);

        Ellipse(hdc, BallRect.left, BallRect.top, BallRect.right - 1, BallRect.bottom - 1);
    }
}

//Redraw ball in other position (moving)
void CBall::Move(int platformPos_X,  int platformWidth, CLevel* level, CHitChecker *hitChecker)
{
    bool objectIntersection;
    float nextBallPos_X, nextBallPos_Y = 0.0f;
    float stepSize = 1.0f / CConfig::ResolutionScale; //1 pxl (min distance in game)

    if (BallState != EBallState::BallNormal)
        return;

    OldBallRect = BallRect;
    restDistance += ballSpeed;

    while (restDistance >= stepSize)
    {
        //Calculate new ball direction and position
        nextBallPos_X = ballCenterPos_X + stepSize * cos(ballDirection);
        nextBallPos_Y = ballCenterPos_Y - stepSize * sin(ballDirection);

        objectIntersection = hitChecker->CheckHit(nextBallPos_X, nextBallPos_Y, this);

        ////Check new position for collision with platform
        //if (nextBallPos_Y > CConfig::PlatformPos_Y - CConfig::BallSize)
        //{
        //    if (nextBallPos_X >= platformPos_X && nextBallPos_X <= platformPos_X + platformWidth)
        //    {
        //        nextBallPos_Y = CConfig::PlatformPos_Y - CConfig::BallSize - (nextBallPos_Y - (CConfig::PlatformPos_Y - CConfig::BallSize));
        //        ballDirection = -ballDirection;
        //    }
        //}

        //level->CheckBallHitBrick(nextBallPos_Y, ballDirection);

        if (!objectIntersection)
        {
            //Set new ball position if ball doesn't interact with any object
            ballCenterPos_X = nextBallPos_X;
            ballCenterPos_Y = nextBallPos_Y;

            restDistance -= stepSize;
        }
    }
    ReDraw();
}

void CBall::SetState(EBallState newballstate, float pos_x)
{
    switch (newballstate)
    {
    case BallNormal:
        ballCenterPos_X = pos_x;
        ballCenterPos_Y = StartBallPos_Y;
        ballSpeed = 3.0f;
        restDistance = 0.0f;
        ballDirection = (static_cast<float>(M_PI - M_PI / 4));
        ReDraw();
        break;

    case BallMissing:
        ballSpeed = 0.0f;
        break;

    case BallOnPlatform:
        ballCenterPos_X = pos_x;
        ballCenterPos_Y = StartBallPos_Y;
        ballSpeed = 0.0f;
        restDistance = 0.0f;
        ballDirection = (static_cast<float>(M_PI - M_PI / 4));
        ReDraw();
        break;
    default:
        break;
    }

    BallState = newballstate;
}

EBallState CBall::GetState()
{
    return BallState;
}

void CBall::ReDraw()
{
    BallRect.left = static_cast<int>((ballCenterPos_X - Radius) * CConfig::ResolutionScale);
    BallRect.top = static_cast<int>((ballCenterPos_Y - Radius) * CConfig::ResolutionScale);
    BallRect.right = static_cast<int>((ballCenterPos_X + Radius) * CConfig::ResolutionScale);
    BallRect.bottom = static_cast<int>((ballCenterPos_Y + Radius) * CConfig::ResolutionScale);

    InvalidateRect(CConfig::HWnd, &OldBallRect, FALSE);
    InvalidateRect(CConfig::HWnd, &BallRect, FALSE);
}