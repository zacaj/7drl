#include "stdafx.h"
#include "Draw.h"
#include <windows.h>
#include "Room.h"
namespace Draw
{
	 int cx=0,cy=0,ncx=0,ncy=0;
HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;
}

void genRoom( Room* o,int n )
{
	Room *room=NULL;
	int dx=o->dx[n];
	int dy=o->dy[n];
	int tries=0;
start:
	if(tries++>50)
	{
		return;
	}
	if(room!=NULL)
		delete room;
	int makeRoom=rand()%3+rand()%2*o->isHall();
	int w=rand()%20+10;
	int h=rand()%20+10;
	if(!o->isHall() || makeRoom==0)//make hall
	{
		if(dx==o->x2)//horizontal hall to right
		{
			room=new Room(dx,dy-1,w,3);
			if(!room->isValid())
				goto start;
		}
		else if(dx==o->x)//horizontal hall to left
		{
			room=new Room(dx-w+1,dy-1,w,3);
			if(!room->isValid())
				goto start;
		}
		else if(dy==o->y2)//vertical hall down
		{
			room=new Room(dx-1,dy,3,h);
			if(!room->isValid())
				goto start;
		}
		else //vertical hall up
		{
			room=new Room(dx-1,dy-h+1,3,h);
			if(!room->isValid())
				goto start;
		}
	}
	else//off a hall, make a room
	{
		if(dx==o->x)//room to left
		{
			room=new Room(dx-w,dy-rand()%(h-2),w,h);
			if(!room->isValid())
				goto start;
		}
		else if(dx==o->x2)//room to right
		{
			room=new Room(dx,dy-rand()%(h-2),w,h);
			if(!room->isValid())
				goto start;
		}
		else if(dy==o->y)//room above
		{
			room=new Room(dx-rand()%(w-2),dy-h,w,h);
			if(!room->isValid())
				goto start;
		}
		else //room below
		{
			room=new Room(dx-rand()%(w-2),dy,w,h);
			if(!room->isValid())
				goto start;
		}
	}
	//room valid
	rooms.insert(room);
	o->neighbors[n]=room;
	room->neighbors.push_back(o);
	room->dx.push_back(dx);
	room->dy.push_back(dy);

	//add more doors
	if(room->isHall())
	{
		int x,y;
		if(room->w==3)//vertical
		{
			if(room->dy[0]==room->y)//down
			{
				switch (rand()%3)
				{
				case 0:
					x=room->x+1;
					y=room->y2;
					break;
				case 1:
					x=room->x;
					y=room->y2-1;
					break;
				case 2:
					x=room->x2;
					y=room->y2-1;
					break;
				}
			}
			else//up
			{
				switch (rand()%3)
				{
				case 0:
					x=room->x+1;
					y=room->y;
					break;
				case 1:
					x=room->x;
					y=room->y+1;
					break;
				case 2:
					x=room->x2;
					y=room->y+1;
					break;
				}
			}
		}
		else//horizontal
		{
			if(room->dx[0]==room->x)//left
			{
				switch (rand()%3)
				{
				case 0:
					x=room->x;
					y=room->y+1;
					break;
				case 1:
					x=room->x+1;
					y=room->y;
					break;
				case 2:
					x=room->x+1;
					y=room->y2;
					break;
				}
			}
			else//up
			{
				switch (rand()%3)
				{
				case 0:
					x=room->x2;
					y=room->y+1;
					break;
				case 1:
					x=room->x2-1;
					y=room->y;
					break;
				case 2:
					x=room->x2-1;
					y=room->y2;
					break;
				}
			}
		}
		room->neighbors.push_back(NULL);
		room->dx.push_back(x);
		room->dy.push_back(y);
	}
	else
	{
		int nDoor=rand()%4+1;
		for(int i=0;i<nDoor;i++)
		{
			int x,y;
			if(rand()%2==0)//vertical wall door
			{
				y=rand()%(room->h-2)+room->y+1;
				x=rand()%2==0?room->x:room->x2;
			}
			else
			{
				x=rand()%(room->w-2)+room->x+1;
				y=rand()%2==0?room->h:room->y2;
			}
			room->neighbors.push_back(NULL);
			room->dx.push_back(x);
			room->dy.push_back(y);
		}
	}
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

bool Draw::p( int x,int y,char c )
{
	x-=cx;
	y-=cy;
	if(x>=0 && y>=0 && x<WSW && y<WSH)
	{
		screen[y][x]=c;
		return 1;
	}
	return 0;
}

char screen[H][W];