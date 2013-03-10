#pragma once
#include <vector>
#include <set>
using namespace std;
class Room
{
public:
	int x,y,w,h,x2,y2;
	vector<Room*> neighbors;
	vector<int> dx;
	vector<int> dy;
	Room(int _x,int _y,int _w,int _h);
	virtual ~Room(void);
	virtual void update();
	void connectTo(Room *room,int x,int y);
	bool isHall();
	bool isValid();
};


extern set<Room*> rooms;