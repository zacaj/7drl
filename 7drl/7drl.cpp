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
set<Object*> objects;
Console console;
COORD coord(int x,int y)
{
	COORD bufferSize = {x, y};
	return bufferSize;
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	bool quit=0;
	Draw::init();
	console.console.push_back("1sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg1");
	console.console.push_back("2sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg2");
	console.console.push_back("3sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg3");
	console.console.push_back("4sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg4");
	console.console.push_back("5sfdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfgdfgdgdg5");
	objects.insert(new Player(40,15));

	while(!quit)
	{
		
		console.draw();
		WriteConsoleOutputCharacterA(Draw::wHnd,(char*)screen,W*H,coord(0,0),NULL);
		SetConsoleCursorPosition(Draw::wHnd,coord(0,HMO));
		for(int i=0;i<W;i++)
			for(int j=0;j<H;j++)
				screen[j][i]=' ';
		for(int i=20;i<60;i++)
			for(int j=5;j<20;j++)
			{
				if(i==20 || i==59 || j==5 || j==19)
					screen[j][i]='#';
				else
					screen[j][i]='.';
			}
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
		}
		for(auto it:objects)
		{
			if(it->update(c))
				c=0;
		}
	}
	return 0;
}

