#include "Config.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CColor
CColor::CColor(unsigned char r, unsigned char g, unsigned char b)
    : R(r), G(g), B(b)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CConfig
bool CConfig::LevelHasFloor = false;
int CConfig::TimerCounter = 0;

const CColor CConfig::backgroundColor(0, 0, 0);
const CColor CConfig::brickPurpleColor(255, 85, 255);
const CColor CConfig::brickBlueColor(85, 255, 255);

HPEN CConfig::backgroundPen;
HBRUSH CConfig::backgroundBrush;
HWND CConfig::HWnd;

//Assigns color to pens and brushes
void CConfig::CreatePenAndBrush(const CColor& color, HPEN& pen, HBRUSH& brush)
{
    pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
    brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}

void CConfig::CreatePenAndBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));
}

//Generates pseudorandom number in range: [0,..., range -1]
int CConfig::Rand(int range)
{
    return rand() * range / RAND_MAX;
}