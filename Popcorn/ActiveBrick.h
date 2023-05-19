#pragma once

#include "Config.h"

enum EBrickColor
{
    NONE,
    PURPLE,
    BLUE
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CActiveBrick
class CActiveBrick
{
public:
    //CActiveBrick() = default;
    CActiveBrick(EBrickColor brickColor);

    void Act();
    void Draw(HDC hdc, RECT& paintArea);

    static void SetupFadingBrickColors();
    
private:
    EBrickColor BrickColor;

    int fadeStep;

    RECT disappearingBrick;

    static unsigned char GetFadingBrickColorComponent(unsigned char color, unsigned char bgColor, int step);
    static void GetFadingBrickColor(const CColor& color, int step, HPEN& pen, HBRUSH& brush);

    static const int fadeStepMax = 80;

    static HPEN fadingBrickPurplePens[fadeStepMax];
    static HBRUSH fadingBrickPurpleBrushes[fadeStepMax];
    static HPEN fadingBrickBluePens[fadeStepMax];
    static HBRUSH fadingBrickBlueBrushes[fadeStepMax];
};