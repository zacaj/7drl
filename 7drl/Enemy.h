#pragma once
#include "object.h"
#include "Weapon.h"
class Enemy :
	public Object
{
public:
	Weapon *weapon;
	int hp;
	Enemy(int h,int a,int p,int c);
	virtual ~Enemy(void);

	virtual bool update( int key );

	virtual bool collidedWith( Object *obj );
	bool isEnemy(){return 1;}

};

