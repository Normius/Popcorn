#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <cmath>



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

enum EKeyType
{
    LeftArrowKey,
    RightArrowKey,
    SpaceKey
};

const unsigned int TimerID = WM_USER + 1;

class CGameEngine;

class CBall
{
public:
    CBall();

    void Draw(HDC hdc, RECT& painArea, CGameEngine* gameEngine);
    void Move(CGameEngine *gameEngine);

    float BallDirection;

    static const int BallSize = 4;
private:
    int BallPos_X;
    int BallPos_Y;
    float BallSpeed;
    RECT BallRect, OldBallRect;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CGameEngine
{
public:

    CGameEngine();

    HWND HWnd;
    HPEN WhitePen, BlackPen;
    HBRUSH WhiteBrush, BlackBrush;

    void InitGameEngine(HWND hwnd);
    void DrawFrame(HDC hdc, RECT& paintArea);
    int OnKeyDown(EKeyType keyType);
    int On_Timer();
    void CheckBallHitBrick(int& nextBallPos_Y);

    int PlatformWidth;
    int PlatformPos_X;
    int PlatformStep_X;

    static const int ResolutionScale = 3;
    static const int CellWidth = 16; //BrickWidth + 1 pxl for gorizontal space between bricks
    static const int CellHeight = 8; //Brickheight + 1 pxl for vertical space between bricks
    static const int LevelWidthSize = 12; //Width in Bricks
    static const int LevelHeightSize = 14; //Height in Bricks
    static const int LevelOffset_X = 8;
    static const int LevelOffset_Y = 6;
    static const int MaxLevelPos_X = LevelOffset_X + CellWidth * LevelWidthSize;
    static const int MaxLevelPos_Y = 199 - CBall::BallSize;
    static const int PlatformPos_Y = 185;
    static const int BorderOffset_X = 6;
    static const int BorderOffset_Y = 4;

private:
    void CreatePenAndBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
    void DrawBrick(HDC hdc, int x, int y, char brickColor);
    void SwapBrickColors(HPEN& frontSidePen, HPEN& backSidePen, HBRUSH& frontSideBrush, HBRUSH& backSideBrush);
    void DrawLetterBrick(HDC hdc, int x, int y, EBrickColor brickMainColor, EBrickLetter brickLetter, int rotationStep);
    void DrawLevel(HDC hdc);
    void DrawPlatform(HDC hdc, int x, int y);
    void ReDrawPlatform();
    void DrawVerticalBorderElement(HDC hdc, int x, int y);
    void DrawHorizontalBorderElement(HDC hdc, int x, int y);
    void DrawBorder(HDC hdc);

    HPEN PurplePen, BluePen, OrangePen, GreenPen, GreyPen, WhiteLetterPen;
    HBRUSH PurpleBrush, BlueBrush, OrangeBrush, GreenBrush, GreyBrush;

    int SpaceBetweenCircles;

    RECT PlatformRect, OldPlatformRect;
    RECT LevelRect;

    CBall Ball;

    static const int BrickWidth = 15;
    static const int BrickHeight = 7;
    static const int CircleSize = 7;
    static const int PlatformHeight = 7;
};

