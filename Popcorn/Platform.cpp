#include "Platform.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPlatform
//Ctor
CPlatform::CPlatform()
    :Width(28), Pos_X((CConfig::BorderOffset_X + CConfig::MaxLevelPos_X - Width) / 2), Step_X(CConfig::ResolutionScale), SpaceBetweenCircles(21), PlatformRect{},
    OldPlatformRect{}, platformOrangePen(0), platformGreenPen(0), platformGreyPen(0), platformOrangeBrush(0), platformGreenBrush(0), platformGreyBrush(0)
{
}

//Start init
void CPlatform::Init()
{
    CConfig::CreatePenAndBrush(255, 128, 0, platformOrangePen, platformOrangeBrush);

    CConfig::CreatePenAndBrush(0, 128, 0, platformGreenPen, platformGreenBrush);

    CConfig::CreatePenAndBrush(225, 225, 225, platformGreyPen, platformGreyBrush);

    //Initialize first platform position for background rectangle (for clear background after moving platform)
    PlatformRect.left = Pos_X * CConfig::ResolutionScale;
    PlatformRect.top = CConfig::PlatformPos_Y * CConfig::ResolutionScale;
    PlatformRect.right = PlatformRect.left + Width * CConfig::ResolutionScale;
    PlatformRect.bottom = PlatformRect.top + Height * CConfig::ResolutionScale;

}

// Draw platform
void CPlatform::Draw(HDC hdc, RECT& paintArea, HPEN bgBlackPen, HBRUSH bgBlackBrush)
{
    int x = Pos_X;
    int y = CConfig::PlatformPos_Y;

    RECT intersectionRect;
    if (!IntersectRect(&intersectionRect, &paintArea, &PlatformRect))
    {
        return;
    }
    //At first draw a rectangle with platform size and background color before drawing platform itself to clear after redraw
    SelectObject(hdc, bgBlackPen);
    SelectObject(hdc, bgBlackBrush);

    Rectangle(hdc, OldPlatformRect.left, OldPlatformRect.top, OldPlatformRect.right, OldPlatformRect.bottom);

    //Cirlce platform side parts
    SelectObject(hdc, platformOrangePen);
    SelectObject(hdc, platformOrangeBrush);

    Ellipse(hdc, x * CConfig::ResolutionScale, y * CConfig::ResolutionScale, (x + CircleSize) * CConfig::ResolutionScale, (y + CircleSize) * CConfig::ResolutionScale); //Draw a ellipse in rectangle
    Ellipse(hdc, (x + SpaceBetweenCircles) * CConfig::ResolutionScale, y * CConfig::ResolutionScale, (x + SpaceBetweenCircles + CircleSize) * CConfig::ResolutionScale, (y + CircleSize) * CConfig::ResolutionScale);

    //Rectangle platform part
    SelectObject(hdc, platformGreenPen);
    SelectObject(hdc, platformGreenBrush);

    RoundRect(hdc, (x + CConfig::BallSize) * CConfig::ResolutionScale, (y + 1) * CConfig::ResolutionScale, (x + 4 + 20) * CConfig::ResolutionScale, (y + 1 + 5) * CConfig::ResolutionScale, 4 * CConfig::ResolutionScale, 4 * CConfig::ResolutionScale);

    //Highlight from light
    SelectObject(hdc, platformGreyPen);
    SelectObject(hdc, platformGreyBrush);

    Rectangle(hdc, (x + 4 + 3) * CConfig::ResolutionScale, (y + 1 + 2) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale + 24, (y + 1) * CConfig::ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 9) * CConfig::ResolutionScale, (y + 1 + 2) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale + 33, (y + 1) * CConfig::ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 14) * CConfig::ResolutionScale, (y + 1 + 2) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale + 45, (y + 1) * CConfig::ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 21) * CConfig::ResolutionScale, (y + 1 + 1) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale + 66, (y + 1) * CConfig::ResolutionScale + 1);
    Rectangle(hdc, (x + 4 - 2) * CConfig::ResolutionScale, (y + 1 + 1) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale - 3, (y + 1) * CConfig::ResolutionScale + 1);
    Rectangle(hdc, (x + 4 - 3) * CConfig::ResolutionScale, (y + 1 + 1) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale - 7, (y + 1) * CConfig::ResolutionScale + 9);
}

// Redraw platform in other position (moving)
void CPlatform::ReDraw(HWND hWnd)
{
    OldPlatformRect = PlatformRect;

    PlatformRect.left = Pos_X * CConfig::ResolutionScale;
    PlatformRect.top = CConfig::PlatformPos_Y * CConfig::ResolutionScale;
    PlatformRect.right = PlatformRect.left + Width * CConfig::ResolutionScale;
    PlatformRect.bottom = PlatformRect.top + Height * CConfig::ResolutionScale;

    InvalidateRect(hWnd, &OldPlatformRect, FALSE);
    InvalidateRect(hWnd, &PlatformRect, FALSE);
}