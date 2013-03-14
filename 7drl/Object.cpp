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

void Object::moveTo( int newx,int newy,bool canOpenDoors/*=0*/ )
{
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
				else if(currentRoom->neighbors[i].isOpenable() && canOpenDoors)
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
			vector<Object *> objs=objectAt(newx,newy);
			bool clear=1;
			for(int i=0;i<objs.size();i++)
			{
				if(!collidedWith(objs[i]))	
					objs[i]->collidedWith(this);
					if(!objs[i]->shouldRemove && !objs[i]->ethereal)
						clear=0;
			}
			if(!clear)
				goto end;
			x=newx;
			y=newy;
end:;
		}
}

vector<Object*> objectAt( int x,int y )
{
	vector<Object *> ret;
	for(auto it:objects)
	{
		if(it->x==x && it->y==y)
			ret.push_back(it);
	}
	return ret;
}
