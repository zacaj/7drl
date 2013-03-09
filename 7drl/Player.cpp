#include "stdafx.h"
#include "Player.h"
#define LEFT 19424
#define RIGHT 19936
#define UP 18656
#define DOWN 20704
#include "Draw.h"


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
	switch(key)
	{
	case LEFT:
		x--;
		ret=1;
		break;
	case RIGHT:
		x++;
		ret=1;
		break;
	case UP:
		y--;
		ret=1;
		break;
	case DOWN:
		y++;
		ret=1;
		break;
	}
	Draw::p(x,y,c);


	for(int i=0;i<AH;i++)
		screen[i][WSW]='|';
	return ret;
}
