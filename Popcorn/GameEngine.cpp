#include "GameEngine.h"

#define _USE_MATH_DEFINES
#include <cmath>

///////////////////MINE///////////////////////////////////////////////////////////////////////////////////////////////////////

enum EBrickLetter
{
    Letter_NONE,
    Letter_O
};

enum EBrickColor
{
    NONE,
    PURPLE,
    BLUE
};

HPEN PurplePen, BluePen, OrangePen, GreenPen, GreyPen, WhitePen;
HBRUSH PurpleBrush, BlueBrush, OrangeBrush, GreenBrush, GreyBrush, WhiteBrush;

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

    GreyPen = CreatePen(PS_SOLID, 0, RGB(225, 225, 225));
    WhitePen = CreatePen(PS_SOLID, ResolutionScale, RGB(255, 255, 255));
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

void SwapSideColors(HPEN& frontSidePen, HPEN& backSidePen, HBRUSH& frontSideBrush, HBRUSH& backSideBrush) //Swap frontside with backside colors
{
    HPEN tempPen = frontSidePen;
    HBRUSH tempBrush = frontSideBrush;

    frontSidePen = backSidePen;
    frontSideBrush = backSideBrush;

    backSidePen = tempPen;
    backSideBrush = tempBrush;
}

void DrawLetterBrick(HDC hdc, int x, int y, EBrickColor brickMainColor, EBrickLetter brickLetter, int rotationStep)
{
    float rotationAngle;
    float offset = 0.0f;

    int brickMiddleAxis_Y = BrickHeight * ResolutionScale / 2; //brick Rotation Axis
    int backSideBrickOffset = 0;

    XFORM xForm, oldXForm;

    HPEN frontSidePen, backSidePen;

    HBRUSH frontSideBrush, backSideBrush;

    rotationStep = rotationStep % 16;

    if (rotationStep < 8)
    {
        rotationAngle = 2.0f * static_cast<float>(M_PI) / 16.0f * static_cast<float>(rotationStep); //Brick turning around 16x times while falling. Multiply rotationAngle (1/16) by rotation step
    }
    else
    {
        rotationAngle = 2.0f * static_cast<float>(M_PI) / 16.0f * static_cast<float>(rotationStep - 8);
    }

    if (brickMainColor == PURPLE) //Define front/backside color
    {
        frontSidePen = PurplePen;
        frontSideBrush = PurpleBrush;

        backSidePen = BluePen;
        backSideBrush = BlueBrush;
    }
    else
    {
        frontSidePen = BluePen;
        frontSideBrush = BlueBrush;

        backSidePen = PurplePen;
        backSideBrush = PurpleBrush;
    }

    if (rotationStep > 4 && rotationStep < 13)
    {
        SwapSideColors(frontSidePen, backSidePen, frontSideBrush, backSideBrush);
    }

    if (rotationStep == 4 || rotationStep == 12) //Steps for 90 degrees rotation without transformation (display bottom and top sides)
    {
        //Display front side
        SelectObject(hdc, frontSidePen);
        SelectObject(hdc, frontSideBrush);

        Rectangle(hdc, x, y + brickMiddleAxis_Y - ResolutionScale, x + BrickWidth * ResolutionScale, y+brickMiddleAxis_Y);

        //Display backside
        SelectObject(hdc, backSidePen);
        SelectObject(hdc, backSideBrush);

        Rectangle(hdc, x, y + brickMiddleAxis_Y, x + BrickWidth * ResolutionScale, y + brickMiddleAxis_Y + ResolutionScale - 1);
    }
    else
    {
        SetGraphicsMode(hdc, GM_ADVANCED); //Allows WorldTransformations (rotate plane, not the object itself)

        xForm.eM11 = 1.0f;
        xForm.eM12 = 0.0f;
        xForm.eM21 = 0.0f;
        xForm.eM22 = cos(rotationAngle);
        xForm.eDx = (float)x;
        xForm.eDy = (float)y + static_cast<float>(brickMiddleAxis_Y);

        GetWorldTransform(hdc, &oldXForm); //Save default transformation
        SetWorldTransform(hdc, &xForm); 

        //Rotation dynamic offset for front and backside
        offset = 3.0f * (1.0f - fabs(xForm.eM22)) * static_cast<float>(ResolutionScale); // TO DO: Change rotation function (from 0 to 3, then 3 to 0 etc) May be use COS or SIN
        backSideBrickOffset = static_cast<int>(round(offset));

        //Display front side
        SelectObject(hdc, frontSidePen);
        SelectObject(hdc, frontSideBrush);

        Rectangle(hdc, 0, -brickMiddleAxis_Y - backSideBrickOffset, BrickWidth * ResolutionScale, brickMiddleAxis_Y - backSideBrickOffset);

        //Display backside
        SelectObject(hdc, backSidePen);
        SelectObject(hdc, backSideBrush);

        Rectangle(hdc, 0, -brickMiddleAxis_Y, BrickWidth * ResolutionScale, brickMiddleAxis_Y);

        if (rotationStep > 4 && rotationStep < 13)
        {
            SelectObject(hdc, WhitePen);
            switch (brickLetter)
            {
                case Letter_O:
                {
                    Ellipse(hdc, 0 + 5 * ResolutionScale, -brickMiddleAxis_Y + ResolutionScale, 0 + 10 * ResolutionScale, brickMiddleAxis_Y - ResolutionScale);
                }
                break;
                default: //Nothing, empty
                    return;
            }
            
        }
        
        SetWorldTransform(hdc, &oldXForm);
    }
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
    for (int i = 0; i < 16; ++i)
    {
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 100, EBrickColor::BLUE, EBrickLetter::Letter_O, i);
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 130, EBrickColor::PURPLE, EBrickLetter::Letter_O, i);
    }
}