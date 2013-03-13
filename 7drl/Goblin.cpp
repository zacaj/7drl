#include "stdafx.h"
#include "Goblin.h"


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
	return 0;
}
