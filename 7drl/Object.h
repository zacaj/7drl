#pragma once
class Room;
class Object
{
public:
	int x,y;
	char c;
	Room *currentRoom;
	Object(void);
	virtual ~Object(void);
	virtual bool update(int key)=0;
	virtual void draw()=0;
};

