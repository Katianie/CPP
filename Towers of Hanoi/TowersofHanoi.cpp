/** Towers of Hanoi - Main.cpp
* This code shows how to solve the famous Towers of Hanoi
* problem recursively. In this case, the recursive solution is
* more efficient and has less code.
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* Eddie O'Hagan
* Copyright © 2009 Katianie.com
*/
#include <iostream>
#include <vector>

using namespace std;

//Recursive Solution
void move(char tow1, char tow2, char tow3, int num)
{
	//Do this so the counter wont change for each recursive method call
	static int count = 0;

	//Base-case, when we are first moving on the first try we have to move ring 1
	if (num == 1)
	{
		cout << ++count << ": Move ring " << num << " from tower " << tow1 << " to tower " << tow2 << ". " << endl;
		return;
	}

	if (num != 1)
	{
		move(tow1, tow3, tow2, num - 1);
		cout << ++count << ": Move ring " << num << " from tower " << tow1 << " to tower " << tow2 << ". " << endl;
	}

	move(tow3, tow2, tow1, num - 1);

	return;
}

//Iterative Solution
int main()
{
	int count = 0;
	int currentTurn = 1;
	int canidate;
	int lowest;
	int currentOnesTower = 0;
	int currentTower = 0;
	int numRings;
	bool isFirstTime = true;
	vector<int> towers[3];

	cout << "How many Rings?" << endl;
	cin >> numRings;

	towers[0].push_back(-1);
	towers[1].push_back(-1);
	towers[2].push_back(-1);

	for (int i = numRings; i > 0; i--)
	{
		towers[0].push_back(i);
	}

	canidate = towers[0].back();

	while (towers[1].size() - 1 < numRings)
	{
		//if turn is even then use the lowest that is not ring 1
		if (currentTurn % 2 == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				if (towers[i].back() != canidate && towers[i].back() != -1)
				{
					lowest = towers[i].back();
					currentTower = i;
					break;
				}
			}

			for (int i = 0; i < 3; i++)
			{
				if (towers[i].back() < lowest && towers[i].back() != canidate && towers[i].back() != -1)
				{
					lowest = towers[i].back();
					currentTower = i;
				}
			}

			canidate = lowest;

			if (towers[(currentTower + 1) % 3].back() > canidate || towers[(currentTower + 1) % 3].back() == -1)
			{
				cout << ++count << ": Move ring " << canidate << " from tower " << currentTower << " to tower " << (currentTower + 1) % 3 << ". " << endl;
				towers[currentTower].pop_back();
				towers[(currentTower + 1) % 3].push_back(canidate);
			}
			else if (towers[(currentTower + 2) % 3].back() > canidate || towers[(currentTower + 2) % 3].back() == -1)
			{
				cout << ++count << ": Move ring " << canidate << " from tower " << currentTower << " to tower " << (currentTower + 2) % 3 << ". " << endl;
				towers[currentTower].pop_back();
				towers[(currentTower + 2) % 3].push_back(canidate);
			}

		}
		else //move only ring 1 to the right by 1
		{
			if (numRings % 2 == 0)
			{
				canidate = towers[currentOnesTower].back();

				cout << ++count << ": Move ring " << canidate << " from tower " << currentOnesTower << " to tower " << (currentOnesTower + 2) % 3 << ". " << endl;
				towers[currentOnesTower].pop_back();
				towers[(currentOnesTower + 2) % 3].push_back(canidate);
				currentOnesTower = (currentOnesTower + 2) % 3;
			}
			else
			{
				canidate = towers[currentOnesTower].back();

				cout << ++count << ": Move ring " << canidate << " from tower " << currentOnesTower << " to tower " << (currentOnesTower + 1) % 3 << ". " << endl;
				towers[currentOnesTower].pop_back();
				towers[(currentOnesTower + 1) % 3].push_back(canidate);
				currentOnesTower = (currentOnesTower + 1) % 3;
			}
		}

		currentTurn++;
	}

	system("pause");
	return 0;
}