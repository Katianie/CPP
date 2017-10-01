/** 8 Queens Without 2D Array - Main.cpp
* This Program was designed to demonstrate how to solve
* the 8 Queens problem without using goto statements and
* only use a 1D array instead of a 2D. I also included
* a fancy print function.
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
Box* chessBoard[8][8];
Box whiteBox;
Box whiteQueen;
Box blackBox;
Box blackQueen;
int bord[8] = { 0 };

//Prints 1 in the space of a queen, 0 if empty space.
void print(int solutionNum)
{
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

	cout << "-------------------" << solutionNum << endl;
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

void printFancy(int solutionNum)
{
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

	//Print each row,column (pictures) FOR EACH row,column (actual queens and spaces).
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

	cout << "-----------------------------------------------------------" << solutionNum << endl;
}

bool isValidSpot(int columnIndex)
{
	if (columnIndex > 0)
	{
		for (int currIndex = 0; currIndex < columnIndex; currIndex++)
		{
			if ( (bord[columnIndex] == bord[currIndex]) || (abs(bord[columnIndex] - bord[currIndex]) == abs(columnIndex - currIndex)) )
			{
				return false;
			}
		}
	}

	return true;
}

int main()
{
	int solutionNum = 0;
	int currentRow = 0;
	int currentCol = 1;

	while (true)
	{
		if (currentRow >= 8 || currentCol >= 8)
		{
			//Found a solution so print it and move on.
			if (currentCol >= 8)
			{
				solutionNum++;

				print(solutionNum);
				//printFancy(solutionNum);
			}

			bord[currentCol] = 0;
			currentCol--;

			if (currentCol < 0)
			{
				break;
			}

			bord[currentCol]++;
			currentRow = bord[currentCol];

		}
		else if (isValidSpot(currentCol) == false)
		{
			currentRow++;
			bord[currentCol] = currentRow;
		}
		else
		{
			bord[currentCol] = currentRow;
			currentRow = 0;
			currentCol++;
		}
	}

	system("pause");
	return 0;
}