#pragma once
#include <vector>
#include <set>
using namespace std;
class Room;
#define OPEN 0
#define CLOSED_H 2
#define CLOSED_V 3
#define CLOSED 4
#define INVISIBLE 1
struct Door;
class Room
{
public:
	bool calmed;
	int id;
	int x,y,w,h,x2,y2;
	bool shouldRemove;
	bool updatedThisFrame;
	bool drawnThisFrame;
	int addedDoors;
	int doorAdjustment;
	vector<Door> neighbors;
	Room(int _x,int _y,int _w,int _h);
	virtual ~Room(void);
	virtual bool update(int level=0);
	virtual void draw(int level=0);
	void connectTo(Room *room,int x,int y,int t=0);
	bool isHall();
	bool isValid();
	void addDoors();
	void changed();
	bool isVisible();
};
extern bool drawAll;
struct Door
{
	Room *room;
	int x,y;
	int t;
	Door(Room *_room,int _x,int _y,int _t)
	{
		room=_room;
		x=_x;
		y=_y;
		t=_t;
		if(t==CLOSED)
		{
			close();
		}
	}
	bool isOpen()
	{
		return t==OPEN || t==INVISIBLE;
	}
	bool isOpenable()
	{
		return t==CLOSED_V || t==CLOSED_H;
	}
	void close()
	{
		if(x==room->x || x==room->x2)
			t=CLOSED_V;
		else
			t=CLOSED_H;

	}
};
static int rid=0;

extern vector<Room*> rooms;