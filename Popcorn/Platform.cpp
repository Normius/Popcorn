#include "Platform.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPlatform
//Ctor
CPlatform::CPlatform()
    :width(NormalWidth), pos_X(CConfig::BorderOffset_X), step_X(CConfig::ResolutionScale * 2), PlatformState(EPlatformState::Normal), spaceBetweenCircles(NormalSpaceBetweenCircles), rollingStep(0),
    PlatformRect{}, OldPlatformRect{}, platformCircleOrangePen(0), platformGreenPen(0), platformGreyPen(0), platformCircleOrangeBrush(0), platformGreenBrush(0), platformGreyBrush(0)
{
    pos_X = (CConfig::MaxLevelPos_X - width) / 2;
}

//Start init
void CPlatform::Init()
{
    CConfig::CreatePenAndBrush(255, 128, 0, platformCircleOrangePen, platformCircleOrangeBrush);

    CConfig::CreatePenAndBrush(0, 128, 0, platformGreenPen, platformGreenBrush);

    CConfig::CreatePenAndBrush(225, 225, 225, platformGreyPen, platformGreyBrush);

    //Initialize first platform position for background rectangle (for clear background after moving platform)
    /*PlatformRect.left = pos_X * CConfig::ResolutionScale;
    PlatformRect.top = CConfig::PlatformPos_Y * CConfig::ResolutionScale;
    PlatformRect.right = PlatformRect.left + width * CConfig::ResolutionScale;
    PlatformRect.bottom = PlatformRect.top + Height * CConfig::ResolutionScale;*/
}

// Redraw platform on timer when it is meltdown
void CPlatform::Act()
{
    switch (PlatformState)
    {
    case Meltdown:
    case Rolling:
    case Expanding:
        ReDraw();
        break;
    default:
        break;
    }
}

// Redraw platform on timer when it is meltdown
void CPlatform::SetState(EPlatformState state)
{
    int size = 0;
    if (PlatformState == state)
        return;

    switch (state)
    {
    case EPlatformState::Meltdown:
        size = sizeof(meltingPlatformPos_Y) / sizeof(meltingPlatformPos_Y[0]);

        for (int i = 0; i < size; ++i)
        {
            meltingPlatformPos_Y[i] = PlatformRect.bottom;
        }
        break;
    case EPlatformState::Rolling:
        pos_X = CConfig::MaxLevelPos_X - 1;
        rollingStep = rollingStepMax - 1;
        break;

    default:
        break;
    }
    
    PlatformState = state;
}

// Draw platform
void CPlatform::Draw(HDC hdc, RECT& paintArea)
{
    RECT intersectionRect;

    if (!IntersectRect(&intersectionRect, &paintArea, &PlatformRect))
        return;

    switch (PlatformState)
    {
    case Normal:
        DrawNormalState(hdc, paintArea);
        break;

    case Meltdown:
        DrawMeltingState(hdc, paintArea);
        break;

    case Rolling:
        DrawRollingState(hdc, paintArea);
        break;
    case Expanding:
        DrawExpandingState(hdc, paintArea);
        break;
    case Missing:
        break;
    default:
        break;
    }
}

// Redraw platform in other position (moving)
void CPlatform::ReDraw()
{
    OldPlatformRect = PlatformRect;

    int platformWidth = 0;

    if (PlatformState == EPlatformState::Rolling)
        platformWidth = CircleSize; //+ 3 to 
    else
        platformWidth = width;

    PlatformRect.left = pos_X * CConfig::ResolutionScale;
    PlatformRect.top = CConfig::PlatformPos_Y * CConfig::ResolutionScale;
    PlatformRect.right = PlatformRect.left + platformWidth * CConfig::ResolutionScale;
    PlatformRect.bottom = PlatformRect.top + Height * CConfig::ResolutionScale;

    if (PlatformState == EPlatformState::Meltdown)
        OldPlatformRect.bottom = (CConfig::MaxLevelPos_Y + 1) * CConfig::ResolutionScale;

    InvalidateRect(CConfig::HWnd, &OldPlatformRect, FALSE);
    InvalidateRect(CConfig::HWnd, &PlatformRect, FALSE);
}

// Redraw platform in other position (moving)
void CPlatform::ClearBackground(HDC hdc)
{
    //At first draw a rectangle with platform size and background color before drawing platform itself to clear after redraw
    SelectObject(hdc, CConfig::backgroundPen);
    SelectObject(hdc, CConfig::backgroundBrush);

    Rectangle(hdc, OldPlatformRect.left, OldPlatformRect.top, OldPlatformRect.right, OldPlatformRect.bottom);
}

// Draw normal platform
void CPlatform::DrawNormalState(HDC hdc, RECT& paintArea)
{
    int x = pos_X;
    int y = CConfig::PlatformPos_Y;
    
    ClearBackground(hdc);

    //Round platform side parts
    SelectObject(hdc, platformCircleOrangePen);
    SelectObject(hdc, platformCircleOrangeBrush);

    Ellipse(hdc, x * CConfig::ResolutionScale, y * CConfig::ResolutionScale, (x + CircleSize) * CConfig::ResolutionScale, (y + CircleSize) * CConfig::ResolutionScale); //Draw a ellipse in rectangle
    Ellipse(hdc, (x + spaceBetweenCircles) * CConfig::ResolutionScale, y * CConfig::ResolutionScale, (x + spaceBetweenCircles + CircleSize) * CConfig::ResolutionScale, (y + CircleSize) * CConfig::ResolutionScale);

    //Highlight from light
    SelectObject(hdc, platformGreyPen);
    SelectObject(hdc, platformGreyBrush);

    //Highlight at round part
    Rectangle(hdc, (x + 4 - 2) * CConfig::ResolutionScale, (y + 1 + 1) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale - 3, (y + 1) * CConfig::ResolutionScale + 1);
    Rectangle(hdc, (x + 4 - 3) * CConfig::ResolutionScale, (y + 1 + 1) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale - 7, (y + 1) * CConfig::ResolutionScale + 9);

    //Rectangle platform part
    SelectObject(hdc, platformGreenPen);
    SelectObject(hdc, platformGreenBrush);

    RoundRect(hdc, (x + 4) * CConfig::ResolutionScale, (y + 1) * CConfig::ResolutionScale, (x + 4 + spaceBetweenCircles - 1) * CConfig::ResolutionScale, (y + 1 + 5) * CConfig::ResolutionScale, 3 * CConfig::ResolutionScale, 3 * CConfig::ResolutionScale);

    ////Highlight at rectangle part
    //Rectangle(hdc, (x + 4 + 3) * CConfig::ResolutionScale, (y + 1 + 2) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale + 24, (y + 1) * CConfig::ResolutionScale + 5);
    //Rectangle(hdc, (x + 4 + 9) * CConfig::ResolutionScale, (y + 1 + 2) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale + 33, (y + 1) * CConfig::ResolutionScale + 5);
    //Rectangle(hdc, (x + 4 + 14) * CConfig::ResolutionScale, (y + 1 + 2) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale + 45, (y + 1) * CConfig::ResolutionScale + 5);
    //Rectangle(hdc, (x + 4 + 21) * CConfig::ResolutionScale, (y + 1 + 1) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale + 66, (y + 1) * CConfig::ResolutionScale + 1);

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

// Draw rolling platform at the level start
void CPlatform::DrawRollingState(HDC hdc, RECT& paintArea)
{
    int x = pos_X * CConfig::ResolutionScale;
    int y = CConfig::PlatformPos_Y * CConfig::ResolutionScale;
    int rollingBallSize = CircleSize * CConfig::ResolutionScale;
    XFORM xForm, oldXForm;
    float rotationAngle = 0.0f;

    ClearBackground(hdc);

    //Draw a rollingBall
    SelectObject(hdc, platformCircleOrangePen);
    SelectObject(hdc, platformCircleOrangeBrush);

    Ellipse(hdc, x, y, x + rollingBallSize, y + rollingBallSize);

    //Draw divided line
    SetGraphicsMode(hdc, GM_ADVANCED); //Allows WorldTransformations (rotate plane, not the object itself)

    rotationAngle = -2.0f * static_cast<float>(M_PI) / static_cast<float>(rollingStepMax) * static_cast<float>(rollingStep);

    xForm.eM11 = cos(rotationAngle);
    xForm.eM12 = sin(rotationAngle);
    xForm.eM21 = -sin(rotationAngle);
    xForm.eM22 = cos(rotationAngle);
    xForm.eDx = static_cast<float>(x + rollingBallSize / 2);
    xForm.eDy = static_cast<float>(y + rollingBallSize / 2);

    GetWorldTransform(hdc, &oldXForm); //Save default transformation
    SetWorldTransform(hdc, &xForm);

    SelectObject(hdc, CConfig::backgroundPen);
    SelectObject(hdc, CConfig::backgroundBrush);

    Rectangle(hdc, -CConfig::ResolutionScale / 2, -rollingBallSize / 2, CConfig::ResolutionScale / 2, rollingBallSize / 2); //divide rollingBall by half
    
    SetWorldTransform(hdc, &oldXForm);

    //Draw Highlight from light
    SelectObject(hdc, platformGreyPen);
    SelectObject(hdc, platformGreyBrush);

    Rectangle(hdc, x + CConfig::ResolutionScale, y + 2 * CConfig::ResolutionScale, x + CConfig::ResolutionScale * 2, y + 4 * CConfig::ResolutionScale);
    Rectangle(hdc, x + CConfig::ResolutionScale * 2, y + CConfig::ResolutionScale, x + CConfig::ResolutionScale * 3, y + 2 * CConfig::ResolutionScale);

    ++rollingStep;

    if (rollingStep >= rollingStepMax)
        rollingStep -= rollingStepMax;
        
    pos_X -= rollingPlatformSpeed;

    if (pos_X <= stopRollingPos_X)
    {
        pos_X += rollingPlatformSpeed;
        PlatformState = EPlatformState::Expanding;
        spaceBetweenCircles = 1;
    }
}

// Draw rolling platform at the level start
void CPlatform::DrawExpandingState(HDC hdc, RECT& paintArea)
{
    DrawNormalState(hdc, paintArea);

    --pos_X;
    spaceBetweenCircles += 2;

    if (spaceBetweenCircles >= NormalSpaceBetweenCircles)
    {
        spaceBetweenCircles = NormalSpaceBetweenCircles;

        PlatformState = EPlatformState::Normal;
        ReDraw();
    }
}