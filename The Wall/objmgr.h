/** The Wall - objmgr.H
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
#define MAXOBJ 1000
#define NAMESZ 128

typedef struct obj
{
	char objname[NAMESZ];
	int objnum;
	float iClock;  // Internal clock
	float power;  // lifespan, whatever
} GDKOBJ;

extern int MakeObject(char* name);
extern char* GetObjectName(int objnum);
extern void InitObjMgr(void);
extern int DelObject(int objnum);
extern float GetiClock(int objnum);
extern int SetiClock(int objnum, float time);
extern void dumpobjmgr(void);
extern float GetPower(int objnum);
extern int SetPower(int objnum, float power);
int GetObjectNum(char name[]);