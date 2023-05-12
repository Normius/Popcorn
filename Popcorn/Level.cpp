#include "Level.h"

char CLevel::Level_01[CConfig::LevelHeightSize][CConfig::LevelWidthSize]
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CLevel
//Ctor
CLevel::CLevel()
    :brickPurplePen(0), brickBluePen(0), brickWhiteLetterPen(0), brickPurpleBrush(0), brickBlueBrush(0), LevelRect{}
{
}

//StartInit for ball
void CLevel::Init()
{
    CConfig::CreatePenAndBrush(255, 85, 255, brickPurplePen, brickPurpleBrush);

    CConfig::CreatePenAndBrush(85, 255, 255, brickBluePen, brickBlueBrush);

    brickWhiteLetterPen = CreatePen(PS_SOLID, CConfig::ResolutionScale, RGB(255, 255, 255));

    LevelRect.left = CConfig::LevelOffset_X * CConfig::ResolutionScale;
    LevelRect.top = CConfig::LevelOffset_Y * CConfig::ResolutionScale;
    LevelRect.right = LevelRect.left + CConfig::CellWidth * CConfig::LevelHeightSize * CConfig::ResolutionScale;
    LevelRect.bottom = LevelRect.top + CConfig::CellHeight * CConfig::LevelWidthSize * CConfig::ResolutionScale;
}

// Draw all bricks on level
void CLevel::Draw(HWND hwnd, HDC hdc, RECT& paintArea)
{
    RECT intersectionRect;

    if (!IntersectRect(&intersectionRect, &paintArea, &LevelRect))
    {
        return;
    }

    for (int i = 0; i < CConfig::LevelHeightSize; ++i) //14 LevelHeight
    {
        for (int j = 0; j < CConfig::LevelWidthSize; ++j) //12 LevelWidth
        {
            DrawBrick(hdc, CConfig::LevelOffset_X + j * CConfig::CellWidth, CConfig::LevelOffset_Y + i * CConfig::CellHeight, Level_01[i][j]); //TO DO: Cast to ENUM or NOT? ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }

    ActiveBrick.Draw(hdc, paintArea);
}

//Check new position for collision with level bricks and change direction and position consider reflection
void CLevel::CheckBallHitBrick(int& nextBallPos_Y, float& ballDirection)
{
    int brickPos_Y = CConfig::LevelOffset_Y + CConfig::LevelHeightSize * CConfig::CellHeight;

    for (int i = CConfig::LevelHeightSize - 1; i >= 0; --i)
    {
        for (int j = CConfig::LevelWidthSize - 1; j >= 0; --j)
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

        brickPos_Y -= CConfig::CellHeight;
    }

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
    RoundRect(hdc, x * CConfig::ResolutionScale, y * CConfig::ResolutionScale, (x + CConfig::BrickWidth) * CConfig::ResolutionScale, (y + CConfig::BrickHeight) * CConfig::ResolutionScale, 2 * CConfig::ResolutionScale, 2 * CConfig::ResolutionScale);
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

    int brickMiddleAxis_Y = CConfig::BrickHeight * CConfig::ResolutionScale / 2; //brick Rotation Axis
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

        Rectangle(hdc, x, y + brickMiddleAxis_Y - CConfig::ResolutionScale, x + CConfig::BrickWidth * CConfig::ResolutionScale, y + brickMiddleAxis_Y);

        //Display backside
        SelectObject(hdc, backSidePen);
        SelectObject(hdc, backSideBrush);

        Rectangle(hdc, x, y + brickMiddleAxis_Y, x + CConfig::BrickWidth * CConfig::ResolutionScale, y + brickMiddleAxis_Y + CConfig::ResolutionScale - 1);
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
        offset = 3.0f * (1.0f - fabs(xForm.eM22)) * static_cast<float>(CConfig::ResolutionScale); // TO DO: Change rotation function (from 0 to 3, then 3 to 0 etc) May be use COS or SIN
        backSideBrickOffset = static_cast<int>(round(offset));

        //Display front side
        SelectObject(hdc, frontSidePen);
        SelectObject(hdc, frontSideBrush);

        Rectangle(hdc, 0, -brickMiddleAxis_Y - backSideBrickOffset, CConfig::BrickWidth * CConfig::ResolutionScale, brickMiddleAxis_Y - backSideBrickOffset);

        //Display backside
        SelectObject(hdc, backSidePen);
        SelectObject(hdc, backSideBrush);

        Rectangle(hdc, 0, -brickMiddleAxis_Y, CConfig::BrickWidth * CConfig::ResolutionScale, brickMiddleAxis_Y);

        if (rotationStep > 4 && rotationStep < 13)
        {
            SelectObject(hdc, brickWhiteLetterPen);
            switch (brickLetter)
            {
            case Letter_O:
            {
                Ellipse(hdc, 0 + 5 * CConfig::ResolutionScale, -brickMiddleAxis_Y + CConfig::ResolutionScale, 0 + 10 * CConfig::ResolutionScale, brickMiddleAxis_Y - CConfig::ResolutionScale);
            }
            break;
            default: //Nothing, empty
                return;
            }

        }

        SetWorldTransform(hdc, &oldXForm);
    }
}
