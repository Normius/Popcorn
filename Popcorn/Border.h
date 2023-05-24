#pragma once

#include "Ball.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBorder
class CBorder: public CHitChecker
{
public:
    CBorder();

    virtual bool CheckHit(float nextBallPos_X, float nextBallPos_Y, CBall* ball) override;

    void Init();
    void Draw(HDC hdc, RECT& paintArea);
    

private:
    void DrawVerticalElement(HDC hdc, int x, int y);
    void DrawHorizontalElement(HDC hdc, int x, int y);

    HPEN borderBluePen, borderWhitePen;
    HBRUSH borderBlueBrush, borderWhiteBrush;
};
