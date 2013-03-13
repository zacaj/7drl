#pragma once
#include "item.h"
class Weapon :
	public Item
{
public:
	int atk,pls,crt;
	Weapon(int _a,int _p,int _crt,string _name="");
	~Weapon(void);

	void use(Player *player);
};

