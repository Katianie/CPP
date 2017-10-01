/** Balanced Binary Tree - Main.cpp
* This is an implementation of a balanced binary tree.
* You can add a bunch of data at once and then balance,
* the whole tree once instead of balancing after each insertion.
*
* Having an unbalanced tree causes the data structure to become
* as inneficent as a regular linked list.
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* Eddie O'Hagan
* Copyright © 2016 Katianie.com
*/
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "BinaryTree.h"
#include "PrettyPrint.h"

using namespace std;

//Returns an array of unique randomly generated integers.
int* generateRandomNums(int startRange, int endRange, int amountOfNums)
{
	int* nums = NULL;
	int currIndex = 0;
	int currRandNum = 0;
	vector<int> generatedValues;

	if (amountOfNums > 0 && endRange > startRange && (endRange - startRange) >= amountOfNums)
	{
		nums = (int*)malloc(amountOfNums * sizeof(int));

		while (currIndex < amountOfNums)
		{
			currRandNum = rand() % endRange + startRange;

			//Check to see if the current value is NOT already in the list.
			if (std::find(generatedValues.begin(), generatedValues.end(), currRandNum) == generatedValues.end())
			{
				generatedValues.push_back(currRandNum);
				currIndex++;
			}
		}
	}

	for (int i = 0; i < generatedValues.size(); i++)
	{
		nums[i] = generatedValues.at(i);
	}

	return nums;
}

int main()
{
	BinaryTree tree;
	int* nums;
	int length;
	int lowestNum;
	int highestNum;

	cout << "Number of integers you would like to generate and add:";
	cin >> length;
	cout << "Lowest integer to generate:";
	cin >> lowestNum;
	cout << "Highest integer to generate:";
	cin >> highestNum;

	srand(time(NULL));

	nums = generateRandomNums(lowestNum, highestNum, length);

	if (nums != NULL)
	{
		cout << endl;
		for (int i = 0; i < length; i++)
		{
			cout << nums[i] << " ";
		}
		cout << "\n" << endl;

		//Adds an array of numbers and then balances after bulk adding.
		tree.AddItemsToTree(nums, length);
	}

	system("pause");
	return 0;
}