#include "GameEngine.h"

///////////////////MINE///////////////////////////////////////////////////////////////////////////////////////////////////////
char Level_01[CLevel::LevelHeightSize][CLevel::LevelWidthSize]
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
//Ctor
CBall::CBall()
    :BallPos_X(20), BallPos_Y(170), BallSpeed(3.0f), BallDirection(static_cast<float>(M_PI - M_PI / 4))
{
};

//StartInit for ball
void CBall::Init()
{
    CGameEngine::CreatePenAndBrush(255, 255, 255, ballWhitePen, ballWhiteBrush);
}

// Draw a ball
void CBall::Draw(HDC hdc, RECT &paintArea, CGameEngine *gameEngine)
{
    RECT intersectionRect;

    //Check for intersection levelRECT with paint RECT
    if (!IntersectRect(&intersectionRect, &paintArea, &BallRect))
    {
        return;
    }
    //At first draw a rectangle with platform size and background color before drawing platform itself to clear after redraw
    SelectObject(hdc, gameEngine->bgBlackPen);
    SelectObject(hdc, gameEngine->bgBlackBrush);

    Ellipse(hdc, OldBallRect.left, OldBallRect.top, OldBallRect.right - 1, OldBallRect.bottom - 1);

    //Draw a ball
    SelectObject(hdc, ballWhitePen);
    SelectObject(hdc, ballWhiteBrush);

    Ellipse(hdc, BallRect.left, BallRect.top, BallRect.right - 1, BallRect.bottom - 1);
}

//Redraw ball in other position (moving)
void CBall::Move(CGameEngine* gameEngine, CLevel* level, CPlatform* platform)
{
    int nextBallPos_X, nextBallPos_Y;

    OldBallRect = BallRect;

    //Calculate new ball direction and position
    nextBallPos_X = BallPos_X + static_cast<int>(BallSpeed * cos(BallDirection));
    nextBallPos_Y = BallPos_Y - static_cast<int>(BallSpeed * sin(BallDirection));

    //Check new position for collision with level border and change direction and new position consider reflection
    //Left border reflection
    if (nextBallPos_X < CBorder::BorderOffset_X)
    {
        nextBallPos_X = CBorder::BorderOffset_X - (nextBallPos_X - CBorder::BorderOffset_X);
        BallDirection = static_cast<float>(M_PI) - BallDirection;
    }
    //Top border reflection
    if (nextBallPos_Y < CBorder::BorderOffset_Y)
    {
        nextBallPos_Y = CBorder::BorderOffset_Y - (nextBallPos_Y - CBorder::BorderOffset_Y);
        BallDirection = -BallDirection;
    }
    //Right border reflection
    if (nextBallPos_X > CLevel::MaxLevelPos_X - BallSize)
    {
        nextBallPos_X = CLevel::MaxLevelPos_X - BallSize - (nextBallPos_X - (CLevel::MaxLevelPos_X - BallSize)); //consider ball speed (MaxLevelPos_X + BallSpeed)
        BallDirection = static_cast<float>(M_PI) - BallDirection;
    }
    //Bottom border reflection
    if (nextBallPos_Y > CLevel::MaxLevelPos_Y - BallSize)
    {
        nextBallPos_Y = CLevel::MaxLevelPos_Y - BallSize - (nextBallPos_Y - (CLevel::MaxLevelPos_Y - BallSize));
        BallDirection = static_cast<float>(M_PI) + static_cast<float>(M_PI) - BallDirection;
    }

    //Check new position for collision with platform
    if (nextBallPos_Y > CPlatform::Pos_Y - BallSize)
    {
        if (nextBallPos_X >= platform->Pos_X && nextBallPos_X <= platform->Pos_X + platform->Width)
        {
            nextBallPos_Y = CPlatform::Pos_Y - BallSize - (nextBallPos_Y - (CPlatform::Pos_Y - BallSize));
            BallDirection = static_cast<float>(M_PI) + static_cast<float>(M_PI) - BallDirection;
        }
    }

    level->CheckBallHitBrick(nextBallPos_Y, BallDirection);

    //Set new ball position
    BallPos_X = nextBallPos_X;
    BallPos_Y = nextBallPos_Y;

    BallRect.left = BallPos_X * CGameEngine::ResolutionScale;
    BallRect.top = BallPos_Y * CGameEngine::ResolutionScale;
    BallRect.right = BallRect.left + BallSize * CGameEngine::ResolutionScale;
    BallRect.bottom = BallRect.top + BallSize * CGameEngine::ResolutionScale;

    InvalidateRect(gameEngine->HWnd, &OldBallRect, FALSE);
    InvalidateRect(gameEngine->HWnd, &BallRect, FALSE);
}

//StartInit for ball
void CLevel::Init()
{
    CGameEngine::CreatePenAndBrush(255, 85, 255, brickPurplePen, brickPurpleBrush);

    CGameEngine::CreatePenAndBrush(85, 255, 255, brickBluePen, brickBlueBrush);

    brickWhiteLetterPen = CreatePen(PS_SOLID, CGameEngine::ResolutionScale, RGB(255, 255, 255));

    LevelRect.left = CLevel::LevelOffset_X * CGameEngine::ResolutionScale;
    LevelRect.top = CLevel::LevelOffset_Y * CGameEngine::ResolutionScale;
    LevelRect.right = LevelRect.left + CLevel::CellWidth * CLevel::LevelHeightSize * CGameEngine::ResolutionScale;
    LevelRect.bottom = LevelRect.top + CLevel::CellHeight * CLevel::LevelWidthSize * CGameEngine::ResolutionScale;
}

// Draw a brick
void CLevel::DrawBrick(HDC hdc, int x, int y, char brickColor) //TO DO: change char param to ENUM ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
    switch (brickColor)
    {
    case PURPLE:
    {
        SelectObject(hdc, brickPurplePen);
        SelectObject(hdc, brickPurpleBrush);
    }
    break;
    case BLUE:
    {
        SelectObject(hdc, brickBluePen);
        SelectObject(hdc, brickBlueBrush);
    }
    break;
    default: //Nothing, empty
        return;
    }
    RoundRect(hdc, x * CGameEngine::ResolutionScale, y * CGameEngine::ResolutionScale, (x + BrickWidth) * CGameEngine::ResolutionScale, (y + BrickHeight) * CGameEngine::ResolutionScale, 2 * CGameEngine::ResolutionScale, 2 * CGameEngine::ResolutionScale);
}

// Swap 2 colors (one brick side with other)
void CLevel::SwapBrickColors(HPEN& frontSidePen, HPEN& backSidePen, HBRUSH& frontSideBrush, HBRUSH& backSideBrush) //Swap frontside with backside colors
{
    HPEN tempPen = frontSidePen;
    HBRUSH tempBrush = frontSideBrush;

    frontSidePen = backSidePen;
    frontSideBrush = backSideBrush;

    backSidePen = tempPen;
    backSideBrush = tempBrush;
}

// Draw a brick with letter
void CLevel::DrawLetterBrick(HDC hdc, int x, int y, EBrickColor brickMainColor, EBrickLetter brickLetter, int rotationStep)
{
    float rotationAngle;
    float offset = 0.0f;

    int brickMiddleAxis_Y = BrickHeight * CGameEngine::ResolutionScale / 2; //brick Rotation Axis
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
        frontSidePen = brickPurplePen;
        frontSideBrush = brickPurpleBrush;

        backSidePen = brickBluePen;
        backSideBrush = brickBlueBrush;
    }
    else
    {
        frontSidePen = brickBluePen;
        frontSideBrush = brickBlueBrush;

        backSidePen = brickPurplePen;
        backSideBrush = brickPurpleBrush;
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

        Rectangle(hdc, x, y + brickMiddleAxis_Y - CGameEngine::ResolutionScale, x + BrickWidth * CGameEngine::ResolutionScale, y + brickMiddleAxis_Y);

        //Display backside
        SelectObject(hdc, backSidePen);
        SelectObject(hdc, backSideBrush);

        Rectangle(hdc, x, y + brickMiddleAxis_Y, x + BrickWidth * CGameEngine::ResolutionScale, y + brickMiddleAxis_Y + CGameEngine::ResolutionScale - 1);
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
        offset = 3.0f * (1.0f - fabs(xForm.eM22)) * static_cast<float>(CGameEngine::ResolutionScale); // TO DO: Change rotation function (from 0 to 3, then 3 to 0 etc) May be use COS or SIN
        backSideBrickOffset = static_cast<int>(round(offset));

        //Display front side
        SelectObject(hdc, frontSidePen);
        SelectObject(hdc, frontSideBrush);

        Rectangle(hdc, 0, -brickMiddleAxis_Y - backSideBrickOffset, BrickWidth * CGameEngine::ResolutionScale, brickMiddleAxis_Y - backSideBrickOffset);

        //Display backside
        SelectObject(hdc, backSidePen);
        SelectObject(hdc, backSideBrush);

        Rectangle(hdc, 0, -brickMiddleAxis_Y, BrickWidth * CGameEngine::ResolutionScale, brickMiddleAxis_Y);

        if (rotationStep > 4 && rotationStep < 13)
        {
            SelectObject(hdc, brickWhiteLetterPen);
            switch (brickLetter)
            {
            case Letter_O:
            {
                Ellipse(hdc, 0 + 5 * CGameEngine::ResolutionScale, -brickMiddleAxis_Y + CGameEngine::ResolutionScale, 0 + 10 * CGameEngine::ResolutionScale, brickMiddleAxis_Y - CGameEngine::ResolutionScale);
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
void CLevel::Draw(HDC hdc, RECT &paintArea)
{
    RECT intersectionRect;

    if (!IntersectRect(&intersectionRect, &paintArea, &LevelRect))
    {
        return;
    }

    for (int i = 0; i < LevelHeightSize; ++i) //14 LevelHeight
    {
        for (int j = 0; j < LevelWidthSize; ++j) //12 LevelWidth
        {
            DrawBrick(hdc, LevelOffset_X + j * CellWidth, LevelOffset_Y + i * CellHeight, Level_01[i][j]); //TO DO: Cast to ENUM or NOT? ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }
}

//Check new position for collision with level bricks and change direction and position consider reflection
void CLevel::CheckBallHitBrick(int& nextBallPos_Y, float& ballDirection)
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
                ballDirection = -ballDirection;
            }
        }

        brickPos_Y -= CellHeight;
    }

}

//ctor
CPlatform::CPlatform()
    : Width(28), Pos_X((CBorder::BorderOffset_X + CLevel::MaxLevelPos_X - Width) / 2), Step_X(CGameEngine::ResolutionScale), SpaceBetweenCircles(21)
{
}

//Start init
void CPlatform::Init()
{
    CGameEngine::CreatePenAndBrush(255, 128, 0, platformOrangePen, platformOrangeBrush);

    CGameEngine::CreatePenAndBrush(0, 128, 0, platformGreenPen, platformGreenBrush);
    
    CGameEngine::CreatePenAndBrush(225, 225, 225, platformGreyPen, platformGreyBrush);

    //Initialize first platform position for background rectangle (for clear background after moving platform)
    PlatformRect.left = Pos_X * CGameEngine::ResolutionScale;
    PlatformRect.top = Pos_Y * CGameEngine::ResolutionScale;
    PlatformRect.right = PlatformRect.left + Width * CGameEngine::ResolutionScale;
    PlatformRect.bottom = PlatformRect.top + Height * CGameEngine::ResolutionScale;

}

// Draw platform
void CPlatform::Draw(HDC hdc, RECT& paintArea, CGameEngine* gameEngine)
{
    int x = Pos_X;
    int y = Pos_Y;

    RECT intersectionRect;
    if (!IntersectRect(&intersectionRect, &paintArea, &PlatformRect))
    {
        return;
    }
    //At first draw a rectangle with platform size and background color before drawing platform itself to clear after redraw
    SelectObject(hdc, gameEngine->bgBlackPen);
    SelectObject(hdc, gameEngine->bgBlackBrush);

    Rectangle(hdc, OldPlatformRect.left, OldPlatformRect.top, OldPlatformRect.right, OldPlatformRect.bottom);

    //Cirlce platform side parts
    SelectObject(hdc, platformOrangePen);
    SelectObject(hdc, platformOrangeBrush);

    Ellipse(hdc, x * CGameEngine::ResolutionScale, y * CGameEngine::ResolutionScale, (x + CircleSize) * CGameEngine::ResolutionScale, (y + CircleSize) * CGameEngine::ResolutionScale); //Draw a ellipse in rectangle
    Ellipse(hdc, (x + SpaceBetweenCircles) * CGameEngine::ResolutionScale, y * CGameEngine::ResolutionScale, (x + SpaceBetweenCircles + CircleSize) * CGameEngine::ResolutionScale, (y + CircleSize) * CGameEngine::ResolutionScale);

    //Rectangle platform part
    SelectObject(hdc, platformGreenPen);
    SelectObject(hdc, platformGreenBrush);

    RoundRect(hdc, (x + CBall::BallSize) * CGameEngine::ResolutionScale, (y + 1) * CGameEngine::ResolutionScale, (x + 4 + 20) * CGameEngine::ResolutionScale, (y + 1 + 5) * CGameEngine::ResolutionScale, 4 * CGameEngine::ResolutionScale, 4 * CGameEngine::ResolutionScale);

    //Highlight from light
    SelectObject(hdc, platformGreyPen);
    SelectObject(hdc, platformGreyBrush);

    Rectangle(hdc, (x + 4 + 3) * CGameEngine::ResolutionScale, (y + 1 + 2) * CGameEngine::ResolutionScale, (x + 4) * CGameEngine::ResolutionScale + 24, (y + 1) * CGameEngine::ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 9) * CGameEngine::ResolutionScale, (y + 1 + 2) * CGameEngine::ResolutionScale, (x + 4) * CGameEngine::ResolutionScale + 33, (y + 1) * CGameEngine::ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 14) * CGameEngine::ResolutionScale, (y + 1 + 2) * CGameEngine::ResolutionScale, (x + 4) * CGameEngine::ResolutionScale + 45, (y + 1) * CGameEngine::ResolutionScale + 5);
    Rectangle(hdc, (x + 4 + 21) * CGameEngine::ResolutionScale, (y + 1 + 1) * CGameEngine::ResolutionScale, (x + 4) * CGameEngine::ResolutionScale + 66, (y + 1) * CGameEngine::ResolutionScale + 1);
    Rectangle(hdc, (x + 4 - 2) * CGameEngine::ResolutionScale, (y + 1 + 1) * CGameEngine::ResolutionScale, (x + 4) * CGameEngine::ResolutionScale - 3, (y + 1) * CGameEngine::ResolutionScale + 1);
    Rectangle(hdc, (x + 4 - 3) * CGameEngine::ResolutionScale, (y + 1 + 1) * CGameEngine::ResolutionScale, (x + 4) * CGameEngine::ResolutionScale - 7, (y + 1) * CGameEngine::ResolutionScale + 9);
}

// Redraw platform in other position (moving)
void CPlatform::ReDraw(CGameEngine *gameEngine)
{
    OldPlatformRect = PlatformRect;

    PlatformRect.left = Pos_X * CGameEngine::ResolutionScale;
    PlatformRect.top = Pos_Y * CGameEngine::ResolutionScale;
    PlatformRect.right = PlatformRect.left + Width * CGameEngine::ResolutionScale;
    PlatformRect.bottom = PlatformRect.top + Height * CGameEngine::ResolutionScale;

    InvalidateRect(gameEngine->HWnd, &OldPlatformRect, FALSE);
    InvalidateRect(gameEngine->HWnd, &PlatformRect, FALSE);
}

//Start init
void CBorder::Init()
{
    CGameEngine::CreatePenAndBrush(255, 255, 255, borderWhitePen, borderWhiteBrush);

    CGameEngine::CreatePenAndBrush(85, 255, 255, borderBluePen, borderBlueBrush);
}

//Draw vertical element (tile) of level border 
void CBorder::DrawVerticalElement(HDC hdc, int x, int y, CGameEngine *gameEngine)
{
    //Blue border line
    SelectObject(hdc, borderBluePen);
    SelectObject(hdc, borderBlueBrush);

    Rectangle(hdc, (x + 1) * CGameEngine::ResolutionScale, y * CGameEngine::ResolutionScale, (x + 4) * CGameEngine::ResolutionScale, (y + 4) * CGameEngine::ResolutionScale);

    //White border line
    SelectObject(hdc, borderWhitePen);
    SelectObject(hdc, borderWhiteBrush);

    Rectangle(hdc, x * CGameEngine::ResolutionScale, y * CGameEngine::ResolutionScale, (x + 1) * CGameEngine::ResolutionScale, (y + 4) * CGameEngine::ResolutionScale);

    //Holes in border line
    SelectObject(hdc, gameEngine->bgBlackPen);
    SelectObject(hdc, gameEngine->bgBlackBrush);

    Rectangle(hdc, (x + 2) * CGameEngine::ResolutionScale, (y + 1) * CGameEngine::ResolutionScale, (x + 3) * CGameEngine::ResolutionScale, (y + 2) * CGameEngine::ResolutionScale);
}

//Draw horizontal element (tile) of level border 
void CBorder::DrawHorizontalElement(HDC hdc, int x, int y, CGameEngine* gameEngine)
{
    SelectObject(hdc, borderBluePen);
    SelectObject(hdc, borderBlueBrush);

    Rectangle(hdc, x * CGameEngine::ResolutionScale, (y + 1) * CGameEngine::ResolutionScale, (x + 4) * CGameEngine::ResolutionScale, (y + 4) * CGameEngine::ResolutionScale);

    //White border line
    SelectObject(hdc, borderWhitePen);
    SelectObject(hdc, borderWhiteBrush);

    Rectangle(hdc, x * CGameEngine::ResolutionScale, y * CGameEngine::ResolutionScale, (x + 4) * CGameEngine::ResolutionScale, (y + 1) * CGameEngine::ResolutionScale);

    //Holes in border line
    SelectObject(hdc, gameEngine->bgBlackPen);
    SelectObject(hdc, gameEngine->bgBlackBrush);

    Rectangle(hdc, (x + 2) * CGameEngine::ResolutionScale, (y + 2) * CGameEngine::ResolutionScale, (x + 3) * CGameEngine::ResolutionScale, (y + 3) * CGameEngine::ResolutionScale);
}

//Draw left, right and top level border using vert and hz elements
void CBorder::Draw(HDC hdc, RECT &paintArea, CGameEngine* gameEngine)
{
    //Draw left level border
    for (int i = 0; i < 50; ++i)
    {
        DrawVerticalElement(hdc, 2, 1 + i * 4, gameEngine);
    }

    //Draw right level border
    for (int i = 0; i < 50; ++i)
    {
        DrawVerticalElement(hdc, 201, 1 + i * 4, gameEngine);
    }

    //Draw top level border
    for (int i = 0; i < 50; ++i)
    {
        DrawHorizontalElement(hdc, 3 + i * 4, 0, gameEngine);
    }
}

//ctor
CGameEngine::CGameEngine()
{
}

//Setup and initialize game resources on start
void CGameEngine::InitGameEngine(HWND hWnd)
{
    //Initialize window handle by value from Main
    HWnd = hWnd;

    CreatePenAndBrush(0, 0, 0, bgBlackPen, bgBlackBrush);

    Level.Init();
    Ball.Init();
    Platform.Init();
    Border.Init();

    //Windows timer function
    SetTimer(HWnd, TimerID, 20, 0);
}

// Draw every frame in game
void CGameEngine::DrawFrame(HDC hdc, RECT& paintArea)
{
    Level.Draw(hdc, paintArea);
    Platform.Draw(hdc, paintArea, this);

   /* for (int i = 0; i < 16; ++i)
    {
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 100, EBrickColor::BLUE, EBrickLetter::Letter_O, i);
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 130, EBrickColor::PURPLE, EBrickLetter::Letter_O, i);
    }*/
    
    Ball.Draw(hdc, paintArea, this);

    Border.Draw(hdc, paintArea, this);
}

//Handles keybord's keys
int CGameEngine::OnKeyDown(EKeyType keyType)
{
    switch (keyType)
    {
    case LeftArrowKey:
        Platform.Pos_X -= Platform.Step_X;

        if (Platform.Pos_X <= CBorder::BorderOffset_X)
        {
            Platform.Pos_X = CBorder::BorderOffset_X;
        }

        Platform.ReDraw(this);
        break;

    case RightArrowKey:
        Platform.Pos_X += Platform.Step_X;

        if (Platform.Pos_X >= CLevel::MaxLevelPos_X - Platform.Width)
        {
            Platform.Pos_X = CLevel::MaxLevelPos_X - Platform.Width;
        }

        Platform.ReDraw(this);
        break;

    case SpaceKey:
        break;
    }
    return 0;
}

int CGameEngine::On_Timer()
{
    Ball.Move(this, &Level, &Platform);
    return 0;
}

//Assigns color to pens and brushes
void CGameEngine::CreatePenAndBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));
}