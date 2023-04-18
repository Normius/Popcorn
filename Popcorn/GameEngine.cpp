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

HWND HWnd;
HPEN PurplePen, BluePen, OrangePen, GreenPen, GreyPen, WhitePen, BlackPen, WhiteLetterPen;
HBRUSH PurpleBrush, BlueBrush, OrangeBrush, GreenBrush, GreyBrush, WhiteBrush, BlackBrush;

const int ResolutionScale = 3;
const int BrickWidth = 15;
const int BrickHeight = 7;
const int CellWidth = 16; //BrickWidth + 1 pxl for gorizontal space between bricks
const int CellHeight = 8; //Brickheight + 1 pxl for vertical space between bricks
const int CircleSize = 7;
const int BallSize = 4;
const int PlatformHeight = 7;

const int LevelWidthSize = 12; //Width in Bricks
const int LevelHeightSize = 14; //Height in Bricks

const int LevelOffset_X = 8;
const int LevelOffset_Y = 6;
const int PlatformPos_Y = 185;
const int MaxLevelPos_X = 201;
const int MaxLevelPos_Y = 200 - BallSize;

const int BorderOffset_X = 6;
const int BorderOffset_Y = 4;

int PlatformWidth = 28;
int PlatformPos_X = (BorderOffset_X + MaxLevelPos_X - PlatformWidth) / 2;
int PlatformStep_X = ResolutionScale;
int SpaceBetweenCircles = 21;

int BallPos_X = 20;
int BallPos_Y = 170;
int BallMove_X = 1;
int BallMove_Y = 1;
float BallSpeed = 3.0f;
float BallDirection = static_cast<float>(M_PI - M_PI / 4);


RECT PlatformRect, OldPlatformRect;
RECT LevelRect;
RECT BallRect, OldBallRect;

char Level_01[LevelHeightSize][LevelWidthSize]
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

//Setup and initialize game resources on start
void InitGameEngine(HWND hWnd)
{
    //Initialize window handle by value from Main
    HWnd = hWnd;

    CreatePenAndBrush(255, 85, 255, PurplePen, PurpleBrush);

    CreatePenAndBrush(85, 255, 255, BluePen, BlueBrush);

    CreatePenAndBrush(255, 128, 0, OrangePen, OrangeBrush);

    CreatePenAndBrush(0, 128, 0, GreenPen, GreenBrush);

    CreatePenAndBrush(0, 0, 0, BlackPen, BlackBrush);

    CreatePenAndBrush(255, 255, 255, WhitePen, WhiteBrush);

    GreyPen = CreatePen(PS_SOLID, 0, RGB(225, 225, 225));
    WhiteLetterPen = CreatePen(PS_SOLID, ResolutionScale, RGB(255, 255, 255));

    //Initialize first platform position for background rectangle (for clear background after moving platform)
    PlatformRect.left = PlatformPos_X * ResolutionScale;
    PlatformRect.top = PlatformPos_Y * ResolutionScale;
    PlatformRect.right = PlatformRect.left + PlatformWidth * ResolutionScale;
    PlatformRect.bottom = PlatformRect.top + PlatformHeight * ResolutionScale;

    LevelRect.left = BorderOffset_X * ResolutionScale;
    LevelRect.top = BorderOffset_Y * ResolutionScale;
    LevelRect.right = LevelRect.left + CellWidth * LevelHeightSize * ResolutionScale;
    LevelRect.bottom = LevelRect.top + CellHeight * LevelWidthSize * ResolutionScale;

    SetTimer(HWnd, TimerID, 20, 0);
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

void DrawBall(HDC hdc)
{
    //At first draw a rectangle with platform size and background color before drawing platform itself to clear after redraw
    SelectObject(hdc, BlackPen);
    SelectObject(hdc, BlackBrush);

    Ellipse(hdc, OldBallRect.left, OldBallRect.top, OldBallRect.right - 1, OldBallRect.bottom - 1);

    //Draw a ball
    SelectObject(hdc, WhitePen);
    SelectObject(hdc, WhiteBrush);

    Ellipse(hdc, BallRect.left, BallRect.top, BallRect.right - 1, BallRect.bottom - 1);
}

//Check new position for collision with level bricks and change direction and position consider reflection
void CheckBallHitBrick(int &nextBallPos_Y)
{
    int brickPos_Y = LevelOffset_Y + LevelHeightSize * CellHeight;

    for (int i = LevelHeightSize - 1; i >= 0; --i)
    {
        for (int j = LevelWidthSize - 1; j >= 0; --j)
        {
            if (Level_01[i][j] == 0)
            {
                continue;
            }
            if (nextBallPos_Y < brickPos_Y)
            {
                nextBallPos_Y = brickPos_Y - (nextBallPos_Y - brickPos_Y);
                BallDirection = -BallDirection;
            }
        }

        brickPos_Y -= CellHeight;
    }

}

//Redraw ball in other position (moving)
void ReDrawBall()
{
    int nextBallPos_X, nextBallPos_Y;

    OldBallRect = BallRect;

    //Calculate new ball direction and position
    nextBallPos_X = BallPos_X + static_cast<int>(BallSpeed * cos(BallDirection));
    nextBallPos_Y = BallPos_Y - static_cast<int>(BallSpeed * sin(BallDirection));

    //Check new position for collision with level border and change direction and new position consider reflection
    //Left border reflection
    if (nextBallPos_X < BorderOffset_X)
    {
        nextBallPos_X = BorderOffset_X - (nextBallPos_X - BorderOffset_X);
        BallDirection = static_cast<float>(M_PI) - BallDirection;
    }
    //Top border reflection
    if (nextBallPos_Y < BorderOffset_Y)
    {
        nextBallPos_Y = BorderOffset_Y - (nextBallPos_Y - BorderOffset_Y);
        BallDirection = - BallDirection;
    }
    //Right border reflection
    if (nextBallPos_X > MaxLevelPos_X - BallSize)
    {
        nextBallPos_X = MaxLevelPos_X - BallSize - (nextBallPos_X - (MaxLevelPos_X - BallSize)); //consider ball speed (MaxLevelPos_X + BallSpeed)
        BallDirection = static_cast<float>(M_PI) - BallDirection;
    }
    //Bottom border reflection
    if (nextBallPos_Y > MaxLevelPos_Y - BallSize)
    {
        nextBallPos_Y = MaxLevelPos_Y - BallSize - (nextBallPos_Y - (MaxLevelPos_Y - BallSize));
        BallDirection = static_cast<float>(M_PI) + static_cast<float>(M_PI) - BallDirection;
    }

    //Check new position for collision with platform
    if (nextBallPos_Y > PlatformPos_Y - BallSize)
    {
        if (nextBallPos_X >= PlatformPos_X && nextBallPos_X <= PlatformPos_X + PlatformWidth)
        {
            nextBallPos_Y = PlatformPos_Y - BallSize - (nextBallPos_Y - (PlatformPos_Y - BallSize));
            BallDirection = static_cast<float>(M_PI) + static_cast<float>(M_PI) - BallDirection;
        }
    }

    CheckBallHitBrick(nextBallPos_Y);

    //Set new ball position
    BallPos_X = nextBallPos_X;
    BallPos_Y = nextBallPos_Y;

    BallRect.left = BallPos_X * ResolutionScale;
    BallRect.top = BallPos_Y * ResolutionScale;
    BallRect.right = BallRect.left + BallSize * ResolutionScale;
    BallRect.bottom = BallRect.top + BallSize * ResolutionScale;

    InvalidateRect(HWnd, &OldBallRect, FALSE);
    InvalidateRect(HWnd, &BallRect, FALSE);
}

void SwapBrickColors(HPEN& frontSidePen, HPEN& backSidePen, HBRUSH& frontSideBrush, HBRUSH& backSideBrush) //Swap frontside with backside colors
{
    HPEN tempPen = frontSidePen;
    HBRUSH tempBrush = frontSideBrush;

    frontSidePen = backSidePen;
    frontSideBrush = backSideBrush;

    backSidePen = tempPen;
    backSideBrush = tempBrush;
}

// Draw a brick with letter
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
        SwapBrickColors(frontSidePen, backSidePen, frontSideBrush, backSideBrush);
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
    for (int i = 0; i < LevelHeightSize; ++i) //14 LevelHeight
    {
        for (int j = 0; j < LevelWidthSize; ++j) //12 LevelWidth
        {
            DrawBrick(hdc, LevelOffset_X + j * CellWidth, LevelOffset_Y + i * CellHeight, Level_01[i][j]); //TO DO: Cast to ENUM or NOT? ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }
}

// Draw platform
void DrawPlatform(HDC hdc, int x, int y)
{
    //At first draw a rectangle with platform size and background color before drawing platform itself to clear after redraw
    SelectObject(hdc, BlackPen);
    SelectObject(hdc, BlackBrush);

    Rectangle(hdc, OldPlatformRect.left, OldPlatformRect.top, OldPlatformRect.right, OldPlatformRect.bottom);

    //Cirlce platform side parts
    SelectObject(hdc, OrangePen);
    SelectObject(hdc, OrangeBrush);

    Ellipse(hdc, x * ResolutionScale, y * ResolutionScale, (x + CircleSize) * ResolutionScale, (y + CircleSize) * ResolutionScale); //Draw a ellipse in rectangle
    Ellipse(hdc, (x + SpaceBetweenCircles) * ResolutionScale, y * ResolutionScale, (x + SpaceBetweenCircles + CircleSize) * ResolutionScale, (y + CircleSize) * ResolutionScale);

    //Rectangle platform part
    SelectObject(hdc, GreenPen);
    SelectObject(hdc, GreenBrush);

    RoundRect(hdc, (x + BallSize) * ResolutionScale, (y + 1) * ResolutionScale, (x + 4 + 20) * ResolutionScale, (y + 1 + 5) * ResolutionScale, 4 * ResolutionScale, 4 * ResolutionScale);
    
    //Highlight from light
    SelectObject(hdc, GreyPen);
    SelectObject(hdc, GreyBrush);

    Rectangle(hdc, (x + 4 + 3) * ResolutionScale, (y + 1 + 2) * ResolutionScale, (x + 4) * ResolutionScale + 24, (y + 1) * ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 9) * ResolutionScale, (y + 1 + 2) * ResolutionScale, (x + 4) * ResolutionScale + 33, (y + 1) * ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 14) * ResolutionScale, (y + 1 + 2) * ResolutionScale, (x + 4) * ResolutionScale + 45, (y + 1) * ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 21) * ResolutionScale, (y + 1 + 1) * ResolutionScale, (x + 4) * ResolutionScale + 66, (y + 1) * ResolutionScale + 1);
    Rectangle(hdc, (x + 4 - 2) * ResolutionScale, (y + 1 + 1) * ResolutionScale, (x + 4) * ResolutionScale - 3, (y + 1) * ResolutionScale + 1);
    Rectangle(hdc, (x + 4 - 3) * ResolutionScale, (y + 1 + 1) * ResolutionScale, (x + 4) * ResolutionScale - 7, (y + 1) * ResolutionScale + 9);
}

// Redraw platform in other position (moving)
void ReDrawPlatform()
{
    OldPlatformRect = PlatformRect;

    PlatformRect.left = PlatformPos_X * ResolutionScale;
    PlatformRect.top = PlatformPos_Y * ResolutionScale;
    PlatformRect.right = PlatformRect.left + PlatformWidth * ResolutionScale;
    PlatformRect.bottom = PlatformRect.top + PlatformHeight * ResolutionScale;

    InvalidateRect(HWnd, &OldPlatformRect, FALSE);
    InvalidateRect(HWnd, &PlatformRect, FALSE);
}

//Draw level border element (tile)
void DrawVerticalBorderElement(HDC hdc, int x, int y)
{
    //Blue border line
    SelectObject(hdc, BluePen);
    SelectObject(hdc, BlueBrush);

    Rectangle(hdc, (x + 1) * ResolutionScale, y * ResolutionScale, (x + 4) * ResolutionScale, (y + 4) * ResolutionScale);

    //White border line
    SelectObject(hdc, WhitePen);
    SelectObject(hdc, WhiteBrush);

    Rectangle(hdc, x * ResolutionScale, y * ResolutionScale, (x + 1) * ResolutionScale, (y + 4) * ResolutionScale);

    //Holes in border line
    SelectObject(hdc, BlackPen);
    SelectObject(hdc, BlackBrush);

    Rectangle(hdc, (x + 2) * ResolutionScale, (y + 1) * ResolutionScale, (x + 3) * ResolutionScale, (y + 2) * ResolutionScale);
}

void DrawHorizontalBorderElement(HDC hdc, int x, int y)
{
    SelectObject(hdc, BluePen);
    SelectObject(hdc, BlueBrush);

    Rectangle(hdc, x * ResolutionScale, (y + 1) * ResolutionScale, (x + 4) * ResolutionScale, (y + 4) * ResolutionScale);

    //White border line
    SelectObject(hdc, WhitePen);
    SelectObject(hdc, WhiteBrush);

    Rectangle(hdc, x * ResolutionScale, y * ResolutionScale, (x + 4) * ResolutionScale, (y + 1) * ResolutionScale);

    //Holes in border line
    SelectObject(hdc, BlackPen);
    SelectObject(hdc, BlackBrush);

    Rectangle(hdc, (x + 2) * ResolutionScale, (y + 2) * ResolutionScale, (x + 3) * ResolutionScale, (y + 3) * ResolutionScale);
}


void DrawBorder(HDC hdc)
{
    //Draw left level border
    for (int i = 0; i < 50; ++i)
    {
        DrawVerticalBorderElement(hdc, 2, 1 + i * 4);
    }

    //Draw right level border
    for (int i = 0; i < 50; ++i)
    {
        DrawVerticalBorderElement(hdc, 201, 1 + i * 4);
    }

    //Draw top level border
    for (int i = 0; i < 50; ++i)
    {
        DrawHorizontalBorderElement(hdc, 3 + i * 4, 0);
    }
}

// Draw every frame in game
void DrawFrame(HDC hdc, RECT &paintarea)
{
    RECT intersectionRect;

    if (IntersectRect(&intersectionRect, &paintarea, &LevelRect))
    {
        DrawLevel(hdc);
    }
    
    if (IntersectRect(&intersectionRect, &paintarea, &PlatformRect))
    {
        DrawPlatform(hdc, PlatformPos_X, PlatformPos_Y);
    }

    /*for (int i = 0; i < 16; ++i)
    {
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 100, EBrickColor::BLUE, EBrickLetter::Letter_O, i);
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 130, EBrickColor::PURPLE, EBrickLetter::Letter_O, i);
    }*/

    if (IntersectRect(&intersectionRect, &paintarea, &BallRect))
    {
        DrawBall(hdc);
    }

    DrawBorder(hdc);
}

int OnKeyDown(EKeyType keyType)
{
    switch (keyType)
    {
    case LeftArrowKey:
        PlatformPos_X -= PlatformStep_X;

        if (PlatformPos_X <= BorderOffset_X)
        {
            PlatformPos_X = BorderOffset_X;
        }

        ReDrawPlatform();
        break;

    case RightArrowKey:
        PlatformPos_X += PlatformStep_X;

        if (PlatformPos_X >= MaxLevelPos_X - PlatformWidth)
        {
            PlatformPos_X = MaxLevelPos_X - PlatformWidth;
        }

        ReDrawPlatform();
        break;

    case SpaceKey:
        break;
    }
    return 0;
}

int On_Timer()
{
    ReDrawBall();
    return 0;
}