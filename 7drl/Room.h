#pragma once
#include <vector>
using namespace std;
class Room
{
public:
	int x,y,w,h;
	vector<Room*> neighbors;
	vector<int> dx;
	vector<int> dy;
	Room(int _x,int _y,int _w,int _h);
	virtual ~Room(void);
	virtual void update();
};

