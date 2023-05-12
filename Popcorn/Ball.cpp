#include "Ball.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBall
//Ctor
CBall::CBall()
    :BallPos_X(20), BallPos_Y(170), BallSpeed(3.0f), BallDirection(static_cast<float>(M_PI - M_PI / 4)), ballWhitePen(0), ballWhiteBrush(0), BallRect{}, OldBallRect{}
{
}

//StartInit for ball
void CBall::Init()
{
    CConfig::CreatePenAndBrush(255, 255, 255, ballWhitePen, ballWhiteBrush);
}

// Draw a ball
void CBall::Draw(HDC hdc, RECT& paintArea, HPEN bgBlackPen, HBRUSH bgBlackBrush)
{
    RECT intersectionRect;

    //Check for intersection levelRECT with paint RECT
    if (!IntersectRect(&intersectionRect, &paintArea, &BallRect))
    {
        return;
    }
    //At first draw a rectangle with platform size and background color before drawing platform itself to clear after redraw
    SelectObject(hdc, bgBlackPen);
    SelectObject(hdc, bgBlackBrush);

    Ellipse(hdc, OldBallRect.left, OldBallRect.top, OldBallRect.right - 1, OldBallRect.bottom - 1);

    //Draw a ball
    SelectObject(hdc, ballWhitePen);
    SelectObject(hdc, ballWhiteBrush);

    Ellipse(hdc, BallRect.left, BallRect.top, BallRect.right - 1, BallRect.bottom - 1);
}

//Redraw ball in other position (moving)
void CBall::Move(HWND hWnd, CLevel* level, int platformPos_X,  int platformWidth)
{
    int nextBallPos_X, nextBallPos_Y;

    OldBallRect = BallRect;

    //Calculate new ball direction and position
    nextBallPos_X = BallPos_X + static_cast<int>(BallSpeed * cos(BallDirection));
    nextBallPos_Y = BallPos_Y - static_cast<int>(BallSpeed * sin(BallDirection));

    //Check new position for collision with level border and change direction and new position consider reflection
    //Left border reflection
    if (nextBallPos_X < CConfig::BorderOffset_X)
    {
        nextBallPos_X = CConfig::BorderOffset_X - (nextBallPos_X - CConfig::BorderOffset_X);
        BallDirection = static_cast<float>(M_PI) - BallDirection;
    }
    //Top border reflection
    if (nextBallPos_Y < CConfig::BorderOffset_Y)
    {
        nextBallPos_Y = CConfig::BorderOffset_Y - (nextBallPos_Y - CConfig::BorderOffset_Y);
        BallDirection = -BallDirection;
    }
    //Right border reflection
    if (nextBallPos_X > CConfig::MaxLevelPos_X - CConfig::BallSize)
    {
        nextBallPos_X = CConfig::MaxLevelPos_X - CConfig::BallSize - (nextBallPos_X - (CConfig::MaxLevelPos_X - CConfig::BallSize)); //consider ball speed (MaxLevelPos_X + BallSpeed)
        BallDirection = static_cast<float>(M_PI) - BallDirection;
    }
    //Bottom border reflection
    if (nextBallPos_Y > CConfig::MaxLevelPos_Y - CConfig::BallSize)
    {
        nextBallPos_Y = CConfig::MaxLevelPos_Y - CConfig::BallSize - (nextBallPos_Y - (CConfig::MaxLevelPos_Y - CConfig::BallSize));
        BallDirection = static_cast<float>(M_PI) + static_cast<float>(M_PI) - BallDirection;
    }

    //Check new position for collision with platform
    if (nextBallPos_Y > CConfig::PlatformPos_Y - CConfig::BallSize)
    {
        if (nextBallPos_X >= platformPos_X && nextBallPos_X <= platformPos_X + platformWidth)
        {
            nextBallPos_Y = CConfig::PlatformPos_Y - CConfig::BallSize - (nextBallPos_Y - (CConfig::PlatformPos_Y - CConfig::BallSize));
            BallDirection = static_cast<float>(M_PI) + static_cast<float>(M_PI) - BallDirection;
        }
    }

    level->CheckBallHitBrick(nextBallPos_Y, BallDirection);

    //Set new ball position
    BallPos_X = nextBallPos_X;
    BallPos_Y = nextBallPos_Y;

    BallRect.left = BallPos_X * CConfig::ResolutionScale;
    BallRect.top = BallPos_Y * CConfig::ResolutionScale;
    BallRect.right = BallRect.left + CConfig::BallSize * CConfig::ResolutionScale;
    BallRect.bottom = BallRect.top + CConfig::BallSize * CConfig::ResolutionScale;

    InvalidateRect(hWnd, &OldBallRect, FALSE);
    InvalidateRect(hWnd, &BallRect, FALSE);
}