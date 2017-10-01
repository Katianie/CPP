/** The Wall - Main.cpp
* Dark GDK - The Game Creators - www.thegamecreators.com. The wizard has 
* created a very simple project that uses Dark GDK, it contains the basic 
* code for a GDK application whenever using Dark GDK you must ensure you include
* the header file.
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
#include "shooter.h"
#include "stdlib.h"
#include "time.h"

extern void Wall( float left, float base, float zposition, int cols, int rows);

//The main entry point for the application is this function
void DarkGDK ( void )
{
	float i;
	float t = 0;
	int currentRound = 1;
	int background;
	int backgroundImage;//a number acociated with the image for the background
	int ground;
	int groundImage;//a number acociated with the image for the gorund
	int shot;
	int gamestate = AIM;
	int collision;
	char CurrRoundHud[32];
	char CurrPlayerHud[32];
	char PlayerOneScoreHud[256];
	char PlayerTwoScoreHud[256];
	char TimeHud[256];
	char WinningPlayerHud[64];
	char CurrentBallHud[32];
	char GameOverBuff[1512];
	int playerOneLottoNums[10];
	int playerOneCurrentBallIndex = 0;
	int playerOneScore = 0;
	int playerTwoLottoNums[10];
	int playerTwoCurrentBallIndex = 0;
	int playerTwoScore = 0;
	char buff[256];
	int gameSoundtrackArray[3];
	int currSong = 0;
	int isExit = 0;
	int jk;
	char playerOneBallsBuff[128];
	char playerTwoBallsBuff[128];
	
	dbSyncOff();
	dbSyncRate(60);
	InitObjMgr();
	srand(time(NULL));
	
	sprintf(PlayerOneScoreHud, "Player One Score 0");
	sprintf(PlayerTwoScoreHud, "Player Two Score 0");

	for(jk = 0; jk < 10; jk++)
	{
		playerOneLottoNums[jk] = (rand() % 40 + 1);
		playerTwoLottoNums[jk] = (rand() % 40 + 1);
		/*sprintf(playerOneBallsBuff,"P1: Index = %d Num = %d",jk,playerOneLottoNums[jk]);
		sprintf(playerTwoBallsBuff,"P2: Index = %d Num = %d",jk,playerTwoLottoNums[jk]);
		dbText(10,10,playerOneBallsBuff);
		dbText(20,20,playerTwoBallsBuff);
		dbWaitKey();*/
	}
	
	LoadImages();
	background = MakeObject("background");
	
	dbLoadImage("brick1.bmp",backgroundImage = MakeObject("brick1.bmp"));	
	
	dbMakeObjectPlane(background, 200, 200);	
	
	dbPositionObject(background, 0, 0, 50);	
	
	dbTextureObject(background,backgroundImage);	
	
	dbScaleObjectTexture(background, 10, 10);
	
	ground = MakeObject("ground");
	
	dbLoadImage("Granite_chippings_T.BMP", groundImage = MakeObject("Granite_chippings_T.BMP"));
	
	dbMakeObjectPlane(ground, 200, 200);
	
	dbPositionObject(ground, 0, -5, 0);
	
	dbTextureObject(ground, groundImage);
	
	dbScaleObjectTexture(ground, 10, 10);
	
	dbXRotateObject(ground, -90);
	
	dbMakeObjectCollisionBox(ground, -200, -1, -200, 200, 1, 200, 0);
	
	dbSetGlobalCollisionOn();

	dbLoadMusic("Beam - On Your Mind (Sean Tyas Hard Dub Remix).mp3",gameSoundtrackArray[0] = MakeObject("Song1"));
	dbLoadMusic("Anti Matter - Absolute Zero.mp3",gameSoundtrackArray[1] = MakeObject("Song2"));
	dbLoadMusic("Aalto - 5 (Original Mix).mp3",gameSoundtrackArray[2] = MakeObject("Song3"));
	dbSync();
	
	Wall(-7, -2 , 9, 15, 14, FALSE);
	
	while (LoopGDK() && !isExit)
	{
		ResetGunView();
		
		sprintf(CurrRoundHud,"Round = %d",currentRound);
		
		dbText(0, 0, CurrRoundHud);
		dbText(0, 20, CurrentBallHud);
		dbText(0, 30, PlayerOneScoreHud);
		dbText(0, 40, PlayerTwoScoreHud);
		
		if(dbKeyState(50))//m key
		{
			dbPauseMusic(gameSoundtrackArray[currSong]);
		}

		if(dbKeyState(2))//1 key
		{
			dbStopMusic(gameSoundtrackArray[currSong]);
			currSong = 0;
			dbPlayMusic(gameSoundtrackArray[currSong]);
		}
		
		if(dbKeyState(3))//2 key
		{
			dbStopMusic(gameSoundtrackArray[currSong]);
			currSong = 1;
			dbPlayMusic(gameSoundtrackArray[currSong]);
		}
		
		if(dbKeyState(4))//3 key
		{
			dbStopMusic(gameSoundtrackArray[currSong]);
			currSong = 2;
			dbPlayMusic(gameSoundtrackArray[currSong]);
		}
		
		if(playerOneCurrentBallIndex < 10 && playerTwoCurrentBallIndex < 10)
		{
			if(gamestate == AIM)
			{
				gamestate = AimGun();
				
				if(currentRound % 2 == 0)
				{
					sprintf(CurrPlayerHud,"Player 2's turn");
					sprintf(CurrentBallHud,"Current Ball = %d",playerTwoLottoNums[playerTwoCurrentBallIndex]);
				}
				else
				{
					sprintf(CurrPlayerHud,"Player 1's turn");
					sprintf(CurrentBallHud,"Current Ball = %d",playerOneLottoNums[playerOneCurrentBallIndex]);
				}
				
				dbText(0,10,CurrPlayerHud);
				dbText(0,20,CurrentBallHud);
			}
			if(gamestate == FIRE)
			{
				if(currentRound == 19)
				{
					sprintf(GameOverBuff,"Game Over: Would you like to play again?\nY for yes N for no");
					dbCenterText(400,150,GameOverBuff);
				}
				
				if(currentRound % 2 == 0)//player 2's turn
				{
					shot = MakeShot(playerTwoLottoNums[playerTwoCurrentBallIndex++],0,0,0); 
				}
				else
				{
					shot = MakeShot(playerOneLottoNums[playerOneCurrentBallIndex++],0,0,0); 
				}
				
				FireGun(shot,t);
				gamestate = SHOOT;
			}
			if(gamestate == SHOOT)
			{
				Bullet(shot, t);
				collision = 0;
				collision = dbObjectCollision(shot,0);
				
				if(collision == ground)
				{
					DelObject(shot);
					currentRound++;
					gamestate = AIM;
				}
				else if( collision)
				{
					sprintf(buff,"collision = %d", collision);
					dbText(10, 0, buff);
					
					if(currentRound % 2 == 0)
					{
						playerTwoScore += (collision + playerTwoLottoNums[playerTwoCurrentBallIndex - 1]);
						sprintf(PlayerTwoScoreHud,"Player Two Score = %d",playerTwoScore);
					}
					else
					{
						playerOneScore += (collision + playerOneLottoNums[playerOneCurrentBallIndex - 1]);
						sprintf(PlayerOneScoreHud,"Player One Score = %d",playerOneScore);
					}
					
					DelObject(collision);
				}
				else 
				{
					//sprintf(buff,"traveling %f collision=%d", t, collision);
				}
			}
			
			t += 0.1;
			dbSync();
		}
		else //game over
		{		
			if(dbKeyState(21))
			{
				for(jk = 0; jk < 10; jk++)
				{
					playerOneLottoNums[jk] = (rand() % 40 + 1);
					playerTwoLottoNums[jk] = (rand() % 40 + 1);
				}
				
				currentRound = 1;				
				playerTwoScore = 0;
				playerTwoCurrentBallIndex = 0;

				playerOneScore = 0;
				playerOneCurrentBallIndex = 0;				
				
				sprintf(PlayerOneScoreHud,"Player One Score 0");
				sprintf(PlayerTwoScoreHud,"Player Two Score 0");
			
				deleteWall();
				Wall( -7, -2 , 9, 15, 14, FALSE);
				ResetGunView();
				gamestate = AIM;
			}
			else if(dbKeyState(49))
			{
				isExit = 1;
			}
		}
	}
	
	// return back to windows
	return;
}