#include "stdafx.h"
#include "pixels.h"

unsigned int pixelBuffer[1000][1000];	//÷°ª∫≥Â
//unsigned short zBuffer[1000][1000];		//…Ó∂»ª∫¥Ê
unsigned int curColor;

void clearPixel(unsigned int bgColor)
{
	//memset(pixelBuffer, bgColor, sizeof(pixelBuffer)/sizeof(unsigned));
	for (int i = 0; i < 1000; i++)
		for (int j = 0; j < 1000; j++)
			pixelBuffer[i][j] = bgColor;
}

void setColor(unsigned int color)
{
	curColor = color;
}

void writePixel(int x, int y)
{
	pixelBuffer[x + 500][y + 500] = curColor;
}

unsigned int readPixel(int x, int y)
{
	return pixelBuffer[x + 500][y + 500];
}