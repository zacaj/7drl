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
set<Object*> objects;
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
	console.console.push_back("1sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg1");
	console.console.push_back("2sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg2");
	console.console.push_back("3sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg3");
	console.console.push_back("4sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg4");
	console.console.push_back("5sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg5");
	objects.insert(player=new Player(30,15));
	Room *room;
	rooms.insert(room=new Room(10,5,30,15));
	player->currentRoom=room;
	rooms.insert(new Room(39,14,10,3));
	room->connectTo(*--rooms.end(),39,15);
	room=*--rooms.end();
	rooms.insert(new Room(48,10,20,10));
	room->connectTo(*--rooms.end(),48,15);
	room=*--rooms.end();
	room->neighbors.push_back(NULL);
	room->dx.push_back(room->x2);
	room->dy.push_back(15);

	while(!quit)
	{
		for(int i=0;i<W;i++)
			for(int j=0;j<H;j++)
				screen[j][i]=' ';
		for(auto it=rooms.begin();it!=rooms.end();)
		{ 
			Room *room=*it;
			room->updatedThisFrame=0;
			room->drawnThisFrame=0;
			if(room->shouldRemove)
			{
				auto it2=it;
				++it;
				rooms.erase(it2);
				delete room;
			}
			else
			{
				++it;
			}
		}
		for(int i=0;i<1;i++)
			player->currentRoom->update();

		for(auto it=rooms.begin();it!=rooms.end();it++)
			(*it)->draw();
		//player->currentRoom->draw();
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
		}
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
			if(it->currentRoom==NULL)
				objects.erase(it);
			else
			if(it->update(c))
				c=0;
		}
	}
	return 0;
}

