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
	if(o->shouldRemove)
		return;
	Room *room=NULL;
	int dx=o->neighbors[n].x;
	int dy=o->neighbors[n].y;
	int tries=0;
start:
	if(tries++>50)
	{
		return;
	}
	if(room!=NULL)
		delete room;
	int makeRoom=rand()%3+rand()%2*o->isHall();
	int w=rand()%10+7;
	int h=(((float)(rand()%800))/1000+.5)*w;
	if(!o->isHall() || (makeRoom==0 && !(o->w==3 && (dy==o->y || dy==o->y2)) && !(o->h==3 && (dx==o->x || dx==o->x2))))//make hall
	{
		//w-=rand()%2;
		//h-=rand()%2;
		w=min(w,7);
		h=min(h,7);
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
			room=new Room(dx-w+1,dy-rand()%(h-2)-1,w,h);
			if(!room->isValid())
				goto start;
		}
		else if(dx==o->x2)//room to right
		{
			room=new Room(dx,dy-rand()%(h-2)-1,w,h);
			if(!room->isValid())
				goto start;
		}
		else if(dy==o->y)//room above
		{
			room=new Room(dx-rand()%(w-2)-1,dy-h+1,w,h);
			if(!room->isValid())
				goto start;
		}
		else //room below
		{
			room=new Room(dx-rand()%(w-2)-1,dy,w,h);
			if(!room->isValid())
				goto start;
		}
	}
	size_t s=rooms.size();
	Room **rs=rooms.data();
	for(size_t i=0;i<s;i++,rs++)
		{
			Room *it=*rs;
			if((it->isHall()!=room->isHall()) && rand()%6!=0)
				continue;
			{
				int x=-99999;
				if(it->x==room->x2)
					x=it->x;
				else if(it->x2==room->x)
					x=it->x2;
				if(x!=-99999)
				{
					int y1=max(it->y,room->y);
					int y2=min(it->y2,room->y2);
					for(int y=y1+1;y<y2;y++)
					{
						it->connectTo(room,x,y,1);
					}
				}
			}
			{
				int y=-99999;
				if(it->y==room->y2)
					y=it->y;
				else if(it->y2==room->y)
					y=it->y2;
				if(y!=-99999)
				{
					int x1=max(it->x,room->x);
					int x2=min(it->x2,room->x2);
					for(int x=x1+1;x<x2;x++)
					{
						it->connectTo(room,x,y,1);
					}
				}
			}
		}
	//room valid
	rooms.push_back(room);
	o->neighbors[n].room=room;
	o->neighbors[n].close();
	room->neighbors.push_back(Door(o,dx,dy,CLOSED));

	//add more doors
	room->addDoors();
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
bool Draw::pg( int x,int y,char c )
{
	if(x>=0 && y>=0 && x<W && y<H)
	{
		screen[y][x]=c;
		return 1;
	}
	return 0;
}
void Draw::str( char *str,int i,int y )
{
	for(char *c=str;*c!='\0';c++,i++)
		Draw::p(i,y,*c);
}
void Draw::strg( const char *str,int i,int y )
{
	for(char *c=(char*)str;*c!='\0';c++,i++)
		Draw::pg(i,y,*c);
}

char screen[H][W];