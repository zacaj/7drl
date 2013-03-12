#include "stdafx.h"
#include "Player.h"
#define LEFT 19424
#define RIGHT 19936
#define UP 18656
#define DOWN 20704
#include "Draw.h"
#include "Room.h"


Player::Player(int _x,int _y)
{
	x=_x;
	y=_y;
	c='@';
}


Player::~Player(void)
{
}

bool Player::update( int key )
{
	bool ret=0;
	int newx=x,newy=y;
	switch(key)
	{
	case LEFT:
		newx--;
		ret=1;
		break;
	case RIGHT:
		newx++;
		ret=1;
		break;
	case UP:
		newy--;
		ret=1;
		break;
	case DOWN:
		newy++;
		ret=1;
		break;
	}
	if(newx!=x || newy!=y)
		if(newx==currentRoom->x || newx==currentRoom->x+currentRoom->w-1 || newy==currentRoom->y || newy==currentRoom->y+currentRoom->h-1)
		{
			int i;
			for(i=0;i<currentRoom->neighbors.size();i++)
			{
				if(currentRoom->neighbors[i].x==newx && currentRoom->neighbors[i].y==newy && currentRoom->neighbors[i].room!=NULL)
					break;
			}
			if(i==currentRoom->neighbors.size())
			{

			}
			else
			{
				if(currentRoom->neighbors[i].isOpen())
				{
					currentRoom=currentRoom->neighbors[i].room;
					goto move;
				}
				else if(currentRoom->neighbors[i].isOpenable())
				{
					currentRoom->neighbors[i].t=OPEN;
					for(int j=0;j<currentRoom->neighbors[i].room->neighbors.size();j++)
						if(currentRoom->neighbors[i].room->neighbors[j].room==currentRoom)
							currentRoom->neighbors[i].room->neighbors[j].t=OPEN;
				}
			}

		}
		else
		{
			move:
	#define CAMERA_MARGIN 20
			if(x-Draw::cx<CAMERA_MARGIN)
				Draw::cx-=CAMERA_MARGIN-(x-Draw::cx);
			if(y-Draw::cy<CAMERA_MARGIN)
				Draw::cy-=CAMERA_MARGIN-(y-Draw::cy);
			if(Draw::cx+WSW-x<CAMERA_MARGIN)
				Draw::cx+=CAMERA_MARGIN-(Draw::cx+WSW-x);
			if(Draw::cy+WSH-y<CAMERA_MARGIN)
				Draw::cy+=CAMERA_MARGIN-(Draw::cy+WSH-y);
			x=newx;
			y=newy;
		}
	return ret;
}

void Player::draw()
{

	Draw::p(x,y,c);

	char str[10];
	sprintf_s(str,"%i,%i",x,y);
	Draw::str(str,0+Draw::cx,Draw::cy);

	for(int i=0;i<AH;i++)
		screen[i][WSW]='|';
}
