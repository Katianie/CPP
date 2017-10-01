/** The Wall - gun.cpp
* Fires bullet under trajectory formuala.
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
#include "stdlib.h"
#include "shooter.h"
#include "objmgr.h"

#define CH_RADIUS 1
#define CAM_RAD -0.1

static SHOT_VALUES apex, max_dist;
static int crosshairs = 0;	// crosshirs = the object number.  used as flag as to whether the crosshirs need to be positioned
float theta = 3.14/4.0;
float phi = 0.0;
char PhiHud[128];
char ThetaHud[128];

//3D Projectile motion formula to position the cannon ball in flight.
float z(float t)
{
	return(Vo * cos(theta) * t * cos(phi));
}

float y(float t)
{
	return Vo * t * sin(theta) - 0.5 * G * (t * t);
}

float x(float t)
{
	return Vo * sin(phi) * cos(theta) * t;
}

//find the max height - not used or tested -
static void FindApex()
{
	apex.t_val = Vo*sin(theta) / G;
	apex.x_val = x(apex.t_val);
	apex.y_val = y(apex.t_val);
	apex.z_val = z(apex.t_val);
}

void MakeCrossHairs()
{
	crosshairs = MakeObject("crosshairs");
	
	dbMakeObjectPlain(crosshairs, 0.15, 0.15);
	
	dbSetObjectCollisionOff(crosshairs);
	
	dbSetObjectTransparency(crosshairs, 1);
	
	dbLoadImage("crosshairs2.png", crosshairs);
	
	dbTextureObject(crosshairs, crosshairs);
	
	dbPositionObject(crosshairs, CH_RADIUS * sin(phi) * cos(theta), CH_RADIUS * sin(theta), CH_RADIUS * cos(theta) * cos(phi));
	
	return;
}

// find the point when the shot hits the ground - not used or tested
static void GroundPosition()
{
	max_dist.t_val = 2*Vo * sin(theta)/G;
	max_dist.x_val = x(max_dist.t_val);
	max_dist.y_val = y(max_dist.t_val);
	max_dist.z_val = z(max_dist.t_val);
	return;
}
void ResetAngles()
{
	phi = 0.0;
	theta = 3.14/4.0;
}
void ResetGunView()
{
	if(crosshairs)
	{
		dbPositionObject(crosshairs, CH_RADIUS * sin(phi) * cos(theta), CH_RADIUS * sin(theta), CH_RADIUS * cos(theta) * cos(phi));
		dbYRotateObject(crosshairs, RAD_2_DEG(phi));
	}
	
	dbPositionCamera(CAM_RAD*sin(phi), 0, CAM_RAD*cos(phi));
	dbYRotateCamera(RAD_2_DEG(phi));
	dbPointCamera(0,0,0);
	
	return;
}

// Aims the gun.  CR returns FIRE
int AimGun( void)
{
	int rc = AIM;
	if (dbLeftKey())	
	{
		phi -= 0.01;	
	}
	else if (dbRightKey())	
	{
		phi += 0.01;	
	}
	else if (dbUpKey())	
	{
		theta += 0.01;	
	}
	else if (dbDownKey())
	{
		theta -=0.01;	
	}
	else if (dbReturnKey())	
	{
		rc = FIRE;	
	}	
	
	dbPositionCamera(CAM_RAD*sin(phi), 0, CAM_RAD*cos(phi));
	
	dbPointCamera(0,0,0);
	
	dbYRotateCamera(RAD_2_DEG(phi));
	
	sprintf(PhiHud,"Phi = %3.1f",RAD_2_DEG(phi));
	sprintf(ThetaHud,"Theta = %3.1f",RAD_2_DEG(theta));
	
	dbText(530,0,PhiHud);
	dbText(530,10,ThetaHud);
	
	//apex = max height not used in this game
	//FindApex();	
	
	if(crosshairs)
	{
		dbPositionObject(crosshairs,CH_RADIUS*sin(phi)*cos(theta), CH_RADIUS*sin(theta), CH_RADIUS*cos(theta)*cos(phi));
		dbYRotateObject(crosshairs,RAD_2_DEG(phi));
	}
	
	return rc;
}

void FireGun(int shot, float t)
{
	SetiClock(shot,t);
}

void Bullet(int shot, float t)
{
	dbPositionObject(shot, x(t - GetiClock(shot)), y(t - GetiClock(shot)), z(t - GetiClock(shot)));
}

//The bullet number is texture.  values other than 0,0,0 have not been tested
int  MakeShot(int bullet_num, float x0, float y0, float z0)
{
	char buf[256];
	int shot;
	
	shot = MakeObject("Shot");	
	sprintf(buf,"%d.jpg",bullet_num);
	
	dbMakeObjectSphere(shot, 0.5);
	
	dbLoadImage(buf, shot);
	
	dbTextureObject(shot, shot);
	
	dbScaleObjectTexture(shot, 2, 1);
	
	dbYRotateObject(shot, 90.0 ); // rotate it 90 degree so the number faces front
	
	dbPositionObject(shot, x0, y0, z0);
	
	return shot;
}
