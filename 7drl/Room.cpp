#include "stdafx.h"
#include "Room.h"
#include "Draw.h"


Room::Room(int _x,int _y,int _w,int _h)
{
	x=_x;
	y=_y;
	w=_w;
	h=_h;
}


Room::~Room(void)
{
}

void Room::update()
{
	for(int i=x;i<x+w;i++)
		for(int j=y;j<y+h;j++)
		{
			if(i==x || i==x+w-1 || j==y || j==y+h-1)
				Draw::p(i,j,'#');
			else
				Draw::p(i,j,'.');
		}
}
