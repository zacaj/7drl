#pragma once
#include <string>
#include "Object.h"
using namespace std;
class Item:public Object
{
public:
	Item(char _c,string _name,string _desc="");
	virtual ~Item(void);

	virtual bool update( int key );


	virtual bool collidedWith( Player *player );

};

