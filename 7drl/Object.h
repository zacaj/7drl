#pragma once
#include <set>
class Player;
#include "Console.h"
class Room;
using namespace std;
class Object
{
public:
	int x,y;
	char c;
	bool ethereal;
	bool shouldRemove;
	bool drawnThisFrame;
	Room *currentRoom;
	string desc;
	string name;
	Object(void);
	virtual ~Object(void);
	virtual bool update(int key)=0;
	virtual void draw();
	virtual bool isEnemy(){return 0;};
	virtual bool collidedWith(Player *player){return 0;}

};

Object* objectAt(int x,int y);
extern vector<Object*> objects;