﻿#pragma once

#include "Ball.h"

enum EPlatformState
{
    PlatformMissing,
    PlatformNormal,
    PlatformHoldingBall,
    PlatformMeltdown,
    PlatformRolling,
    PlatformExpanding
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPlatform
class CPlatform: public CHitChecker
{
public:
    CPlatform();

    virtual bool CheckHit(float nextBallPos_X, float nextBallPos_Y, CBall* ball) override;

    void Init();
    void Act();
    EPlatformState GetState();
    void SetState(EPlatformState newState);
    void Draw(HDC hdc, RECT& paintArea);
    void ReDraw();

    int width;
    int pos_X;
    int step_X;

private:
    void ClearBackground(HDC hdc);
    void DrawNormalState(HDC hdc, RECT& paintArea);
    void DrawMeltingState(HDC hdc, RECT& paintArea);
    void DrawRollingState(HDC hdc, RECT& paintArea);
    void DrawExpandingState(HDC hdc, RECT& paintArea);

    EPlatformState PlatformState;
    static const int NormalWidth = 28;

    int spaceBetweenCircles;
    int meltingPlatformPos_Y[NormalWidth * CConfig::ResolutionScale];
    int rollingStep;

    RECT PlatformRect, OldPlatformRect;
    HPEN platformCircleOrangePen, platformGreenPen, platformGreyPen;
    HBRUSH platformCircleOrangeBrush, platformGreenBrush, platformGreyBrush;

    static const int CircleSize = 7;
    static const int Height = 7;
    static const int NormalSpaceBetweenCircles = NormalWidth - CircleSize;
    static const int MeltingSpeed = 3;
    static const int rollingStepMax = 16;
    static const int stopRollingPos_X = 99;
    static const int rollingPlatformSpeed = 3;
};
