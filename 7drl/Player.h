#pragma once
#include "object.h"
class Player :
	public Object
{
public:
	Player(int _x,int _y);
	~Player(void);

	virtual bool update( int key );

	virtual void draw();

};

