#pragma once
#include "object.h"
#include "Item.h"
class Chest :
	public Object
{
public:
	vector<Item*> contents;
	Chest(int x,int y);
	~Chest(void);

	virtual bool update( int key );

	virtual bool collidedWith( Object *obj);

};

