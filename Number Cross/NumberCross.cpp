/** Number Cross - Main.cpp
* This program was designed to solve the Number Cross problem.
* Basically you have a cross with 2 columns and 1 row and you have
* to get numbers 1 - 8 in the cross without consecutive numbers
* touching across or next to each-other. This is what the cross looks like.
*
*             - -
*           - - - -
*             - -
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* Eddie O'Hagan
* Copyright © 2009 Katianie.com
*/
#include <iostream>

using namespace std;

int compArray[8][8];
int printArray[3][4];
int calcArray[8];

void preCompute()
{
	for (int currIndex = 0; currIndex < 8; currIndex++)
	{
		calcArray[currIndex] = 0;
	}

	for (int currRow = 0; currRow < 8; currRow++)
	{
		for (int currColumn = 0; currColumn < 8; currColumn++)
		{
			compArray[currRow][currColumn] = -1;
		}
	}

	compArray[1][0] = 0;

	compArray[2][0] = 1;

	compArray[3][0] = 0;
	compArray[3][1] = 1;
	compArray[3][2] = 2;

	compArray[4][0] = 1;
	compArray[4][1] = 2;
	compArray[4][2] = 3;

	compArray[5][0] = 2;
	compArray[5][1] = 4;

	compArray[6][0] = 0;
	compArray[6][1] = 3;
	compArray[6][2] = 4;

	compArray[7][0] = 3;
	compArray[7][1] = 4;
	compArray[7][2] = 5;
	compArray[7][3] = 6;
}

void printGraph()
{
	static int solutionNum = 0;

	//Initialize the printArray.
	for (int currRow = 0; currRow < 3; currRow++)
	{
		for (int currColumn = 0; currColumn < 4; currColumn++)
		{
			printArray[currRow][currColumn] = -1;
		}
	}

	printArray[1][0] = calcArray[0];

	printArray[0][1] = calcArray[1];
	printArray[0][2] = calcArray[2];
	printArray[1][1] = calcArray[3];
	printArray[1][2] = calcArray[4];
	printArray[1][3] = calcArray[5];
	printArray[2][1] = calcArray[6];
	printArray[2][2] = calcArray[7];

	for (int currRow = 0; currRow < 3; currRow++)
	{
		for (int currColumn = 0; currColumn < 4; currColumn++)
		{
			if (printArray[currRow][currColumn] == -1)
			{
				cout << " ";
			}
			else
			{
				cout << printArray[currRow][currColumn] << " ";
			}
		}
		cout << endl;
	}

	cout << "-------------------" << ++solutionNum << endl;
}

void print()
{
	static int solutionNum = 0;

	for (int currIndex = 0; currIndex < 8; currIndex++)
	{
		cout << calcArray[currIndex] << " ";
	}

	cout << "\n-------------------" << ++solutionNum << endl;
}

void printCompArray()
{
	for (int currRow = 0; currRow < 8; currRow++)
	{
		for (int currColumn = 0; currColumn < 8; currColumn++)
		{
			cout << compArray[currRow][currColumn];
		}

		cout << endl;
	}
}

//In here currentCol is really Current Row when we deal with the comp Array...
bool isValidSpot(int currentCol)
{
	for (int currIndex = 0; currIndex < currentCol; currIndex++)
	{
		if (calcArray[currIndex] == calcArray[currentCol])
		{
			return false;
		}
	}

	for (int currIndex = 0; currIndex < 8 && compArray[currentCol][currIndex] != -1; currIndex++)
	{
		if (abs(calcArray[currentCol] - calcArray[compArray[currentCol][currIndex]]) == 1)
		{
			return false;
		}
	}

	return true;
}

int main()
{
	int currentNum = 1;
	int currentCol = 0;

	preCompute();
	//printCompArray(); //Used for debugging.

	while (true)
	{
		if (currentNum > 8 || currentCol >= 8)
		{
			//Found a solution so print it and move onto next number.
			if (currentCol >= 8)
			{
				printGraph();
			}

			currentCol--;

			if (currentCol < 0)
			{
				break;
			}

			calcArray[currentCol]++;
			currentNum = calcArray[currentCol];

		}
		else if (isValidSpot(currentCol) == false)
		{
			currentNum++;
			calcArray[currentCol] = currentNum;
		}
		else
		{
			calcArray[currentCol] = currentNum;
			currentNum = 1;
			currentCol++;

			if (currentCol < 8)
			{
				calcArray[currentCol] = currentNum;
			}

		}
	}

	system("pause");
	return 0;
}