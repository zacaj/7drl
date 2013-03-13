#pragma once
#include "enemy.h"
class Goblin :
	public Enemy
{
public:
	Goblin(int _x,int _y);
	~Goblin(void);

	virtual bool update( int key );

};

