#pragma once

#include "Config.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPlatform
class CPlatform
{
public:
    CPlatform();

    void Init();
    void Draw(HDC hdc, RECT& paintArea);
    void ReDraw(HWND hWnd);

    int Width;
    int Pos_X;
    int Step_X;

private:
    int SpaceBetweenCircles;

    RECT PlatformRect, OldPlatformRect;
    HPEN platformOrangePen, platformGreenPen, platformGreyPen;
    HBRUSH platformOrangeBrush, platformGreenBrush, platformGreyBrush;

    static const int CircleSize = 7;
    static const int Height = 7;
};
