#pragma once
#include "item.h"
string i2s( int i );

class Weapon :
	public Item
{
public:
	int atk,pls,crt;
	Weapon(int _a,int _p,int _crt,string _name="");
	~Weapon(void);

	void use(Player *player);
	template<class T>
	void attack(T* o)
	{
		int dmg=(atk+rand()%pls)*(rand()%crt==0?2.5:1);
		o->hp-=dmg;
		console.console.push_back((isupper(o->name[0])?"":"The ")+o->name+" take "+i2s(dmg)+" damage");
		if(o->hp<=0)
			o->shouldRemove=1;
	}
};

