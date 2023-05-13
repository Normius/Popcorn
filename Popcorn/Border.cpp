#include "Border.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBorder
//Ctor
CBorder::CBorder()
    :borderBluePen(0), borderWhitePen(0), borderBlueBrush(0), borderWhiteBrush(0)
{
}

//Start init
void CBorder::Init()
{
    CConfig::CreatePenAndBrush(255, 255, 255, borderWhitePen, borderWhiteBrush);

    CConfig::CreatePenAndBrush(85, 255, 255, borderBluePen, borderBlueBrush);
}

//Draw left, right and top level border using vert and hz elements
void CBorder::Draw(HDC hdc, RECT& paintArea)
{
    //Draw left level border
    for (int i = 0; i < 50; ++i)
    {
        DrawVerticalElement(hdc, 2, 1 + i * 4);
    }

    //Draw right level border
    for (int i = 0; i < 50; ++i)
    {
        DrawVerticalElement(hdc, 201, 1 + i * 4);
    }

    //Draw top level border
    for (int i = 0; i < 50; ++i)
    {
        DrawHorizontalElement(hdc, 3 + i * 4, 0);
    }
}

//Draw vertical element (tile) of level border 
void CBorder::DrawVerticalElement(HDC hdc, int x, int y)
{
    //Blue border line
    SelectObject(hdc, borderBluePen);
    SelectObject(hdc, borderBlueBrush);

    Rectangle(hdc, (x + 1) * CConfig::ResolutionScale, y * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale, (y + 4) * CConfig::ResolutionScale);

    //White border line
    SelectObject(hdc, borderWhitePen);
    SelectObject(hdc, borderWhiteBrush);

    Rectangle(hdc, x * CConfig::ResolutionScale, y * CConfig::ResolutionScale, (x + 1) * CConfig::ResolutionScale, (y + 4) * CConfig::ResolutionScale);

    //Holes in border line
    SelectObject(hdc, CConfig::backgroundPen);
    SelectObject(hdc, CConfig::backgroundBrush);

    Rectangle(hdc, (x + 2) * CConfig::ResolutionScale, (y + 1) * CConfig::ResolutionScale, (x + 3) * CConfig::ResolutionScale, (y + 2) * CConfig::ResolutionScale);
}

//Draw horizontal element (tile) of level border 
void CBorder::DrawHorizontalElement(HDC hdc, int x, int y)
{
    SelectObject(hdc, borderBluePen);
    SelectObject(hdc, borderBlueBrush);

    Rectangle(hdc, x * CConfig::ResolutionScale, (y + 1) * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale, (y + 4) * CConfig::ResolutionScale);

    //White border line
    SelectObject(hdc, borderWhitePen);
    SelectObject(hdc, borderWhiteBrush);

    Rectangle(hdc, x * CConfig::ResolutionScale, y * CConfig::ResolutionScale, (x + 4) * CConfig::ResolutionScale, (y + 1) * CConfig::ResolutionScale);

    //Holes in border line
    SelectObject(hdc, CConfig::backgroundPen);
    SelectObject(hdc, CConfig::backgroundBrush);

    Rectangle(hdc, (x + 2) * CConfig::ResolutionScale, (y + 2) * CConfig::ResolutionScale, (x + 3) * CConfig::ResolutionScale, (y + 3) * CConfig::ResolutionScale);
}