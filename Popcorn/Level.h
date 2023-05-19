#pragma once

#include "Config.h"
#include "ActiveBrick.h"

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

    CActiveBrick ActiveBrick;

private:
    void DrawBrick(HDC hdc, int x, int y, char brickColor);
    void SwapBrickColors(HPEN& frontSidePen, HPEN& backSidePen, HBRUSH& frontSideBrush, HBRUSH& backSideBrush);
    void DrawLetterBrick(HDC hdc, int x, int y, EBrickColor brickMainColor, EBrickLetter brickLetter, int rotationStep);

    HPEN brickPurplePen, brickBluePen, brickWhiteLetterPen;
    HBRUSH brickPurpleBrush, brickBlueBrush;
    RECT LevelRect;
    

    static char Level_01[CConfig::LevelHeightSize][CConfig::LevelWidthSize];
};
