#include "stdafx.h"
#include "Room.h"
#include "Draw.h"
#include <set>


set<Room*> rooms;
Room::Room(int _x,int _y,int _w,int _h)
{
	x=_x;
	y=_y;
	w=_w;
	h=_h;
	x2=x+w-1;
	y2=y+h-1;
	id=rid++;
	drawnThisFrame=updatedThisFrame=0;
	shouldRemove=0;
	addedDoors=0;
}


Room::~Room(void)
{
}

bool Room::update(int level)
{
	if(level>5)
		return 0;
	updatedThisFrame=1;
	for(int i=0;i<neighbors.size();)
	{
		if(neighbors[i]==NULL && !drawnThisFrame)
			genRoom(this,i);
		if(neighbors[i]==NULL)
		{
			neighbors.erase(neighbors.begin()+i);
			dx.erase(dx.begin()+i);
			dy.erase(dy.begin()+i);
		}
		else
		{
			i++;
		}

	}

	changed();
	for(int i=0;i<neighbors.size();i++)
	{
		if(neighbors[i]!=NULL)
		if(!neighbors[i]->updatedThisFrame)
			neighbors[i]->update(level+1);
	}
}

void Room::draw(int level)
{
	if(!isVisible())
		return;
	drawnThisFrame=1;
	for(int i=x;i<x+w;i++)
		for(int j=y;j<y+h;j++)
		{
			if(i==x || i==x+w-1 || j==y || j==y+h-1)
				Draw::p(i,j,'#');
			else
				Draw::p(i,j,'.');
		}

	for(int i=0;i<dx.size();i++)
		if(neighbors[i]!=NULL)
			Draw::p(dx[i],dy[i],'\\');
	char str[10];
	sprintf_s(str,"%i",id);
	Draw::str(str,x+2,y+2);
	for(int i=0;i<neighbors.size();i++)
	{
		//if(neighbors[i]!=NULL)
		//if(!neighbors[i]->drawnThisFrame)
		//	neighbors[i]->draw(level+1);
	}
}

void Room::connectTo( Room *room,int x,int y )
{
	room->neighbors.push_back(this);
	neighbors.push_back(room);
	room->dx.push_back(x);
	dx.push_back(x);
	room->dy.push_back(y);
	dy.push_back(y);
}

bool Room::isHall()
{
	return w==3 || h==3;
}

bool Room::isValid()
{
	
	if(isVisible())
		return 0;
	for(auto it:rooms)
	{
		if(it==this)
			continue;
		if(x >= it->x2) continue;
		if(y >= it->y2) continue;
		if(x2 <= it->x) continue;
		if(y2 <= it->y) continue;

		return 0;
	}

	return 1;
}

void Room::addDoors()
{
	if(addedDoors++>2)
		return;
	//if(drawnThisFrame)
	//	return;
	if(isHall())
	{
		int x_,y_;
		if(w==3)//vertical
		{
			if(dy[0]==y)//down
			{
				switch (rand()%3)
				{
				case 0:
					x_=x+1;
					y_=y2;
					break;
				case 1:
					x_=x;
					y_=y2-1;
					break;
				case 2:
					x_=x2;
					y_=y2-1;
					break;
				}
			}
			else//up
			{
				switch (rand()%3)
				{
				case 0:
					x_=x+1;
					y_=y;
					break;
				case 1:
					x_=x;
					y_=y+1;
					break;
				case 2:
					x_=x2;
					y_=y+1;
					break;
				}
			}
		}
		else//horizontal
		{
			if(dx[0]==x2)//left
			{
				switch (rand()%3)
				{
				case 0:
					x_=x;
					y_=y+1;
					break;
				case 1:
					x_=x+1;
					y_=y;
					break;
				case 2:
					x_=x+1;
					y_=y2;
					break;
				}
			}
			else//right
			{
				switch (rand()%3)
				{
				case 0:
					x_=x2;
					y_=y+1;
					break;
				case 1:
					x_=x2-1;
					y_=y;
					break;
				case 2:
					x_=x2-1;
					y_=y2;
					break;
				}
			}
		}
		neighbors.push_back(NULL);
		dx.push_back(x_);
		dy.push_back(y_);
	}
	else
	{
		int nDoor=min(rand()%6+1,6-neighbors.size());
		for(int i=0;i<nDoor;i++)
		{
			int x_,y_;
			if(rand()%2==0)//vertical wall door
			{
				y_=rand()%(h-2)+y+1;
				x_=rand()%2==0?x:x2;
			}
			else
			{
				x_=rand()%(w-2)+x+1;
				y_=rand()%2==0?y:y2;
			}
			neighbors.push_back(NULL);
			dx.push_back(x_);
			dy.push_back(y_);
		}
	}
}

void Room::changed()
{
	if(neighbors.size()==1)
	{
		if(isHall())
		{
			//if(isVisible())
			//	return;
			Room *closest=NULL;
			int closestDist=25;
			int vx=0,vy=0,tx,ty;
			if(dx[0]==x)
			{
				vx=1;
				tx=x2;
				ty=y+1;
			}
			else if(dx[0]==x2)
			{
				vx=-1;
				tx=x;
				ty=y+1;
			}
			else if(dy[0]==y)
			{
				vy=1;
				tx=x+1;
				ty=y2;
			}
			else 
			{
				vy=-1;
				tx=x+1;
				ty=y;
			}

			while(abs(tx-x)+abs(ty-y)<25)
			{
				tx+=vx;
				ty+=vy;
				for(auto it:rooms)
				{
					if(it==this) continue;
					if(tx>it->x && tx<it->x2 && ty>it->y && ty<it->y2 && abs(tx-x)+abs(ty-y)<closestDist)
					{
						closest=it;
						closestDist=abs(tx-x)+abs(ty-y);
					}
				}
			}

			bool dup=0;
			if(closest!=NULL)
				for(int i=0;i<neighbors.size() && !dup;i++)
				{
					Room *n=neighbors[i];
					for(int j=0;j<closest->neighbors.size() && !dup;j++)
					{
						for(int k=0;k<closest->neighbors[j]->neighbors.size() && !dup;k++)
						{
							if(closest->neighbors[j]->neighbors[k]==n)
								dup=1;
						}
					}
				}
			if(closest==NULL || dup)
			{
				int j;
				for(j=0;j<neighbors[0]->neighbors.size();j++)
					if(neighbors[0]->neighbors[j]==this)
						break;
				neighbors[0]->neighbors.erase(neighbors[0]->neighbors.begin()+j);
				neighbors[0]->dx.erase(neighbors[0]->dx.begin()+j);
				neighbors[0]->dy.erase(neighbors[0]->dy.begin()+j);
				neighbors[0]->changed();
				shouldRemove=1;
				return;
			}
			else
			{
				if(vx>0)
				{
					x2=closest->x;
					w=x2-x+1;
					connectTo(closest,x2,ty);
				}
				else if(vx<0)
				{
					x=closest->x2;
					w=x2-x+1;
					connectTo(closest,x,ty);
				}
				if(vy>0)
				{
					y2=closest->y;
					h=y2-y+1;
					connectTo(closest,tx,y2);
				}
				else if(vy<0)
				{
					y=closest->y2;
					h=y2-y+1;
					connectTo(closest,tx,y);
				}
			}
		}
		else
		{
			addDoors();
		}
	}
}

bool Room::isVisible()
{
	bool visible=1;
	if(x-Draw::cx>WSW) visible=0;
	if(y-Draw::cy>WSH) visible=0;
	if(x2-Draw::cx<0) visible=0;
	if(y2-Draw::cy<0) visible=0;
	return visible;
}
