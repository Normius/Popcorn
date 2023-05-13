#include "Config.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CColor
CColor::CColor(unsigned char r, unsigned char g, unsigned char b)
    : R(r), G(g), B(b)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CConfig

const CColor CConfig::backgroundColor(0, 0, 0);
const CColor CConfig::brickPurpleColor(255, 85, 255);
const CColor CConfig::brickBlueColor(85, 255, 255);

HPEN CConfig::backgroundPen;
HBRUSH CConfig::backgroundBrush;

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