#pragma once

#include "Config.h"

enum EBrickColor
{
    NONE,
    PURPLE,
    BLUE
};

enum EBrickLetter
{
    Letter_NONE,
    Letter_O
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CLevel
class CLevel
{
public:
    CLevel();

    void Init();
    void Draw(HDC hdc, RECT& paintArea);
    void CheckBallHitBrick(int& nextBallPos_Y, float& ballDirection);

private:
    void DrawBrick(HDC hdc, int x, int y, char brickColor);
    void SwapBrickColors(HPEN& frontSidePen, HPEN& backSidePen, HBRUSH& frontSideBrush, HBRUSH& backSideBrush);
    void DrawLetterBrick(HDC hdc, int x, int y, EBrickColor brickMainColor, EBrickLetter brickLetter, int rotationStep);

    HPEN brickPurplePen, brickBluePen, brickWhiteLetterPen;
    HBRUSH brickPurpleBrush, brickBlueBrush;
    RECT LevelRect;

    static char Level_01[CConfig::LevelHeightSize][CConfig::LevelWidthSize];

    static const int BrickWidth = 15;
    static const int BrickHeight = 7;
};
