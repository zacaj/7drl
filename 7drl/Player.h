#pragma once
#include "Object.h"
#include "Weapon.h"

#define sign(x) (x>0?1:-1)
enum Mode{MAIN,INVENTORY,EXAMINE};
class Item;
class Player :
	public Object
{
public:
	int hp;
	bool shouldUpdateWorld;
	Weapon *equipped;
	vector<Item*> inventory;
	Mode mode;
	int inv;
	Player(int _x,int _y);
	~Player(void);

	virtual bool update( int key );

	virtual void draw();
	bool isPlayer(){return 1;}

	virtual bool collidedWith( Object *obj );

};

extern Player *player;