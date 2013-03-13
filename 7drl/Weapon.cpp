#include "stdafx.h"
#include "Weapon.h"
#include "Player.h"


Weapon::Weapon(int _a,int _p,int _crt,string _name)
{
	atk=_a;
	pls=_p;
	crt=_crt;
	const char *ss[]={"sword","spear","axe"};
	const char *sa[]={"rusty","","sharp","wooden","bloody"};
	const char *sc="s|P";
	int n=rand()%3;
	int a=rand()%5;
	c=sc[n];
	name=string(sa[a])+" "+ss[n];
}


Weapon::~Weapon(void)
{
}

void Weapon::use(Player *player)
{
	if(player->equipped==this)
		player->equipped=NULL;
	else
		player->equipped=this;
}

