/** Fibonacci - Main.cpp
* This code shows how to generate Fibonacci numbers by using
* a recursive function and a non recursive function. I use unsigned
* long because the numbers get very large once you get past fib(20)
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* Eddie O'Hagan
* Copyright © 2009 Katianie.com
*/
#include<iostream>

using namespace std;

#define BIG_FIBONACCI_SIZE 100000

//Use a list to store the values instead of using the call stack.
//This drastically improves performance because it does not recursively "spiderweb".
//The trade-off is that you need to use a good amount of memory.
unsigned long long bigFib(unsigned long long n)
{
	unsigned long long nums[BIG_FIBONACCI_SIZE];

	nums[0] = 1;
	nums[1] = 1;

	if (n == 1 || n == 2)
	{
		return 1;
	}

	for (int i = 2; i < n; i++)
	{
		nums[i] = nums[i - 1] + nums[i - 2];
	}

	return nums[n - 1];
}

//Traditional Fibonacci formula recursively multiplies the previous number by the previous's previous number.
//Unfortunately, if you use a large number, this function will take longer and longer to calculate.
unsigned long long fib(unsigned long long n)
{
	if (n == 1 || n == 2)
	{
		return 1;
	}

	return fib(n - 1) + fib(n - 2);
}

void main()
{
	for (unsigned long long i = 1; i < 100000; i++)
	{
		//Slows down around i = 30.
		cout << " fib(" << i << ") = " << fib(i) << endl;

		//Doesn't slow down but could overflow eventually.
		//cout << " bigFib(" << i << ") = " << bigFib(i) << endl;
	}
}
