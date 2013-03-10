#include "stdafx.h"
#include "Room.h"
#include "Draw.h"
#include <set>


set<Room*> rooms;
Room::Room(int _x,int _y,int _w,int _h)
{
	x=_x;
	y=_y;
	w=_w;
	h=_h;
	x2=x+w-1;
	y2=y+h-1;
}


Room::~Room(void)
{
}

void Room::update()
{
	bool visible;
	for(int i=x;i<x+w;i++)
		for(int j=y;j<y+h;j++)
		{
			if(i==x || i==x+w-1 || j==y || j==y+h-1)
				visible|=Draw::p(i,j,'#');
			else
				visible|=Draw::p(i,j,'.');
		}
	for(int i=0;i<dx.size();i++)
		Draw::p(dx[i],dy[i],'.');
	if(visible)
	{
		for(int i=0;i<neighbors.size();)
		{
			if(neighbors[i]==NULL)
				genRoom(this,i);
			if(neighbors[i]==NULL)
			{
				neighbors.erase(neighbors.begin()+i);
				dx.erase(dx.begin()+i);
				dy.erase(dy.begin()+i);
			}
			else
			{
				i++;
			}
		}
	}
}

void Room::connectTo( Room *room,int x,int y )
{
	room->neighbors.push_back(this);
	neighbors.push_back(room);
	room->dx.push_back(x);
	dx.push_back(x);
	room->dy.push_back(y);
	dy.push_back(y);
}

bool Room::isHall()
{
	return w==3 || h==3;
}

bool Room::isValid()
{
	for(auto it:rooms)
	{
		if(it==this)
			continue;
		if(x >= it->x2) continue;
		if(y >= it->y2) continue;
		if(x2 <= it->x) continue;
		if(y2 <= it->y) continue;

		return 0;
	}
	return 1;
}
