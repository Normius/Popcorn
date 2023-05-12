#include "GameEngine.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CGameEngine
//ctor
CGameEngine::CGameEngine()
    :HWnd(0), bgBlackPen(0), bgBlackBrush(0)
{
}

//Setup and initialize game resources on start
void CGameEngine::InitGameEngine(HWND hWnd)
{
    //Initialize window handle by value from Main
    HWnd = hWnd;

    CConfig::CreatePenAndBrush(0, 0, 0, bgBlackPen, bgBlackBrush);

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
    Platform.Draw(hdc, paintArea, bgBlackPen, bgBlackBrush);

   /* for (int i = 0; i < 16; ++i)
    {
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 100, EBrickColor::BLUE, EBrickLetter::Letter_O, i);
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 130, EBrickColor::PURPLE, EBrickLetter::Letter_O, i);
    }*/
    
    Ball.Draw(hdc, paintArea, bgBlackPen, bgBlackBrush);

    Border.Draw(hdc, paintArea, bgBlackPen, bgBlackBrush);
}

//Handles keybord's keys
int CGameEngine::OnKeyDown(EKeyType keyType)
{
    switch (keyType)
    {
    case LeftArrowKey:
        Platform.Pos_X -= Platform.Step_X;

        if (Platform.Pos_X <= CConfig::BorderOffset_X)
        {
            Platform.Pos_X = CConfig::BorderOffset_X;
        }

        Platform.ReDraw(HWnd);
        break;

    case RightArrowKey:
        Platform.Pos_X += Platform.Step_X;

        if (Platform.Pos_X >= CConfig::MaxLevelPos_X - Platform.Width + 1)
        {
            Platform.Pos_X = CConfig::MaxLevelPos_X - Platform.Width + 1;
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
    Ball.Move(HWnd, &Level, Platform.Pos_X, Platform.Width);
    return 0;
}