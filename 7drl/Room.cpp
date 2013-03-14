#include "stdafx.h"
#include "Room.h"
#include "Draw.h"
#include <set>
#include "Player.h"
bool drawAll=0;

vector<Room*> rooms;
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
	doorAdjustment=0;
	addedDoors=0;
	calmed=0;
}


Room::~Room(void)
{
}

bool Room::update(int level)
{
	if(level<15)
	{
		updatedThisFrame=1;
		for(int i=0;i<neighbors.size();)
		{
			if(neighbors[i].room==NULL && !drawnThisFrame)
				genRoom(this,i);
			if(neighbors[i].room==NULL || neighbors[i].room->shouldRemove)
			{
				neighbors.erase(neighbors.begin()+i);
			}
			else
			{
				i++;
			}

		}
		changed();
		for(int i=0;i<neighbors.size();i++)
		{
			if(neighbors[i].room!=NULL)
				if(!neighbors[i].room->updatedThisFrame)
					neighbors[i].room->update(level+1);
		}
	}
	//if(level<25)
	//	changed();
	return 0;
}

void Room::draw(int level)
{
	drawnThisFrame=1;
	if(isVisible())
	{

		for(int i=x;i<x+w;i++)
			for(int j=y;j<y+h;j++)
			{
				if(i==x || i==x+w-1 || j==y || j==y+h-1)
					Draw::p(i,j,'#');
				else
					Draw::p(i,j,'.');
			}

			for(int i=0;i<neighbors.size();i++)
				if(neighbors[i].room!=NULL)
				{
					char c='?';
					switch(neighbors[i].t)
					{
					case 0:
						c='\\';
						break;
					case 1:
						c='.';
						break;
					case CLOSED_H:
						c='-';
						break;
					case CLOSED_V:
						c='|';
						break;
					default:
						c='?';
						break;
					}
					Draw::p(neighbors[i].x,neighbors[i].y,c);
				}
				char str[10];
				sprintf_s(str,"%i",id);
				//Draw::str(str,x+2,y+2);
	}
	int drawn=0;
	for(int i=0;i<neighbors.size();i++)
	{
		if(neighbors[i].room!=NULL)
		if(!neighbors[i].room->drawnThisFrame && (neighbors[i].isOpen() || drawAll))
		{
			neighbors[i].room->draw(level+1);
			drawn++;
		}
	}
	if(drawn==0 && level==0 && !calmed)
	{
		player->hp+=15;
		calmed=15;
		console.console.push_back("The dungeon doesn't seem so overpowering now.  You feel a bit better.");
	}
}

void Room::connectTo( Room *room,int x,int y,int t )
{
	if(room->shouldRemove || shouldRemove)
		return;
	for(int i=0;i<neighbors.size();i++)
		if(neighbors[i].x==x && neighbors[i].y==y)
			return;
	for(int i=0;i<room->neighbors.size();i++)
		if(room->neighbors[i].x==x && room->neighbors[i].y==y)
			return;
	room->neighbors.push_back(Door(this,x,y,t));
	neighbors.push_back(Door(room,x,y,t));
	switch(t)
	{
	case 0:
		break;
	case 1:
		doorAdjustment++;
		room->doorAdjustment++;
		break;
	}
}

bool Room::isHall()
{
	return w==3 || h==3;
}

bool Room::isValid()
{
	
	if(isVisible())
		return 0;
	size_t s=rooms.size();
	Room **it=rooms.data();
	for(size_t i=0;i<s;i++,it++)
	{
		Room *r=*it;
		if(r==this)
			continue;
		if(x >= r->x2) continue;
		if(y >= r->y2) continue;
		if(x2 <= r->x) continue;
		if(y2 <= r->y) continue;

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
			if(neighbors[0].y==y)//down
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
			if(neighbors[0].x==x2)//left
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
		neighbors.push_back(Door(NULL,x_,y_,0));
	}
	else
	{
		int nDoor=min(rand()%16+1+doorAdjustment,16-(neighbors.size()-doorAdjustment));
		for(int i=0;i<nDoor;i++)
		{
			int x_,y_;
			if(rand()%3!=0)//vertical wall door
			{
				y_=rand()%(h-2)+y+1;
				x_=rand()%2==0?x:x2;
			}
			else
			{
				x_=rand()%(w-2)+x+1;
				y_=rand()%2==0?y:y2;
			}
			neighbors.push_back(Door(NULL,x_,y_,0));
		}
	}
}

void Room::changed()
{
	if(neighbors.size()-doorAdjustment==1)
	{
		if(isHall())
		{
			//if(isVisible())
			//	return;
			Room *closest=NULL;
			int closestDist=15;
			int vx=0,vy=0,tx,ty;
			if(neighbors[0].x==x)
			{
				vx=1;
				tx=x2;
				ty=y+1;
			}
			else if(neighbors[0].x==x2)
			{
				vx=-1;
				tx=x;
				ty=y+1;
			}
			else if(neighbors[0].y==y)
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

			while(abs(tx-x)+abs(ty-y)<15)
			{
				tx+=vx;
				ty+=vy;
				size_t s=rooms.size();
				Room **rs=rooms.data();
				for(size_t i=0;i<s;i++,rs++)
				{
					Room *it=*rs;
					if(it==this || it->shouldRemove==1) continue;
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
					Room *n=neighbors[i].room;
					if(n!=NULL)
						for(int j=0;j<closest->neighbors.size() && !dup;j++)
						{
							if(closest->neighbors[j].room!=NULL)
								for(int k=0;k<closest->neighbors[j].room->neighbors.size() && !dup;k++)
								{
									if(closest->neighbors[j].room->neighbors[k].room==n)
										dup=1;
								}
						}
				}
			if(closest==NULL || dup)
			{
				int j;
				for(int i=0;i<neighbors.size();i++)
				{
 					if(i>=neighbors.size())
						break;
					if(neighbors[i].room!=NULL)
						for(j=0;i<neighbors.size() && j<neighbors[i].room->neighbors.size();j++)
							if(neighbors[i].room->neighbors[j].room==this)
							{
								if(neighbors[i].room->neighbors[j].t==1)
									neighbors[i].room->doorAdjustment--;
								neighbors[i].room->neighbors.erase(neighbors[i].room->neighbors.begin()+j);
								neighbors[i].room->changed();
							}
				}
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
