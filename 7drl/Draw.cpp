#include "stdafx.h"
#include "Draw.h"
#include <windows.h>
namespace Draw
{
	 int cx=0,cy=0;
HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;
}
COORD coord(int x,int y);
void Draw::init()
{
	// Set up the handles for reading/writing:
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);

	// Change the window title:
	SetConsoleTitle(TEXT("7DRL"));

	// Set up the required window size:
	SMALL_RECT windowSize = {0, 0, WMO, HMO};

	// Change the console window size:
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	// Create a COORD to hold the buffer size:
	COORD bufferSize = {W, H};

	// Change the internal buffer size:
	SetConsoleScreenBufferSize(wHnd, bufferSize);
}

void Draw::setCursor( int x,int y )
{
	SetConsoleCursorPosition(Draw::wHnd,coord(x,y));
}

void Draw::p( int x,int y,char c )
{
	x-=cx;
	y-=cy;
	if(x>=0 && y>=0 && x<WSW && y<WSH)
		screen[y][x]=c;
}

char screen[H][W];