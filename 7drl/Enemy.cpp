#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
string i2s( int i )
{
	char str[100];
	sprintf_s(str,"%i",i);
	return str;
}

Enemy::Enemy(int h,int a,int p,int c)
{
	hp=h;
	atk=a;
	pls=p;
	crt=c;
}


Enemy::~Enemy(void)
{
}

bool Enemy::update( int key )
{
	return 0;
}

bool Enemy::collidedWith( Player *player )
{
	int dmg=0,pdmg=0;
	if(player->equipped!=NULL)
	{
		dmg=(player->equipped->atk+rand()%player->equipped->pls)*(rand()%player->equipped->crt==0?2.5:1);
	}
	else
	{
		console.console.push_back("You attack, but your fists are useless!");
	}
	pdmg=(atk+rand()%pls)*(rand()%crt==0?2.5:1);
	hp-=dmg;
	player->hp-=pdmg;
	console.console.push_back("You take "+i2s(pdmg)+" damage and the "+name+" takes "+i2s(dmg));
	if(hp<=0)
	{
		console.console.push_back("You strike the "+name+" down");
		shouldRemove=1;
	}
	if(player->hp<=0)
	{
		console.console.push_back("You die.");
	}
	return 0;
}
