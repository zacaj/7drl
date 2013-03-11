#pragma once
#include <vector>
#include <set>
using namespace std;
class Room
{
public:
	int id;
	int x,y,w,h,x2,y2;
	bool shouldRemove;
	vector<Room*> neighbors;
	bool updatedThisFrame;
	bool drawnThisFrame;
	int addedDoors;
	vector<int> dx;
	vector<int> dy;
	Room(int _x,int _y,int _w,int _h);
	virtual ~Room(void);
	virtual bool update(int level=0);
	virtual void draw(int level=0);
	void connectTo(Room *room,int x,int y);
	bool isHall();
	bool isValid();
	void addDoors();
	void changed();
	bool isVisible();
};
static int rid=0;

extern set<Room*> rooms;