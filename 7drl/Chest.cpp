#include "stdafx.h"
#include "Chest.h"
#include "Draw.h"
#include "Room.h"
#include "Player.h"

Chest::Chest(int _x,int _y)
{
	x=_x;
	y=_y;
	c='C';
	name="A chest.";
	desc="";
}


Chest::~Chest(void)
{
}

bool Chest::update( int key )
{
	return 0;
}

bool Chest::collidedWith( Player *player )
{
	if(c=='C')
	{
		string str;
		if(contents.empty())
			str="You open the chest, but it's empty.";
		else
		{
			str="You retrieve ";
			for(int i=0;i<contents.size();i++)
			{
				str+="a "+contents[i]->name;
				if(i!=contents.size()-1)
					str+=", ";
				if(i==contents.size()-2)
					str+="and ";
			}
		}
		console.console.push_back(str);
		while(!contents.empty())
		{
			player->inventory.push_back(contents.back());
			contents.pop_back();
		}
		c='c';
		name="A chest, already plundered.";
	}
	return 0;
}
