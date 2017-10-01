/** N Queens - Main.cpp
* This program is designed to solve the 8 queens problem
* but for any number of queens specified by the user. I
* also have a way to print out a "fancy" chess board.
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
Box* chessBoard[100][100];
int currNumQueens = 2;
int numSolutions = 0;

void printSolutions()
{
	cout << "There are " << numSolutions << " solutions to the " << currNumQueens << " queens problem." << endl;
}

void print(int* bord, int numQueens, int solutionNum)
{
	for (int currRow = 0; currRow < numQueens; currRow++)
	{
		for (int currColumn = 0; currColumn < numQueens; currColumn++)
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

void printFancy(int* bord, int numQueens, int solutionNum)
{
	static bool hasMadeBoxes = false;
	if (hasMadeBoxes == false)
	{
		makeBoxes();
		hasMadeBoxes = true;
	}

	for (int currRow = 0; currRow < numQueens; currRow++)
	{
		for (int currColumn = 0; currColumn < numQueens; currColumn++)
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
	for (int currRow = 0; currRow < numQueens; currRow++)
	{
		for (int boxRow = 0; boxRow < 5; boxRow++)
		{
			for (int currColumn = 0; currColumn < numQueens; currColumn++)
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

bool isValidSpot(int* bord, int numQueens, int columnIndex)
{
	if (columnIndex > 0 && columnIndex < numQueens)
	{
		for (int i = 0; i < columnIndex; i++)
		{
			if ((bord[columnIndex] == bord[i]) || (abs(bord[columnIndex] - bord[i]) == abs(columnIndex - i)))
			{
				return false;
			}
		}
	}

	return true;
}

int main()
{
	int* bord = NULL;
	int currentRow = 0;
	int currentCol = 0;
	int numQueens = 0;
	int count = 0;

	cout << "How many Queens are there?" << endl;
	cin >> numQueens;

	while (currNumQueens <= numQueens)
	{
		bord = new int[currNumQueens + 1];
		for (int i = 0; i < currNumQueens; i++)
		{
			bord[i] = 0;
		}

		currentRow = bord[0];
		currentCol++;

		while (true)
		{
			if (currentRow == currNumQueens || currentCol == currNumQueens)
			{
				//Found a valid solution so print it and move onto the next queen.
				if (currentCol == currNumQueens)
				{
					numSolutions++;
					print(bord, currNumQueens, numSolutions);
					//printFancy(bord, currNumQueens, numSolutions);
				}

				bord[currentCol] = 0;
				currentCol--;

				if (currentCol < 0)
				{
					printSolutions();
					currNumQueens++;
					break;
				}

				bord[currentCol]++;
				currentRow = bord[currentCol];

			}
			else if (isValidSpot(bord, currNumQueens, currentCol) == false)
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

		//Delete the old bord before allocating a new one.
		delete bord;
		numSolutions = 0;
	}

	system("pause");
	return 0;
}