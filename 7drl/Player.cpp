#include "stdafx.h"
#include "Player.h"
#define LEFT 19424
#define RIGHT 19936
#define UP 18656
#define DOWN 20704
#include "Draw.h"
#include "Room.h"
#include "Item.h"


Player::Player(int _x,int _y)
{
	x=_x;
	y=_y;
	c='@';
	inventory.push_back(new Item('n',"note","the note reads: \"Never forget The Terror.\""));
	mode=MAIN;
	desc="";
}


Player::~Player(void)
{
}

bool Player::update( int key )
{
	bool ret=0;
	int newx=x,newy=y;
	switch(mode)
	{
	case MAIN:
		switch(key)
		{
		case LEFT:
			newx--;
			ret=1;
			break;
		case RIGHT:
			newx++;
			ret=1;
			break;
		case UP:
			newy--;
			ret=1;
			break;
		case DOWN:
			newy++;
			ret=1;
			break;
		case 'c':
			int i;
			for(i=0;i<currentRoom->neighbors.size();i++)
			{
#define DOOR_TEST(_x,_y)	if(currentRoom->neighbors[i].x==_x && currentRoom->neighbors[i].y==_y && currentRoom->neighbors[i].room!=NULL && currentRoom->neighbors[i].isOpen()) {currentRoom->neighbors[i].close(); ret=1;}
				DOOR_TEST(x+1,y)
					DOOR_TEST(x-1,y)
					DOOR_TEST(x,y+1)
					DOOR_TEST(x,y-1)
#undef DOOR_TEST
			}
			break;
		case 'x':
			mode=EXAMINE;
			ret=1;
			break;
		case 'i':
			mode=INVENTORY;
			inv=0;
			ret=1;
			break;
		}
		break;
	case INVENTORY:
		switch(key)
		{
		case UP:
			inv--;
			if(inv<0) inv=inventory.size()-1;
			ret=1;
			break;
		case DOWN:
			inv++;
			if(inv>=inventory.size()) inv=0;
			ret=1;
			break;
		case 8:
			mode=MAIN;
			ret=1; 
			break;
		case 'x':
			console.console.push_back(inventory[inv]->name+": "+inventory[inv]->desc);
			ret=1;
			break;
		case 'd':
			Item *item=inventory[inv];
			item->x=x;
			item->y=y;
			item->currentRoom=currentRoom;
			inventory.erase(inventory.begin()+inv);
			if(inv==inventory.size())
				inv--;
			if(x!=currentRoom->x2-1)
				item->x=x+1;
			else if(x!=currentRoom->x+1)
				item->x=x-1;
			else if(y!=currentRoom->y2-1)
				item->y=y+1;
			else if(y!=currentRoom->y+1)
				item->y=y-1;
			objects.push_back(item);
			ret=1;
			break;
		}
		break;
	case EXAMINE:
		if(key!=8)
		{
			bool found=0;
			for(auto it:objects)
			{
				if((it->c)==key && it->drawnThisFrame)
					if(it->name.size())
					{
						console.console.push_back((string()+it->c)+": a "+it->name);
						found=1;
					}
			}
			if(!found)
				console.console.push_back("You don't see a \'"+(string()+(char)key)+"\'");
		}
		mode=MAIN;
		break;
	}
	
	if(newx!=x || newy!=y)
		if(newx==currentRoom->x || newx==currentRoom->x+currentRoom->w-1 || newy==currentRoom->y || newy==currentRoom->y+currentRoom->h-1)
		{
			int i;
			for(i=0;i<currentRoom->neighbors.size();i++)
			{
				if(currentRoom->neighbors[i].x==newx && currentRoom->neighbors[i].y==newy && currentRoom->neighbors[i].room!=NULL)
					break;
			}
			if(i==currentRoom->neighbors.size())
			{

			}
			else
			{
				if(currentRoom->neighbors[i].isOpen())
				{
					currentRoom=currentRoom->neighbors[i].room;
					goto move;
				}
				else if(currentRoom->neighbors[i].isOpenable())
				{
					currentRoom->neighbors[i].t=OPEN;
					for(int j=0;j<currentRoom->neighbors[i].room->neighbors.size();j++)
						if(currentRoom->neighbors[i].room->neighbors[j].room==currentRoom)
							currentRoom->neighbors[i].room->neighbors[j].t=OPEN;
				}
			}

		}
		else
		{
move:
			Object *obj;
			while((obj=objectAt(newx,newy)))
			{
				obj->collidedWith(this);
				if(!obj->shouldRemove)
					goto end;
			}
	#define CAMERA_MARGIN 15
			if(x-Draw::cx<CAMERA_MARGIN)
				Draw::cx-=CAMERA_MARGIN-(x-Draw::cx);
			if(y-Draw::cy<CAMERA_MARGIN)
				Draw::cy-=CAMERA_MARGIN-(y-Draw::cy);
			if(Draw::cx+WSW-x<CAMERA_MARGIN)
				Draw::cx+=CAMERA_MARGIN-(Draw::cx+WSW-x);
			if(Draw::cy+WSH-y<CAMERA_MARGIN)
				Draw::cy+=CAMERA_MARGIN-(Draw::cy+WSH-y);
			x=newx;
			y=newy;
			end:;
		}
	return ret;
}

void Player::draw()
{

	Draw::p(x,y,c);
	drawnThisFrame=1;

	char str[100];
	sprintf_s(str,"%i,%i",x,y);
	Draw::str(str,0+Draw::cx,Draw::cy);

	for(int i=0;i<AH;i++)
		screen[i][WSW]='|';
	int n=1;
	int i;
	switch(mode)
	{
#define STR(s) Draw::strg(s,WSW+2,n++)
	case MAIN:
		STR("<^>v     move");
		STR("eXamine      ");
		STR("Inventory    ");
		for(i=0;i<currentRoom->neighbors.size();i++)
		{
#define DOOR_TEST(_x,_y)	if(currentRoom->neighbors[i].x==_x && currentRoom->neighbors[i].y==_y && currentRoom->neighbors[i].room!=NULL && currentRoom->neighbors[i].isOpen()) break;
			DOOR_TEST(x+1,y)
				DOOR_TEST(x-1,y)
				DOOR_TEST(x,y+1)
				DOOR_TEST(x,y-1)
		}
		if(i!=currentRoom->neighbors.size())
			STR("Close door");
		STR("p  show rooms");
		STR("WASD   camera");
		STR("Esc      quit");
		break;
	case EXAMINE:
		STR("Press the key");
		STR("of what you  ");
		STR("would like to");
		STR("examine");
		STR("");
		STR("");
		STR("BACKspace");
		break;
	case INVENTORY:
		STR("Enter:     Use");
		STR("eXamine   Drop");
		STR("--------------");
		for(int i=0;i<inventory.size();i++)
			STR(string((i!=inv?"":"> ")+inventory[i]->name).c_str());
		STR("");
		STR("BACKspace");
		break;
	}
}
