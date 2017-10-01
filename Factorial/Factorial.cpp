/** Factorial.cpp
* This program is a demonstration of calculating abort
* Factorial via recursion vs using an array. An example
* of a Factorial is: 5! = 5 * 4 * 3 * 2 * 1 = 120.
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* Eddie O'Hagan
* Copyright © 2009 Katianie.com
*/
#include <iostream>

using namespace std;

#define BIG_FACTORIAL_SIZE 100000

//Use a list to store the values instead of using the call stack.
//This drastically improves performance because it does not recursively "spiderweb".
//The trade-off is that you need to use a good amount of memory.
unsigned long long BigFactorial(unsigned long long n)
{
	unsigned long long theList[BIG_FACTORIAL_SIZE];
	unsigned long long index;

	if (n >= BIG_FACTORIAL_SIZE)
	{
		return -1;
	}

	for (index = 0; index < BIG_FACTORIAL_SIZE; index++)
	{
		theList[index] = 0;
	}

	theList[0] = 1;
	theList[1] = 2;

	if (n == 1)
	{
		return 1;
	}
	else if (n == 2)
	{
		return 2;
	}

	for (index = 3; index <= n; index++)
	{
		theList[index - 1] = (theList[index - 2] * index);
	}

	return theList[n - 1];
}

//Traditional factorial formula recursively multiplies the current number by the "soon-to-come" number.
//Unfortunately, if you use a large number, this function will take longer and longer to calculate.
unsigned long long Factorial(unsigned long long n)
{
	if (n == 1)
	{
		return 1;
	}
	else
	{
		return n * Factorial(n - 1);
	}
}

int main()
{
	for (int i = 1; i <= 60; i++)
	{
		cout << "Fact(" << i << ")" << BigFactorial(i) << "\n";
	}

	system("pause");
	return 0;
}