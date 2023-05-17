#pragma once

#include "Config.h"

enum EPlatformState
{
    Missing,
    Normal,
    Meltdown,
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPlatform
class CPlatform
{
public:
    CPlatform();

    void Init();
    void Act(HWND hWnd);
    void Draw(HDC hdc, RECT& paintArea);
    void ReDraw(HWND hWnd);

    int width;
    int pos_X;
    int step_X;

private:
    void DrawNormalState(HDC hdc, RECT& paintArea);
    void DrawMeltingState(HDC hdc, RECT& paintArea);

    EPlatformState PlatformState;

    static const int NormalWidth = 28;

    int spaceBetweenCircles;
    int meltingPlatformPos_Y[NormalWidth * CConfig::ResolutionScale];

    RECT PlatformRect, OldPlatformRect;
    HPEN platformOrangePen, platformGreenPen, platformGreyPen;
    HBRUSH platformOrangeBrush, platformGreenBrush, platformGreyBrush;

    static const int CircleSize = 7;
    static const int Height = 7;
    static const int MeltingSpeed = 3;
};
