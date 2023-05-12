﻿#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CConfig
class CConfig
{
public:
    static void CreatePenAndBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

    static const int PlatformPos_Y = 185;
    static const int ResolutionScale = 3;
    static const int BorderOffset_X = 6;
    static const int BorderOffset_Y = 4;
    static const int CellWidth = 16; //BrickWidth + 1 pxl for gorizontal space between bricks
    static const int CellHeight = 8; //Brickheight + 1 pxl for vertical space between bricks
    static const int LevelWidthSize = 12; //Width in Bricks
    static const int LevelHeightSize = 14; //Height in Bricks
    static const int LevelOffset_X = 8;
    static const int LevelOffset_Y = 6;
    static const int BallSize = 4;
    static const int MaxLevelPos_X = LevelOffset_X + CellWidth * LevelWidthSize;
    static const int MaxLevelPos_Y = 199 - BallSize;


};