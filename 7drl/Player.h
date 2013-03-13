#pragma once
#include "Object.h"
enum Mode{MAIN,INVENTORY,EXAMINE};
class Item;
class Player :
	public Object
{
public:
	vector<Item*> inventory;
	Mode mode;
	int inv;
	Player(int _x,int _y);
	~Player(void);

	virtual bool update( int key );

	virtual void draw();

};

