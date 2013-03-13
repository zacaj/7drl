#include "stdafx.h"
#include "Object.h"
#include "Room.h"
#include "Draw.h"

vector<Object*> objects;

Object::Object(void)
{
	currentRoom=NULL;
	ethereal=0;
	shouldRemove=0;
	drawnThisFrame=0;
}


Object::~Object(void)
{
}

void Object::draw()
{
	if(currentRoom->drawnThisFrame)
	{
		Draw::p(x,y,c);
		drawnThisFrame=1;
	}
	else
	{
		drawnThisFrame=0;
	}
}

Object* objectAt( int x,int y )
{
	for(auto it:objects)
	{
		if(it->x==x && it->y==y)
			return it;
	}
	return 0;
}
