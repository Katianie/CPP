/** 8 Queens - Main.cpp
* This Program was designed to demonstrate how to solve
* the 8 Queens problem without using goto statements.
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* Eddie O'Hagan
* Copyright © 2009 Katianie.com
*/
#include <iostream>

using namespace std;

int bord[8][8] = { 0 };

void print()
{
	static int solutionNum = 0;
	for (int currRow = 0; currRow < 8; currRow++)
	{
		for (int currColumn = 0; currColumn < 8; currColumn++)
		{
			cout << bord[currRow][currColumn];
		}

		cout << endl;
	}

	cout << "-------------------" << ++solutionNum << endl;
}

bool isValidSpot(int row, int column)
{
	for (int currColumn = 0; currColumn < column; currColumn++)
	{
		if (bord[row][currColumn] == 1)
		{
			return false;
		}
	}

	//Check to see if in bounds of bord
	for (int currIndex = 1; (row - currIndex) >= 0 && (column - currIndex) >= 0; currIndex++)
	{
		if (bord[row - currIndex][column - currIndex] == 1)
		{
			return false;
		}
	}

	//Down left test
	for (int currIndex = 1; (row + currIndex) <= 7 && (column - currIndex) >= 0; currIndex++)
	{
		if (bord[row + currIndex][column - currIndex] == 1)
		{
			return false;
		}
	}

	return true;
}

int main()
{
	int currentRow = 0;
	int currentCol = 1;

	bord[0][0] = 1;

	while (true)
	{
		if (currentRow >= 8 || currentCol >= 8)
		{
			if (currentCol >= 8)
			{
				//Solution was found.
				print();
			}

			currentCol--;

			if (currentCol < 0)
			{
				break;
			}

			currentRow = 0;
			while (bord[currentRow][currentCol] == 0)
			{
				currentRow++;
			}

			bord[currentRow][currentCol] = 0;
			currentRow++;
		}
		else if (isValidSpot(currentRow, currentCol) == false)
		{
			currentRow++;
		}
		else
		{
			bord[currentRow][currentCol] = 1;
			currentRow = 0;
			currentCol++;
		}
	}

	system("pause");
	return 0;
}