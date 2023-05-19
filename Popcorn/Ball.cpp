#include "Ball.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBall
//Ctor
CBall::CBall()
    :ballPos_X(20), ballPos_Y(170), ballSpeed(3.0f), ballDirection(static_cast<float>(M_PI - M_PI / 4)), ballWhitePen(0), ballWhiteBrush(0), BallRect{}, OldBallRect{}
{
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
    if (!IntersectRect(&intersectionRect, &paintArea, &BallRect))
    {
        return;
    }
    //At first draw a rectangle with platform size and background color before drawing platform itself to clear after redraw
    SelectObject(hdc, CConfig::backgroundPen);
    SelectObject(hdc, CConfig::backgroundBrush);

    Ellipse(hdc, OldBallRect.left, OldBallRect.top, OldBallRect.right - 1, OldBallRect.bottom - 1);

    //Draw a ball
    SelectObject(hdc, ballWhitePen);
    SelectObject(hdc, ballWhiteBrush);

    Ellipse(hdc, BallRect.left, BallRect.top, BallRect.right - 1, BallRect.bottom - 1);
}

//Redraw ball in other position (moving)
void CBall::Move(CLevel* level, int platformPos_X,  int platformWidth)
{
    int nextBallPos_X, nextBallPos_Y;
    int maxLevelPos_x = CConfig::MaxLevelPos_X - CConfig::BallSize; //correcting max ball position accounting ball size for X
    int maxLevelPos_y = CConfig::MaxLevelPos_Y - CConfig::BallSize; //correcting max ball position accounting ball size for Y

    OldBallRect = BallRect;

    //Calculate new ball direction and position
    nextBallPos_X = ballPos_X + static_cast<int>(ballSpeed * cos(ballDirection));
    nextBallPos_Y = ballPos_Y - static_cast<int>(ballSpeed * sin(ballDirection));

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
        nextBallPos_Y = maxLevelPos_y - (nextBallPos_Y - maxLevelPos_y);
        ballDirection = -ballDirection;
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

    BallRect.left = ballPos_X * CConfig::ResolutionScale;
    BallRect.top = ballPos_Y * CConfig::ResolutionScale;
    BallRect.right = BallRect.left + CConfig::BallSize * CConfig::ResolutionScale;
    BallRect.bottom = BallRect.top + CConfig::BallSize * CConfig::ResolutionScale;

    InvalidateRect(CConfig::HWnd, &OldBallRect, FALSE);
    InvalidateRect(CConfig::HWnd, &BallRect, FALSE);
}