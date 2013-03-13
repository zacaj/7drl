#pragma once
#include "object.h"
class Enemy :
	public Object
{
public:
	int hp,atk,pls,crt;
	Enemy(int h,int a,int p,int c);
	virtual ~Enemy(void);

	virtual bool update( int key );

	virtual bool collidedWith( Player *player );

};

