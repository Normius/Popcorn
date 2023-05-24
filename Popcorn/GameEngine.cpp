#include "GameEngine.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CGameEngine
//ctor
CGameEngine::CGameEngine()
    :GameState(EGameState::LevelPlaying)
{
}

//Setup and initialize game resources on start
void CGameEngine::InitGameEngine(HWND hWnd)
{
    //Initialize window handle by value from Main
    CConfig::HWnd = hWnd;

    CActiveBrick::SetupFadingBrickColors();

    Level.Init();
    Platform.Init();
    Ball.Init();
    Border.Init();

    Ball.SetState(EBallState::BallNormal, static_cast<float>(Platform.pos_X + Platform.width / 2));

    Platform.SetState(EPlatformState::PlatformNormal);
    Platform.ReDraw();

    //Windows timer function
    SetTimer(CConfig::HWnd, TimerID, 1000 / CConfig::FPS, 0);
}

// Draw every frame in game
void CGameEngine::DrawFrame(HDC hdc, RECT& paintArea)
{
    Level.Draw(hdc, paintArea);

   /* for (int i = 0; i < 16; ++i)
    {
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 100, EBrickColor::BLUE, EBrickLetter::Letter_O, i);
        DrawLetterBrick(hdc, 20 + i * CellWidth * ResolutionScale, 130, EBrickColor::PURPLE, EBrickLetter::Letter_O, i);
    }*/
    
    Ball.Draw(hdc, paintArea);
    Border.Draw(hdc, paintArea);
    Platform.Draw(hdc, paintArea);
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

        Platform.ReDraw();
        break;

    case RightArrowKey:
        Platform.pos_X += Platform.step_X;

        if (Platform.pos_X >= CConfig::MaxLevelPos_X - Platform.width + 1)
        {
            Platform.pos_X = CConfig::MaxLevelPos_X - Platform.width + 1;
        }

        Platform.ReDraw();
        break;

    case SpaceKey:
        if (Platform.GetState() == EPlatformState::PlatformHoldingBall)
        {
            Ball.SetState(EBallState::BallNormal, static_cast<float>(Platform.pos_X + Platform.width / 2));
            Platform.SetState(EPlatformState::PlatformNormal);
        }
        break;
    }
    return 0;
}

int CGameEngine::On_Timer()
{
    ++CConfig::TimerCounter;

    switch (GameState)
    {
    case LevelPlaying:
        Ball.Move(Platform.pos_X, Platform.width, &Level, &Border); //Bind ball moving to the timer 

        if (Ball.GetState() == EBallState::BallMissing)
        {
            GameState = EGameState::BallLost;
            Platform.SetState(EPlatformState::PlatformMeltdown);
        }
        break;

    case BallLost:
        if (Platform.GetState() == EPlatformState::PlatformMissing)
        { 
            GameState = EGameState::RestartLevel;
            Platform.SetState(EPlatformState::PlatformRolling);
        }
        break;

    case RestartLevel:
        if (Platform.GetState() == EPlatformState::PlatformHoldingBall)
        {
            GameState = EGameState::LevelPlaying;
            Ball.SetState(EBallState::BallOnPlatform, static_cast<float>(Platform.pos_X + Platform.width / 2));
        }
        break;

    default:
        break;
    }
    Platform.Act();
    //Level.ActiveBrick.Act(); //Bind active brick fading to the timer

    return 0;
}