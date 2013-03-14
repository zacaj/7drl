#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
Enemy::Enemy(int h,int a,int p,int c)
{
	hp=h;
	weapon=new Weapon(a,p,c);
}


Enemy::~Enemy(void)
{
	console.console.push_back("You strike the "+Object::name+" down");
}

bool Enemy::update( int key )
{
	
	return 0;
}

bool Enemy::collidedWith( Object *obj )
{
	if(obj->isPlayer())
	{
		weapon->attack((Player*)obj);
		return 1;
	}
	return 0;
}
