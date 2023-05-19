#include "Ball.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBall

const float CBall::StartBallPos_Y = 181.0f;

//Ctor
CBall::CBall()
    :BallState(EBallState::BallNormal), ballPos_X(0.0f), ballPos_Y(StartBallPos_Y), ballSpeed(0.0f), ballDirection(0), ballWhitePen(0), ballWhiteBrush(0), BallRect{}, OldBallRect{}
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
void CBall::Move(CLevel* level, int platformPos_X,  int platformWidth)
{
    float nextBallPos_X, nextBallPos_Y = 0.0f;
    int maxLevelPos_x = CConfig::MaxLevelPos_X - CConfig::BallSize; //correcting max ball position accounting ball size for X
    int maxLevelPos_y = CConfig::MaxLevelPos_Y - CConfig::BallSize; //correcting max ball position accounting ball size for Y

    if (BallState != EBallState::BallNormal)
        return;

    OldBallRect = BallRect;

    //Calculate new ball direction and position
    nextBallPos_X = ballPos_X + ballSpeed * cos(ballDirection);
    nextBallPos_Y = ballPos_Y - ballSpeed * sin(ballDirection);

    //Check new position for collision with level border and change direction and new position consider reflection
    //Left border reflection
    if (nextBallPos_X < CConfig::BorderOffset_X)
    {
        nextBallPos_X = CConfig::BorderOffset_X - (nextBallPos_X - CConfig::BorderOffset_X);
        ballDirection = static_cast<float>(M_PI) - ballDirection;
    }
    //Top border reflection
    if (nextBallPos_Y < CConfig::BorderOffset_Y)
    {
        nextBallPos_Y = CConfig::BorderOffset_Y - (nextBallPos_Y - CConfig::BorderOffset_Y);
        ballDirection = -ballDirection;
    }
    //Right border reflection
    if (nextBallPos_X > maxLevelPos_x)
    {
        nextBallPos_X = maxLevelPos_x - (nextBallPos_X - (maxLevelPos_x)); //consider ball speed (MaxLevelPos_X + BallSpeed)
        ballDirection = static_cast<float>(M_PI) - ballDirection;
    }
    //Bottom border reflection
    if (nextBallPos_Y > maxLevelPos_y)
    {
        if (level->HasFloor)
        {
            nextBallPos_Y = maxLevelPos_y - (nextBallPos_Y - maxLevelPos_y);
            ballDirection = -ballDirection;
        }
        else
        {
            if (nextBallPos_Y > maxLevelPos_y + CConfig::BallSize + 1) //allows ball to fall below the screen
                BallState = EBallState::BallMissing;
        }
    }

    //Check new position for collision with platform
    if (nextBallPos_Y > CConfig::PlatformPos_Y - CConfig::BallSize)
    {
        if (nextBallPos_X >= platformPos_X && nextBallPos_X <= platformPos_X + platformWidth)
        {
            nextBallPos_Y = CConfig::PlatformPos_Y - CConfig::BallSize - (nextBallPos_Y - (CConfig::PlatformPos_Y - CConfig::BallSize));
            ballDirection = -ballDirection;
        }
    }

    level->CheckBallHitBrick(nextBallPos_Y, ballDirection);

    //Set new ball position
    ballPos_X = nextBallPos_X;
    ballPos_Y = nextBallPos_Y;

    ReDraw();
}

void CBall::SetState(EBallState newballstate, int pos_x)
{
    switch (newballstate)
    {
    case BallNormal:
        ballPos_X = static_cast<float>(pos_x - CConfig::BallSize / 2);
        ballPos_Y = StartBallPos_Y;
        ballSpeed = 3.0f;
        ballDirection = (static_cast<float>(M_PI - M_PI / 4));
        ReDraw();
        break;

    case BallMissing:
        ballSpeed = 0.0f;
        break;

    case BallOnPlatform:
        ballPos_X = static_cast<float>(pos_x - CConfig::BallSize / 2);
        ballPos_Y = StartBallPos_Y;
        ballSpeed = 0.0f;
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
    BallRect.left = static_cast<int>(ballPos_X * CConfig::ResolutionScale);
    BallRect.top = static_cast<int>(ballPos_Y * CConfig::ResolutionScale);
    BallRect.right = BallRect.left + CConfig::BallSize * CConfig::ResolutionScale;
    BallRect.bottom = BallRect.top + CConfig::BallSize * CConfig::ResolutionScale;

    InvalidateRect(CConfig::HWnd, &OldBallRect, FALSE);
    InvalidateRect(CConfig::HWnd, &BallRect, FALSE);
}