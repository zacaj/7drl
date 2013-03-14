#pragma once
#include <string>
#include "Object.h"
class Player;
using namespace std;
class Item:public Object
{
public:
	Item(){}
	Item(char _c,string _name,string _desc="");
	virtual ~Item(void);

	virtual bool update( int key );


	virtual bool collidedWith( Object *obj );
	virtual void use(Player *player){};
};

