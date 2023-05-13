#pragma once

#include "Config.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBorder
class CBorder
{
public:
    CBorder();

    void Init();
    void Draw(HDC hdc, RECT& paintArea);

private:
    void DrawVerticalElement(HDC hdc, int x, int y);
    void DrawHorizontalElement(HDC hdc, int x, int y);

    HPEN borderBluePen, borderWhitePen;
    HBRUSH borderBlueBrush, borderWhiteBrush;
};
