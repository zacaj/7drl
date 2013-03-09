#pragma once
class Object
{
public:
	int x,y;
	char c;
	Object(void);
	virtual ~Object(void);
	virtual bool update(int key)=0;
};

