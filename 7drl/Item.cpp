#include "stdafx.h"
#include "Item.h"
#include "Player.h"


Item::Item(char _c,string _name,string _desc)
{
	name=_name;
	desc=_desc;
	c=_c;
}


Item::~Item(void)
{
}

bool Item::update( int key )
{
	return 0;
}
bool Item::collidedWith( Player *player )
{
	player->inventory.push_back(new Item(*this));
	console.console.push_back("You pick up a "+name);
	shouldRemove=1;
	return 0;
}
