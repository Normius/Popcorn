#pragma once

#include "ActiveBrick.h"
#include "Ball.h"

enum EBrickLetter
{
    Letter_NONE,
    Letter_O
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CLevel
class CLevel: public CHitChecker
{
public:
    CLevel();

    virtual bool CheckHit(float nextBallPos_X, float nextBallPos_Y, CBall* ball) override;

    void Init();
    void Draw(HDC hdc, RECT& paintArea);
    

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
