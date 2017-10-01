/** Dumb 8 Queens - Main.cpp
* This demonstrates a different approach to solving the 8 Queens
* problem without using a backtracking algorithm. As you can
* Tell by the name, this is a very inefficient way to solve this
* problem.
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* Eddie O'Hagan
* Copyright © 2009 Katianie.com
*/
#include <iostream>

using namespace std;

typedef char Box[5][7];
Box whiteBox;
Box whiteQueen;
Box blackBox;
Box blackQueen;
Box* chessBoard[8][8];
int bord[9] = { 0 };

void print()
{
	static int solutionNum = 0;
	for (int currRow = 0; currRow < 8; currRow++)
	{
		for (int currColumn = 0; currColumn < 8; currColumn++)
		{
			if (bord[currRow] == currColumn)
			{
				cout << "1";
			}
			else
			{
				cout << "0";
			}
		}

		cout << endl;
	}

	cout << "-------------------" << ++solutionNum << endl;
}

void printDebug()
{
	static int solutionNum = 0;

	for (int currIndex = 0; currIndex < 8; currIndex++)
	{
		cout << bord[currIndex] << " ";
	}

	cout << "-------------------" << ++solutionNum << endl;
}

void makeBoxes()
{
	for (int currRow = 0; currRow < 5; currRow++)
	{
		for (int currColumn = 0; currColumn < 7; currColumn++)
		{
			whiteBox[currRow][currColumn] = 219;
			whiteQueen[currRow][currColumn] = 219;
			blackBox[currRow][currColumn] = ' ';
			blackQueen[currRow][currColumn] = ' ';
		}
	}

	whiteQueen[1][0] = '*';
	whiteQueen[2][1] = '*';
	whiteQueen[3][2] = '*';
	whiteQueen[3][2] = '*';
	whiteQueen[3][3] = '*';
	whiteQueen[2][3] = '*';
	whiteQueen[3][4] = '*';
	whiteQueen[2][5] = '*';
	whiteQueen[1][6] = '*';

	blackQueen[1][0] = '*';
	blackQueen[2][1] = '*';
	blackQueen[3][2] = '*';
	blackQueen[3][2] = '*';
	blackQueen[3][3] = '*';
	blackQueen[2][3] = '*';
	blackQueen[3][4] = '*';
	blackQueen[2][5] = '*';
	blackQueen[1][6] = '*';
}

void printFancy()
{
	static int solutionNum = 0;
	static bool hasMadeBoxes = false;

	if (hasMadeBoxes == false)
	{
		makeBoxes();
		hasMadeBoxes = true;
	}

	for (int currRow = 0; currRow < 8; currRow++)
	{
		for (int currColumn = 0; currColumn < 8; currColumn++)
		{
			if ((currRow + currColumn) % 2 == 0 && bord[currRow] == currColumn)
			{
				chessBoard[currRow][currColumn] = &blackQueen;
			}
			else if ((currRow + currColumn) % 2 != 0 && bord[currRow] == currColumn)
			{
				chessBoard[currRow][currColumn] = &whiteQueen;
			}
			else if ((currRow + currColumn) % 2 == 0 && bord[currRow] != currColumn)
			{
				chessBoard[currRow][currColumn] = &blackBox;
			}
			else
			{
				chessBoard[currRow][currColumn] = &whiteBox;
			}
		}
	}

	//Print each row,column (box/pictures) FOR EACH row,column (actual queens and spaces).
	for (int currRow = 0; currRow < 8; currRow++)
	{
		for (int boxRow = 0; boxRow < 5; boxRow++)
		{
			for (int currColumn = 0; currColumn < 8; currColumn++)
			{
				for (int boxColumn = 0; boxColumn < 7; boxColumn++)
				{
					cout << (*chessBoard[currRow][currColumn])[boxRow][boxColumn];
				}
			}

			cout << endl;
		}
	}

	cout << "-----------------------------------------------------------" << ++solutionNum << endl;
}

bool isValidSpot()
{
	for (int currRow = 0; currRow < 8; currRow++)
	{
		for (int currColumn = (currRow + 1); currColumn < 8; currColumn++)
		{
			if ((bord[currColumn] == bord[currRow]) || (abs(bord[currColumn] - bord[currRow]) == abs(currColumn - currRow)))
			{
				return false;
			}
		}
	}

	return true;
}

int main()
{
	for (int columnOne = 0; columnOne < 8; columnOne++)
	{
		bord[0] = columnOne;

		for (int columnTwo = 0; columnTwo < 8; columnTwo++)
		{
			bord[1] = columnTwo;

			for (int columnThree = 0; columnThree < 8; columnThree++)
			{
				bord[2] = columnThree;

				for (int columnFour = 0; columnFour < 8; columnFour++)
				{
					bord[3] = columnFour;

					for (int columnFive = 0; columnFive < 8; columnFive++)
					{
						bord[4] = columnFive;

						for (int columnSix = 0; columnSix < 8; columnSix++)
						{
							bord[5] = columnSix;

							for (int columnSeven = 0; columnSeven < 8; columnSeven++)
							{
								bord[6] = columnSeven;

								for (int columnEight = 0; columnEight < 8; columnEight++)
								{
									bord[7] = columnEight;

									if (isValidSpot() == true)
									{
										print();
										//printFancy();
									}
								}
							}
						}
					}
				}
			}
		}
	}

	system("pause");
	return 0;
}