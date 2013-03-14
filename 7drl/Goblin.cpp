#include "stdafx.h"
#include "Goblin.h"
#include "Player.h"


Goblin::Goblin(int _x,int _y):Enemy(rand()%10+25,rand()%3+5,rand()%3+1,rand()%4+6)
{
	x=_x;
	y=_y;
	c='g';
	name="goblin";
}


Goblin::~Goblin(void)
{
}

bool Goblin::update( int key )
{
	int newx=x,newy=y;
	if(player->currentRoom==currentRoom)
	{
		if(abs(x-player->x)>abs(y-player->y) || (abs(x-player->x)==abs(y-player->y) && rand()%2))
			newx-=sign(x-player->x);
		else
			newy-=sign(y-player->y);
	}
	moveTo(newx,newy);
	return 0;
}
