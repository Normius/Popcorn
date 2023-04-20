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
class CLevel;
class CPlatform;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBall
class CBall
{
public:
    CBall();

    void Init();
    void Draw(HDC hdc, RECT& painArea, CGameEngine* gameEngine);
    void Move(CGameEngine* gameEngine, CLevel* level, CPlatform* platform);

    float BallDirection;
    static const int BallSize = 4;
private:
    int BallPos_X;
    int BallPos_Y;
    float BallSpeed;

    HPEN ballWhitePen;
    HBRUSH ballWhiteBrush;
    RECT BallRect, OldBallRect;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CLevel
class CLevel
{
public:
    void Init();
    void CheckBallHitBrick(int& nextBallPos_Y, float& ballDirection);
    void Draw(HDC hdc, RECT &paintArea);

    static const int CellWidth = 16; //BrickWidth + 1 pxl for gorizontal space between bricks
    static const int CellHeight = 8; //Brickheight + 1 pxl for vertical space between bricks
    static const int LevelWidthSize = 12; //Width in Bricks
    static const int LevelHeightSize = 14; //Height in Bricks
    static const int LevelOffset_X = 8;
    static const int LevelOffset_Y = 6;
    static const int MaxLevelPos_X = LevelOffset_X + CellWidth * LevelWidthSize;
    static const int MaxLevelPos_Y = 199 - CBall::BallSize;

private:
    void DrawBrick(HDC hdc, int x, int y, char brickColor);
    void SwapBrickColors(HPEN& frontSidePen, HPEN& backSidePen, HBRUSH& frontSideBrush, HBRUSH& backSideBrush);
    void DrawLetterBrick(HDC hdc, int x, int y, EBrickColor brickMainColor, EBrickLetter brickLetter, int rotationStep);

    HPEN brickPurplePen, brickBluePen, brickWhiteLetterPen;
    HBRUSH brickPurpleBrush, brickBlueBrush;
    RECT LevelRect;

    static const int BrickWidth = 15;
    static const int BrickHeight = 7;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPlatform
class CPlatform
{
public:
    CPlatform();

    void Init();
    void Draw(HDC hdc, RECT& paintArea, CGameEngine* gameEngine);
    void ReDraw(CGameEngine* gameEngine);

    static const int Pos_Y = 185;
    static const int CircleSize = 7;
    static const int Height = 7;

    int Width;
    int Pos_X;
    int Step_X;

private:
    int SpaceBetweenCircles;

    RECT PlatformRect, OldPlatformRect;
    HPEN platformOrangePen, platformGreenPen, platformGreyPen;
    HBRUSH platformOrangeBrush, platformGreenBrush, platformGreyBrush;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBorder
class CBorder
{
public:
    void Init();
    void Draw(HDC hdc, RECT& paintArea, CGameEngine* gameEngine);

    static const int BorderOffset_X = 6;
    static const int BorderOffset_Y = 4;
private:
    void DrawVerticalElement(HDC hdc, int x, int y, CGameEngine* gameEngine);
    void DrawHorizontalElement(HDC hdc, int x, int y, CGameEngine* gameEngine);

    HPEN borderBluePen, borderWhitePen;
    HBRUSH borderBlueBrush, borderWhiteBrush;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CGameEngine
class CGameEngine
{
public:
    CGameEngine();

    HWND HWnd;
    HPEN bgBlackPen;
    HBRUSH bgBlackBrush;

    void InitGameEngine(HWND hwnd);
    void DrawFrame(HDC hdc, RECT& paintArea);
    int OnKeyDown(EKeyType keyType);
    int On_Timer();

    static void CreatePenAndBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

    static const int ResolutionScale = 3;

private:
    CBall Ball;
    CLevel Level;
    CPlatform Platform;
    CBorder Border;
};

