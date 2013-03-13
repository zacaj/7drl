#pragma once
#include <vector>
#include "string"
using namespace std;

class Console
{
public:
	vector<string> console;
	void draw();
};

extern Console console;