/** The Wall - TheWall.cpp
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* R.I.P Dr.Richard Glass
* 1950-2010
*
* Eddie O'Hagan
* Copyright © 2009 Katianie.com
*/
#include "DarkGDK.h"
#include "TheWall.h"
#include "objmgr.h"

int tempImages[40];
int bricks[20][20];

void LoadImages()
{
	char buf[32];
	char buf2[32];
	int i = 40;
	int k;
	
	for(k = 0; k < 40; k++)
	{
		tempImages[k] = 0;
	}
	
	while(i)
	{
		sprintf(buf,"%d.jpg",i);
		tempImages[i] = MakeObject(buf);		
		dbLoadImage(buf, tempImages[i]);
		i--;
	}
	
	return;
}

/* builds a wall starting at the left (x), base (Y) and depth zposition (Z)
if offset if TRUE, the bricks are offset in the z position */
void Wall(float left, float base, float zposition, int cols, int rows, int offset)
{
	int  r, c, brickcnt = 1;
	int imagecnt = 0;
	float pos;
	float shift;
	int i = 0;
	for(r = 0; r < rows; r++)
	{
		pos = left + (r % 2 ? L/2.0 : 0);
		for( c = 0; c < cols; c++)
		{
			bricks[r][c] = MakeObject("brick");
			dbMakeObjectBox(bricks[r][c], L, H, D);
			dbTextureObject(bricks[r][c],tempImages[++i]);
			i %= 40;
			shift = (bricks[r][c] %2 ? -1 : 1)*D/4;
			dbPositionObject(bricks[r][c], pos, base  , zposition + (offset ? shift : 0));
			pos += L;
			dbSync();
		}
		base += H; 
	}
}

void deleteWall()
{
	for(int i = 0; i < 20; i++)
	{
		for(int j = 0; j < 20; j++)
		{
			DelObject(bricks[i][j]);
		}
	}
}