#include "Platform.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPlatform
//Ctor
CPlatform::CPlatform()
    :width(NormalWidth), pos_X(CConfig::BorderOffset_X), step_X(CConfig::ResolutionScale), PlatformState(EPlatformState::Normal), spaceBetweenCircles(21), PlatformRect{},
    OldPlatformRect{}, platformOrangePen(0), platformGreenPen(0), platformGreyPen(0), platformOrangeBrush(0), platformGreenBrush(0), platformGreyBrush(0)
{
    pos_X = (CConfig::MaxLevelPos_X - width) / 2;
}

//Start init
void CPlatform::Init()
{
    CConfig::CreatePenAndBrush(255, 128, 0, platformOrangePen, platformOrangeBrush);

    CConfig::CreatePenAndBrush(0, 128, 0, platformGreenPen, platformGreenBrush);

    CConfig::CreatePenAndBrush(225, 225, 225, platformGreyPen, platformGreyBrush);

    //Initialize first platform position for background rectangle (for clear background after moving platform)
    PlatformRect.left = pos_X * CConfig::ResolutionScale;
    PlatformRect.top = CConfig::PlatformPos_Y * CConfig::ResolutionScale;
    PlatformRect.right = PlatformRect.left + width * CConfig::ResolutionScale;
    PlatformRect.bottom = PlatformRect.top + Height * CConfig::ResolutionScale;

}

// Redraw platform on timer when it is meltdown
void CPlatform::Act(HWND hWnd)
{
    if (PlatformState != EPlatformState::Meltdown)
    {
        PlatformState = EPlatformState::Meltdown;

        int size = sizeof(meltingPlatformPos_Y) / sizeof(meltingPlatformPos_Y[0]);

        for (int i =0; i < size; ++i)
        { 
            meltingPlatformPos_Y[i] = PlatformRect.bottom;
        }
    }

    if (PlatformState == EPlatformState::Meltdown)
    {
        ReDraw(hWnd);
    }
}

// Draw platform
void CPlatform::Draw(HDC hdc, RECT& paintArea)
{
    switch (PlatformState)
    {
    case Normal:
        DrawNormalState(hdc, paintArea);
        break;

    case Meltdown:
        DrawMeltingState(hdc, paintArea);
        break;

    case Missing:
        break;
    default:
        break;
    }
}

// Redraw platform in other position (moving)
void CPlatform::ReDraw(HWND hWnd)
{
    OldPlatformRect = PlatformRect;

    PlatformRect.left = pos_X * CConfig::ResolutionScale;
    PlatformRect.top = CConfig::PlatformPos_Y * CConfig::ResolutionScale;
    PlatformRect.right = PlatformRect.left + width * CConfig::ResolutionScale;
    PlatformRect.bottom = PlatformRect.top + Height * CConfig::ResolutionScale;

    if (PlatformState == EPlatformState::Meltdown)
    {
        OldPlatformRect.bottom = (CConfig::MaxLevelPos_Y + 1) * CConfig::ResolutionScale;
    }
    InvalidateRect(hWnd, &OldPlatformRect, FALSE);
    InvalidateRect(hWnd, &PlatformRect, FALSE);
}

// Draw normal platform
void CPlatform::DrawNormalState(HDC hdc, RECT& paintArea)
{
    int x = pos_X;
    int y = CConfig::PlatformPos_Y;

    RECT intersectionRect;
    if (!IntersectRect(&intersectionRect, &paintArea, &PlatformRect))
    {
        return;
    }

    //At first draw a rectangle with platform size and background color before drawing platform itself to clear after redraw
    SelectObject(hdc, CConfig::backgroundPen);
    SelectObject(hdc, CConfig::backgroundBrush);

    Rectangle(hdc, OldPlatformRect.left, OldPlatformRect.top, OldPlatformRect.right, OldPlatformRect.bottom);

    //Cirlce platform side parts
    SelectObject(hdc, platformOrangePen);
    SelectObject(hdc, platformOrangeBrush);

    Ellipse(hdc, x * CConfig::ResolutionScale, y * CConfig::ResolutionScale, (x + CircleSize) * CConfig::ResolutionScale, (y + CircleSize) * CConfig::ResolutionScale); //Draw a ellipse in rectangle
    Ellipse(hdc, (x + spaceBetweenCircles) * CConfig::ResolutionScale, y * CConfig::ResolutionScale, (x + spaceBetweenCircles + CircleSize) * CConfig::ResolutionScale, (y + CircleSize) * CConfig::ResolutionScale);

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

// Draw platform when it is meltdown
void CPlatform::DrawMeltingState(HDC hdc, RECT& paintArea)
{
    int meltingAreaWidth = width * CConfig::ResolutionScale;
    int meltingAreaHeight = Height * CConfig::ResolutionScale + 1;
    int x, y = 0;
    int offset_Y = 0;
    COLORREF pixel = 0;
    COLORREF bgPixel = RGB(CConfig::backgroundColor.R, CConfig::backgroundColor.G, CConfig::backgroundColor.B);
    RECT intersectionRect;
    
    if (!IntersectRect(&intersectionRect, &paintArea, &PlatformRect))
    {
        return;
    }

    for (int i = 0; i < meltingAreaWidth; ++i)
    {
        offset_Y = CConfig::Rand(MeltingSpeed) + 1;
        x = PlatformRect.left + i;

        for (int j = 0; j < meltingAreaHeight; ++j)
        {
            y = meltingPlatformPos_Y[i] - j;
            pixel = GetPixel(hdc, x, y);
            SetPixel(hdc, x, y + offset_Y, pixel);
        }

        for (int k = 0; k < offset_Y; ++k)
        {
            y = meltingPlatformPos_Y[i] - meltingAreaHeight + k + 1;
            SetPixel(hdc, x, y, bgPixel);
        }

        meltingPlatformPos_Y[i] += offset_Y;
    }
}