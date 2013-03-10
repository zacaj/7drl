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
	if(newx==currentRoom->x || newx==currentRoom->x+currentRoom->w-1 || newy==currentRoom->y || newy==currentRoom->y+currentRoom->h-1)
	{
		int i;
		for(i=0;i<currentRoom->dx.size();i++)
		{
			if(currentRoom->dx[i]==newx && currentRoom->dy[i]==newy)
				break;
		}
		if(i==currentRoom->dx.size())
		{

		}
		else
		{
			x=newx;
			y=newy;
			currentRoom=currentRoom->neighbors[i];
		}

	}
	else
	{
		x=newx;
		y=newy;
	}

	if(x-Draw::cx<10)
		Draw::cx-=10-(x-Draw::cx);
	if(y-Draw::cy<10)
		Draw::cy-=10-(y-Draw::cy);
	if(Draw::cx+WSW-x<10)
		Draw::cx+=10-(Draw::cx+WSW-x);
	if(Draw::cy+WSH-y<10)
		Draw::cy+=10-(Draw::cy+WSH-y);
	return ret;
}

void Player::draw()
{

	Draw::p(x,y,c);


	for(int i=0;i<AH;i++)
		screen[i][WSW]='|';
}
