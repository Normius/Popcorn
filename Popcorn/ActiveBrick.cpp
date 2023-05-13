#include "ActiveBrick.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CActiveBrick
HPEN CActiveBrick::fadingBrickPurplePens[fadeStepMax];
HBRUSH CActiveBrick::fadingBrickPurpleBrushes[fadeStepMax];
HPEN CActiveBrick::fadingBrickBluePens[fadeStepMax];
HBRUSH CActiveBrick::fadingBrickBlueBrushes[fadeStepMax];

//ctor
CActiveBrick::CActiveBrick(EBrickColor brickColor)
    :fadeStep(0), disappearingBrick{}, BrickColor(brickColor)
{
}

//Redraw fading bricks
void CActiveBrick::Act(HWND hwnd)
{
    if (fadeStep < fadeStepMax - 1)
    {
        ++fadeStep;
        InvalidateRect(hwnd, &disappearingBrick, FALSE);
    }
}

void CActiveBrick::Draw(HDC hdc, RECT& paintArea)
{
    HPEN pen = 0;
    HBRUSH brush = 0;

    switch (BrickColor)
    {
    case PURPLE:
    {
        pen = fadingBrickPurplePens[fadeStep];
        brush = fadingBrickPurpleBrushes[fadeStep];
    }
    break;
    case BLUE:
    {
        pen = fadingBrickBluePens[fadeStep];
        brush = fadingBrickBlueBrushes[fadeStep];
    }
    break;
    default:
        return;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    disappearingBrick.left = (CConfig::LevelOffset_X + CConfig::CellWidth) * CConfig::ResolutionScale;
    disappearingBrick.top = (CConfig::LevelOffset_Y + CConfig::CellHeight) * CConfig::ResolutionScale;
    disappearingBrick.right = disappearingBrick.left + CConfig::BrickWidth * CConfig::ResolutionScale;
    disappearingBrick.bottom = disappearingBrick.top + CConfig::BrickHeight * CConfig::ResolutionScale;

    RoundRect(hdc, disappearingBrick.left, disappearingBrick.top, disappearingBrick.right, disappearingBrick.bottom, 2 * CConfig::ResolutionScale, 2 * CConfig::ResolutionScale);
}

//Initialize color arrays with fading colors for active bricks
void CActiveBrick::SetupFadingBrickColors()
{
    for (int i = 0; i < fadeStepMax; ++i)
    {
        GetFadingBrickColor(CConfig::brickPurpleColor, i, fadingBrickPurplePens[i], fadingBrickPurpleBrushes[i]);
        GetFadingBrickColor(CConfig::brickBlueColor, i, fadingBrickBluePens[i], fadingBrickBlueBrushes[i]);
    }
}

unsigned char CActiveBrick::GetFadingBrickColorComponent(unsigned char color, unsigned char bgColor, int step)
{
    return color - step * (color - bgColor) / (fadeStepMax - 1);
}

void CActiveBrick::GetFadingBrickColor(const CColor& color, int step, HPEN& pen, HBRUSH& brush)
{
    unsigned char r = 0, g = 0, b = 0;
    r = GetFadingBrickColorComponent(color.R, CConfig::backgroundColor.R, step);
    g = GetFadingBrickColorComponent(color.G, CConfig::backgroundColor.G, step);
    b = GetFadingBrickColorComponent(color.B, CConfig::backgroundColor.B, step);

    CConfig::CreatePenAndBrush(r, g, b, pen, brush);
}