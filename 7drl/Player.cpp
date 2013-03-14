#include "stdafx.h"
#include "Player.h"
#define LEFT 19424
#define RIGHT 19936
#define UP 18656
#define DOWN 20704
#include "Draw.h"
#include "Room.h"
#include "Item.h"
#include "Enemy.h"


Player::Player(int _x,int _y)
{
	x=_x;
	y=_y;
	hp=100;
	c='@';
	inventory.push_back(new Item('n',"note","the note reads: \"Never forget The Terror.\""));
	mode=MAIN;
	desc="";
	equipped=NULL;
	inventory.push_back(new Weapon(10,5,12));
	name="You";
}


Player::~Player(void)
{
}

bool Player::update( int key )
{
	shouldUpdateWorld=1;
	bool ret=0;
	int newx=x,newy=y;
	Item *item;
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
#define DOOR_TEST(_x,_y)	if(currentRoom->neighbors[i].x==_x && currentRoom->neighbors[i].y==_y && currentRoom->neighbors[i].room!=NULL && currentRoom->neighbors[i].isOpen()) {currentRoom->neighbors[i].close(); for(int j=0;j<currentRoom->neighbors[i].room->neighbors.size();j++){if(currentRoom->neighbors[i].room->neighbors[j].room==currentRoom) currentRoom->neighbors[i].room->neighbors[j].close();}ret=1;}
				DOOR_TEST(x+1,y)
					DOOR_TEST(x-1,y)
					DOOR_TEST(x,y+1)
					DOOR_TEST(x,y-1)
#undef DOOR_TEST
			}
			break;
		case 'x':
			mode=EXAMINE;
			shouldUpdateWorld=0;
			ret=1;
			break;
		case 'i':
			mode=INVENTORY;
			shouldUpdateWorld=0;
			inv=0;
			ret=1;
			break;
		}
		break;
	case INVENTORY:
		shouldUpdateWorld=0;
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
			shouldUpdateWorld=1;
			ret=1; 
			break;
		case 'x':
			console.console.push_back(inventory[inv]->name+": "+inventory[inv]->desc);
			ret=1;
			break;
		case 'd':
			item=inventory[inv];
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
		case '\r':
			inventory[inv]->use(this);
			mode=MAIN;
			ret=1;
			break;
		}
		break;
	case EXAMINE:
		shouldUpdateWorld=0;
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
		shouldUpdateWorld=1;
		break;
	}
	
	moveTo(newx,newy,true);
#define CAMERA_MARGIN 15
	if(x-Draw::cx<CAMERA_MARGIN)
		Draw::cx-=CAMERA_MARGIN-(x-Draw::cx);
	if(y-Draw::cy<CAMERA_MARGIN)
		Draw::cy-=CAMERA_MARGIN-(y-Draw::cy);
	if(Draw::cx+WSW-x<CAMERA_MARGIN)
		Draw::cx+=CAMERA_MARGIN-(Draw::cx+WSW-x);
	if(Draw::cy+WSH-y<CAMERA_MARGIN)
		Draw::cy+=CAMERA_MARGIN-(Draw::cy+WSH-y);
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
		STR("v<^>     move");
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
		STR("");
		STR("");
		STR("Debug:");
		STR("p  show rooms");
		STR("WASD   camera");
		STR("Esc      quit");
		sprintf_s(str,"HP: %i",hp);
		Draw::strg(str,WSW+2,AH-2);
		if(equipped!=NULL)
		{
			sprintf_s(str,"%-3i/%-3i/%-4i",equipped->atk,equipped->pls,equipped->crt);
			Draw::strg(equipped->name.c_str(),WSW+2,AH-5);
			Draw::strg("ATK/PLS/CRIT",WSW+2,AH-4);
			Draw::strg(str,WSW+2,AH-3);
		}
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

bool Player::collidedWith( Object *obj )
{
	if(obj->isEnemy())
	{
		if(equipped==NULL)

			console.console.push_back("You attack, but your fists are useless!");
		else
		{
			equipped->attack((Enemy*)obj);
		}
		return 1;
	}
	return 0;
}
