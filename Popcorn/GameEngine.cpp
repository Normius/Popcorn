#include "GameEngine.h"

///////////////////MINE///////////////////////////////////////////////////////////////////////////////////////////////////////
enum EBrickColor
{
    NONE,
    PURPLE,
    BLUE
};

HPEN PurplePen, BluePen;
HBRUSH PurpleBrush, BlueBrush;

const unsigned int ResolutionScale = 3;
const unsigned int BrickWidth = 15;
const unsigned int BrickHeight = 7;
const unsigned int CellWidth = 16; //BrickWidth + 1 pxl for gorizontal space between bricks
const unsigned int CellHeight = 8; //Brickheight + 1 pxl for vertical space between bricks
const int LevelOffset_X = 8;
const int LevelOffset_Y = 6;

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

//Настройка и инициализация игры при старте 
void Init()
{
    PurplePen = CreatePen(PS_SOLID, 0, RGB(255, 85, 255));
    PurpleBrush = CreateSolidBrush(RGB(255, 85, 255));

    BluePen = CreatePen(PS_SOLID, 0, RGB(85, 255, 255));
    BlueBrush = CreateSolidBrush(RGB(85, 255, 255));
}

////////////////MINE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw every frame in game

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

void DrawFrame(HDC hdc)
{
    DrawLevel(hdc);
}