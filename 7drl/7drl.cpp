// 7drl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <windows.h>
int px=40,py=10;
using namespace std;
#include "Console.h"
#include "Draw.h"
#include <set>
#include "Object.h"
#include "Player.h"
#include "Room.h"
#include "Chest.h"
#include "Item.h"
#include "Goblin.h"
Console console;
COORD coord(int x,int y)
{
	COORD bufferSize = {x, y};
	return bufferSize;
}
Player *player;
int _tmain(int argc, _TCHAR* argv[])
{
	
	bool quit=0;
	Draw::init();
	/*console.console.push_back("1sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg1");
	console.console.push_back("2sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg2");
	console.console.push_back("3sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg3");
	console.console.push_back("4sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg4");
	console.console.push_back("5sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg5");*/
	objects.push_back(player=new Player(30,15));
	objects.push_back(new Goblin(15,14));
	Room *room;
	rooms.push_back(room=new Room(10,5,30,15));
	player->currentRoom=room;
	rooms.push_back(new Room(39,14,10,3));
	room->connectTo(*--rooms.end(),39,15);
	room=*--rooms.end();
	rooms.push_back(new Room(48,10,20,10));
	room->connectTo(*--rooms.end(),48,15);
	room=*--rooms.end();
	room->neighbors.push_back(Door(NULL,room->x2,15,0));
	Chest *chest;
	objects.push_back(chest=new Chest(29,15));
	chest->contents.push_back(new Item('p',"red potion"));
	chest->contents.push_back(new Item('w',"whistle"));
	chest->contents.push_back(new Item('s',"rusty sword"));

	for(auto it:objects)
	{
		if(it->currentRoom==NULL || !(it->x>=it->currentRoom->x && it->x<it->currentRoom->x+it->currentRoom->w && it->y>=it->currentRoom->y && it->y<it->currentRoom->y+it->currentRoom->h))
		{
			for(auto it2:rooms)
				if(it->x>=it2->x && it->x<it2->x+it2->w && it->y>=it2->y && it->y<it2->y+it2->h)
				{
					it->currentRoom=it2;
					break;
				}
		}
	}
	while(!quit)
	{
		for(int i=0;i<W;i++)
			for(int j=0;j<H;j++)
				screen[j][i]=' ';
		size_t s=rooms.size();
		Room **it=rooms.data();
		for(size_t i=0;i<s;)
		{ 
			Room *room=*it;
			room->updatedThisFrame=0;
			room->drawnThisFrame=0;
			if(room->shouldRemove)
			{
				rooms.erase(rooms.begin()+i);
				s--;
				delete room;
			}
			else
			{
				i++;
				++it;
			}
		}
		for(int i=0;i<10;i++)
			player->currentRoom->update();

		//for(auto it=rooms.begin();it!=rooms.end();it++)
		//	(*it)->draw();
		player->currentRoom->draw();
		for(auto it:objects)
			it->draw();

		console.draw();
		char str[100];
		sprintf_s(str,"Rooms: %i",rooms.size());
		Draw::str(str,Draw::cx,Draw::cy+1);

		LPDWORD t=new DWORD;
		WriteConsoleOutputCharacterA(Draw::wHnd,(char*)screen,W*H,coord(0,0),t);
		delete t;
		SetConsoleCursorPosition(Draw::wHnd,coord(0,HMO));

		int c=_getch();
		if(c==0 || c==0xE0)
		{
			c|=_getch()<<8;
		}
		switch(c)
		{
		case 27:
			quit=1;
			break;
		case 'w':
			Draw::cy--;
			break;
		case 's':
			Draw::cy++;
			break;
		case 'a':
			Draw::cx--;
			break;
		case 'd':
			Draw::cx++;
			break;
		case 'p':
			drawAll=!drawAll;
			break;
		}
		for(int i=0;i<objects.size();i++)
		{
			Object *it=objects[i];
			if(it->currentRoom==NULL || !(it->x>=it->currentRoom->x && it->x<it->currentRoom->x+it->currentRoom->w && it->y>=it->currentRoom->y && it->y<it->currentRoom->y+it->currentRoom->h))
			{
				for(auto it2:rooms)
					if(it->x>=it2->x && it->x<it2->x+it2->w && it->y>=it2->y && it->y<it2->y+it2->h)
					{
						it->currentRoom=it2;
						break;
					}
			}
			if(it->currentRoom==NULL || it->shouldRemove)
			{
				delete it;
				objects.erase(objects.begin()+i);
			}
			else 
			{
				if(it->update(c))
					c=0;
				it++;
			}
		}
	}
	return 0;
}

