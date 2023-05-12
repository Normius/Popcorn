#pragma once

#include "Config.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CActiveBrick
class CActiveBrick
{
public:
    CActiveBrick();

    void Act(HWND hwnd);
    void Draw(HDC hdc, RECT& paintArea);
private:
    int fadeStep;

    RECT disappearingBrick;

    const int fadeStepMax = 80;
};