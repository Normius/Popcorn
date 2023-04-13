#include "GameEngine.h"

#define _USE_MATH_DEFINES
#include <cmath>

///////////////////MINE///////////////////////////////////////////////////////////////////////////////////////////////////////
enum EBrickColor
{
    NONE,
    PURPLE,
    BLUE
};

HPEN PurplePen, BluePen, OrangePen, GreenPen, GreyPen;
HBRUSH PurpleBrush, BlueBrush, OrangeBrush, GreenBrush, GreyBrush;

const unsigned int ResolutionScale = 3;
const unsigned int BrickWidth = 15;
const unsigned int BrickHeight = 7;
const unsigned int CellWidth = 16; //BrickWidth + 1 pxl for gorizontal space between bricks
const unsigned int CellHeight = 8; //Brickheight + 1 pxl for vertical space between bricks
const int LevelOffset_X = 8;
const int LevelOffset_Y = 6;
const unsigned int CircleSize = 7;

int SpaceBetweenCircles = 21;

char LevelFirst[14][12]
{
    0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
};
/////////////////////////////////////////////////////////////

void CreatePenAndBrush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));
}

//Настройка и инициализация игры при старте 
void Init()
{
    CreatePenAndBrush(255, 85, 255, PurplePen, PurpleBrush);

    CreatePenAndBrush(85, 255, 255, BluePen, BlueBrush);

    CreatePenAndBrush(255, 128, 0, OrangePen, OrangeBrush);

    CreatePenAndBrush(0, 128, 0, GreenPen, GreenBrush);

    CreatePenAndBrush(225, 225, 225, GreyPen, GreyBrush);
}

////////////////MINE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw a brick
void DrawBrick(HDC hdc, int x, int y, char brickColor) //TO DO: change char param to ENUM ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
    switch (brickColor)
    {
    case PURPLE:
    {
        SelectObject(hdc, PurplePen);
        SelectObject(hdc, PurpleBrush);
    }
    break;
    case BLUE:
    {
        SelectObject(hdc, BluePen);
        SelectObject(hdc, BlueBrush);
    }
    break;
    default: //Nothing, empty
        return;
    }
    RoundRect(hdc, x * ResolutionScale, y * ResolutionScale, (x + BrickWidth) * ResolutionScale, (y + BrickHeight) * ResolutionScale, 2 * ResolutionScale, 2 * ResolutionScale);
}


void DrawLetterBrick(HDC hdc, int rotationStep)
{
    float rotationAngle = 2.0f * static_cast<float>(M_PI) / 16.0f * static_cast<float>(rotationStep); //Bricks turning around 16x times while falling. Multiply rotationAngle (1/16) by rotation step
    XFORM xForm, oldXForm;

    SetGraphicsMode(hdc, GM_ADVANCED);

    xForm.eM11 = cos(rotationAngle);
    xForm.eM12 = sin(rotationAngle);
    xForm.eM21 = -sin(rotationAngle);
    xForm.eM22 = cos(rotationAngle);
    xForm.eDx = (FLOAT)100.0;
    xForm.eDy = (FLOAT)100.0;
    GetWorldTransform(hdc, &oldXForm);
    SetWorldTransform(hdc, &xForm);


    SelectObject(hdc, BluePen);
    SelectObject(hdc, BlueBrush);

    Rectangle(hdc, 0, 0, 15 * ResolutionScale, 7 * ResolutionScale);
    SetWorldTransform(hdc, &xForm);
}

// Draw all bricks on level
void DrawLevel(HDC hdc)
{
    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 12; ++j)
        {
            DrawBrick(hdc, LevelOffset_X + j * CellWidth, LevelOffset_Y + i * CellHeight, LevelFirst[i][j]); //TO DO: Cast to ENUM or NOT? ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }
}

// Draw platform
void DrawPlatform(HDC hdc, int x, int y)
{
    SelectObject(hdc, OrangePen);
    SelectObject(hdc, OrangeBrush);

    //Cirlce platform side parts
    Ellipse(hdc, x * ResolutionScale, y * ResolutionScale, (x + CircleSize) * ResolutionScale, (y + CircleSize) * ResolutionScale); //Draw a ellipse in rectangle
    Ellipse(hdc, (x + SpaceBetweenCircles) * ResolutionScale, y * ResolutionScale, (x + SpaceBetweenCircles + CircleSize) * ResolutionScale, (y + CircleSize) * ResolutionScale);

    SelectObject(hdc, GreenPen);
    SelectObject(hdc, GreenBrush);

    //Rectangle platform part
    RoundRect(hdc, (x + 4) * ResolutionScale, (y + 1) * ResolutionScale, (x + 4 + 20) * ResolutionScale, (y + 1 + 5) * ResolutionScale, 4 * ResolutionScale, 4 * ResolutionScale);
    
    SelectObject(hdc, GreyPen);
    SelectObject(hdc, GreyBrush);

    //Highlight from light
    Rectangle(hdc, (x + 4 + 3) * ResolutionScale, (y + 1 + 2) * ResolutionScale, (x + 4) * ResolutionScale + 24, (y + 1) * ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 9) * ResolutionScale, (y + 1 + 2) * ResolutionScale, (x + 4) * ResolutionScale + 33, (y + 1) * ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 14) * ResolutionScale, (y + 1 + 2) * ResolutionScale, (x + 4) * ResolutionScale + 45, (y + 1) * ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 21) * ResolutionScale, (y + 1 + 1) * ResolutionScale, (x + 4) * ResolutionScale + 66, (y + 1) * ResolutionScale + 1);
    Rectangle(hdc, (x + 4 - 2) * ResolutionScale, (y + 1 + 1) * ResolutionScale, (x + 4) * ResolutionScale - 3, (y + 1) * ResolutionScale + 1);
    Rectangle(hdc, (x + 4 - 3) * ResolutionScale, (y + 1 + 1) * ResolutionScale, (x + 4) * ResolutionScale - 7, (y + 1) * ResolutionScale + 9);


}
// Draw every frame in game
void DrawFrame(HDC hdc)
{
    //DrawLevel(hdc);

    //DrawPlatform(hdc, 50, 100);
    for (int i = 1; i < 17; ++i)
    {
        DrawLetterBrick(hdc, i);
    }
}