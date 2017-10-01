/** The Wall - objmgr.cpp
* Utilities for The Wall.
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
#include "stdlib.h"
#include "DarkGDK.h"
#include "objmgr.h"

char* strcpy(char* s, char* t)
{
	char* rc;
	for(rc = s; *s++ = *t++;);
	return(rc);
}

static GDKOBJ *objects[MAXOBJ];

void InitObjMgr()
{
	int i;
	
	for(i = 0; i < MAXOBJ; i++)	
	{
		objects[i] = (GDKOBJ *)0;	
	}
	
	//Object 0 is invalid so make it point somewhere
	objects[0] = (GDKOBJ *) malloc(sizeof( GDKOBJ) );
	
	return;
}

static int allocobjnum()
{
	int i;	//Increment i until we find a null in objects[i] or we get to the end.
	for (i = 1; i < MAXOBJ && objects[i]; i++);	//Return -1 if i == MAXOBJ.
	return (i == MAXOBJ ? -1 : i);
}

int DelObject(int objnum)
{
	if ( objects[objnum])
	{
		free (objects[objnum]);
		objects[objnum] = (GDKOBJ *) 0;
		dbDeleteObject(objnum);
	}
	else	
	{
		objnum = -1;	
	}
	
	return objnum;
}

int MakeObject(char*Nname)
{
	GDKOBJ* temp;
	int obj;
	//char buff[256];
	
	temp = (GDKOBJ *) calloc( 1,sizeof(GDKOBJ));
	strcpy(temp->objname, name);	
	obj = allocobjnum();
	
	//sprintf(buff,"obj = %d",obj);
	//dbText(10,10,buff);
	
	temp->objnum = obj;
	objects[obj] = temp;
	
	return(temp->objnum);
}

char* GetObjectName(int objnum)
{
	int i;
	
	for(i = 1; i < MAXOBJ; i++)	
	{
		if(objects[i] && objnum == objects[i]->objnum)		
		{
			return(objects[i]->objname);		
		}	
	}
	
	return NULL;
}

int GetObjectNum(char name[])
{
	int i;
	
	for(int i = 1; i < MAXOBJ &&  name[i]; i++)
	{
		if(objects[i] && !strcmp(name, objects[i]->objname))
		{
			return objects[i]->objnum;
		}
	}
}

int SetiClock(int objnum, float time)
{
	if(objnum && objects[objnum])	
	{
		objects[objnum]->iClock = time;	
	}
	else	
	{
		return -1;	
	}	
	
	return objnum;
}

float GetiClock(int objnum)
{
	if(objnum && objects[objnum])	
	{
		return objects[objnum]->iClock;	
	}
	
	return objnum;
}

int SetPower(int objnum, float power)
{
	if(objnum && objects[objnum])	
	{
		objects[objnum]->power = power;	
	}
	else	
	{
		return -1;	
	
	}	
	return objnum;
}

float GetPower(int objnum)
{
	if(objnum && objects[objnum])	
	{
		return objects[objnum]->power;	
	}
	
	return 0;
}

void dumpobjmgr()
{
	FILE* f;
	int i;
	
	if (f = fopen("objdump.txt","w"))
	{
		for( i = 1; i < MAXOBJ; i++)
		{
			if( objects[i] )
			{
				fprintf(f, 
						"i = %d objnum = %d Name = +%s+ clock=%f %f \n", 
						i, 
						objects[i]->objnum, 
						objects[i]->objname, 
						objects[i]->iClock,
						GetiClock(objects[i]->objnum));	
			}
			else	
			{
				fprintf(f,"objnum %d NULL\n", i);		
			}
		}
	}
	
	fclose(f);
}