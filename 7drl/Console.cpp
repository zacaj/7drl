#include "stdafx.h"
#include "Console.h"
#include "Draw.h"


void Console::draw()
{
	for(int i=0;i<CW;i++)
		screen[H-CH][i]='-';
	int y=HMO;
	for(int i=console.size()-1;i>=0 && y>H-CH;i--)
	{
		int l=console[i].size();
		int lines=l/CW+1;
		Draw::setCursor(0,y);
		for(int j=lines-1;j>=0;j--)
		{
			int c=j*CW;
			for(int x=0;x<CW && c<l;x++,c++)
			{
				screen[y][x]=console[i][c];
			}
			y--;
			if(y<=H-CH)
				break;
		}
	}
}
