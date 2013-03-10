#pragma once
#include <windows.h>
namespace Draw
{
	void init();
	extern HANDLE wHnd;    // Handle to write to the console.
	extern HANDLE rHnd;
	void setCursor(int x,int y);
	bool p(int x,int y,char c);
	extern int cx,cy,ncx,ncy;
}

#define CW W
#define CH 8
#define W 80
#define WMO W-1
#define H 40
#define HMO H-1
#define WSH H-CH
#define AW 16
#define AH H-CH
#define WSW W-AW

extern char screen[H][W];
class Room;
void genRoom(Room* o,int n);