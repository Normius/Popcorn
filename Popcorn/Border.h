#pragma once

#include "Config.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBorder
class CBorder
{
public:
    CBorder();

    void Init();
    void Draw(HDC hdc, RECT& paintArea, HPEN bgBlackPen, HBRUSH bgBlackBrush);

private:
    void DrawVerticalElement(HDC hdc, int x, int y, HPEN bgBlackPen, HBRUSH bgBlackBrush);
    void DrawHorizontalElement(HDC hdc, int x, int y, HPEN bgBlackPen, HBRUSH bgBlackBrush);

    HPEN borderBluePen, borderWhitePen;
    HBRUSH borderBlueBrush, borderWhiteBrush;
};
