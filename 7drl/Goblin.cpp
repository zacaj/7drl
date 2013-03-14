#include "stdafx.h"
#include "Goblin.h"
#include "Player.h"
#include "Room.h"
#include <unordered_set>


Goblin::Goblin(int _x,int _y):Enemy(rand()%10+25,rand()%3+5,rand()%3+1,rand()%4+6)
{
	x=_x;
	y=_y;
	c='g';
	name="goblin";
}


Goblin::~Goblin(void)
{
}
unordered_set<Room*> checked;
bool findPlayer(int &tx,int &ty,Room *room,int x,int y,int depth=0)
{
	if(depth>5 || checked.find(room)!=checked.end())
		return 0;
	if(room==player->currentRoom)
	{
		tx=x;
		ty=y;
		return 1;
	}
	checked.insert(room);
	vector<Door> &n=room->neighbors;
	for(int i=0;i<n.size();i++)
	{	
		if(n[i].isOpen())
			if(findPlayer(tx,ty,n[i].room,n[i].x,n[i].y,depth+1))
			{
				tx=n[i].x;
				ty=n[i].y;
				return 1;
			}
	}
	return 0;
}
bool Goblin::update( int key )
{
	int newx=x,newy=y;
	int tx=rand()%10+x-5,ty=rand()%10-5+y;
	checked.clear();
	findPlayer(tx,ty,currentRoom,player->x,player->y);
start:
	newx=x;
	newy=y;
	if(tx!=x && ty==y)
		goto mx;
	if(ty!=y && tx==x)
		goto my;
	if(ty!=y && tx!=x)
		if(rand()%2==0)
			goto mx;
		else goto my;
	goto end;
mx:
	newx=x-sign(x-tx);
	if(moveTo(newx,newy))
		return 0;
	if((newx!=x || newy!=y) && ty!=y)
	{
		newx=x;
		tx=x;
		goto my;
	}
	goto end;
my:
	newy=y-sign(y-ty);
	if(moveTo(newx,newy))
		return 0;
	if((newx!=x || newy!=y) && tx!=x)
	{
		newy=y;
		ty=y;
		goto mx;
	}
	goto end;
end:
	if(newx!=x)
	{
		ty+=rand()%3-1;
		goto start;
	}
	if(newy!=y)
	{
		tx+=rand()%3-1;
		goto start;
	}
	return 0;
}
