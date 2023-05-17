﻿#include "GameEngine.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CGameEngine
//ctor
CGameEngine::CGameEngine()
    :HWnd(0)
{
}

//Setup and initialize game resources on start
void CGameEngine::InitGameEngine(HWND hWnd)
{
    //Initialize window handle by value from Main
    HWnd = hWnd;

    CActiveBrick::SetupFadingBrickColors();

    Level.Init();
    Ball.Init();
    Platform.Init();
    Border.Init();

    //Windows timer function
    SetTimer(HWnd, TimerID, 1000 / CConfig::FPS, 0);
}

// Draw every frame in game
void CGameEngine::DrawFrame(HDC hdc, RECT& paintArea)
{
    Level.Draw(HWnd, hdc, paintArea);
    Platform.Draw(hdc, paintArea);

   /* for (int i = 0; i < 16; ++i)
    {
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 100, EBrickColor::BLUE, EBrickLetter::Letter_O, i);
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 130, EBrickColor::PURPLE, EBrickLetter::Letter_O, i);
    }*/
    
    Ball.Draw(hdc, paintArea);

    Border.Draw(hdc, paintArea);
}

//Handles keybord's keys
int CGameEngine::OnKeyDown(EKeyType keyType)
{
    switch (keyType)
    {
    case LeftArrowKey:
        Platform.pos_X -= Platform.step_X;

        if (Platform.pos_X <= CConfig::BorderOffset_X)
        {
            Platform.pos_X = CConfig::BorderOffset_X;
        }

        Platform.ReDraw(HWnd);
        break;

    case RightArrowKey:
        Platform.pos_X += Platform.step_X;

        if (Platform.pos_X >= CConfig::MaxLevelPos_X - Platform.width + 1)
        {
            Platform.pos_X = CConfig::MaxLevelPos_X - Platform.width + 1;
        }

        Platform.ReDraw(HWnd);
        break;

    case SpaceKey:
        break;
    }
    return 0;
}

int CGameEngine::On_Timer()
{
    ++CConfig::TimerCounter;

    Ball.Move(HWnd, &Level, Platform.pos_X, Platform.width); //Bind ball moving to the timer 

    Level.ActiveBrick.Act(HWnd); //Bind active brick fading to the timer

    Platform.Act(HWnd);

    return 0;
}