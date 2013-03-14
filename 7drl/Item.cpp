#include "stdafx.h"
#include "Item.h"
#include "Player.h"


Item::Item(char _c,string _name,string _desc)
{
	name=_name;
	desc=_desc;
	c=_c;
	ethereal=1;
}


Item::~Item(void)
{
}

bool Item::update( int key )
{
	return 0;
}
bool Item::collidedWith( Object *obj )
{
	if(obj->isPlayer())
	{
		Player *player=(Player*)obj;
		player->inventory.push_back(new Item(*this));
		console.console.push_back("You pick up a "+name);
		shouldRemove=1;
		return 1;
	}
	return 0;
}
