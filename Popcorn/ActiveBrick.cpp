#include "ActiveBrick.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CLevel
CActiveBrick::CActiveBrick()
    :fadeStep(0), disappearingBrick{}
{
}

void CActiveBrick::Act(HWND hwnd)
{
    if (fadeStep < fadeStepMax)
    {
        ++fadeStep;
        InvalidateRect(hwnd, &disappearingBrick, FALSE);
    }
}

void CActiveBrick::Draw(HDC hdc, RECT& paintArea)
{
    HPEN pen;
    HBRUSH brush;

    CConfig::CreatePenAndBrush(85 - (85 / fadeStepMax) * fadeStep, 255 - (255 / fadeStepMax) * fadeStep, 255 - (255 / fadeStepMax) * fadeStep, pen, brush);

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    disappearingBrick.left = (CConfig::LevelOffset_X + CConfig::CellWidth) * CConfig::ResolutionScale;
    disappearingBrick.top = (CConfig::LevelOffset_Y + CConfig::CellHeight) * CConfig::ResolutionScale;
    disappearingBrick.right = disappearingBrick.left + CConfig::BrickWidth * CConfig::ResolutionScale;
    disappearingBrick.bottom = disappearingBrick.top + CConfig::BrickHeight * CConfig::ResolutionScale;

    RoundRect(hdc, disappearingBrick.left, disappearingBrick.top, disappearingBrick.right, disappearingBrick.bottom, 2 * CConfig::ResolutionScale, 2 * CConfig::ResolutionScale);
}