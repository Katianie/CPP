/** The Wall - shooter.h
* Represents the cannon/gun.
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
#define Vo 2
#define G 0.25
#define AIM 1
#define SHOOT 2
#define FIRE 3
#define HIT 4
#define MISS 5
#define UNK 6
#define RAD_2_DEG(a) (a*180./3.14)

typedef struct shot_charactistics
{
	float x_val;
	float y_val; 
	float z_val; // the maximum distrance
	float t_val; // time to max distance
} SHOT_VALUES;

extern float theta;
extern float phi;

extern int  MakeShot(int powerval, float x0, float y0, float z0);
extern int AimGun(void);
extern void FireGun(int shotobjnum ,float t);
extern void Bullet(int shot,float t);
extern void MakeCrossHairs(void);
extern float x(float);
extern float y(float);
extern float z(float);
extern void  ResetGunView(void);
extern void ResetAngles(void);
