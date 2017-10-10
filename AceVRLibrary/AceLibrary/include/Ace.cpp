#include "Ace.h"

/**
* Allocates a buffer of specified size and
* zeros it out.
*
* @author Katianie
* @date 11/5/2016
*
* @code:
* 	try
* 	{
* 		char* buffer = (char*)Ace::_Aligned_Calloc(1024);
* 		if (strcpy_s(buffer, 1024, "My name is Eddie and I like spaghetti.") != 0)
* 		{
* 			throw "Error: Failed to copy string.";
* 		}
* 		printf("%s\n", buffer);
* 		_aligned_free(buffer);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
*
* @endcode
* Output:My name is Eddie and I like spaghetti.
*
* @param size The number of bytes to allocate.
* @param alignment The byte alignment of the buffer.
*
* @return A zeroed out buffer of specified size.
* 		  Throws an exception if error occurs.
*/
void* Ace::_Aligned_Calloc(size_t size, size_t alignment)
{
	void* buffer = _aligned_malloc(size, alignment);

	//Ensure the buffer was successfully allocated.
	if (buffer == NULL)
	{
		throw "Ace Error: _Aligned_Calloc() buffer could not be allocated.";
		return NULL;
	}

	//Zero out the buffer so it automatically has null terminators.
	if (SecureZeroMemory(buffer, size) == NULL)
	{
		_aligned_free(buffer);
		throw "Ace Error: _Aligned_Calloc() Failed to zero out buffer.";
		return NULL;
	}

	return buffer;
}

/**
* Calculates the absolute value of a number in compliance
* with CERT Coding Standard. The absolute value is the positive
* version of value (example: |3-4| = 1).
*
* @author Katianie
* @date 10/8/2016
*
* @code:
* 	double retVal = Ace::AbsoluteValue(-2.3);
* 	printf("%f\n", retVal);
*
*
* @endcode
* Output:2.300000
*
* @param num The number to use to determine the absolute value.
*
* @return The absolute value of num.
*/
double Ace::AbsoluteValue(double num)
{
	return abs(num);
}

/**
* Calculates the absolute value of a number in compliance
* with CERT Coding Standard. The absolute value is the positive
* version of value (example: |3-4| = 1).
*
* @author Katianie
* @date 10/8/2016
*
* @code:
* 	float retVal = Ace::AbsoluteValue(-2.3f);
* 	printf("%f\n", retVal);
*
*
* @endcode
* Output:2.300000
*
* @param num The number to use to determine the absolute value.
*
* @return The absolute value of num.
*/
float Ace::AbsoluteValue(float num)
{
	return abs(num);
}

/**
* Calculates the absolute value of a number in compliance
* with CERT Coding Standard. The absolute value is the positive
* version of value (example: |3-4| = 1).
*
* @author Katianie
* @date 10/19/2016
*
* @code:
* 	int retVal = Ace::AbsoluteValue(-2);
* 	printf("%d\n", retVal);
*
* @endcode
* Output:2
*
* @param num The number to use to determine the absolute value.
*
* @return The absolute value of num.
*/
int Ace::AbsoluteValue(int num)
{
	return abs(num);
}

/**
* Allocates a buffer and copies a provided traditional string into the buffer.
*
* @author Katianie
* @date 7/9/2016
*
* @code
* 	try
* 	{
* 		char* strToCopy = "An eraser of love...";
* 		char* buffer = Ace::AllocateAndCopyString(strToCopy);
* 		printf("%s\n", buffer);
* 		_aligned_free(buffer);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:An eraser of love...
*
* @param [in,out] strToCopy If non-null, to copy.
*
* @return A newly allocated buffer containing strToCopy.
* 		  Throws an exception if there is an error.
*/
char* Ace::AllocateAndCopyString(char* strToCopy)
{
	size_t bufferSize = 0;
	char* buffer = NULL;

	if (strToCopy != NULL)
	{
		//Calculate the size of the wide string buffer.
		bufferSize = (strlen(strToCopy) + 1) * sizeof(char);

		try
		{
			//Allocate aligned buffer to hold the string.
			buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::AllocateAndCopyString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: AllocateAndCopyString() Encountered an error.";
		}

		//Copy the string into the newly created buffer.
		if (strcpy_s(buffer, bufferSize, strToCopy) != 0)
		{
			throw "Ace Error: AllocateAndCopyString() Failed to copy string to buffer.";
			return NULL;
		}
	}
	else
	{
		throw "Ace Error: AllocateAndCopyString() strToCopy cannot be NULL.";
	}

	return buffer;
}

/**
* Allocates a buffer and copies a provided traditional wide string into the buffer.
*
* @author Katianie
* @date 7/9/2016
*
* @code
* 	try
* 	{
*		wchar_t* strToCopy = L"An eraser of love...";
*		wchar_t* buffer = Ace::AllocateAndCopyString(strToCopy);
*		printf("%S\n", buffer);
*		_aligned_free(buffer);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:An eraser of love...
*
* @param [in,out] wideStrToCopy If non-null, the wide string to copy.
*
* @return A newly allocated buffer containing wideStrToCopy.
* 		  Throws an exception if there is an error.
*/
wchar_t* Ace::AllocateAndCopyString(wchar_t* wideStrToCopy)
{
	size_t bufferSize = 0;
	size_t convertedChars = 0;
	wchar_t* wideStrBuffer = NULL;

	if (wideStrToCopy != NULL)
	{
		//Calculate the size of the wide string buffer.
		bufferSize = (wcslen(wideStrToCopy) + 1) * sizeof(wchar_t);

		try
		{
			//Allocate aligned buffer to hold the wide string.
			wideStrBuffer = (wchar_t*)Ace::_Aligned_Calloc(bufferSize);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::AllocateAndCopyString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: AllocateAndCopyString() Encountered an error.";
		}

		//Copy the string into the newly created buffer.
		if (wcscpy_s(wideStrBuffer, wcslen(wideStrToCopy) + 1, wideStrToCopy) != 0)
		{
			throw "Ace Error: AllocateAndCopyString() Failed to copy string to buffer.";
			return NULL;
		}
	}
	else
	{
		throw "Ace Error: AllocateAndCopyString() wideStrToCopy cannot be NULL.";
	}

	return wideStrBuffer;
}

/**
* Calculates the ArcCos in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/3/2016
*
* @code
* 	try
* 	{
* 		const double PI = 3.14159265;
* 		double result = Ace::ArcCos(PI / 4.0);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.667457
*
* @param numInRadians Number in radians to calculate ArcCos for.
*
* @return The ArcCos of numInRadians. If a mathematical error occurs
* 		  or numInRadians is not between -1 and 1, then an exception
* 		  is thrown.
*/
double Ace::ArcCos(double numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Ensure num is within correct domain.
	//is functions prevent mathematical exception from being thrown.
	if (islessequal(-1.0, numInRadians) == true && islessequal(numInRadians, 1.0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = acos(numInRadians);

		//Look for any mathematical errors as a result of calling acos().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcCos() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcCos() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcCos() numInRadians is not in valid range.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the ArcCos in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.14159265f;
* 		float result = Ace::ArcCos(PI / 4.0f);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.667457
*
* @param numInRadians Number in radians to calculate ArcCos for.
*
* @return The ArcCos of numInRadians. If a mathematical error occurs
* 		  or numInRadians is not between -1 and 1, then an exception
* 		  is thrown.
*/
float Ace::ArcCos(float numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Ensure num is within correct domain.
	//is functions prevent mathematical exception from being thrown.
	if (islessequal(-1.0f, numInRadians) == true && islessequal(numInRadians, 1.0f) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = acos(numInRadians);

		//Look for any mathematical errors as a result of calling acos().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcCos() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcCos() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcCos() numInRadians is not in valid range.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the ArcCos in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/19/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.14159265f;
* 		int result = Ace::ArcCos((int)(PI / 4));
* 		printf("%d\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:1
*
* @param numInRadians Number in radians to calculate ArcCos for.
*
* @return The ArcCos of numInRadians. If a mathematical error occurs
* 		  or numInRadians is not between -1 and 1, then an exception
* 		  is thrown.
*/
int Ace::ArcCos(int numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Ensure num is within correct domain.
	//is functions prevent mathematical exception from being thrown.
	if (islessequal(-1, numInRadians) == true && islessequal(numInRadians, 1) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = acos(numInRadians);

		//Look for any mathematical errors as a result of calling acos().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcCos() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcCos() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcCos() numInRadians is not in valid range.";
		return -1;
	}

	return result;
}

/**
* Calculates the ArcCosH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/3/2016
*
* @code
*	try
*	{
*		double result = Ace::ArcCosH(1.5);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.962424
*
* @param numInRadians Number in radians to calculate ArcCosH for.
*
* @return The ArcCosH of numInRadians. If a mathematical error occurs
* 		  or numInRadians is less than 1, then an exception
* 		  is thrown.
*/
double Ace::ArcCosH(double numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Ensure num is within the domain.
	//is functions prevent mathematical exception from being thrown.
	if (isgreaterequal(numInRadians, 1.0) == true)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = acosh(numInRadians);

		//Look for any mathematical errors as a result of calling acosh().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcCosH() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcCosH() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcCosH() numInRadians is not in valid range.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the ArcCosH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float result = Ace::ArcCosH(1.5f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.962424
*
* @param numInRadians Number in radians to calculate ArcCosH for.
*
* @return The ArcCosH of numInRadians. If a mathematical error occurs
* 		  or numInRadians is less than 1, then an exception
* 		  is thrown.
*/
float Ace::ArcCosH(float numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Ensure num is within the domain.
	//is functions prevent mathematical exception from being thrown.
	if (isgreaterequal(numInRadians, 1.0f) == true)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = acosh(numInRadians);

		//Look for any mathematical errors as a result of calling acosh().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcCosH() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcCosH() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcCosH() numInRadians is not in valid range.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the ArcCosH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/19/2016
*
* @code
*	try
*	{
*		int result = Ace::ArcCosH(2);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:1
*
* @param numInRadians Number in radians to calculate ArcCosH for.
*
* @return The ArcCosH of numInRadians. If a mathematical error occurs
* 		  or numInRadians is less than 1, then an exception
* 		  is thrown.
*/
int Ace::ArcCosH(int numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Ensure num is within the domain.
	//is functions prevent mathematical exception from being thrown.
	if (isgreaterequal(numInRadians, 1) == true)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = acosh(numInRadians);

		//Look for any mathematical errors as a result of calling acosh().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcCosH() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcCosH() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcCosH() numInRadians is not in valid range.";
		return -1;
	}

	return result;
}

/**
* Calculates the ArcSin in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
* 	try
* 	{
* 		const double PI = 3.14159265;
* 		double result = Ace::ArcSin(PI / 4.0);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.903339
*
* @param numInRadians Number in radians to calculate ArcSin for.
*
* @return The ArcSin of numInRadians. If a mathematical error occurs
* 		  or numInRadians is less than or equal to -1 or numInRadians
* 		  is greater then 1, then an exception is thrown.
*/
double Ace::ArcSin(double numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Ensure num is within the domain.
	//is functions prevent mathematical exception from being thrown.
	if (isless(-1.0, numInRadians) == true && islessequal(numInRadians, 1.0) == true)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = asin(numInRadians);

		//Look for any mathematical errors as a result of calling asin().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcSin() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcSin() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcSin() numInRadians is not in valid range.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the ArcSin in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.14159265f;
* 		float result = Ace::ArcSin(PI / 4.0f);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.903339
*
* @param numInRadians Number in radians to calculate ArcSin for.
*
* @return The ArcSin of numInRadians. If a mathematical error occurs
* 		  or numInRadians is less than or equal to -1 or numInRadians
* 		  is greater then 1, then an exception is thrown.
*/
float Ace::ArcSin(float numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Ensure num is within the domain.
	//is functions prevent mathematical exception from being thrown.
	if (isless(-1.0f, numInRadians) == true && islessequal(numInRadians, 1.0f) == true)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = asin(numInRadians);

		//Look for any mathematical errors as a result of calling asin().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcSin() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcSin() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcSin() numInRadians is not in valid range.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the ArcSin in compliance with CERT Coding Standard.
* This function is useless but is provided for completeness.
*
* @author Katianie
* @date 10/19/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.14159265f;
* 		int result = Ace::ArcSin((int)(PI / 4));
* 		printf("%d\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0
*
* @param numInRadians Number in radians to calculate ArcSin for.
*
* @return The ArcSin of numInRadians. If a mathematical error occurs
* 		  or numInRadians is less than or equal to -1 or numInRadians
* 		  is greater then 1, then an exception is thrown.
*/
int Ace::ArcSin(int numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Ensure num is within the domain.
	//is functions prevent mathematical exception from being thrown.
	if (isless(-1, numInRadians) == true && islessequal(numInRadians, 1) == true)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = asin(numInRadians);

		//Look for any mathematical errors as a result of calling asin().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcSin() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcSin() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcSin() numInRadians is not in valid range.";
		return -1;
	}

	return result;
}

/**
* Calculates the ArcSinH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
* 	try
* 	{
* 		const double PI = 3.14159265;
* 		double result = Ace::ArcSinH(PI / 4.0);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.721225
*
* @param numInRadians Number in radians to calculate ArcSinH for.
*
* @return The ArcSinH of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
double Ace::ArcSinH(double numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous mathematical errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = asinh(numInRadians);

	//Look for any mathematical errors as a result of calling asinh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ArcSinH() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ArcSinH() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the ArcSinH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.14159265f;
* 		float result = Ace::ArcSinH(PI / 4.0f);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.721225
*
* @param numInRadians Number in radians to calculate ArcSinH for.
*
* @return The ArcSinH of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
float Ace::ArcSinH(float numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous mathematical errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = asinh(numInRadians);

	//Look for any mathematical errors as a result of calling asinh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ArcSinH() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ArcSinH() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the ArcSinH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/19/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.14159265f;
* 		int result = Ace::ArcSinH((int)(PI / 4.0f));
* 		printf("%d\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0
*
* @param numInRadians Number in radians to calculate ArcSinH for.
*
* @return The ArcSinH of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
int Ace::ArcSinH(int numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous mathematical errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = asinh(numInRadians);

	//Look for any mathematical errors as a result of calling asinh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ArcSinH() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ArcSinH() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates the ArcTan in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
* 	try
* 	{
* 		const double PI = 3.14159265;
* 		double result = Ace::ArcTan(PI / 4.0);
* 		printf("%f\n", result);
* 	}
*	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.665774
*
* @param numInRadians Number in radians to calculate ArcTan for.
*
* @return The ArcTan of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
double Ace::ArcTan(double numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous mathematical errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = atan(numInRadians);

	//Look for any mathematical errors as a result of calling atan().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ArcTan() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ArcTan() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the ArcTan in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.14159265f;
* 		float result = Ace::ArcTan(PI / 4.0f);
* 		printf("%f\n", result);
* 	}
*	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.665774
*
* @param numInRadians Number in radians to calculate ArcTan for.
*
* @return The ArcTan of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
float Ace::ArcTan(float numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous mathematical errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = atan(numInRadians);

	//Look for any mathematical errors as a result of calling atan().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ArcTan() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ArcTan() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the ArcTan in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/19/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.14159265f;
* 		int result = Ace::ArcTan((int)(PI / 4));
* 		printf("%d\n", result);
* 	}
*	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0
*
* @param numInRadians Number in radians to calculate ArcTan for.
*
* @return The ArcTan of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
int Ace::ArcTan(int numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous mathematical errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = atan(numInRadians);

	//Look for any mathematical errors as a result of calling atan().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ArcTan() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ArcTan() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates the ArcTanH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/3/2016
*
* @code
*	try
*	{
*		const double PI = 3.14159265;
*		double result = Ace::ArcTanH(PI / 4.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:1.059306
*
* @param numInRadians Number in radians to calculate ArcTanH for.
*
* @return The ArcTanH of numInRadians. If a mathematical error occurs
* 		  or if numInRadians is less than or equal to -1 or if numInRadians
* 		  is greater than or equal to 1, then an exception is thrown.
*/
double Ace::ArcTanH(double numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Ensure num is within the domain.
	//is functions prevent mathematical exception from being thrown.
	if (isless(-1.0, numInRadians) == true && isless(numInRadians, 1.0) == true)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = atanh(numInRadians);

		//Look for any mathematical errors as a result of calling atanh().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcTanH() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcTanH() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcTanH() numInRadians is not in valid range.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the ArcTanH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		const float PI = 3.14159265f;
*		float result = Ace::ArcTanH(PI / 4.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:1.059306
*
* @param numInRadians Number in radians to calculate ArcTanH for.
*
* @return The ArcTanH of numInRadians. If a mathematical error occurs
* 		  or if numInRadians is less than or equal to -1 or if numInRadians
* 		  is greater than or equal to 1, then an exception is thrown.
*/
float Ace::ArcTanH(float numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Ensure num is within the domain.
	//is functions prevent mathematical exception from being thrown.
	if (isless(-1.0f, numInRadians) == true && isless(numInRadians, 1.0f) == true)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = atanh(numInRadians);

		//Look for any mathematical errors as a result of calling atanh().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcTanH() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcTanH() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcTanH() numInRadians is not in valid range.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the ArcTanH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/19/2016
*
* @code
*	try
*	{
*		const float PI = 3.14159265f;
*		int result = Ace::ArcTanH((int)(PI / 4));
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0
*
* @param numInRadians Number in radians to calculate ArcTanH for.
*
* @return The ArcTanH of numInRadians. If a mathematical error occurs
* 		  or if numInRadians is less than or equal to -1 or if numInRadians
* 		  is greater than or equal to 1, then an exception is thrown.
*/
int Ace::ArcTanH(int numInRadians)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Ensure num is within the domain.
	//is functions prevent mathematical exception from being thrown.
	if (isless(-1, numInRadians) == true && isless(numInRadians, 1) == true)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = atanh(numInRadians);

		//Look for any mathematical errors as a result of calling atanh().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcTanH() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcTanH() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcTanH() numInRadians is not in valid range.";
		return -1;
	}

	return result;
}

/**
* Calculates the ArcTan2 in compliance with CERT Coding Standard.
* To compute the value, the function takes into account the sign
* of both arguments in order to determine the quadrant.
*
* @author Katianie
* @date 10/4/2016
*
* @code
* 	try
*	{
* 		double result = Ace::ArcTan2(-10.0, 5.0);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:-1.107149
*
* @param y Value representing the proportion of the y-coordinate.
* @param x Value representing the proportion of the x-coordinate.
*
* @return Arc Tangent of y/x, in radians. If y = 0 or x = 0,
* 		  or a mathematical error occurs, an exception is thrown.
*/
double Ace::ArcTan2(double y, double x)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Ensure x an y are within their domain.
	if (x != 0 && y != 0)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = atan2(y, x);

		//Look for any mathematical errors as a result of calling atan2().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcTan2() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcTan2() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcTan2() x and y are not in valid range.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the ArcTan2 in compliance with CERT Coding Standard.
* To compute the value, the function takes into account the sign
* of both arguments in order to determine the quadrant.
*
* @author Katianie
* @date 10/6/2016
*
* @code
* 	try
*	{
* 		float result = Ace::ArcTan2(-10.0f, 5.0f);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:-1.107149
*
* @param y Value representing the proportion of the y-coordinate.
* @param x Value representing the proportion of the x-coordinate.
*
* @return Arc Tangent of y/x, in radians. If y = 0 or x = 0,
* 		  or a mathematical error occurs, an exception is thrown.
*/
float Ace::ArcTan2(float y, float x)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Ensure x an y are within their domain.
	if (x != 0 && y != 0)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = atan2(y, x);

		//Look for any mathematical errors as a result of calling atan2().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcTan2() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcTan2() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcTan2() x and y are not in valid range.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the ArcTan2 in compliance with CERT Coding Standard.
* To compute the value, the function takes into account the sign
* of both arguments in order to determine the quadrant.
*
* @author Katianie
* @date 10/19/2016
*
* @code
* 	try
*	{
* 		int result = Ace::ArcTan2(-10, 5);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0
*
* @param y Value representing the proportion of the y-coordinate.
* @param x Value representing the proportion of the x-coordinate.
*
* @return Arc Tangent of y/x, in radians. If y = 0 or x = 0,
* 		  or a mathematical error occurs, an exception is thrown.
*/
int Ace::ArcTan2(int y, int x)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Ensure x an y are within their domain.
	if (x != 0 && y != 0)
	{
		//Clear all previous mathematical errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = atan2(y, x);

		//Look for any mathematical errors as a result of calling atan2().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: ArcTan2() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: ArcTan2() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Domain Error: ArcTan2() x and y are not in valid range.";
		return -1;
	}

	return result;
}

/**
* Adjusts the resolution to the correct aspect ratio for the destination.
*
* @author Katianie
* @date 10/18/2016
*
* @code
*	try
*	{
*		int outNewWidth;
*		int outNewHeight;
*		Ace::CalculateAspectRatio(1920, 1080, 1024, 768, outNewWidth, outNewHeight);
*		printf("New Width:%d    New Height:%d\n", outNewWidth, outNewHeight);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:New Width:1024    New Height:576
*
* @param newWidth The width for the image destination.
* @param newHeight The height for the image destination.
* @param originalWidth The original width of the source.
* @param originalHeight The original height of the source.
* @param [out] outAdjustedWidth The calculated width.
* @param [out] outAdjustedHeight The calculated height.
*
* @return Throws an exception if parameters are not in valid range.
*/
void Ace::CalculateAspectRatio(int newWidth, int newHeight, int originalWidth, int originalHeight, int& outAdjustedWidth, int& outAdjustedHeight)
{
	//Ensure all values are valid before making any mathematical calculations.
	if (newWidth > 0 && newHeight > 0 && originalWidth > 0 && originalHeight > 0)
	{
		//Technically we only need to adjust the height of the image.
		//Formula: (newHeight / newWidth) x originalWidth = adjustedHeight
		outAdjustedWidth = originalWidth;
		outAdjustedHeight = ((float)newHeight / (float)newWidth) * (float)originalWidth;
	}
	else
	{
		throw "Ace Error: CalculateAspectRatio() parameters are not in valid range.";
	}
}

/**
* Uses fseek() to calculate the size of the file by moving the file pointer to the end.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	try
*	{
*		int fileSize = Ace::CalculateFileSize("C:\\dicks.txt");
*		printf("%d\n", fileSize);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:37
*
* @param [in] theFile A pointer to the File.
*
* @return Size of the file in bytes. Throws an exception
* 		  if there is an error.
*/
LONGLONG Ace::CalculateFileSize(char* theFilePath)
{
	HANDLE file;
	LARGE_INTEGER file_size;

	if (theFilePath != NULL)
	{
		file = CreateFileA(theFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == NULL)
		{
			throw "Ace Error: CalculateFileSize() Could not open file.";
			return -1;
		}

		if (GetFileSizeEx(file, &file_size) == false)
		{
			throw "Ace Error: CalculateFileSize() Failed to obtain the file size from GetFileSizeEx().";
			return -1;
		}

		return file_size.QuadPart;
	}
	else
	{
		throw "Ace Error: CalculateFileSize() theFilePath cannot be NULL.";
		return -1;
	}
}

/**
* Gets the size of the file in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 9/4/2016
*
* @code
*	try
*	{
*		int fileSize = Ace::CalculateFileSize(L"C:\\dicks.txt");
*		printf("%d\n", fileSize);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:37
*
* @param [in] theFilePath The full file path.
*
* @return Size of the file in bytes. Throws an exception
* 		  if there is an error.
*/
LONGLONG Ace::CalculateFileSize(wchar_t* theFilePath)
{
	HANDLE file;
	LARGE_INTEGER file_size;

	if (theFilePath != NULL)
	{
		file = CreateFileW(theFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == NULL)
		{
			throw "Ace Error: CalculateFileSize() Could not open file.";
			return -1;
		}

		if (GetFileSizeEx(file, &file_size) == false)
		{
			throw "Ace Error: CalculateFileSize() Failed to obtain the file size from GetFileSizeEx().";
			return -1;
		}

		return file_size.QuadPart;
	}
	else
	{
		throw "Ace Error: CalculateFileSize() theFilePath cannot be NULL.";
		return -1;
	}
}

/**
* Counts the number of digits of num.
*
* @author Katianie
* @date 10/23/2016
*
* @code
*	try
*	{
*		int numLength = Ace::CalculateNumLength(9999999);
*		printf("%d\n", numLength);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:7
*
* @param num Number to count digits of.
*
* @return The length (number of digits) of a given num. Throws an
* 		  exception if there is an error.
*/
int Ace::CalculateNumLength(int num)
{
	try
	{
		if (num == 0)
		{
			return 1;
		}
		else
		{
			return Ace::Floor(Ace::LogBaseTen(Ace::AbsoluteValue(num))) + 1;
		}
	}
	catch (char* exceptionMessage)
	{
		//Create a stack trace of the exception.
		char combinedMessage[1024];
		if (SecureZeroMemory(combinedMessage, 1024) != NULL)
		{
			if (sprintf_s(combinedMessage, 1024, "Ace::CalculateNumLength()->%s\n", exceptionMessage) > 0)
			{
				throw combinedMessage;
			}
		}

		throw "Ace Error: CalculateNumLength() Encountered an error.";
	}

	return -1;
}

/**
* Counts the number of digits of num.
*
* @author Katianie
* @date 10/23/2016
*
* @code
*	try
*	{
*		long num = 9999999;
*		long numLength = Ace::CalculateNumLength(num);
*		printf("%d\n", numLength);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:7
*
* @param num Number to count digits of.
*
* @return The length (number of digits) of a given num. Throws an
* 		  exception if there is an error.
*/
long Ace::CalculateNumLength(long num)
{
	try
	{
		if (num == 0)
		{
			return 1;
		}
		else
		{
			return Ace::Floor(Ace::LogBaseTen(Ace::AbsoluteValue(num))) + 1;
		}
	}
	catch (char* exceptionMessage)
	{
		//Create a stack trace of the exception.
		char combinedMessage[1024];
		if (SecureZeroMemory(combinedMessage, 1024) != NULL)
		{
			if (sprintf_s(combinedMessage, 1024, "Ace::CalculateNumLength()->%s\n", exceptionMessage) > 0)
			{
				throw combinedMessage;
			}
		}

		throw "Ace Error: CalculateNumLength() Encountered an error.";
	}

	return -1;
}

/**
* Calculates the smallest integer value greater than
* or equal to num.
*
* @author Katianie
* @date 10/7/2016
*
* @code
*	try
*	{
*		double retVal = Ace::Ceiling(1.01);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:2.000000
*
* @param num The number to convert.
*
* @return The smallest integral value not less than num.
* 		  Throws an exception if a mathematical error occurs.
*/
double Ace::Ceiling(double num)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous mathematical errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = ceil(num);

	//Look for any mathematical errors as a result of calling ceil().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Ceiling() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Ceiling() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the smallest integer value greater than
* or equal to num.
*
* @author Katianie
* @date 10/7/2016
*
* @code
*	try
*	{
*		float retVal = Ace::Ceiling(1.01f);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:2.000000
*
* @param num The number to convert.
*
* @return The smallest integral value not less than num.
* 		  Throws an exception if a mathematical error occurs.
*/
float Ace::Ceiling(float num)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous mathematical errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = ceil(num);

	//Look for any mathematical errors as a result of calling ceil().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Ceiling() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Ceiling() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the smallest integer value greater than
* or equal to num.
*
* @author Katianie
* @date 10/19/2016
*
* @code
*	try
*	{
*		int retVal = Ace::Ceiling(5);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:5
*
* @param num The number to convert.
*
* @return The smallest integral value not less than num.
* 		  Throws an exception if a mathematical error occurs.
*/
int Ace::Ceiling(int num)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous mathematical errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = ceil(num);

	//Look for any mathematical errors as a result of calling ceil().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Ceiling() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Ceiling() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Returns a single character in str at a specified index.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = "Has anyone ever been as far as decided to be there and then look more like?";
* 		char temp = Ace::CharAt(data, 11);
* 		printf("%c", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:e
*
* @param [in] str String to search.
* @param index The (0 based) index of what character to get.
*
* @return A single character from str. Throws an exception
* 		  if str is NULL or index is not in valid range.
*/
char Ace::CharAt(char* str, int index)
{
	if (str != NULL)
	{
		if (index >= 0 && index < (int)strlen(str))
		{
			return *(str + index);
		}
		else
		{
			throw "Ace Error: CharAt() index is not in valid range.";
			return '\0';
		}
	}
	else
	{
		throw "Ace Error: CharAt() str cannot be NULL.";
		return '\0';
	}
}

/**
* Returns a single character in str at a specified index.
*
* @author Katianie
* @date 9/4/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = L"Has anyone ever been as far as decided to be there and then look more like?";
* 		wchar_t temp = Ace::CharAt(data, 11);
* 		printf("%c", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:e
*
* @param [in] str String to search.
* @param index The (0 based) index of what character to get.
*
* @return A single character from str. Throws an exception
* 		  if str is NULL or index is not in valid range.
*/
wchar_t Ace::CharAt(wchar_t* str, int index)
{
	if (str != NULL)
	{
		if (index >= 0 && index < (int)wcslen(str))
		{
			return *(str + index);
		}
		else
		{
			throw "Ace Error: CharAt() index is not in valid range.";
			return L'\0';
		}
	}
	else
	{
		throw "Ace Error: CharAt() str cannot be NULL.";
		return L'\0';
	}
}

/**
* Calculates the Complementary Error Function integral from
* num to infinity in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
* 	try
* 	{
* 		double result = Ace::ComplementaryErrorFunction(1.0);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.157299
*
* @param num The starting point for the integral.
*
* @return The result of the Complementary Error Function integral.
* 		  If a mathematical error occurs then an exception is thrown.
*/
double Ace::ComplementaryErrorFunction(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = erfc(num);

	//Look for any mathematical errors as a result of calling erfc().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ComplementaryErrorFunction() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ComplementaryErrorFunction() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the Complementary Error Function integral from
* num to infinity in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
* 	try
* 	{
* 		float result = Ace::ComplementaryErrorFunction(1.0f);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.157299
*
* @param num The starting point for the integral.
*
* @return The result of the Complementary Error Function integral.
* 		  If a mathematical error occurs then an exception is thrown.
*/
float Ace::ComplementaryErrorFunction(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = erfc(num);

	//Look for any mathematical errors as a result of calling erfc().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ComplementaryErrorFunction() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ComplementaryErrorFunction() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the Complementary Error Function integral from
* num to infinity in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/19/2016
*
* @code
* 	try
* 	{
* 		int result = Ace::ComplementaryErrorFunction(4);
* 		printf("%d\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0
*
* @param num The starting point for the integral.
*
* @return The result of the Complementary Error Function integral.
* 		  If a mathematical error occurs then an exception is thrown.
*/
int Ace::ComplementaryErrorFunction(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = erfc(num);

	//Look for any mathematical errors as a result of calling erfc().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ComplementaryErrorFunction() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ComplementaryErrorFunction() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Appends an array of strings together and returns the combined string.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	try
*	{
*		char** stringArray = (char**)_aligned_malloc(4 * sizeof(char*), 16);
*		for (int currIndex = 0; currIndex < 4; currIndex++)
*		{
*			stringArray[currIndex] = Ace::AllocateAndCopyString("Everything");
*		}
*		char* completeStr = Ace::ConcatenateStrings(4, stringArray);
*		printf("%s\n", completeStr);
*		for (int currIndex = 0; currIndex < 4; currIndex++)
*		{
*			_aligned_free(stringArray[currIndex]);
*		}
*		_aligned_free(stringArray);
*		_aligned_free(completeStr);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:EverythingEverythingEverythingEverything
*
* @param numStrings	Number of strings to append.
* @param [in] stringArray The array of strings to append.
*
* @return A combined string of all the strings in the array.
* 		  If an error occurs, an exception is thrown.
*/
char* Ace::ConcatenateStrings(int numStrings, char** stringArray)
{
	size_t buffSize = 0;
	char* buff = NULL;
	char* currStr = NULL;
	int totalStrLen = 0;

	if (stringArray != NULL && numStrings > 0 && numStrings < MAX_STRING_BUFFER_SIZE)
	{
		//Calculate the length of each string so we know how much to allocate for the buffer.
		for (int currIndex = 0; currIndex < numStrings; currIndex++)
		{
			if (stringArray[currIndex] != NULL)
			{
				totalStrLen += strlen(stringArray[currIndex]);
			}
		}

		//+1 for the Null terminating char.
		totalStrLen += 1;

		//Allocate the buffer to hold our concatenated strings.
		buffSize = totalStrLen * sizeof(char);
		buff = (char*)_aligned_malloc(buffSize, BYTE_ALIGNMENT);

		//Ensure the buffer was allocated successfully.
		if (buff == NULL)
		{
			throw "Ace Error: ConcatenateStrings() Buffer could not be allocated.";
			return NULL;
		}

		if (SecureZeroMemory(buff, buffSize) == NULL)
		{
			throw "Ace Error: ConcatenateStrings() Failed to zero out buffer.";
			return NULL;
		}

		//Copy the first string into the buffer to initialize it.
		if (strcpy_s(buff, buffSize, stringArray[0]) != 0)
		{
			throw "Ace Error: ConcatenateStrings() Failed to initialize buffer.";
			return NULL;
		}

		//Concatenate the other strings to our newly created buffer.
		for (int currIndex = 1; currIndex < numStrings; currIndex++)
		{
			//Concatenate the two strings.
			if (strcat_s(buff, totalStrLen, stringArray[currIndex]) != 0)
			{
				throw "Ace Error: ConcatenateStrings() Failed to concatenate strings.";
			}
		}
	}
	else
	{
		throw "Ace Error: ConcatenateStrings() Parameters are not in valid range.";
	}

	return buff;
}

/**
* Appends an array of strings together and returns the combined string.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	try
*	{
*		wchar_t** stringArray = (wchar_t**)_aligned_malloc(4 * sizeof(wchar_t*), 16);
*		for (int currIndex = 0; currIndex < 4; currIndex++)
*		{
*			stringArray[currIndex] = Ace::AllocateAndCopyString(L"Everything");
*		}
*		wchar_t* completeStr = Ace::ConcatenateStrings(4, stringArray);
*		printf("%S\n", completeStr);
*		for (int currIndex = 0; currIndex < 4; currIndex++)
*		{
*			_aligned_free(stringArray[currIndex]);
*		}
*		_aligned_free(stringArray);
*		_aligned_free(completeStr);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:EverythingEverythingEverythingEverything
*
* @param numStrings	Number of strings to append.
* @param [in] stringArray The array of strings to append.
*
* @return A combined string of all the strings in the array.
* 		  If an error occurs, an exception is thrown.
*/
wchar_t* Ace::ConcatenateStrings(int numStrings, wchar_t** stringArray)
{
	size_t buffSize = 0;
	wchar_t* buff = NULL;
	wchar_t* currStr = NULL;
	int totalStrLen = 0;

	if (stringArray != NULL && numStrings > 0 && numStrings < MAX_STRING_BUFFER_SIZE)
	{
		//Calculate the length of each string so we know how much to allocate for the buffer.
		for (int currIndex = 0; currIndex < numStrings; currIndex++)
		{
			if (stringArray[currIndex] != NULL)
			{
				totalStrLen += wcslen(stringArray[currIndex]);
			}
		}

		//+1 for the Null terminating char.
		totalStrLen += 1;

		//Allocate the buffer to hold our concatenated strings.
		buffSize = totalStrLen * sizeof(wchar_t);
		buff = (wchar_t*)_aligned_malloc(buffSize, BYTE_ALIGNMENT);

		//Ensure the buffer was allocated successfully.
		if (buff == NULL)
		{
			throw "Ace Error: ConcatenateStrings() Buffer could not be allocated.";
			return NULL;
		}

		if (SecureZeroMemory(buff, buffSize) == NULL)
		{
			throw "Ace Error: ConcatenateStrings() Failed to zero out buffer.";
			return NULL;
		}

		//Copy the first string into the buffer to initialize it.
		if (wcscpy_s(buff, totalStrLen, stringArray[0]) != 0)
		{
			throw "Ace Error: ConcatenateStrings() Failed to initialize buffer.";
			return NULL;
		}

		//Concatenate the other strings to our newly created buffer.
		for (int currIndex = 1; currIndex < numStrings; currIndex++)
		{
			//Concatenate the two strings.
			if (wcscat_s(buff, totalStrLen, stringArray[currIndex]) != 0)
			{
				throw "Ace Error: ConcatenateStrings() Failed to concatenate strings.";
			}
		}
	}
	else
	{
		throw "Ace Error: ConcatenateStrings() Parameters are not in valid range.";
	}

	return buff;
}

/**
* Appends a vector array of strings together and returns the combined string.
*
* @author Katianie
* @date 5/6/2016
*
* @code
* 	try
* 	{
* 		vector<char*> stringVectorArray;
* 		for (int currIndex = 1; currIndex <= 4; currIndex++)
* 		{
* 			stringVectorArray.push_back("Everything");
* 		}
* 		char* completeStr = Ace::ConcatenateStrings(stringVectorArray);
* 		printf("%s", completeStr);
* 		_aligned_free(completeStr);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:EverythingEverythingEverythingEverything.
*
* @param [in,out] stringVectorArray The vector array of strings to append.
*
* @return A combined string of all the strings in the array.
* 		  If an error occurs, an exception is thrown.
*/
char* Ace::ConcatenateStrings(vector<char*> stringVectorArray)
{
	char* retVal = NULL;
	char** stringArray = NULL;
	int stringArrayLen = 0;
	int stringVectorArrayLen = 0;
	int currStringLen = 0;

	//Get the length so we don't have to keep calling .size().
	stringVectorArrayLen = stringVectorArray.size();

	if (stringVectorArrayLen > 0 && stringVectorArrayLen < MAX_STRING_BUFFER_SIZE)
	{
		//Allocate a traditional string array to hold our strings.
		stringArray = Ace::ConvertStringVectorArrayToStringArray(stringVectorArray, stringArrayLen);

		if (stringArray != NULL)
		{
			//Concatenate all of the strings in the array together.
			retVal = Ace::ConcatenateStrings(stringArrayLen, stringArray);

			//Delete the array contents and the array itself.
			for (int currIndex = 0; currIndex < stringArrayLen; currIndex++)
			{
				_aligned_free(stringArray[currIndex]);
			}
			_aligned_free(stringArray);
		}
		else
		{
			throw "Ace Error: ConcatenateStrings() Returned stringArray is NULL.";
		}
	}
	else
	{
		throw "Ace Error: ConcatenateStrings() Vector Array length is not in valid range.";
	}

	return retVal;
}

/**
* Appends a vector array of wide strings together and returns the combined wide string.
*
* @author Katianie
* @date 9/4/2016
*
* @code
* 	try
* 	{
* 		vector<wchar_t*> stringVectorArray;
* 		for (int currIndex = 1; currIndex <= 4; currIndex++)
* 		{
* 			stringVectorArray.push_back(L"Everything");
* 		}
* 		wchar_t* completeStr = Ace::ConcatenateStrings(stringVectorArray);
* 		printf("%S", completeStr);
* 		_aligned_free(completeStr);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:EverythingEverythingEverythingEverything.
*
* @param [in,out] stringVectorArray The vector array of strings to append.
*
* @return A combined string of all the strings in the array.
* 		  If an error occurs, an exception is thrown.
*/
wchar_t* Ace::ConcatenateStrings(vector<wchar_t*> stringVectorArray)
{
	wchar_t* retVal = NULL;
	wchar_t** stringArray = NULL;
	int stringArrayLen = 0;
	int stringVectorArrayLen = 0;
	int currStringLen = 0;

	//Get the length so we don't have to keep calling .size().
	stringVectorArrayLen = stringVectorArray.size();

	if (stringVectorArrayLen > 0 && stringVectorArrayLen < MAX_STRING_BUFFER_SIZE)
	{
		//Allocate a traditional string array to hold our strings.
		stringArray = Ace::ConvertStringVectorArrayToStringArray(stringVectorArray, stringArrayLen);

		if (stringArray != NULL)
		{
			//Concatenate all of the strings in the array together.
			retVal = Ace::ConcatenateStrings(stringArrayLen, stringArray);

			//Delete the array contents and the array itself.
			for (int currIndex = 0; currIndex < stringArrayLen; currIndex++)
			{
				_aligned_free(stringArray[currIndex]);
			}
			_aligned_free(stringArray);
		}
		else
		{
			throw "Ace Error: ConcatenateStrings() Returned stringArray is NULL.";
		}
	}
	else
	{
		throw "Ace Error: ConcatenateStrings() Vector Array length is not in valid range.";
	}

	return retVal;
}

/**
* Appends a series of strings together.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* completeStr = Ace::Concatenate(3, "Everything", "Everything", "Everything");
* 		printf("%s\n", completeStr);
* 		_aligned_free(completeStr);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:EverythingEverythingEverything.
*
* @param [in] numStrings Number of strings to append.
* @param [in] ... Variable arguments providing additional strings.
*
* @return A combined string of all the strings in the arguments.
* 		  Throws an exception if an error occurs.
*/
char* Ace::Concatenate(int numStrings, ...)
{
	vector<char*> stringVectorArray;
	va_list vargList;

	if (numStrings > 0 && numStrings < MAX_STRING_BUFFER_SIZE)
	{
		//Store each of the arguments so we can iterate through them later.
		va_start(vargList, numStrings);
		for (int currIndex = 0; currIndex < numStrings; currIndex++)
		{
			stringVectorArray.push_back((char*)va_arg(vargList, char*));
		}
		va_end(vargList);

		return Ace::ConcatenateStrings(stringVectorArray);
	}
	else
	{
		throw "Ace Error: Concatenate() numStrings is not in valid range.";
		return NULL;
	}
}

/**
* Appends a series of strings together.
*
* @author Katianie
* @date 9/5/2016
*
* @code
* 	try
* 	{
* 		wchar_t* completeStr = Ace::ConcatenateW(3, L"Everything", L"Everything", L"Everything");
* 		printf("%S\n", completeStr);
* 		_aligned_free(completeStr);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:EverythingEverythingEverything
*
* @param [in] numStrings Number of strings to append.
* @param [in] ... Variable arguments providing additional strings.
*
* @return A combined string of all the strings in the arguments.
* 		  Throws an exception if an error occurs.
*/
wchar_t* Ace::ConcatenateW(int numWideStrings, ...)
{
	vector<wchar_t*> stringVectorArray;
	va_list vargList;

	if (numWideStrings > 0 && numWideStrings < MAX_STRING_BUFFER_SIZE)
	{
		//Store each of the arguments so we can iterate through them later.
		va_start(vargList, numWideStrings);
		for (int currIndex = 0; currIndex < numWideStrings; currIndex++)
		{
			stringVectorArray.push_back((wchar_t*)va_arg(vargList, wchar_t*));
		}
		va_end(vargList);

		return Ace::ConcatenateStrings(stringVectorArray);
	}
	else
	{
		throw "Ace Error: ConcatenateW() numStrings is not in valid range.";
		return NULL;
	}
}

/**
* Searches strToSearch for the first occurrence of strToLookFor and returns
* true if found.
*
* @author Katianie
* @date 5/26/2016
*
* @code
* 	try
* 	{
* 		bool result = Ace::Contains("My name is Eddie and I like cheese.", "Eddie");
* 		if (result == true)
* 		{
* 			printf("%s\n", "Contains the word.");
* 		}
* 		else
* 		{
* 			printf("%s\n", "Does not contain the word.");
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Contains the word.
*
* @param [in] strToSearch  String to search starting from left to right.
* @param [in] strToLookFor String to look for inside of strToSearch.
*
* @return true if strToSearch contains strToLookFor, false
*		  otherwise. Throws an exception if error occurs.
*/
bool Ace::Contains(char* strToSearch, char* strToLookFor)
{
	try
	{
		return Ace::IndexOf(strToSearch, strToLookFor) >= 0;
	}
	catch (char* exceptionMessage)
	{
		//Create a stack trace of the exception.
		char combinedMessage[1024];
		if (SecureZeroMemory(combinedMessage, 1024) != NULL)
		{
			if (sprintf_s(combinedMessage, 1024, "Ace::Contains()->%s\n", exceptionMessage) > 0)
			{
				throw combinedMessage;
			}
		}

		throw "Ace Error: Contains() Encountered an error.";
	}

	return false;
}

/**
* Searches strToSearch for the first occurrence of strToLookFor and returns
* true if found.
*
* @author Katianie
* @date 9/5/2016
*
* @code
* 	try
* 	{
* 		bool result = Ace::Contains(L"My name is Eddie and I like cheese.", L"Eddie");
* 		if (result == true)
* 		{
* 			printf("%S\n", L"Contains the word.");
* 		}
* 		else
* 		{
* 			printf("%S\n", L"Does not contain the word.");
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Contains the word.
*
* @param [in] strToSearch  Wide String to search starting from left to right.
* @param [in] strToLookFor Wide String to look for inside of strToSearch.
*
* @return true if strToSearch contains strToLookFor, false
*		  otherwise. Throws an exception if error occurs.
*/
bool Ace::Contains(wchar_t* strToSearch, wchar_t* strToLookFor)
{
	try
	{
		return Ace::IndexOf(strToSearch, strToLookFor) >= 0;
	}
	catch (char* exceptionMessage)
	{
		//Create a stack trace of the exception.
		char combinedMessage[1024];
		if (SecureZeroMemory(combinedMessage, 1024) != NULL)
		{
			if (sprintf_s(combinedMessage, 1024, "Ace::Contains()->%s\n", exceptionMessage) > 0)
			{
				throw combinedMessage;
			}
		}

		throw "Ace Error: Contains() Encountered an error.";
	}

	return false;
}

/**
* Searches strToSearch for the first occurrence of strToLookFor (regardless of
* case) and returns true if found.
*
* @author Katianie
* @date 5/26/2016
*
* @code
* 	try
* 	{
* 		bool result = Ace::ContainsIgnoreCase("My name is eddIe and I like cheese.", "EdDIe");
* 		if (result == true)
* 		{
* 			printf("%s\n", "Contains the word.");
*		}
* 		else
* 		{
* 			printf("%s\n", "Does not contain the word.");
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Contains the word.
*
* @param [in] strToSearch  String to search starting from left to right.
* @param [in] strToLookFor String to look for inside of strToSearch.
*
* @return true if strToSearch contains strToLookFor, false
*		  otherwise. Throws an exception if error occurs.
*/
bool Ace::ContainsIgnoreCase(char* strToSearch, char* strToLookFor)
{
	try
	{
		return Ace::IndexOfIgnoreCase(strToSearch, strToLookFor) >= 0;
	}
	catch (char* exceptionMessage)
	{
		//Create a stack trace of the exception.
		char combinedMessage[1024];
		if (SecureZeroMemory(combinedMessage, 1024) != NULL)
		{
			if (sprintf_s(combinedMessage, 1024, "Ace::ContainsIgnoreCase()->%s\n", exceptionMessage) > 0)
			{
				throw combinedMessage;
			}
		}

		throw "Ace Error: ContainsIgnoreCase() Encountered an error.";
	}

	return false;
}

/**
* Searches strToSearch for the first occurrence of strToLookFor (regardless of
* case) and returns true if found.
*
* @author Katianie
* @date 9/5/2016
*
* @code
* 	try
* 	{
* 		bool result = Ace::ContainsIgnoreCase(L"My name is eddIe and I like cheese.", L"EdDIe");
* 		if (result == true)
* 		{
* 			printf("%S\n", L"Contains the word.");
*		}
* 		else
* 		{
* 			printf("%S\n", L"Does not contain the word.");
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Contains the word.
*
* @param [in] strToSearch  Wide String to search starting from left to right.
* @param [in] strToLookFor Wide String to look for inside of strToSearch.
*
* @return true if strToSearch contains strToLookFor, false
*		  otherwise. Throws an exception if error occurs.
*/
bool Ace::ContainsIgnoreCase(wchar_t* strToSearch, wchar_t* strToLookFor)
{
	try
	{
		return Ace::IndexOfIgnoreCase(strToSearch, strToLookFor) >= 0;
	}
	catch (char* exceptionMessage)
	{
		//Create a stack trace of the exception.
		char combinedMessage[1024];
		if (SecureZeroMemory(combinedMessage, 1024) != NULL)
		{
			if (sprintf_s(combinedMessage, 1024, "Ace::ContainsIgnoreCase()->%s\n", exceptionMessage) > 0)
			{
				throw combinedMessage;
			}
		}

		throw "Ace Error: ContainsIgnoreCase() Encountered an error.";
	}

	return false;
}

/**
* By definition, false is equal to 0 and true is equal to anything but 0. In
* other words, any positive or negative number returns true, if num is 0 then
* false.
*
* @author Katianie
* @date 10/20/2016
*
* @code
* 	bool data = Ace::ConvertDoubleToBool(0.0);
* 	if (data == true)
* 	{
* 		printf("%s", "true");
* 	}
* 	else
* 	{
* 		printf("%s", "false");
* 	}
*
* @endcode
* Output:false
*
* @param [in] num The number to determine if true or false.
*
* @return true or false based on the value of num.
*/
bool Ace::ConvertDoubleToBool(double num)
{
	if (num != 0.0)
	{
		return true;
	}
	else //num == 0.0
	{
		return false;
	}
}

/**
* By definition, false is equal to 0 and true is equal to anything but 0. In
* other words, any positive or negative number returns true, if num is 0 then
* false.
*
* @author Katianie
* @date 10/20/2016
*
* @code
* 	bool data = Ace::ConvertFloatToBool(0.0f);
* 	if (data == true)
* 	{
* 		printf("%s", "true");
* 	}
* 	else
* 	{
* 		printf("%s", "false");
* 	}
*
* @endcode
* Output:false
*
* @param [in] num The number to determine if true or false.
*
* @return true or false based on the value of num.
*/
bool Ace::ConvertFloatToBool(float num)
{
	if (num != 0.0f)
	{
		return true;
	}
	else //num == 0.0
	{
		return false;
	}
}

/**
* By definition, false is equal to 0 and true is equal to anything but 0. In
* other words, any positive or negative number returns true, if num is 0 then
* false.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	bool data = Ace::ConvertIntToBool(0);
* 	if (data == true)
* 	{
* 		printf("%s", "true");
* 	}
* 	else
* 	{
* 		printf("%s", "false");
* 	}
*
* @endcode
* Output:false
*
* @param [in] num The number to determine if true or false.
*
* @return true or false based on the value of num.
*/
bool Ace::ConvertIntToBool(int num)
{
	if (num != 0)
	{
		return true;
	}
	else //num == 0
	{
		return false;
	}
}

/**
* Returns a buffer containing the result of _itoa_s()
* using integerToConvert.
*
* @author Katianie
* @date 10/19/2016
*
* @code
* 	try
* 	{
* 		char* str = Ace::ConvertIntToString(989969);
* 		printf("%s\n", str);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:989969
*
* @param integerToConvert The integer to pass to _itoa_s().
*
* @return A buffer containing the converted integerToConvert.
*/
char* Ace::ConvertIntToString(int integerToConvert)
{
	int bufferSize = 0;
	char* buffer = NULL;

	try
	{
		//Determine how big our buffer needs to be.
		bufferSize = (Ace::CalculateNumLength(integerToConvert) + 1) * sizeof(char);
		buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
	}
	catch (char* exceptionMessage)
	{
		//Create a stack trace of the exception.
		char combinedMessage[1024];
		if (SecureZeroMemory(combinedMessage, 1024) != NULL)
		{
			if (sprintf_s(combinedMessage, 1024, "Ace::ConvertIntToString()->%s\n", exceptionMessage) > 0)
			{
				throw combinedMessage;
				return NULL;
			}
		}

		throw "Ace Error: ConvertIntToString() Encountered an error.";
		return NULL;
	}

	if (_itoa_s(integerToConvert, buffer, bufferSize / sizeof(char), 10) != 0)
	{
		throw "Ace Error: ConvertIntToString() Could not convert integerToConvert.";
		return NULL;
	}

	return buffer;
}

/**
* Returns a buffer containing the result of _itow_s()
* using integerToConvert.
*
* @author Katianie
* @date 10/19/2016
*
* @code
* 	try
* 	{
* 		wchar_t* str = Ace::ConvertIntToWideString(989969);
* 		printf("%S\n", str);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:989969
*
* @param integerToConvert The integer to pass to _itow_s().
*
* @return A buffer containing the converted integerToConvert.
*/
wchar_t* Ace::ConvertIntToWideString(int integerToConvert)
{
	int bufferSize = 0;
	wchar_t* buffer = NULL;

	try
	{
		//Determine how big our buffer needs to be.
		bufferSize = (Ace::CalculateNumLength(integerToConvert) + 1) * sizeof(wchar_t);
		buffer = (wchar_t*)Ace::_Aligned_Calloc(bufferSize);
	}
	catch (char* exceptionMessage)
	{
		//Create a stack trace of the exception.
		char combinedMessage[1024];
		if (SecureZeroMemory(combinedMessage, 1024) != NULL)
		{
			if (sprintf_s(combinedMessage, 1024, "Ace::ConvertIntToWideString()->%s\n", exceptionMessage) > 0)
			{
				throw combinedMessage;
				return NULL;
			}
		}

		throw "Ace Error: ConvertIntToWideString() Encountered an error.";
		return NULL;
	}

	if (_itow_s(integerToConvert, buffer, bufferSize / sizeof(wchar_t), 10) != 0)
	{
		throw "Ace Error: ConvertIntToWideString() Could not convert integerToConvert.";
		return NULL;
	}

	return buffer;
}

/**
* Converts a traditional string array to a vector string array.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char** strArray = (char**)_aligned_malloc(4 * sizeof(char*), 16);
* 		for (int currIndex = 0; currIndex < 4; currIndex++)
* 		{
* 			strArray[currIndex] = (char*)_aligned_malloc(32 * sizeof(char), 16);
* 			memset(strArray[currIndex], 0, 32 * sizeof(char));
* 			_itoa_s(currIndex, strArray[currIndex], 32, 10);
* 		}
* 		vector<char*> stringVectorArray = Ace::ConvertStringArrayToStringVectorArray(strArray);
* 		for (int currIndex = 0; currIndex < 4; currIndex++)
* 		{
* 			printf("strVectorArray[%d]:%s\n", currIndex, stringVectorArray[currIndex]);
* 			_aligned_free(strArray[currIndex]);
* 		}
* 		_aligned_free(strArray);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:strVectorArray[0]:0
* 		 strVectorArray[1]:1
* 		 strVectorArray[2]:2
* 		 strVectorArray[3]:3
*
* @param [in] stringArray The string array to convert.
*
* @return A string vector array. If stringArray is NULL,
* 		  an exception is thrown.
*/
vector<char*> Ace::ConvertStringArrayToStringVectorArray(char** stringArray)
{
	vector<char*> strVectorArray;

	if (stringArray != NULL)
	{
		for (int currIndex = 0; stringArray[currIndex] != NULL; currIndex++)
		{
			strVectorArray.push_back(stringArray[currIndex]);
		}
	}
	else
	{
		throw "Ace Error: ConvertStringArrayToStringVectorArray() stringArray cannot be NULL";
	}

	return strVectorArray;
}

/**
* Converts a traditional wide string array to a vector wide string array.
*
* @author Katianie
* @date 9/5/2016
*
* @code
* 	try
* 	{
* 		wchar_t** strArray = (wchar_t**)_aligned_malloc(4 * sizeof(wchar_t*), 16);
* 		for (int currIndex = 0; currIndex < 4; currIndex++)
* 		{
* 			strArray[currIndex] = (wchar_t*)_aligned_malloc(32 * sizeof(wchar_t), 16);
* 			memset(strArray[currIndex], 0, 32 * sizeof(wchar_t));
* 			_itow_s(currIndex, strArray[currIndex], 32, 10);
* 		}
* 		vector<wchar_t*> stringVectorArray = Ace::ConvertStringArrayToStringVectorArray(strArray);
* 		for (int currIndex = 0; currIndex < 4; currIndex++)
* 		{
* 			printf("strVectorArray[%d]:%S\n", currIndex, stringVectorArray[currIndex]);
* 			_aligned_free(strArray[currIndex]);
* 		}
* 		_aligned_free(strArray);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:strVectorArray[0]:0
* 		 strVectorArray[1]:1
* 		 strVectorArray[2]:2
* 		 strVectorArray[3]:3
*
* @param [in] stringArray The wide string array to convert.
*
* @return A wide string vector array. If stringArray is NULL,
* 		  an exception is thrown.
*/
vector<wchar_t*> Ace::ConvertStringArrayToStringVectorArray(wchar_t** stringArray)
{
	vector<wchar_t*> strVectorArray;

	if (stringArray != NULL)
	{
		for (int currIndex = 0; stringArray[currIndex] != NULL; currIndex++)
		{
			strVectorArray.push_back(stringArray[currIndex]);
		}
	}
	else
	{
		throw "Ace Error: ConvertStringArrayToStringVectorArray() stringArray cannot be NULL";
	}

	return strVectorArray;
}

/**
* Converts a given string to a bool, If str is a string like "False", "FALSE", or
* "tRuE", then the function will return false, false, true respectively. If
* str is an integer for example; "0" will return false and any other number will return
* true.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	try
*	{
*		bool data = Ace::ConvertStringToBool("tRuE");
*		if (data == true)
*		{
*			printf("%s", "true");
*		}
*		else
*		{
*			printf("%s", "false");
*		}
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:true
*
* @param [in] str String to convert.
*
* @return true if str is equal to "true" (any case combination) or if str
*			   is an integer not equal to 0; false otherwise. Throws an
*			   exception if str is NULL.
*/
bool Ace::ConvertStringToBool(char* str)
{
	char* strToUpper = NULL;

	if (str != NULL)
	{
		strToUpper = Ace::ConvertToUpperCase(str);

		//Check to see if str is equal to "false" or "true".
		if (strcmp(strToUpper, "TRUE") == 0)
		{
			_aligned_free(strToUpper);
			return true;
		}
		else if (strcmp(strToUpper, "FALSE") == 0)
		{
			_aligned_free(strToUpper);
			return false;
		}
		else
		{
			_aligned_free(strToUpper);

			//Check to see if str is an integer.
			if (Ace::IsNumber(str) == true)
			{
				return Ace::ConvertIntToBool(atoi(str));
			}
			else if (strlen(str) == 1)
			{
				//Small chance str could be equal to "T" or "F"
				if (toupper(*str) == 'T')
				{
					return true;
				}
				else if (toupper(*str) == 'F')
				{
					return false;
				}
			}
		}
	}
	else
	{
		throw "Ace Error: ConvertStringToBool() str cannot be NULL";
	}

	return false;
}

/**
* Converts a given wide string to a bool, If str is a wide string like "False", "FALSE", or
* "tRuE", then the function will return false, false, true respectively. If
* str is an integer for example; "0" will return false and any other number will return
* true.
*
* @author Katianie
* @date 9/7/2016
*
* @code
*	try
*	{
*		bool data = Ace::ConvertStringToBool("tRuE");
*		if (data == true)
*		{
*			printf("%S", "true");
*		}
*		else
*		{
*			printf("%S", "false");
*		}
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:true
*
* @param [in] str Wide String to convert.
*
* @return true if str is equal to "true" (any case combination) or if str
*			   is an integer not equal to 0; false otherwise. Throws an
*			   exception if str is NULL.
*/
bool Ace::ConvertStringToBool(wchar_t* str)
{
	wchar_t* strToUpper = L'\0';

	if (str != NULL)
	{
		strToUpper = Ace::ConvertToUpperCase(str);

		//Check to see if str is equal to "false" or "true".
		if (wcscmp(strToUpper, L"TRUE") == 0)
		{
			_aligned_free(strToUpper);
			return true;
		}
		else if (wcscmp(strToUpper, L"FALSE") == 0)
		{
			_aligned_free(strToUpper);
			return false;
		}
		else
		{
			_aligned_free(strToUpper);

			//Check to see if str is an integer.
			if (Ace::IsNumber(str) == true)
			{
				return Ace::ConvertIntToBool(_wtoi(str));
			}
			else if (wcslen(str) == 1)
			{
				//Small chance str could be equal to "T" or "F"
				if (towupper(*str) == L'T')
				{
					return true;
				}
				else if (towupper(*str) == L'F')
				{
					return false;
				}
			}
		}
	}
	else
	{
		throw "Ace Error: ConvertStringToBool() str cannot be NULL";
	}

	return false;
}

/**
* Converts a tokenized string (a string with commas separating numbers for example) to an array
* of floats.
*
* @author Katianie
* @date 6/26/2016
*
* @code
* 	try
* 	{
* 		int numItems = 0;
* 		char* strPosition = "0.0, 1.0f, 2.0";
* 		float* floatArray = Ace::ConvertStringToFloatArray(strPosition, ",", numItems);
* 		for (int currIndex = 0; currIndex < numItems; currIndex++)
* 		{
* 			printf("floatArray[%d]: %f\n", currIndex, floatArray[currIndex]);
* 		}
* 		_aligned_free(floatArray);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:floatArray[0]: 0.000000
* 		 floatArray[1]: 1.000000
* 		 floatArray[2]: 2.000000
*
* @param [in] strToConvert String to convert.
* @param [in] delimiter String that separates each number.
* @param [out] outNumItems Size of the array.
*
* @return A newly allocated array of floats. Throws an exception
* 		  if an error occurs.
*/
float* Ace::ConvertStringToFloatArray(char* strToConvert, char* delimiter, int& outNumItems)
{
	char** tokenArray = NULL;
	float* floatArray = NULL;

	if (strToConvert != NULL && delimiter != NULL)
	{
		outNumItems = 0;

		try
		{
			//Break the string up into an array.
			tokenArray = Ace::Tokenize(strToConvert, delimiter, outNumItems);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::ConvertStringToFloatArray()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
					return NULL;
				}
			}

			throw "Ace Error: ConvertStringToFloatArray() Encountered an error.";
			return NULL;
		}

		floatArray = (float*)_aligned_malloc(outNumItems * sizeof(float), BYTE_ALIGNMENT);

		//Ensure the buffer was allocated successfully.
		if (floatArray == NULL)
		{
			throw "Ace Error: ConvertStringToFloatArray() Buffer could not be allocated.";
			return NULL;
		}

		for (int currIndex = 0; currIndex < outNumItems; currIndex++)
		{
			floatArray[currIndex] = (float)atof(tokenArray[currIndex]);
			_aligned_free(tokenArray[currIndex]);
		}
		_aligned_free(tokenArray);
	}
	else
	{
		throw "Ace Error: ConvertStringToFloatArray() Parameters cannot be NULL.";
	}

	return floatArray;
}

/**
* Converts a tokenized wide string (a string with commas separating numbers for example) to an array
* of floats.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		int numItems = 0;
* 		wchar_t* strPosition = L"0.0, 1.0f, 2.0";
* 		float* floatArray = Ace::ConvertStringToFloatArray(strPosition, L",", numItems);
* 		for (int currIndex = 0; currIndex < numItems; currIndex++)
* 		{
* 			printf("floatArray[%d]: %f\n", currIndex, floatArray[currIndex]);
* 		}
* 		_aligned_free(floatArray);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:floatArray[0]: 0.000000
* 		 floatArray[1]: 1.000000
* 		 floatArray[2]: 2.000000
*
* @param [in] strToConvert String to convert.
* @param [in] delimiter String that separates each number.
* @param [out] outNumItems Size of the array.
*
* @return A newly allocated array of floats. Throws an exception
* 		  if an error occurs.
*/
float* Ace::ConvertStringToFloatArray(wchar_t* strToConvert, wchar_t* delimiter, int& outNumItems)
{
	wchar_t** tokenArray = NULL;
	float* floatArray = NULL;

	if (strToConvert != NULL && delimiter != NULL)
	{
		outNumItems = 0;

		try
		{
			//Break the string up into an array.
			tokenArray = Ace::Tokenize(strToConvert, delimiter, outNumItems);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::ConvertStringToFloatArray()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
					return NULL;
				}
			}

			throw "Ace Error: ConvertStringToFloatArray() Encountered an error.";
			return NULL;
		}

		floatArray = (float*)_aligned_malloc(outNumItems * sizeof(float), BYTE_ALIGNMENT);

		//Ensure the buffer was allocated successfully.
		if (floatArray == NULL)
		{
			throw "Ace Error: ConvertStringToFloatArray() Buffer could not be allocated.";
			return NULL;
		}

		for (int currIndex = 0; currIndex < outNumItems; currIndex++)
		{
			floatArray[currIndex] = (float)_wtof(tokenArray[currIndex]);
			_aligned_free(tokenArray[currIndex]);
		}
		_aligned_free(tokenArray);
	}
	else
	{
		throw "Ace Error: ConvertStringToFloatArray() Parameters cannot be NULL.";
	}

	return floatArray;
}

/**
* Converts a string to its hex value.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	unsigned int data = Ace::ConvertStringToHex("0xDEADBEEF");
* 	printf("Hex Value: %x\n", data);
* 	printf("Integer Value: %u\n", data);
*
* @endcode
* Output:Hex Value: deadbeef
* 		 Integer Value: 305419896.
*
* @param [in] str String to convert to hex.
*
* @return The hex value of the string.
*/
unsigned int Ace::ConvertStringToHex(char* str)
{
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> x;

	return (unsigned int)x;
}

/**
* Converts a string to its hex value.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	unsigned int data = Ace::ConvertStringToHex(L"0xDEADBEEF");
* 	printf("Hex Value: %x\n", data);
* 	printf("Integer Value: %u\n", data);
*
* @endcode
* Output:Hex Value: deadbeef
* 		 Integer Value: 305419896.
*
* @param [in] str String to convert to hex.
*
* @return The hex value of the string.
*/
unsigned int Ace::ConvertStringToHex(wchar_t* str)
{
	unsigned int x;
	std::wstringstream ss;
	ss << std::hex << str;
	ss >> x;

	return (unsigned int)x;
}

/**
* Converts a traditional string to a traditional string that is all on one line, In other words;
* this function removes any occurrence of \n from stringToConvert.
*
* @author Katianie
* @date 6/12/2016
*
* @code
* 	try
*	{
*		char* data = "Krusty\nKrab\nUn-Fair\n!";
*		char* oneLineData = Ace::ConvertStringToOneLine(data);
*		printf("%s", oneLineData);
*		_aligned_free(oneLineData);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:KrustyKrabUn-Fair!
*
* @param [in] strToConvert The string to convert to one line.
*
* @return A string with every occurrence of \n removed.
* 		  Throws an exception if strToConvert is NULL.
*/
char* Ace::ConvertStringToOneLine(char* strToConvert)
{
	char* oneLineData = NULL;

	if (strToConvert != NULL)
	{
		oneLineData = Ace::AllocateAndCopyString(strToConvert);
		Ace::ConvertStringToOneLineInPlace(oneLineData);
	}
	else
	{
		throw "Ace Error: ConvertStringToOneLine() strToConvert cannot be NULL";
	}

	return oneLineData;
}

/**
* Converts a traditional wide string to a traditional wide string that is all on one line, In other words;
* this function removes any occurrence of \n from stringToConvert.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
*	{
*		wchar_t* data = L"Krusty\nKrab\nUn-Fair\n!";
*		wchar_t* oneLineData = Ace::ConvertStringToOneLine(data);
*		printf("%S", oneLineData);
*		_aligned_free(oneLineData);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:KrustyKrabUn-Fair!
*
* @param [in] strToConvert The wide string to convert to one line.
*
* @return A wide string with every occurrence of \n removed.
* 		  Throws an exception if strToConvert is NULL.
*/
wchar_t* Ace::ConvertStringToOneLine(wchar_t* strToConvert)
{
	wchar_t* oneLineData = NULL;

	if (strToConvert != NULL)
	{
		oneLineData = Ace::AllocateAndCopyString(strToConvert);
		Ace::ConvertStringToOneLineInPlace(oneLineData);
	}
	else
	{
		throw "Ace Error: ConvertStringToOneLine() strToConvert cannot be NULL";
	}

	return oneLineData;
}

/**
* Converts a traditional string to a traditional string that is all on one line, In other words;
* this function removes any occurrence of \n from stringToConvert.
* strToConvert must be an allocated string since this function works directly on it.
*
* @author Katianie
* @date 6/12/2016
*
* @code
* 	try
*	{
*		char* oneLineData = Ace::AllocateAndCopyString("Krusty\nKrab\nUn-Fair\n!");
*		Ace::ConvertStringToOneLineInPlace(oneLineData);
*		printf("%s", oneLineData);
*		_aligned_free(oneLineData);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:KrustyKrabUn-Fair!
*
* @param [in,out] strToConvert The string to directly convert to one line.
* 				  Throws an exception if strToConvert is NULL.
*/
void Ace::ConvertStringToOneLineInPlace(char* strToConvert)
{
	if (strToConvert != NULL)
	{
		Ace::RemoveCharactersFromStringInPlace(strToConvert, '\n');
	}
	else
	{
		throw "Ace Error: ConvertStringToOneLineInPlace() strToConvert cannot be NULL.";
	}
}

/**
* Converts a traditional wide string to a traditional wide string that is all on one line, In other words;
* this function removes any occurrence of \n from stringToConvert.
* strToConvert must be an allocated wide string since this function works directly on it.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
*	{
*		wchar_t* oneLineData = Ace::AllocateAndCopyString(L"Krusty\nKrab\nUn-Fair\n!");
*		Ace::ConvertStringToOneLineInPlace(oneLineData);
*		printf("%S", oneLineData);
*		_aligned_free(oneLineData);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:KrustyKrabUn-Fair!
*
* @param [in,out] strToConvert The wide string to directly convert to one line.
* 				  Throws an exception if strToConvert is NULL.
*/
void Ace::ConvertStringToOneLineInPlace(wchar_t* strToConvert)
{
	if (strToConvert != NULL)
	{
		Ace::RemoveCharactersFromStringInPlace(strToConvert, L'\n');
	}
	else
	{
		throw "Ace Error: ConvertStringToOneLineInPlace() strToConvert cannot be NULL.";
	}
}

/**
* Converts a traditional string to a wide string. A wide string is a string containing Unicode
* characters (non English).
*
* @author Katianie
* @date 6/25/2016
*
* @code
* 	try
* 	{
* 		char* normalString = "The ceiling is right Squidward.";
* 		wchar_t* wideString = Ace::ConvertStringToWCharString(normalString);
* 		printf("Wide string: %S\n", wideString);
* 		_aligned_free(wideString);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Wide string: The ceiling is right Squidward.
*
* @param [in] strToConvert The string to convert to a wide string.
*
* @return The string converted to wide character string.
* 		  Throws an exception if an error occurs.
*/
wchar_t* Ace::ConvertStringToWCharString(const char* strToConvert)
{
	size_t buffSize = 0;
	size_t convertedChars = 0;
	wchar_t* wcstring = NULL;

	if (strToConvert != NULL)
	{
		//Calculate the size of the wide string buffer.
		buffSize = (strlen(strToConvert) + 1) * sizeof(wchar_t);

		//Allocate aligned buffer to hold the wide string.
		wcstring = (wchar_t*)Ace::_Aligned_Calloc(buffSize);

		//Do the actual conversion of char* to wchar_t*.
		if (mbstowcs_s(&convertedChars, wcstring, strlen(strToConvert) + 1, strToConvert, _TRUNCATE) != 0)
		{
			throw "Ace Error: ConvertStringToWCharString() Could not convert string to wide string.";
			return NULL;
		}
	}
	else
	{
		throw "Ace Error: ConvertStringToWCharString() strToConvert cannot be NULL";
	}

	return wcstring;
}

/**
* Converts a string vector array to a traditional string array.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		vector<char*> strVectorArray;
* 		strVectorArray.push_back("My");
* 		strVectorArray.push_back("name");
* 		strVectorArray.push_back("is");
* 		strVectorArray.push_back("Eddie.");
* 		int numStrs = strVectorArray.size();
* 		char** strArray = Ace::ConvertStringVectorArrayToStringArray(strVectorArray, numStrs);
* 		for (int currIndex = 0; currIndex < numStrs; currIndex++)
* 		{
* 			printf("Array[%d]:%s\n", currIndex, strArray[currIndex]);
* 		 	_aligned_free(strArray[currIndex]);
*		}
*		_aligned_free(strArray);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:Array[0]:My
* 		 Array[1]:name
* 		 Array[2]:is
* 		 Array[3]:Eddie.
*
* @param [in] stringVectorArray The string vector array to convert.
*
* @return A newly created traditional string array.
* 		  Throws an exception if error occurs.
*/
char** Ace::ConvertStringVectorArrayToStringArray(vector<char*> stringVectorArray, int& outStringArrayLength)
{
	int stringVectorArrayLen = stringVectorArray.size();
	int stringArrayIndex = 0;
	char** stringArray = NULL;

	if (stringVectorArrayLen > 0 && stringVectorArrayLen < MAX_STRING_BUFFER_SIZE)
	{
		//Allocate a traditional string array to hold our strings.
		stringArray = (char**)_aligned_malloc(stringVectorArrayLen * sizeof(char*), BYTE_ALIGNMENT);

		//Ensure the buffer was allocated successfully.
		if (stringArray == NULL)
		{
			throw "Ace Error: ConvertStringVectorArrayToStringArray() Buffer could not be allocated.";
			return NULL;
		}

		//Size of array will shrink if the vector array has NULL(s).
		outStringArrayLength = 0;
		for (int currIndex = 0; currIndex < stringVectorArrayLen; currIndex++)
		{
			if (stringVectorArray[currIndex] != NULL)
			{
				//Copy the string from the vector array to the traditional char array.
				stringArray[outStringArrayLength++] = Ace::AllocateAndCopyString(stringVectorArray[currIndex]);
			}
		}
	}
	else
	{
		throw "Ace Error: ConvertStringVectorArrayToStringArray() stringVectorArray's length is not in valid range.";
	}

	return stringArray;
}

/**
* Converts a wide string vector array to a traditional wide string array.
*
* @author Katianie
* @date 9/4/2016
*
* @code
* 	try
* 	{
* 		vector<wchar_t*> strVectorArray;
* 		strVectorArray.push_back(L"My");
* 		strVectorArray.push_back(L"name");
* 		strVectorArray.push_back(L"is");
* 		strVectorArray.push_back(L"Eddie.");
* 		int numStrs = strVectorArray.size();
* 		wchar_t** strArray = Ace::ConvertStringVectorArrayToStringArray(strVectorArray, numStrs);
* 		for (int currIndex = 0; currIndex < numStrs; currIndex++)
* 		{
* 			printf("Array[%d]:%S\n", currIndex, strArray[currIndex]);
* 		 	_aligned_free(strArray[currIndex]);
*		}
*		_aligned_free(strArray);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:Array[0]:My
* 		 Array[1]:name
* 		 Array[2]:is
* 		 Array[3]:Eddie.
*
* @param [in] stringVectorArray The string vector array to convert.
*
* @return A newly created traditional string array.
* 		  Throws an exception if error occurs.
*/
wchar_t** Ace::ConvertStringVectorArrayToStringArray(vector<wchar_t*> stringVectorArray, int& outStringArrayLength)
{
	int stringVectorArrayLen = stringVectorArray.size();
	wchar_t** stringArray = NULL;

	if (stringVectorArrayLen > 0 && stringVectorArrayLen < MAX_STRING_BUFFER_SIZE)
	{
		//Allocate a traditional string array to hold our strings.
		stringArray = (wchar_t**)_aligned_malloc(stringVectorArrayLen * sizeof(wchar_t*), BYTE_ALIGNMENT);

		//Ensure the buffer was allocated successfully.
		if (stringArray == NULL)
		{
			throw "Ace Error: ConvertStringVectorArrayToStringArray() Buffer could not be allocated.";
			return NULL;
		}

		//Size of array will shrink if the vector array has NULL(s).
		outStringArrayLength = 0;
		for (int currIndex = 0; currIndex < stringVectorArrayLen; currIndex++)
		{
			if (stringVectorArray[currIndex] != NULL)
			{
				//Copy the string from the vector array to the traditional char array.
				stringArray[outStringArrayLength++] = Ace::AllocateAndCopyString(stringVectorArray[currIndex]);
			}
		}
	}
	else
	{
		throw "Ace Error: ConvertStringVectorArrayToStringArray() stringVectorArray's length is not in valid range.";
	}

	return stringArray;
}

/**
* Converts a character to lowercase.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	char retVal = Ace::ConvertToLowerCase('A');
*	printf("%c", retVal);
*
* @endcode
* Output:a
*
* @param [in] character The character to convert to lowercase.
*
* @return The converted character.
*/
char Ace::ConvertToLowerCase(char character)
{
	return tolower(character);
}

/**
* Converts a wide character to lowercase.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	wchar_t retVal = Ace::ConvertToLowerCase(L'A');
*	printf("%c", retVal);
*
* @endcode
* Output:a
*
* @param [in] character The wide character to convert to lowercase.
*
* @return The converted character.
*/
wchar_t Ace::ConvertToLowerCase(wchar_t character)
{
	return towlower(character);
}

/**
* Converts a character to uppercase.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	char retVal = Ace::ConvertToUpperCase('a');
*	printf("%c", retVal);
*
* @endcode
* Output:A
*
* @param [in] character The character to convert to uppercase.
*
* @return The converted character.
*/
char Ace::ConvertToUpperCase(char character)
{
	return toupper(character);
}

/**
* Converts a wide character to uppercase.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	wchar_t retVal = Ace::ConvertToUpperCase('a');
*	printf("%c", retVal);
*
* @endcode
* Output:A
*
* @param [in] character The wide character to convert to uppercase.
*
* @return The converted character.
*/
wchar_t Ace::ConvertToUpperCase(wchar_t character)
{
	return towupper(character);
}

/**
* Converts a string to all lowercase letters.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	try
*	{
*		char* data = Ace::ConvertToLowerCase("ABCDEF");
*		printf("%s\n", data);
*		_aligned_free(data);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:abcdef
*
* @param [in] str The string to determine if true or false.
*
* @return true or false based on the value of str.
* 		  Throws an exception if str is NULL.
*/
char* Ace::ConvertToLowerCase(char* str)
{
	char* buffer = NULL;

	if (str != NULL)
	{
		buffer = Ace::AllocateAndCopyString(str);
		Ace::ConvertToLowerCaseInPlace(buffer);
	}
	else
	{
		throw "Ace Error: ConvertToLowerCase() str cannot be NULL";
	}

	return buffer;
}

/**
* Converts a wide string to all lowercase letters.
*
* @author Katianie
* @date 9/7/2016
*
* @code
*	try
*	{
*		wchar_t* data = Ace::ConvertToLowerCase(L"ABCDEF");
*		printf("%S\n", data);
*		_aligned_free(data);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:abcdef
*
* @param [in] str The string to determine if true or false.
*
* @return true or false based on the value of str.
* 		  Throws an exception if str is NULL.
*/
wchar_t* Ace::ConvertToLowerCase(wchar_t* str)
{
	wchar_t* buffer = NULL;

	if (str != NULL)
	{
		buffer = Ace::AllocateAndCopyString(str);
		Ace::ConvertToLowerCaseInPlace(buffer);
	}
	else
	{
		throw "Ace Error: ConvertToLowerCase() str cannot be NULL";
	}

	return buffer;
}

/**
* Directly modifies str to have all its letters converted to lower case.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	try
*	{
*		char* data = Ace::AllocateAndCopyString("ABCDEF");
*		Ace::ConvertToLowerCaseInPlace(data);
*		printf("%s", data);
*		_aligned_free(data);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:abcdef
*
* @param [in,out] str The string to determine if true or false.
* 				  Throws an exception if str is NULL.
*/
void Ace::ConvertToLowerCaseInPlace(char* str)
{
	char* strToLower = '\0';

	if (str != NULL)
	{
		while (*str != NULL)
		{
			*str = tolower(*str);
			str++;
		}
	}
	else
	{
		throw "Ace Error: ConvertToLowerCaseInPlace() str cannot be NULL.";
	}
}

/**
* Directly modifies str to have all its letters converted to lower case.
*
* @author Katianie
* @date 9/7/2016
*
* @code
*	try
*	{
*		wchar_t* data = Ace::AllocateAndCopyString(L"ABCDEF");
*		Ace::ConvertToLowerCaseInPlace(data);
*		printf("%S", data);
*		_aligned_free(data);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:abcdef
*
* @param [in,out] str The wide string to determine if true or false.
* 				  Throws an exception if str is NULL.
*/
void Ace::ConvertToLowerCaseInPlace(wchar_t* str)
{
	wchar_t* strToLower = L'\0';

	if (str != NULL)
	{
		while (*str != NULL)
		{
			*str = towlower(*str);
			str++;
		}
	}
	else
	{
		throw "Ace Error: ConvertToLowerCaseInPlace() str cannot be NULL.";
	}
}

/**
* Converts a string to all uppercase letters.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	try
*	{
*		char* data = Ace::ConvertToUpperCase("abcdef");
*		printf("%s\n", data);
*		_aligned_free(data);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:ABCDEF
*
* @param [in] str The string to determine if true or false.
*
* @return A newly created string containing all the letters in str in uppercase.
* 		  Throws an exception if str is NULL.
*/
char* Ace::ConvertToUpperCase(char* str)
{
	char* buffer = NULL;

	if (str != NULL)
	{
		buffer = Ace::AllocateAndCopyString(str);
		Ace::ConvertToUpperCaseInPlace(buffer);
	}
	else
	{
		throw "Ace Error: ConvertToUpperCase() str cannot be NULL.";
	}

	return buffer;
}

/**
* Converts a string to all uppercase letters.
*
* @author Katianie
* @date 9/5/2016
*
* @code
*	try
*	{
*		wchar_t* data = Ace::ConvertToUpperCase(L"abcdef");
*		printf("%S\n", data);
*		_aligned_free(data);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:ABCDEF
*
* @param [in] str The string to determine if true or false.
*
* @return A newly created string containing all the letters in str in uppercase.
* 		  Throws an exception if str is NULL.
*/
wchar_t* Ace::ConvertToUpperCase(wchar_t* str)
{
	wchar_t* buffer = NULL;

	if (str != NULL)
	{
		buffer = Ace::AllocateAndCopyString(str);
		Ace::ConvertToUpperCaseInPlace(buffer);
	}
	else
	{
		throw "Ace Error: ConvertToUpperCase() str cannot be NULL";
	}

	return buffer;
}

/**
* Directly modifies str to have all its letters converted to upper case.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	try
*	{
*		char* data = Ace::AllocateAndCopyString("abcdef");
*		Ace::ConvertToUpperCaseInPlace(data);
*		printf("%s", data);
*		_aligned_free(data);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:ABCDEF
*
* @param [in] str The string to convert to all uppercase letters.
* 			  Throws an exception if str is NULL.
*/
void Ace::ConvertToUpperCaseInPlace(char* str)
{
	char* strToUpper = '\0';

	if (str != NULL)
	{
		while (*str != NULL)
		{
			*str = toupper(*str);
			str++;
		}
	}
	else
	{
		throw "Ace Error: ConvertToUpperCaseInPlace() str cannot be NULL";
	}
}

/**
* Directly modifies str to have all its letters converted to upper case.
*
* @author Katianie
* @date 9/7/2016
*
* @code
*	try
*	{
*		wchar_t* data = Ace::AllocateAndCopyString(L"abcdef");
*		Ace::ConvertToUpperCaseInPlace(data);
*		printf("%S", data);
*		_aligned_free(data);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:ABCDEF
*
* @param [in] str The string to convert to all uppercase letters.
* 			  Throws an exception if str is NULL.
*/
void Ace::ConvertToUpperCaseInPlace(wchar_t* str)
{
	wchar_t* strToUpper = L'\0';

	if (str != NULL)
	{
		while (*str != NULL)
		{
			*str = towupper(*str);
			str++;
		}
	}
	else
	{
		throw "Ace Error: ConvertToUpperCaseInPlace() str cannot be NULL";
	}
}

/**
* Calculates the Cosine (cos) of numInRadians in compliance
* with CERT Coding Standard.
*
* @author Katianie
* @date 10/8/2016
*
* @code
*	try
*	{
*		const double PI = 3.14159265358979;
*		double retVal = Ace::Cos(PI / 4.0);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.707107
*
* @param numInRadians The number in radians to take the cosine of.
*
* @return The cosine of the numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
double Ace::Cos(double numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = cos(numInRadians);

	//Look for any mathematical errors as a result of calling cos().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Cos() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Cos() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the Cosine (cos) of numInRadians in compliance
* with CERT Coding Standard.
*
* @author Katianie
* @date 10/8/2016
*
* @code
*	try
*	{
*		const float PI = 3.14159265f;
*		float retVal = Ace::Cos(PI / 4.0f);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.707107
*
* @param numInRadians The number in radians to take the cosine of.
*
* @return The cosine of the numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
float Ace::Cos(float numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = cos(numInRadians);

	//Look for any mathematical errors as a result of calling cos().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Cos() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Cos() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the Cosine (cos) of numInRadians in compliance
* with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		const float PI = 3.14159265f;
*		int retVal = Ace::Cos((int)(PI / 4));
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:1
*
* @param numInRadians The number in radians to take the cosine of.
*
* @return The cosine of the numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
int Ace::Cos(int numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = cos(numInRadians);

	//Look for any mathematical errors as a result of calling cos().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Cos() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Cos() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates the CosH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
* 	try
* 	{
* 		const double PI = 3.14159265;
* 		double result = Ace::CosH(PI / 4.0);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:1.324609
*
* @param numInRadians Number in radians to calculate CosH for.
*
* @return The CosH of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
double Ace::CosH(double numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = cosh(numInRadians);

	//Look for any mathematical errors as a result of calling cosh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: CosH() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: CosH() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the CosH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.14159265f;
* 		float result = Ace::CosH(PI / 4.0f);
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:1.324609
*
* @param numInRadians Number in radians to calculate CosH for.
*
* @return The CosH of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
float Ace::CosH(float numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = cosh(numInRadians);

	//Look for any mathematical errors as a result of calling cosh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: CosH() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: CosH() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the CosH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.14159265f;
* 		int result = Ace::CosH((int)(PI / 4));
* 		printf("%f\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:1
*
* @param numInRadians Number in radians to calculate CosH for.
*
* @return The CosH of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
int Ace::CosH(int numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = cosh(numInRadians);

	//Look for any mathematical errors as a result of calling cosh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: CosH() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: CosH() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Counts the number of times charToLookFor appears in strToSearch.
*
* @author Katianie
* @date 6/11/2016
*
* @code
* 	try
* 	{
* 		char* data = "Has anyone ever been as far as decided to be there and then look more like?";
* 		int numOccurences = Ace::CountNumOccurrences(data, 'e');
* 		printf("Number of occurrences of %c: %d\n", 'e', numOccurences);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Number of occurrences of e: 13
*
* @param [in] strToSearch A string containing the data to be searched.
* @param [in] charToLookFor A string character to look for.
*
* @return The number of times charToLookFor occurred in strToSearch.
* 		  Throws an exception if strToSearch is NULL.
*/
int Ace::CountNumOccurrences(char* strToSearch, char charToLookFor)
{
	int count = 0;

	if (strToSearch != NULL)
	{
		for (size_t currIndex = 0; currIndex < strlen(strToSearch); currIndex++)
		{
			if (strToSearch[currIndex] == charToLookFor)
			{
				count++;
			}
		}
	}
	else
	{
		throw "Ace Error: CountNumOccurrences() strToSearch cannot be NULL.";
	}

	return count;
}

/**
* Counts the number of times charToLookFor appears in strToSearch.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = L"Has anyone ever been as far as decided to be there and then look more like?";
* 		int numOccurences = Ace::CountNumOccurrences(data, L'e');
* 		printf("Number of occurrences of %c: %d\n", L'e', numOccurences);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Number of occurrences of e: 13
*
* @param [in] strToSearch A wide string containing the data to be searched.
* @param [in] charToLookFor A wide character to look for.
*
* @return The number of times charToLookFor occurred in strToSearch.
* 		  Throws an exception if strToSearch is NULL.
*/
int Ace::CountNumOccurrences(wchar_t* strToSearch, wchar_t charToLookFor)
{
	int count = 0;

	if (strToSearch != NULL)
	{
		for (size_t currIndex = 0; currIndex < wcslen(strToSearch); currIndex++)
		{
			if (strToSearch[currIndex] == charToLookFor)
			{
				count++;
			}
		}
	}
	else
	{
		throw "Ace Error: CountNumOccurrences() strToSearch cannot be NULL.";
	}

	return count;
}

/**
* Counts the number of times strToLookFor appears in strToSearch. Assumes
* itemToLookFor is always smaller than word.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = "Has anyone ever been as far as decided to be there and then look more like?";
* 		int numOccurences = Ace::CountNumOccurrences(data, "as");
* 		printf("Number of occurrences of %s: %d", "as", numOccurences);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Number of occurrences of as: 3
*
* @param [in] strToSearch A string containing the data to be searched.
* @param [in] strToLookFor A string representing the substring to look for.
*
* @return The number of times strToLookFor occurred in strToSearch.
* 		  Throws an exception if strToSearch or strToLookFor is NULL.
*/
int Ace::CountNumOccurrences(char* strToSearch, char* strToLookFor)
{
	int num = 0;
	int strToLookForLen = 0;
	int strToSearchLen = 0;
	char* currSubstring = "\0";

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling strlen().
		strToLookForLen = strlen(strToLookFor);
		strToSearchLen = strlen(strToSearch);

		//The item to look for must be smaller then the word.
		if (strToLookForLen <= strToSearchLen)
		{
			try
			{
				//Search the entire string for an occurrence of strToLookFor.
				for (int currIndex = 0; currIndex <= (strToSearchLen - strToLookForLen); currIndex++)
				{
					currSubstring = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);

					//Check to see if the strings are equal.
					if (strcmp(currSubstring, strToLookFor) == 0)
					{
						//Occurrence found.
						num++;
					}

					//Cleanup the currSubstring since SubString() allocates memory.
					_aligned_free(currSubstring);
				}
			}
			catch (char* exceptionMessage)
			{
				//Create a stack trace of the exception.
				char combinedMessage[1024];
				if (SecureZeroMemory(combinedMessage, 1024) != NULL)
				{
					if (sprintf_s(combinedMessage, 1024, "Ace::CountNumOccurrences()->%s\n", exceptionMessage) > 0)
					{
						throw combinedMessage;
						return -1;
					}
				}

				throw "Ace Error: CountNumOccurrences() Encountered an error.";
				return -1;
			}
		}
		else
		{
			throw "Ace Error: CountNumOccurrences() strToLookFor must be smaller then strToSearch.";
		}
	}
	else
	{
		throw "Ace Error: CountNumOccurrences() Parameters cannot be NULL.";
	}

	return num;
}

/**
* Counts the number of times strToLookFor appears in strToSearch. Assumes
* itemToLookFor is always smaller than word.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = L"Has anyone ever been as far as decided to be there and then look more like?";
* 		int numOccurences = Ace::CountNumOccurrences(data, L"as");
* 		printf("Number of occurrences of %S: %d", L"as", numOccurences);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Number of occurrences of as: 3
*
* @param [in] strToSearch A wide string containing the data to be searched.
* @param [in] strToLookFor A wide string representing the substring to look for.
*
* @return The number of times strToLookFor occurred in strToSearch.
* 		  Throws an exception if strToSearch or strToLookFor is NULL.
*/
int Ace::CountNumOccurrences(wchar_t* strToSearch, wchar_t* strToLookFor)
{
	int num = 0;
	int strToLookForLen = 0;
	int strToSearchLen = 0;
	wchar_t* currSubstring = L"\0";

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling wcslen().
		strToLookForLen = wcslen(strToLookFor);
		strToSearchLen = wcslen(strToSearch);

		//The item to look for must be smaller then the word.
		if (strToLookForLen <= strToSearchLen)
		{
			try
			{
				//Search the entire string for an occurrence of strToLookFor.
				for (int currIndex = 0; currIndex <= (strToSearchLen - strToLookForLen); currIndex++)
				{
					currSubstring = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);

					//Check to see if the strings are equal,
					if (wcscmp(currSubstring, strToLookFor) == 0)
					{
						//Occurrence found.
						num++;
					}

					//Cleanup the currSubstring since SubString() allocates memory.
					_aligned_free(currSubstring);
				}
			}
			catch (char* exceptionMessage)
			{
				//Create a stack trace of the exception.
				char combinedMessage[1024];
				if (SecureZeroMemory(combinedMessage, 1024) != NULL)
				{
					if (sprintf_s(combinedMessage, 1024, "Ace::CountNumOccurrences()->%s\n", exceptionMessage) > 0)
					{
						throw combinedMessage;
						return -1;
					}
				}

				throw "Ace Error: CountNumOccurrences() Encountered an error.";
				return -1;
			}
		}
		else
		{
			throw "Ace Error: CountNumOccurrences() strToLookFor must be smaller then strToSearch.";
		}
	}
	else
	{
		throw "Ace Error: CountNumOccurrences() Parameters cannot be NULL.";
	}

	return num;
}

/**
* Calculates the Cube Root in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/8/2016
*
* @code
*	try
*	{
*		double retVal = Ace::CubeRoot(1728.0);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:12.000000
*
* @param num The number to take the Cube Root of.
*
* @return The Cube Root of num. If a mathematical error occurs
* 		  then an exception is thrown.
*/
double Ace::CubeRoot(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = cbrt(num);

	//Look for any mathematical errors as a result of calling cbrt().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: CubeRoot() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: CubeRoot() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the Cube Root in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/8/2016
*
* @code
*	try
*	{
*		float retVal = Ace::CubeRoot(1728.0f);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:12.000000
*
* @param num The number to take the Cube Root of.
*
* @return The Cube Root of num. If a mathematical error occurs
* 		  then an exception is thrown.
*/
float Ace::CubeRoot(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = cbrt(num);

	//Look for any mathematical errors as a result of calling cbrt().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: CubeRoot() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: CubeRoot() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the Cube Root in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int retVal = Ace::CubeRoot(1728);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:12
*
* @param num The number to take the Cube Root of.
*
* @return The Cube Root of num. If a mathematical error occurs
* 		  then an exception is thrown.
*/
int Ace::CubeRoot(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = cbrt(num);

	//Look for any mathematical errors as a result of calling cbrt().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: CubeRoot() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: CubeRoot() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Converts a number from degrees to radians.
*
* @author Katianie
* @date 5/16/2016
*
* @code
* 	double deg = 90.0;
* 	double rad = Ace::DegreesToRadians(deg);
* 	printf("%f\n", rad);
*
* @endcode
* Output:1.570796
*
* @param degrees Degrees to convert.
*
* @return degrees converted to radians.
*/
double Ace::DegreesToRadians(double degrees)
{
	return (degrees * PI) / 180.0;
}

/**
* Converts a number from degrees to radians.
*
* @author Katianie
* @date 10/6/2016
*
* @code
* 	float deg = 90.0f;
* 	float rad = Ace::DegreesToRadians(deg);
* 	printf("%f\n", rad);
*
* @endcode
* Output:1.570796
*
* @param degrees Degrees to convert.
*
* @return degrees converted to radians.
*/
float Ace::DegreesToRadians(float degrees)
{
	return (degrees * PI) / 180.0f;
}

/**
* Converts a number from degrees to radians. This function
* is somewhat useless but provided for completeness.
*
* @author Katianie
* @date 10/20/2016
*
* @code
* 	int deg = 90;
* 	int rad = Ace::DegreesToRadians(deg);
* 	printf("%d\n", rad);
*
* @endcode
* Output:1
*
* @param [in] degrees Degrees to convert.
*
* @return Radians.
*/
int Ace::DegreesToRadians(int degrees)
{
	return (degrees * PI) / 180;
}

/**
* Returns true if the specified file exists, false otherwise.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		if (Ace::DoesFileExist("C:\\Images\\dickbutt.png") == true)
* 		{
* 			printf("%s\n", "File exists.");
* 		}
* 		else
* 		{
* 			printf("%s\n", "File does NOT exist.");
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:File does NOT exist.
*
* @param fullFilePathWithNameAndExt File to check.
*
* @return Returns true if the specified file exists, false otherwise.
*/
bool Ace::DoesFileExist(char* fullFilePathWithNameAndExt)
{
	if (fullFilePathWithNameAndExt != NULL)
	{
		struct stat buffer;
		return (stat(fullFilePathWithNameAndExt, &buffer) == 0);
	}
	else
	{
		throw "Ace Error: DoesFileExist() fullFilePathWithNameAndExt cannot be NULL.";
		return false;
	}
}

/**
* Returns true if the specified file exists, false otherwise.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		if (Ace::DoesFileExist(L"C:\\Images\\dickbutt.png") == true)
* 		{
* 			printf("%S\n", L"File exists.");
* 		}
* 		else
* 		{
* 			printf("%S\n", L"File does NOT exist.");
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:File does NOT exist.
*
* @param [in] fullFilePathWithNameAndExt File to check.
*
* @return Returns true if the specified file exists, false otherwise.
*/
bool Ace::DoesFileExist(wchar_t* fullFilePathWithNameAndExt)
{
	if (fullFilePathWithNameAndExt != NULL)
	{
		struct _stat buffer;
		return (_wstat(fullFilePathWithNameAndExt, &buffer) == 0);
	}
	else
	{
		throw "Ace Error: DoesFileExist() fullFilePathWithNameAndExt cannot be NULL.";
		return false;
	}
}

/**
* Calculates the Error Function integral from 0 to
* num in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		double result = Ace::ErrorFunction(1.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.842701
*
* @param num End point for the integral.
*
* @return The result of the Error Function from 0 to num. If a
* 		  mathematical error occurs then an exception is thrown.
*/
double Ace::ErrorFunction(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = erf(num);

	//Look for any mathematical errors as a result of calling erf().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ErrorFunction() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ErrorFunction() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the Error Function integral from 0 to
* num in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float result = Ace::ErrorFunction(1.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.842701
*
* @param num End point for the integral.
*
* @return The result of the Error Function from 0 to num. If a
* 		  mathematical error occurs then an exception is thrown.
*/
float Ace::ErrorFunction(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = erf(num);

	//Look for any mathematical errors as a result of calling erf().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ErrorFunction() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ErrorFunction() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the Error Function integral from 0 to
* num in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int result = Ace::ErrorFunction(5);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0
*
* @param num End point for the integral.
*
* @return The result of the Error Function from 0 to num. If a
* 		  mathematical error occurs then an exception is thrown.
*/
int Ace::ErrorFunction(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = erf(num);

	//Look for any mathematical errors as a result of calling erf().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: ErrorFunction() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: ErrorFunction() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Compares two traditional strings together.
*
* @author Katianie
* @date 5/16/2016
*
* @code
* 	char* strOne = "Eddie";
* 	char* strTwo = "Katianie";
* 	if (Ace::Equals(strOne, strTwo) == true)
* 	{
* 		printf("%s\n", "Strings are equal.");
* 	}
* 	else
* 	{
* 		printf("%s\n", "Strings are NOT equal.");
* 	}
*
* @endcode
* Output:Strings are NOT equal.
*
* @param [in] strToCompOne First string to compare.
* @param [in] strToCompTwo Second string to compare.
*
* @return true if the strings are equal, false otherwise.
*/
bool Ace::Equals(char* strToCompOne, char* strToCompTwo)
{
	if (strToCompOne != NULL && strToCompTwo != NULL)
	{
		return strcmp(strToCompOne, strToCompTwo) == 0;
	}

	return false;
}

/**
* Compares two traditional wide strings together.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	wchar_t* strOne = L"Eddie";
* 	wchar_t* strTwo = L"Katianie";
* 	if (Ace::Equals(strOne, strTwo) == true)
* 	{
* 		printf("%S\n", L"Strings are equal.");
* 	}
* 	else
* 	{
* 		printf("%S\n", L"Strings are NOT equal.");
* 	}
*
* @endcode
* Output:Strings are NOT equal.
*
* @param [in] strToCompOne First wide string to compare.
* @param [in] strToCompTwo Second wide string to compare.
*
* @return true if the wide strings are equal, false otherwise.
*/
bool Ace::Equals(wchar_t* strToCompOne, wchar_t* strToCompTwo)
{
	if (strToCompOne != NULL && strToCompTwo != NULL)
	{
		return wcscmp(strToCompOne, strToCompTwo) == 0;
	}

	return false;
}

/**
* Compares two traditional strings together but ignores upper/lower case.
*
* @author Katianie
* @date 5/16/2016
*
* @code
* 	char* strOne = "Katianie";
* 	char* strTwo = "kATIaNiE";
* 	if (Ace::EqualsIgnoreCase(strOne, strTwo) == true)
* 	{
* 		printf("%s\n", "Strings are equal.");
* 	}
* 	else
* 	{
* 		printf("%s\n", "Strings are NOT equal.");
* 	}
*
* @endcode
* Output:Strings are equal.
*
* @param [in] strToCompOne First string to compare.
* @param [in] strToCompTwo Second string to compare.
*
* @return true if the strings are equal, false otherwise.
*/
bool Ace::EqualsIgnoreCase(char* strToCompOne, char* strToCompTwo)
{
	char* normStrOne;
	char* normStrTwo;
	bool result = false;

	if (strToCompOne != NULL && strToCompTwo != NULL)
	{
		//Normalize the strings so all characters are the same case.
		normStrOne = Ace::ConvertToUpperCase(strToCompOne);
		normStrTwo = Ace::ConvertToUpperCase(strToCompTwo);

		result = Ace::Equals(normStrOne, normStrTwo);

		_aligned_free(normStrOne);
		_aligned_free(normStrTwo);
	}

	return result;
}

/**
* Compares two traditional wide strings together but ignores upper/lower case.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	wchar_t* strOne = L"Katianie";
* 	wchar_t* strTwo = L"kATIaNiE";
* 	if (Ace::EqualsIgnoreCase(strOne, strTwo) == true)
* 	{
* 		printf("%S\n", L"Strings are equal.");
* 	}
* 	else
* 	{
* 		printf("%S\n", L"Strings are NOT equal.");
* 	}
*
* @endcode
* Output:Strings are equal.
*
* @param [in] strToCompOne First wide string to compare.
* @param [in] strToCompTwo Second wide string to compare.
*
* @return true if the strings are equal, false otherwise.
*/
bool Ace::EqualsIgnoreCase(wchar_t* strToCompOne, wchar_t* strToCompTwo)
{
	wchar_t* normStrOne;
	wchar_t* normStrTwo;
	bool result = false;

	if (strToCompOne != NULL && strToCompTwo != NULL)
	{
		//Normalize the strings so all characters are the same case.
		normStrOne = Ace::ConvertToUpperCase(strToCompOne);
		normStrTwo = Ace::ConvertToUpperCase(strToCompTwo);

		result = Ace::Equals(normStrOne, normStrTwo);

		_aligned_free(normStrOne);
		_aligned_free(normStrTwo);
	}

	return result;
}

/**
* Calculates e^x in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		double result = Ace::EToTheXPower(5.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:148.413159
*
* @param power Number to raise e to (i.e. power = x in e^x).
*
* @return The result of raising e to power. If a mathematical
* 		  error occurs then an exception is thrown.
*/
double Ace::EToTheXPower(double power)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = exp(power);

	//Look for any mathematical errors as a result of calling exp().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: EToTheXPower() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: EToTheXPower() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates e^x in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float result = Ace::EToTheXPower(5.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:148.413159
*
* @param power Number to raise e to (i.e. power = x in e^x).
*
* @return The result of raising e to power. If a mathematical
* 		  error occurs then an exception is thrown.
*/
float Ace::EToTheXPower(float power)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = exp(power);

	//Look for any mathematical errors as a result of calling exp().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: EToTheXPower() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: EToTheXPower() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates e^x in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int result = Ace::EToTheXPower(5);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:148
*
* @param power Number to raise e to (i.e. power = x in e^x).
*
* @return The result of raising e to power. If a mathematical
* 		  error occurs then an exception is thrown.
*/
int Ace::EToTheXPower(int power)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = exp(power);

	//Look for any mathematical errors as a result of calling exp().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: EToTheXPower() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: EToTheXPower() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates (e^x) - 1 in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		double result = Ace::EToTheXPowerMinusOne(5.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:147.413159
*
* @param power Number to raise e to (i.e. power = x in e^x).
*
* @return The result of raising e to power then subtracting 1.
* 		  If a mathematical error occurs then an exception is thrown.
*/
double Ace::EToTheXPowerMinusOne(double power)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = expm1(power);

	//Look for any mathematical errors as a result of calling expm1().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: EToTheXPowerMinusOne() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: EToTheXPowerMinusOne() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates (e^x) - 1 in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float result = Ace::EToTheXPowerMinusOne(5.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:147.413159
*
* @param power Number to raise e to (i.e. power = x in e^x).
*
* @return The result of raising e to power then subtracting 1.
* 		  If a mathematical error occurs then an exception is thrown.
*/
float Ace::EToTheXPowerMinusOne(float power)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = expm1(power);

	//Look for any mathematical errors as a result of calling expm1().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: EToTheXPowerMinusOne() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: EToTheXPowerMinusOne() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates (e^x) - 1 in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int result = Ace::EToTheXPowerMinusOne(5);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:147
*
* @param power Number to raise e to (i.e. power = x in e^x).
*
* @return The result of raising e to power then subtracting 1.
* 		  If a mathematical error occurs then an exception is thrown.
*/
int Ace::EToTheXPowerMinusOne(int power)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = expm1(power);

	//Look for any mathematical errors as a result of calling expm1().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: EToTheXPowerMinusOne() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: EToTheXPowerMinusOne() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Returns the first character in str.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = "Has anyone ever been as far as decided to be there and then look more like?";
* 		char temp = Ace::FirstChar(data);
* 		printf("%c", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:H
*
* @param [in] str String to search.
*
* @return The first character in str. If str is NULL
* 		  then an exception is thrown.
*/
char Ace::FirstChar(char* str)
{
	if (str != NULL)
	{
		return *(str + 0);
	}
	else
	{
		throw "Ace Error: FirstChar() str cannot be NULL.";
	}

	return '\0';
}

/**
* Returns the first character in str.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = L"Has anyone ever been as far as decided to be there and then look more like?";
* 		wchar_t temp = Ace::FirstChar(data);
* 		printf("%c", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:H
*
* @param [in] str Wide String to search.
*
* @return The first character in str. If str is NULL
* 		  then an exception is thrown.
*/
wchar_t Ace::FirstChar(wchar_t* str)
{
	if (str != NULL)
	{
		return *(str + 0);
	}
	else
	{
		throw "Ace Error: FirstChar() str cannot be NULL.";
	}

	return L'\0';
}

/**
* Calculates the largest integer value not greater than num.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/8/2016
*
* @code
*	try
*	{
*		double retVal = Ace::Floor(2.9);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:2.000000
*
* @param num The number to use to calculate.
*
* @return The largest integer value not greater than num.
* 		  If a mathematical error occurs then an exception
* 		  is thrown.
*/
double Ace::Floor(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = floor(num);

	//Look for any mathematical errors as a result of calling floor().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Floor() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Floor() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the largest integer value not greater than num.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/8/2016
*
* @code
*	try
*	{
*		float retVal = Ace::Floor(2.9f);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:2.000000
*
* @param num The number to use to calculate.
*
* @return The largest integer value not greater than num.
* 		  If a mathematical error occurs then an exception
* 		  is thrown.
*/
float Ace::Floor(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = floor(num);

	//Look for any mathematical errors as a result of calling floor().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Floor() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Floor() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the largest integer value not greater than num.
* This function is somewhat useless but provided for completeness.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
**	try
*	{
*		int retVal = Ace::Floor(2);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:2
*
* @param num The number to use to calculate.
*
* @return The largest integer value not greater than num.
* 		  If a mathematical error occurs then an exception
* 		  is thrown.
*/
int Ace::Floor(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = floor(num);

	//Look for any mathematical errors as a result of calling floor().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Floor() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Floor() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates The Gamma Function ( (0)integral(Inf)[t^num-1 * e^-t dt] )
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		double result = Ace::GammaFunction(5.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:24.000000
*
* @param num Number to use for t^num-1 in the equation.
*
* @return The result of calculating The Gamma Function for t^num-1.
* 		  If a mathematical error occurs or if num is less than or
* 		  equal to 0.0, then an exception is thrown.
*/
double Ace::GammaFunction(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//is functions prevent an exception from being raised.
	if (isgreater(num, 0.0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = tgamma(num);

		//Look for any mathematical errors as a result of calling tgamma().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: GammaFunction() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: GammaFunction() Calculation resulted in a range error.";
			return -1.0;
		}
	}

	return result;
}

/**
* Calculates The Gamma Function ( (0)integral(Inf)[t^num-1 * e^-t dt] )
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float result = Ace::GammaFunction(5.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:24.000000
*
* @param num Number to use for t^num-1 in the equation.
*
* @return The result of calculating The Gamma Function for t^num-1.
* 		  If a mathematical error occurs or if num is less than or
* 		  equal to 0.0, then an exception is thrown.
*/
float Ace::GammaFunction(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//is functions prevent an exception from being raised.
	if (isgreater(num, 0.0f) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = tgamma(num);

		//Look for any mathematical errors as a result of calling tgamma().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: GammaFunction() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: GammaFunction() Calculation resulted in a range error.";
			return -1.0f;
		}
	}

	return result;
}

/**
* Calculates The Gamma Function ( (0)integral(Inf)[t^num-1 * e^-t dt] )
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int result = Ace::GammaFunction(5);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:24
*
* @param num Number to use for t^num-1 in the equation.
*
* @return The result of calculating The Gamma Function for t^num-1.
* 		  If a mathematical error occurs or if num is less than or
* 		  equal to 0, then an exception is thrown.
*/
int Ace::GammaFunction(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//is functions prevent an exception from being raised.
	if (isgreater(num, 0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = tgamma(num);

		//Look for any mathematical errors as a result of calling tgamma().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: GammaFunction() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: GammaFunction() Calculation resulted in a range error.";
			return -1;
		}
	}

	return result;
}

/**
* Some might think this is all unnecessary but I think it's important to seed the
* RandomNumberGenerator properly with a proper seed value. Too often are games plagued with
* predictable outcomes all leading back to not seeding the RNG properly. Generates a "super"
* random number using a random seed at launch.
*
* @author Katianie
* @date  5/7/2016
*
* @code
* 	int randomNum = -1;
* 	//Generate 10 random numbers.
* 	for (int currIndex = 1; currIndex <= 10; currIndex++)
* 	{
* 		randomNum = Ace::GenerateRandomNum(1, 100);
* 		printf("%d ", randomNum);
* 	}
*
* @endcode
* Output:35 82 10 52 10 34 56 21 37 14
*
* @param min Lowest number to generate.
* @param max Highest number to generate.
*
* @return A random number between the given range.
*/
int Ace::GenerateRandomNum(int min, int max)
{
	static int randSeed = -1;
	int randNum;
	int randIndex;
	int currTime;
	uniform_int_distribution<int> rngDistribution(min, max);	//Generates number in the range min to max.

	if (randSeed == -1)
	{
		//Get the current time and generate seeds based off it.
		currTime = (int)time(0);
		seed_seq seedSequence{ rand() % currTime + 1 };

		//Generate our seed numbers.
		vector<int> randSeeds(MAX_SEEDS);
		seedSequence.generate(randSeeds.begin(), randSeeds.end());

		//Pick a seed at random to use.
		srand(randSeeds.at(currTime % randSeeds.size()));
		randIndex = rand() % randSeeds.size();
		randSeed = randSeeds.at(randIndex);
	}

	//Seed the RNG with a random seed from our vector.
	//We only want to seed it once because reseeding each time just generates the same number.
	static mt19937 rngGenerator(randSeed);

	//Get a random number.
	randNum = rngDistribution(rngGenerator);

	return randNum;
}

/**
* Gets the corresponding detailed description of the HRESULT.
*
* @author Katianie
* @date  5/7/2016
*
* @code
* 	try
* 	{
* 		char* result = Ace::GetHResultDescription((HRESULT)69);
* 		printf("%s\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:The network BIOS session limit was exceeded.
*
* @param result The HRESULT description.
*
* @return The corresponding detailed description of the HRESULT.
* 		  An exception is thrown if there is an error.
*/
char* Ace::GetHResultDescription(HRESULT result)
{
	unsigned long retVal = 0;
	int descriptionSize = 1024;
	char* description = NULL;

	try
	{
		description = (char*)Ace::_Aligned_Calloc(descriptionSize);
	}
	catch (char* exceptionMessage)
	{
		//Create a stack trace of the exception.
		char combinedMessage[1024];
		if (SecureZeroMemory(combinedMessage, 1024) != NULL)
		{
			if (sprintf_s(combinedMessage, 1024, "Ace::GetHResultDescription()->%s\n", exceptionMessage) > 0)
			{
				throw combinedMessage;
				return NULL;
			}
		}

		throw "Ace Error: GetHResultDescription() Encountered an error.";
		return NULL;
	}

	retVal = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		result,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		description,
		descriptionSize,
		NULL);

	if (retVal == 0)
	{
		throw "Ace Error: GetHResultDescription() Could not get description from FormatMessage().";
		return NULL;
	}

	return description;
}

/**
* Gets the corresponding detailed description of the HRESULT.
*
* @author Katianie
* @date  5/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* result = Ace::GetHResultDescriptionW((HRESULT)69);
* 		printf("%S\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:The network BIOS session limit was exceeded.
*
* @param result The HRESULT description.
*
* @return The corresponding detailed description of the HRESULT.
* 		  Throws an exception if an error occurs.
*/
wchar_t* Ace::GetHResultDescriptionW(HRESULT result)
{
	unsigned long retVal = 0;
	int descriptionSize = 2048;
	wchar_t* description = NULL;

	try
	{
		description = (wchar_t*)Ace::_Aligned_Calloc(descriptionSize);
	}
	catch (char* exceptionMessage)
	{
		//Create a stack trace of the exception.
		char combinedMessage[1024];
		if (SecureZeroMemory(combinedMessage, 1024) != NULL)
		{
			if (sprintf_s(combinedMessage, 1024, "Ace::GetHResultDescriptionW()->%s\n", exceptionMessage) > 0)
			{
				throw combinedMessage;
				return NULL;
			}
		}

		throw "Ace Error: GetHResultDescriptionW() Encountered an error.";
		return NULL;
	}

	retVal = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		result,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		description,
		descriptionSize,
		NULL);

	if (retVal == 0)
	{
		throw "Ace Error: GetHResultDescriptionW() Could not get description from FormatMessage().";
		return NULL;
	}


	return description;
}

/**
* Gets the pixel dimensions of the provided image (filePath).
*
* @author Katianie
* @date  11/2/2016
*
* @code
* 	try
* 	{
* 		int width;
* 		int height;
* 		Ace::GetImageSize("C:\\Cockmongler.jpg", &width, &height);
* 		printf("Width:%d Height:%d\n", width, height);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Width:600 Height:791
*
* @param filePath [in] The full file path of the image.
* @param outWidth [out] The width of the image.
* @param outHeight [out] The height of the image
*
* @return Throws an exception if an error occurs.
*/
void Ace::GetImageSize(char* filePath, int* outWidth, int* outHeight)
{
	FILE* file = NULL;
	long len = 0;
	long pos = 2;
	unsigned char buf[24];

	if (fopen_s(&file, filePath, "rb") != 0)
	{
		throw "Ace Error: GetImageSize() Cannot open file.";
	}

	if (fseek(file, 0, SEEK_END) != 0)
	{
		fclose(file);
		throw "Ace Error: GetImageSize() fseek() failed to read file.";
	}

	len = ftell(file);
	if (len < 24)
	{
		fclose(file);
		throw "Ace Error: GetImageSize() File not large enough.";
	}

	if (fseek(file, 0, SEEK_SET) != 0)
	{
		fclose(file);
		throw "Ace Error: GetImageSize() fseek() failed to read file.";
	}

	// Strategy:
	// reading GIF dimensions requires the first 10 bytes of the file
	// reading PNG dimensions requires the first 24 bytes of the file
	// reading JPEG dimensions requires scanning through jpeg chunks
	// In all formats, the file is at least 24 bytes big, so we'll read that always
	if (fread(buf, 1, 24, file) != 24)
	{
		fclose(file);
		throw "Ace Error: GetImageSize() fread() failed to read file.";
	}

	// For JPEGs, we need to read the first 12 bytes of each chunk.
	// We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 && buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F')
	{
		while (buf[2] == 0xFF)
		{
			if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 || buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA || buf[3] == 0xCB)
			{
				fclose(file);
				break;
			}

			pos += 2 + (buf[4] << 8) + buf[5];
			if (pos + 12 > len)
			{
				fclose(file);
				break;
			}
			if (fseek(file, pos, SEEK_SET) != 0)
			{
				fclose(file);
				throw "Ace Error: GetImageSize() fseek() failed to read file.";
			}

			if (fread(buf + 2, 1, 12, file) != 12)
			{
				fclose(file);
				throw "Ace Error: GetImageSize() fread() failed to read file.";
			}
		}
	}

	//Some images contain Exif data so check for that.
	if ((buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 && buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F') ||
		(buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE1 && buf[6] == 'E' && buf[7] == 'x' && buf[8] == 'i' && buf[9] == 'f'))
	{
		while (buf[2] == 0xFF)
		{
			if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 || buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA || buf[3] == 0xCB)
			{
				fclose(file);
				break;
			}

			pos += 2 + (buf[4] << 8) + buf[5];
			if (pos + 12 > len)
			{
				fclose(file);
				break;
			}
			if (fseek(file, pos, SEEK_SET) != 0)
			{
				fclose(file);
				throw "Ace Error: GetImageSize() fseek() failed to read file.";
			}

			if (fread(buf + 2, 1, 12, file) != 12)
			{
				fclose(file);
				throw "Ace Error: GetImageSize() fread() failed to read file.";
			}
		}
	}

	fclose(file);

	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF)
	{
		//JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
		*outHeight = (buf[7] << 8) + buf[8];
		*outWidth = (buf[9] << 8) + buf[10];

		fclose(file);
	}
	else if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
	{
		//GIF: first three bytes say "GIF", next three give version number. Then dimensions
		*outWidth = buf[6] + (buf[7] << 8);
		*outHeight = buf[8] + (buf[9] << 8);

		fclose(file);
	}
	else if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A && buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
	{
		//PNG: the first frame is by definition an IHDR frame, which gives dimensions
		*outWidth = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
		*outHeight = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);

		fclose(file);
	}
	else
	{
		*outWidth = -1;
		*outHeight = -1;

		fclose(file);

		throw "Ace Error: GetImageSize() file is not an image format.";
	}
}

/**
* Gets the pixel dimensions of the provided image (filePath).
*
* @author Katianie
* @date  11/2/2016
*
* @code
* 	try
* 	{
* 		int width;
* 		int height;
* 		Ace::GetImageSizeW(L"C:\\Cockmongler.jpg", &width, &height);
* 		printf("Width:%d Height:%d\n", width, height);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Width:600 Height:791
*
* @param filePath [in] The full file path of the image.
* @param outWidth [out] The width of the image.
* @param outHeight [out] The height of the image
*
* @return Throws an exception if an error occurs.
*/
void Ace::GetImageSizeW(wchar_t* filePath, int* outWidth, int* outHeight)
{
	FILE* file = NULL;
	long len = 0;
	long pos = 2;
	unsigned char buf[24];

	if (_wfopen_s(&file, filePath, L"rb") != 0)
	{
		throw "Ace Error: GetImageSize() Cannot open file.";
	}

	if (fseek(file, 0, SEEK_END) != 0)
	{
		fclose(file);
		throw "Ace Error: GetImageSize() fseek() failed to read file.";
	}

	len = ftell(file);
	if (len < 24)
	{
		fclose(file);
		throw "Ace Error: GetImageSize() File not large enough.";
	}

	if (fseek(file, 0, SEEK_SET) != 0)
	{
		fclose(file);
		throw "Ace Error: GetImageSize() fseek() failed to read file.";
	}

	// Strategy:
	// reading GIF dimensions requires the first 10 bytes of the file
	// reading PNG dimensions requires the first 24 bytes of the file
	// reading JPEG dimensions requires scanning through jpeg chunks
	// In all formats, the file is at least 24 bytes big, so we'll read that always
	if (fread(buf, 1, 24, file) != 24)
	{
		fclose(file);
		throw "Ace Error: GetImageSize() fread() failed to read file.";
	}

	// For JPEGs, we need to read the first 12 bytes of each chunk.
	// We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 && buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F')
	{
		while (buf[2] == 0xFF)
		{
			if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 || buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA || buf[3] == 0xCB)
			{
				fclose(file);
				break;
			}

			pos += 2 + (buf[4] << 8) + buf[5];
			if (pos + 12 > len)
			{
				fclose(file);
				break;
			}
			if (fseek(file, pos, SEEK_SET) != 0)
			{
				fclose(file);
				throw "Ace Error: GetImageSize() fseek() failed to read file.";
			}

			if (fread(buf + 2, 1, 12, file) != 12)
			{
				fclose(file);
				throw "Ace Error: GetImageSize() fread() failed to read file.";
			}
		}
	}

	//Some images contain Exif data so check for that.
	if ((buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 && buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F') ||
		(buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE1 && buf[6] == 'E' && buf[7] == 'x' && buf[8] == 'i' && buf[9] == 'f'))
	{
		while (buf[2] == 0xFF)
		{
			if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 || buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA || buf[3] == 0xCB)
			{
				fclose(file);
				break;
			}

			pos += 2 + (buf[4] << 8) + buf[5];
			if (pos + 12 > len)
			{
				fclose(file);
				break;
			}
			if (fseek(file, pos, SEEK_SET) != 0)
			{
				fclose(file);
				throw "Ace Error: GetImageSize() fseek() failed to read file.";
			}

			if (fread(buf + 2, 1, 12, file) != 12)
			{
				fclose(file);
				throw "Ace Error: GetImageSize() fread() failed to read file.";
			}
		}
	}

	fclose(file);

	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF)
	{
		//JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
		*outHeight = (buf[7] << 8) + buf[8];
		*outWidth = (buf[9] << 8) + buf[10];

		fclose(file);
	}
	else if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
	{
		//GIF: first three bytes say "GIF", next three give version number. Then dimensions
		*outWidth = buf[6] + (buf[7] << 8);
		*outHeight = buf[8] + (buf[9] << 8);

		fclose(file);
	}
	else if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A && buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
	{
		//PNG: the first frame is by definition an IHDR frame, which gives dimensions
		*outWidth = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
		*outHeight = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);

		fclose(file);
	}
	else
	{
		*outWidth = -1;
		*outHeight = -1;

		fclose(file);

		throw "Ace Error: GetImageSize() file is not an image format.";
	}
}

/**
* Solves for c in Pythagoras theorem (a^2 + b^2 = c^2);
* i.e square root of (x^2 + y^2). Created in compliance
* with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		double result = Ace::Hypotenuse(3.0, 4.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:5.000000
*
* @param x Value of one of the sides of the triangle.
* @param y Value of the other side of the triangle.
*
* @return The Hypotenuse (the longest side) of the triangle.
* 		  If a mathematical error occurs then an exception is thrown.
*/
double Ace::Hypotenuse(double x, double y)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = hypot(x, y);

	//Look for any mathematical errors as a result of calling hypot().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Hypotenuse() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Hypotenuse() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Solves for c in Pythagoras theorem (a^2 + b^2 = c^2);
* i.e square root of (x^2 + y^2). Created in compliance
* with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		float result = Ace::Hypotenuse(3.0f, 4.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:5.000000
*
* @param x Value of one of the sides of the triangle.
* @param y Value of the other side of the triangle.
*
* @return The Hypotenuse (the longest side) of the triangle.
* 		  If a mathematical error occurs then an exception is thrown.
*/
float Ace::Hypotenuse(float x, float y)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = hypot(x, y);

	//Look for any mathematical errors as a result of calling hypot().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Hypotenuse() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Hypotenuse() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Solves for c in Pythagoras theorem (a^2 + b^2 = c^2);
* i.e square root of (x^2 + y^2). Created in compliance
* with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int result = Ace::Hypotenuse(3, 4);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:5
*
* @param x Value of one of the sides of the triangle.
* @param y Value of the other side of the triangle.
*
* @return The Hypotenuse (the longest side) of the triangle.
* 		  If a mathematical error occurs then an exception is thrown.
*/
int Ace::Hypotenuse(int x, int y)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = hypot(x, y);

	//Look for any mathematical errors as a result of calling hypot().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Hypotenuse() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Hypotenuse() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Searches strToSearch for the first occurrence of charToLookFor and returns the
* (0 based) index of its location.
*
* @author Katianie
* @date 10/31/2016
*
* @code
* 	try
* 	{
* 		char* data = "Has anyone ever been as far as decided to be there and then look more like?";
* 		int temp = Ace::IndexOf(data, 'b');
* 		printf("%d\n", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:16
*
* @param [in] strToSearch String to search starting from left to right.
* @param charToLookFor Character to look for inside of strToSearch.
*
* @return The (0 based) index of where charToLookFor is located.
* 		  Returns -1 if not found. Throws an exception if there
* 		  is an error.
*/
int Ace::IndexOf(char* strToSearch, char charToLookFor)
{
	int strToSearchLen;

	if (strToSearch != NULL)
	{
		//Record the length so we don't have to keep calling strlen().
		strToSearchLen = strlen(strToSearch);

		//Search the entire string for an occurrence of strToLookFor.
		for (int currIndex = 0; currIndex <= strToSearchLen; currIndex++)
		{
			//If the current character in the string is equal to 
			//what we are looking for, then we found it.
			if (strToSearch[currIndex] == charToLookFor)
			{
				return currIndex;
			}
		}

	}
	else
	{
		throw "Ace Error: IndexOf() Parameters cannot be NULL.";
	}

	//Couldn't find it.
	return -1;
}

/**
* Searches strToSearch for the first occurrence of charToLookFor and returns the
* (0 based) index of its location.
*
* @author Katianie
* @date 10/31/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = L"Has anyone ever been as far as decided to be there and then look more like?";
* 		int temp = Ace::IndexOf(data, L'b');
* 		printf("%d\n", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:16
*
* @param [in] strToSearch String to search starting from left to right.
* @param charToLookFor Character to look for inside of strToSearch.
*
* @return The (0 based) index of where charToLookFor is located.
* 		  Returns -1 if not found. Throws an exception if there
* 		  is an error.
*/
int Ace::IndexOf(wchar_t* strToSearch, wchar_t charToLookFor)
{
	int strToSearchLen;

	if (strToSearch != NULL)
	{
		//Record the length so we don't have to keep calling wcslen().
		strToSearchLen = wcslen(strToSearch);

		//Search the entire string for an occurrence of strToLookFor.
		for (int currIndex = 0; currIndex <= strToSearchLen; currIndex++)
		{
			//If the current character in the string is equal to 
			//what we are looking for, then we found it.
			if (strToSearch[currIndex] == charToLookFor)
			{
				return currIndex;
			}
		}

	}
	else
	{
		throw "Ace Error: IndexOf() Parameters cannot be NULL.";
	}

	//Couldn't find it.
	return -1;
}

/**
* Searches strToSearch for the first occurrence of strToLookFor and returns the
* (0 based) index of its location.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = "Has anyone ever been as far as decided to be there and then look more like?";
* 		int temp = Ace::IndexOf(data, "been");
* 		printf("%d\n", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:16
*
* @param [in] strToSearch String to search starting from left to right.
* @param [in] strToLookFor String to look for inside of strToSearch.
*
* @return The (0 based) index of where strToLookFor is located.
* 		  Returns -1 if not found. Throws an exception if there
* 		  is an error.
*/
int Ace::IndexOf(char* strToSearch, char* strToLookFor)
{
	int strToLookForLen;
	int strToSearchLen;
	char* currSubstring;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling strlen().
		strToLookForLen = strlen(strToLookFor);
		strToSearchLen = strlen(strToSearch);

		//The item to look for must be smaller then the word.
		if (strToLookForLen <= strToSearchLen)
		{
			//Search the entire string for an occurrence of strToLookFor.
			for (int currIndex = 0; currIndex <= (strToSearchLen - strToLookForLen); currIndex++)
			{
				try
				{
					currSubstring = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);
				}
				catch (char* exceptionMessage)
				{
					//Create a stack trace of the exception.
					char combinedMessage[1024];
					if (SecureZeroMemory(combinedMessage, 1024) != NULL)
					{
						if (sprintf_s(combinedMessage, 1024, "Ace::IndexOf()->%s\n", exceptionMessage) > 0)
						{
							throw combinedMessage;
							return -1;
						}
					}

					throw "Ace Error: IndexOf() Encountered an error.";
					return -1;
				}

				//Check to see if currSubstring is equal to what we are looking for.
				if (strcmp(currSubstring, strToLookFor) == 0)
				{
					//Cleanup before returning.
					_aligned_free(currSubstring);

					//Found it!
					return currIndex;
				}

				//Cleanup the currSubstring since SubString allocates memory.
				_aligned_free(currSubstring);
			}
		}
		else
		{
			throw "Ace Error: IndexOf() strToLookFor must be smaller then strToSearch.";
		}
	}
	else
	{
		throw "Ace Error: IndexOf() Parameters cannot be NULL.";
	}

	//Couldn't find it.
	return -1;
}

/**
* Searches strToSearch for the first occurrence of strToLookFor and returns the
* (0 based) index of its location.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = L"Has anyone ever been as far as decided to be there and then look more like?";
* 		int temp = Ace::IndexOf(data, L"been");
* 		printf("%d\n", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:16
*
* @param [in] strToSearch Wide String to search starting from left to right.
* @param [in] strToLookFor Wide String to look for inside of strToSearch.
*
* @return The (0 based) index of where strToLookFor is located.
* 		  Returns -1 if not found. Throws an exception if there
* 		  is an error.
*/
int Ace::IndexOf(wchar_t* strToSearch, wchar_t* strToLookFor)
{
	int strToLookForLen;
	int strToSearchLen;
	wchar_t* currSubstring;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling strlen().
		strToLookForLen = wcslen(strToLookFor);
		strToSearchLen = wcslen(strToSearch);

		//The item to look for must be smaller then the word.
		if (strToLookForLen <= strToSearchLen)
		{
			//Search the entire string for an occurrence of strToLookFor.
			for (int currIndex = 0; currIndex <= (strToSearchLen - strToLookForLen); currIndex++)
			{
				try
				{
					currSubstring = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);
				}
				catch (char* exceptionMessage)
				{
					//Create a stack trace of the exception.
					char combinedMessage[1024];
					if (SecureZeroMemory(combinedMessage, 1024) != NULL)
					{
						if (sprintf_s(combinedMessage, 1024, "Ace::IndexOf()->%s\n", exceptionMessage) > 0)
						{
							throw combinedMessage;
							return -1;
						}
					}

					throw "Ace Error: IndexOf() Encountered an error.";
					return -1;
				}

				//Check to see if currSubstring is equal to what we are looking for.
				if (wcscmp(currSubstring, strToLookFor) == 0)
				{
					//Cleanup before returning.
					_aligned_free(currSubstring);

					//Found it!
					return currIndex;
				}

				//Cleanup the currSubstring since SubString allocates memory.
				_aligned_free(currSubstring);
			}
		}
		else
		{
			throw "Ace Error: IndexOf() strToLookFor must be smaller then strToSearch.";
		}
	}
	else
	{
		throw "Ace Error: IndexOf() Parameters cannot be NULL.";
	}

	//Couldn't find it.
	return -1;
}

/**
* Returns the first index of a letter in a given string. For Example: "23584Anus" would
* return 5.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* str = "23584Anus";
* 		int retVal = Ace::IndexOfFirstLetter(str);
* 		printf("%d\n", retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:5
*
* @param [in] strToSearch String to search for the first occurrence of a alphabetical character.
*
* @return The (0 based) index of where in strToSearch the first alphabetical character
*		  was found. Returns -1 if not found. Throws an exception if there
* 		  is an error.
*/
int Ace::IndexOfFirstLetter(char* strToSearch)
{
	if (strToSearch != NULL)
	{
		for (size_t currIndex = 0; currIndex <= strlen(strToSearch); currIndex++)
		{
			if (Ace::IsAlphabetic(strToSearch[currIndex]) == true)
			{
				return currIndex;
			}
		}
	}
	else
	{
		throw "Ace Error: IndexOfFirstLetter() strToSearch cannot be NULL.";
	}

	return -1;
}

/**
* Returns the first index of a letter in a given wide string. For Example: "23584Anus" would
* return 5.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_s* str = L"23584Anus";
* 		int retVal = Ace::IndexOfFirstLetter(str);
* 		printf("%d\n", retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:5
*
* @param [in] strToSearch Wide String to search for the first occurrence of a alphabetical character.
*
* @return The (0 based) index of where in strToSearch the first alphabetical character
*		  was found. Returns -1 if not found. Throws an exception if there
* 		  is an error.
*/
int Ace::IndexOfFirstLetter(wchar_t* strToSearch)
{
	if (strToSearch != NULL)
	{
		for (size_t currIndex = 0; currIndex <= wcslen(strToSearch); currIndex++)
		{
			if (Ace::IsAlphabetic(strToSearch[currIndex]) == true)
			{
				return currIndex;
			}
		}
	}
	else
	{
		throw "Ace Error: IndexOfFirstLetter() strToSearch cannot be NULL.";
	}

	return -1;
}

/**
* Searches strToSearch for the first occurrence of charToLookFor (regardless of
* case) and returns the (0 based) index of its location.
*
* @author Katianie
* @date 5/26/2016
*
* @code
* 	try
* 	{
* 		int result = Ace::IndexOfIgnoreCase("My name is eddIe and I like cheese.", 'I');
* 		printf("%d\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:8
*
* @param [in] strToSearch String to search starting from left to right.
* @param charToLookFor Character to look for inside of strToSearch.
*
* @return The (0 based) index of where strToLookFor is located.
* 		  Returns -1 if not found. Throws an exception if there
* 		  is an error.
*/
int Ace::IndexOfIgnoreCase(char* strToSearch, char charToLookFor)
{
	char* normStrToSearch;
	char normCharToLookFor;
	int result = -1;

	if (strToSearch != NULL)
	{
		//Normalize the strings so all characters are the same case.
		normStrToSearch = Ace::ConvertToUpperCase(strToSearch);
		normCharToLookFor = Ace::ConvertToUpperCase(charToLookFor);

		result = Ace::IndexOf(normStrToSearch, normCharToLookFor);

		_aligned_free(normStrToSearch);
	}
	else
	{
		throw "Ace Error: IndexOfIgnoreCase() strToSearch cannot be NULL";
	}

	return result;
}

/**
* Searches strToSearch for the first occurrence of charToLookFor (regardless of
* case) and returns the (0 based) index of its location.
*
* @author Katianie
* @date 5/26/2016
*
* @code
* 	try
* 	{
* 		int result = Ace::IndexOfIgnoreCase(L"My name is eddIe and I like cheese.", L'I');
* 		printf("%d\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:8
*
* @param [in] strToSearch String to search starting from left to right.
* @param charToLookFor Character to look for inside of strToSearch.
*
* @return The (0 based) index of where strToLookFor is located.
* 		  Returns -1 if not found. Throws an exception if there
* 		  is an error.
*/
int Ace::IndexOfIgnoreCase(wchar_t* strToSearch, wchar_t charToLookFor)
{
	wchar_t* normStrToSearch;
	wchar_t normCharToLookFor;
	int result = -1;

	if (strToSearch != NULL)
	{
		//Normalize the strings so all characters are the same case.
		normStrToSearch = Ace::ConvertToUpperCase(strToSearch);
		normCharToLookFor = Ace::ConvertToUpperCase(charToLookFor);

		result = Ace::IndexOf(normStrToSearch, normCharToLookFor);

		_aligned_free(normStrToSearch);
	}
	else
	{
		throw "Ace Error: IndexOfIgnoreCase() strToSearch cannot be NULL";
	}

	return result;
}

/**
* Searches strToSearch for the first occurrence of strToLookFor (regardless of
* case) and returns the (0 based) index of its location.
*
* @author Katianie
* @date 5/26/2016
*
* @code
* 	try
* 	{
* 		int result = Ace::IndexOfIgnoreCase("My name is eddIe and I like cheese.", "EdDIe");
* 		printf("%d\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:11
*
* @param [in] strToSearch String to search starting from left to right.
* @param [in] strToLookFor String to look for inside of strToSearch.
*
* @return The (0 based) index of where strToLookFor is located.
* 		  Returns -1 if not found. Throws an exception if there
* 		  is an error.
*/
int Ace::IndexOfIgnoreCase(char* strToSearch, char* strToLookFor)
{
	char* normStrToSearch;
	char* normStrToLookFor;
	int result = -1;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Normalize the strings so all characters are the same case.
		normStrToSearch = Ace::ConvertToUpperCase(strToSearch);
		normStrToLookFor = Ace::ConvertToUpperCase(strToLookFor);

		result = Ace::IndexOf(normStrToSearch, normStrToLookFor);

		_aligned_free(normStrToSearch);
		_aligned_free(normStrToLookFor);
	}
	else
	{
		throw "Ace Error: IndexOfIgnoreCase() Parameters cannot be NULL";
	}

	return result;
}

/**
* Searches strToSearch for the first occurrence of strToLookFor (regardless of
* case) and returns the (0 based) index of its location.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		int result = Ace::IndexOfIgnoreCase(L"My name is eddIe and I like cheese.", L"EdDIe");
* 		printf("%d\n", result);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:11
*
* @param [in] strToSearch Wide String to search starting from left to right.
* @param [in] strToLookFor Wide String to look for inside of strToSearch.
*
* @return The (0 based) index of where strToLookFor is located.
* 		  Returns -1 if not found. Throws an exception if there
* 		  is an error.
*/
int Ace::IndexOfIgnoreCase(wchar_t* strToSearch, wchar_t* strToLookFor)
{
	wchar_t* normStrToSearch;
	wchar_t* normStrToLookFor;
	int result = -1;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Normalize the strings so all characters are the same case.
		normStrToSearch = Ace::ConvertToUpperCase(strToSearch);
		normStrToLookFor = Ace::ConvertToUpperCase(strToLookFor);

		result = Ace::IndexOf(normStrToSearch, normStrToLookFor);

		_aligned_free(normStrToSearch);
		_aligned_free(normStrToLookFor);
	}
	else
	{
		throw "Ace Error: IndexOfIgnoreCase() Parameters cannot be NULL";
	}

	return result;
}

/**
* Inserts the string strToInsert into strToRevcive by "sandwiching"
* the two halves of strToReceive. The first half is from 0 to startIndex-1,
* the last half is from startIndex to the end of strToReceive.
*
* @author Katianie
* @date 10/10/2016
*
* @code
*	try
*	{
*		char* retVal = Ace::InsertString("Eddie is a Computer Scientist.", " O\'Hagan", 5);
*		printf("%s\n", retVal);
*		_aligned_free(retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:Eddie O'Hagan is a Computer Scientist.
*
* @param [in] strToReceive The string to receive strToInsert.
* @param [in] strToInsert The string to sandwich/insert into strToReceive.
* @param startIndex The index in strToReceive to insert at strToInsert.
*
* @return The newly created string with strToInsert inserted into strToReceive.
* 		  Throws an exception if there is an error.
*/
char* Ace::InsertString(char* strToReceive, char* strToInsert, int startIndex)
{
	char* firstPart = NULL;
	char* lastPart = NULL;
	char* finalString = NULL;

	if (strToReceive != NULL && strToInsert != NULL && startIndex >= 0)
	{
		try
		{
			//Separate the two halves.
			firstPart = Ace::SubString(strToReceive, 0, startIndex);
			lastPart = Ace::SubString(strToReceive, startIndex);

			//Sandwich the strToInsert between the two halves.
			finalString = Ace::Concatenate(3, firstPart, strToInsert, lastPart);

			_aligned_free(firstPart);
			_aligned_free(lastPart);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::InsertString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: InsertString() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: InsertString() Parameters cannot be NULL";
	}

	//Return the sandwich.
	return finalString;
}

/**
* Inserts the string strToInsert into strToRevcive by "sandwiching"
* the two halves of strToReceive. The first half is from 0 to startIndex-1,
* the last half is from startIndex to the end of strToReceive.
*
* @author Katianie
* @date 10/10/2016
*
* @code
*	try
*	{
*		wchar_t* retVal = Ace::InsertString(L"Eddie is a Computer Scientist.", L" O\'Hagan", 5);
*		printf("%S\n", retVal);
*		_aligned_free(retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:Eddie O'Hagan is a Computer Scientist.
*
* @param [in] strToReceive The wide string to receive strToInsert.
* @param [in] strToInsert The wide string to sandwich/insert into strToReceive.
* @param startIndex The index in strToReceive to insert at strToInsert.
*
* @return The newly created wide string with strToInsert inserted into strToReceive.
* 		  Throws an exception if there is an error.
*/
wchar_t* Ace::InsertString(wchar_t* strToReceive, wchar_t* strToInsert, int startIndex)
{
	wchar_t* firstPart = NULL;
	wchar_t* lastPart = NULL;
	wchar_t* finalString = NULL;

	if (strToReceive != NULL && strToInsert != NULL && startIndex >= 0)
	{
		try
		{
			//Separate the two halves.
			firstPart = Ace::SubString(strToReceive, 0, startIndex);
			lastPart = Ace::SubString(strToReceive, startIndex);

			//Sandwich the strToInsert between the two halves.
			finalString = Ace::ConcatenateW(3, firstPart, strToInsert, lastPart);

			_aligned_free(firstPart);
			_aligned_free(lastPart);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::InsertString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: InsertString() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: InsertString() Parameters cannot be NULL";
	}

	//Return the sandwich.
	return finalString;
}

/**
* Returns true if a character is a letter from either a to z or A to Z. Letters fall into a
* range on the ASCII chart which makes this an simple test.
*
* ASCII chart: http://www.asciitable.com/index/asciifull.gif.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	if (Ace::IsAlphabetic('E') == true)
* 	{
* 		printf("%s", "true");
* 	}
* 	else
* 	{
* 		printf("%s", "false");
* 	}
*
* @endcode
* Output:true
*
* @param [in] character A single character to test if it's either a-z or A-Z.
*
* @return true if the character is between ranges a-z or A-Z; false otherwise.
*/
bool Ace::IsAlphabetic(char character)
{
	int asciiNum = (int)character;

	//ASCII chart has uppercase letters A-Z between numbers 65-90, lowercase between 97-122.
	//http://www.asciitable.com/index/asciifull.gif
	if (asciiNum >= 97 && asciiNum <= 122)
	{
		return true;
	}
	else if (asciiNum >= 65 && asciiNum <= 90)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
* Returns true if a wide character is not a digit.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	if (Ace::IsAlphabetic(L'E') == true)
* 	{
* 		printf("%S", L"true");
* 	}
* 	else
* 	{
* 		printf("%S", L"false");
* 	}
*
* @endcode
* Output:true
*
* @param [in] character A single wide character to test.
*
* @return true if the character is not a digit.
*/
bool Ace::IsAlphabetic(wchar_t character)
{
	//Simply assume if its not a digit then its Alphabetic.
	return Ace::IsDigit(character) == false;
}

/**
* Returns true if a character is a digit from 0-9. Digits fall into a range on the ASCII
* chart which makes this an simple test.
*
* ASCII chart: http://www.asciitable.com/index/asciifull.gif.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	if (Ace::IsDigit('7') == true)
* 	{
* 		printf("%s", "true");
* 	}
* 	else
* 	{
* 		printf("%s", "false");
* 	}
*
* @endcode
* Output:true
*
* @param [in] character A single character to test if it's between 0-9.
*
* @return true if the character is between 0-9; false otherwise.
*/
bool Ace::IsDigit(char character)
{
	int asciiNum = (int)character;

	//ASCII chart has digits 0-9 between numbers 48-57.
	//http://www.asciitable.com/index/asciifull.gif
	if (asciiNum >= 48 && asciiNum <= 57)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
* Returns true if a wide character is a digit.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	if (Ace::IsDigit(L'7') == true)
* 	{
* 		printf("%S", L"true");
* 	}
* 	else
* 	{
* 		printf("%S", L"false");
* 	}
*
* @endcode
* Output:true
*
* @param [in] character A single wide character to test if it's between 0-9.
*
* @return true if the wide character is a digit; false otherwise.
*/
bool Ace::IsDigit(wchar_t character)
{
	return iswdigit(character) > 0;
}

/**
* Searches a given string to see if it is all empty. In other words, is the string provided all
* whitespace.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	if (Ace::IsEmpty("    ") == true)
* 	{
* 		printf("%s", "true");
* 	}
* 	else
* 	{
* 		printf("%s", "false");
* 	}
*
* @endcode
* Output:true
*
* @param [in] str String to search.
*
* @return true if every character in str is whitespace; false otherwise.
* 		  Throws an exception if str is NULL.
*/
bool Ace::IsEmpty(char* str)
{
	if (str != NULL)
	{
		for (size_t currIndex = 0; currIndex < strlen(str); currIndex++)
		{
			//If this character is not whitespace, return false.
			if (Ace::IsWhitespace(str[currIndex]) == false)
			{
				return false;
			}
		}
	}
	else
	{
		throw "Ace Error: IsEmpty() str cannot be NULL.";
	}

	return true;
}

/**
* Searches a given wide string to see if it is all empty. In other words, is the string provided all
* whitespace.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	if (Ace::IsEmpty(L"    ") == true)
* 	{
* 		printf("%S", L"true");
* 	}
* 	else
* 	{
* 		printf("%S", L"false");
* 	}
*
* @endcode
* Output:true
*
* @param [in] str Wide String to search.
*
* @return true if every wide character in str is whitespace; false otherwise.
* 		  Throws an exception if str is NULL.
*/
bool Ace::IsEmpty(wchar_t* str)
{
	if (str != NULL)
	{
		for (size_t currIndex = 0; currIndex < wcslen(str); currIndex++)
		{
			//If this wide character is not whitespace, return false.
			if (Ace::IsWhitespace(str[currIndex]) == false)
			{
				return false;
			}
		}
	}
	else
	{
		throw "Ace Error: IsEmpty() str cannot be NULL.";
	}

	return true;
}

/**
* Returns true if all characters are a digit.
*
* ASCII chart: http://www.asciitable.com/index/asciifull.gif.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* str = "01234";
* 		if (Ace::IsNumber(str) == true)
* 		{
* 			printf("%s", "true");
* 		}
* 		else
* 		{
* 			printf("%s", "false");
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:true
*
* @param [in] str A string to test if all characters are digits.
*
* @return true if the string is all digits, false otherwise.
* 		  Throws an exception if str is NULL.
*/
bool Ace::IsNumber(char* str)
{
	char* currChar = '\0';

	if (str != NULL)
	{
		for (size_t currIndex = 0; currIndex < strlen(str); currIndex++)
		{
			if (Ace::IsDigit(str[currIndex]) == false)
			{
				return false;
			}
		}

		return true;
	}
	else
	{
		throw "Ace Error: IsNumber() str cannot be NULL.";
	}

	return false;
}

/**
* Returns true if all characters are a digit.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* str = L"01234";
* 		if (Ace::IsNumber(str) == true)
* 		{
* 			printf("%S", L"true");
* 		}
* 		else
* 		{
* 			printf("%S", L"false");
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:true
*
* @param [in] str A wide string to test if all characters are digits.
*
* @return true if the wide string is all digits, false otherwise.
* 		  Throws an exception if str is NULL.
*/
bool Ace::IsNumber(wchar_t* str)
{
	wchar_t* currChar = L'\0';

	if (str != NULL)
	{
		for (size_t currIndex = 0; currIndex < wcslen(str); currIndex++)
		{
			if (Ace::IsDigit(str[currIndex]) == false)
			{
				return false;
			}
		}

		return true;
	}
	else
	{
		throw "Ace Error: IsNumber() str cannot be NULL.";
	}

	return false;
}

/**
* Returns true if the provided path leads to a device rather
* than a file. Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/14/2016
*
* @code
* 	try
* 	{
* 		bool retVal = Ace::IsPathADevice("C:\\dicks.txt");
* 		if (retVal == true)
* 		{
* 			printf("%s\n", "true");
* 		}
* 		else
* 		{
* 			printf("%s\n", "false");
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:false
*
* @param [in] path The file path to test.
*
* @return true if the path leads to a device, false otherwise.
*/
bool Ace::IsPathADevice(const char* path)
{
	//This list of reserved names comes from MSDN.
	static const char* reserved[] =
	{
		"nul", "con", "prn", "aux", "com1", "com2", "com3",
		"com4", "com5", "com6", "com7", "com8", "com9",
		"lpt1", "lpt2", "lpt3", "lpt4", "lpt5", "lpt6",
		"lpt7", "lpt8", "lpt9"
	};
	bool retVal = false;
	size_t arrLen = sizeof(reserved) / sizeof(*reserved);

	//First, check to see if this is a device namespace, which
	//always starts with \\.\, because device namespaces are not valid file paths.
	if (path == NULL || 0 == strncmp(path, "\\\\.\\", 4))
	{
		return true;
	}

	//Compare against the list of ancient reserved names.
	for (size_t i = 0; retVal == false && i < arrLen; i++)
	{
		//Because Windows uses a case-insensitive file system, operate on
		//a lowercase version of the given filename. Note: This ignores
		//globalization issues and assumes ASCII characters.
		if (_stricmp(path, reserved[i]) == 0)
		{
			retVal = true;
		}
	}

	return retVal;
}

/**
* Returns true if the provided path leads to a device rather
* than a file. Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/14/2016
*
* @code
* 	try
* 	{
* 		bool retVal = Ace::IsPathADevice(L"C:\\dicks.txt");
* 		if (retVal == true)
* 		{
* 			printf("%S\n", L"true");
* 		}
* 		else
* 		{
* 			printf("%S\n", L"false");
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:false
*
* @param [in] path The file path to test.
*
* @return true if the path leads to a device, false otherwise.
*/
bool Ace::IsPathADevice(const wchar_t* path)
{
	//This list of reserved names comes from MSDN.
	static const wchar_t* reserved[] =
	{
		L"nul", L"con", L"prn", L"aux", L"com1", L"com2", L"com3",
		L"com4", L"com5", L"com6", L"com7", L"com8", L"com9",
		L"lpt1", L"lpt2", L"lpt3", L"lpt4", L"lpt5", L"lpt6",
		L"lpt7", L"lpt8", L"lpt9"
	};
	bool retVal = false;
	size_t arrLen = sizeof(reserved) / sizeof(*reserved);

	//First, check to see if this is a device namespace, which
	//always starts with \\.\, because device namespaces are not valid file paths.
	if (path == NULL || 0 == wcsncmp(path, L"\\\\.\\", 4))
	{
		return true;
	}

	//Compare against the list of ancient reserved names.
	for (size_t i = 0; retVal == false && i < arrLen; i++)
	{
		//Because Windows uses a case-insensitive file system, operate on
		//a lowercase version of the given filename. 
		if (_wcsicmp(path, reserved[i]) == 0)
		{
			retVal = true;
		}
	}

	return retVal;
}

/**
* Returns true if a character is whitespace.
*
* @author Katianie
* @date 5/7/2016
*
* @code
*	if (Ace::IsWhitespace(' ') == true)
*	{
* 		printf("%s", "true");
*	}
*	else
*	{
* 		printf("%s", "false");
*	}
*
* @endcode
* Output:true
*
* @param [in] character Character to test to see if it is whitespace.
*
* @return true if the character is a space, tab or newline; false otherwise.
*/
bool Ace::IsWhitespace(char character)
{
	return Ace::ConvertIntToBool(isspace(character));
}

/**
* Returns true if a wide character is whitespace.
*
* @author Katianie
* @date 9/7/2016
*
* @code
*	if (Ace::IsWhitespace(L' ') == true)
*	{
*		printf("%S", L"true");
*	}
*	else
*	{
*		printf("%S", L"false");
*	}
*
* @endcode
* Output:true
*
* @param [in] character Character to test to see if it is whitespace.
*
* @return true if the wide character is a space, tab or newline; false otherwise.
*/
bool Ace::IsWhitespace(wchar_t character)
{
	return Ace::ConvertIntToBool(iswspace(character));
}

/**
* Returns the larger of the two values.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	double retVal = Ace::Largest(2.0, 2.01);
*	printf("%f\n", retVal);
*
* @endcode
* Output:2.010000
*
* @param num1 number to compare against.
* @param num1 other number to compare against.
*
* @return The larger of the two values.
*/
double Ace::Largest(double num1, double num2)
{
	return fmax(num1, num2);
}

/**
* Returns the larger of the two values.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	float retVal = Ace::Largest(2.0f, 2.01f);
*	printf("%f\n", retVal);
*
* @endcode
* Output:2.010000
*
* @param num1 number to compare against.
* @param num1 other number to compare against.
*
* @return The larger of the two values.
*/
float Ace::Largest(float num1, float num2)
{
	return fmax(num1, num2);
}

/**
* Returns the larger of the two values.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	int retVal = Ace::Largest(2, 3);
*	printf("%d\n", retVal);
*
* @endcode
* Output:3
*
* @param num1 number to compare against.
* @param num1 other number to compare against.
*
* @return The larger of the two values.
*/
int Ace::Largest(int num1, int num2)
{
	return fmax(num1, num2);
}

/**
* Returns the last character in str.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = "Has anyone ever been as far as decided to be there and then look more like?";
* 		char temp = Ace::LastChar(data);
* 		printf("%c", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:?
*
* @param [in,out] str String to search.
*
* @return The last character in str. Throws an exception
* 		  if str is NULL.
*/
char Ace::LastChar(char* str)
{
	if (str != NULL)
	{
		return *(str + strlen(str) - 1);
	}
	else
	{
		throw "Ace Error: LastChar() str cannot be NULL.";
	}

	return '\0';
}

/**
* Returns the last character in str.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = L"Has anyone ever been as far as decided to be there and then look more like?";
* 		wchar_t temp = Ace::LastChar(data);
* 		printf("%c", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:?
*
* @param [in,out] str Wide String to search.
*
* @return The last character in str. Throws an exception
* 		  if str is NULL.
*/
wchar_t Ace::LastChar(wchar_t* str)
{
	if (str != NULL)
	{
		return *(str + wcslen(str) - 1);
	}
	else
	{
		throw "Ace Error: LastChar() str cannot be NULL.";
	}

	return L'\0';
}

/**
* Searches strToSearch for the last occurrence of strToLookFor and returns the (0
* based) index of its location.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = "Has anyone ever been as far as decided to be there and then look more like?";
* 		int temp = Ace::LastIndexOf(data, "th");
* 		printf("%d\n", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:55
*
* @param [in] strToSearch  String to search starting from right to left.
* @param [in] strToLookFor String to look for inside of strToSearch.
*
* @return The (0 based) index of where strToLookFor is located.
* 		  Returns -1 if not found. Throws an exception if error occurs.
*/
int Ace::LastIndexOf(char* strToSearch, char* strToLookFor)
{
	int strToLookForLen;
	int strToSearchLen;
	char* currSubstring;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling strlen().
		strToLookForLen = strlen(strToLookFor);
		strToSearchLen = strlen(strToSearch);

		//The item to look for must be smaller then the word.
		if (strToLookForLen < strToSearchLen)
		{
			//Search the entire string (but start at the end) for an occurrence of strToLookFor. 
			for (int currIndex = (strToSearchLen - strToLookForLen); currIndex >= 0; currIndex--)
			{
				try
				{
					currSubstring = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);
				}
				catch (char* exceptionMessage)
				{
					//Create a stack trace of the exception.
					char combinedMessage[1024];
					if (SecureZeroMemory(combinedMessage, 1024) != NULL)
					{
						if (sprintf_s(combinedMessage, 1024, "Ace::LastIndexOf()->%s\n", exceptionMessage) > 0)
						{
							throw combinedMessage;
							return -1;
						}
					}

					throw "Ace Error: LastIndexOf() Encountered an error.";
					return -1;
				}

				//Check to see if currSubstring is equal to what we are looking for.
				if (strcmp(currSubstring, strToLookFor) == 0)
				{
					//Cleanup before returning.
					_aligned_free(currSubstring);
					return currIndex;
				}

				//Cleanup the currSubstring since SubString allocates memory.
				_aligned_free(currSubstring);
			}
		}
		else
		{
			throw "Ace Error: LastIndexOf() strToLookFor must be smaller then strToSearch.";
		}
	}
	else
	{
		throw "Ace Error: LastIndexOf() Parameters cannot be NULL.";
	}

	return -1;
}

/**
* Searches strToSearch for the last occurrence of strToLookFor and returns the (0
* based) index of its location.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = L"Has anyone ever been as far as decided to be there and then look more like?";
* 		int temp = Ace::LastIndexOf(data, L"th");
* 		printf("%d\n", temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:55
*
* @param [in] strToSearch  Wide String to search starting from right to left.
* @param [in] strToLookFor Wide String to look for inside of strToSearch.
*
* @return The (0 based) index of where strToLookFor is located.
* 		  Returns -1 if not found. Throws an exception if error occurs.
*/
int Ace::LastIndexOf(wchar_t* strToSearch, wchar_t* strToLookFor)
{
	int strToLookForLen;
	int strToSearchLen;
	wchar_t* currSubstring;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling strlen().
		strToLookForLen = wcslen(strToLookFor);
		strToSearchLen = wcslen(strToSearch);

		//The item to look for must be smaller then the word.
		if (strToLookForLen < strToSearchLen)
		{
			//Search the entire string (but start at the end) for an occurrence of strToLookFor. 
			for (int currIndex = (strToSearchLen - strToLookForLen); currIndex >= 0; currIndex--)
			{
				try
				{
					currSubstring = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);
				}
				catch (char* exceptionMessage)
				{
					//Create a stack trace of the exception.
					char combinedMessage[1024];
					if (SecureZeroMemory(combinedMessage, 1024) != NULL)
					{
						if (sprintf_s(combinedMessage, 1024, "Ace::LastIndexOf()->%s\n", exceptionMessage) > 0)
						{
							throw combinedMessage;
							return -1;
						}
					}

					throw "Ace Error: LastIndexOf() Encountered an error.";
					return -1;
				}

				//Check to see if currSubstring is equal to what we are looking for.
				if (wcscmp(currSubstring, strToLookFor) == 0)
				{
					//Cleanup before returning.
					_aligned_free(currSubstring);
					return currIndex;
				}

				//Cleanup the currSubstring since SubString allocates memory.
				_aligned_free(currSubstring);
			}
		}
		else
		{
			throw "Ace Error: LastIndexOf() strToLookFor must be smaller then strToSearch.";
		}
	}
	else
	{
		throw "Ace Error: LastIndexOf() Parameters cannot be NULL.";
	}

	return -1;
}

/**
* Calculates the logarithm (log) base 10 of num
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	try
*	{
*		double retVal = Ace::LogBaseTen(69.0);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:1.838849
*
* @param num Number to use to calculate its logarithm base 10.
*
* @return The logarithm base 10 of num. If a mathematical
* 		  error occurs or num is negative, then an exception
* 		  is thrown.
*/
double Ace::LogBaseTen(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0.0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = log10(num);

		//Look for any mathematical errors as a result of calling log10().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: LogBaseTen() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: LogBaseTen() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Domain Error: LogBaseTen() num must be greater than or equal to 0.";
	}

	return result;
}

/**
* Calculates the logarithm (log) base 10 of num
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	try
*	{
*		float retVal = Ace::LogBaseTen(69.0f);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:1.838849
*
* @param num Number to use to calculate its logarithm base 10.
*
* @return The logarithm base 10 of num. If a mathematical
* 		  error occurs or num is negative, then an exception
* 		  is thrown.
*/
float Ace::LogBaseTen(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0.0f) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = log10(num);

		//Look for any mathematical errors as a result of calling log10().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: LogBaseTen() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: LogBaseTen()  Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Domain Error: LogBaseTen() num must be greater than or equal to 0.";
	}

	return result;
}

/**
* Calculates the logarithm (log) base 10 of num
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int retVal = Ace::LogBaseTen(69);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:1
*
* @param num Number to use to calculate its logarithm base 10.
*
* @return The logarithm base 10 of num. If a mathematical
* 		  error occurs or num is negative, then an exception
* 		  is thrown.
*/
int Ace::LogBaseTen(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = log10(num);

		//Look for any mathematical errors as a result of calling log10().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: LogBaseTen() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: LogBaseTen() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Domain Error: LogBaseTen() num must be greater than or equal to 0.";
	}

	return result;
}

/**
* Calculates the logarithm (log) base 2 of num
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	try
*	{
*		double retVal = Ace::LogBaseTwo(69.0);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:6.108524
*
* @param num Number to use to calculate its logarithm base 2.
*
* @return The logarithm base 2 of num. If a mathematical
* 		  error occurs or num is negative, then an exception
* 		  is thrown.
*/
double Ace::LogBaseTwo(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0.0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = log2(num);

		//Look for any mathematical errors as a result of calling log2().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: LogBaseTwo() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: LogBaseTwo() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Domain Error: LogBaseTwo() num must be greater than or equal to 0.";
	}

	return result;
}

/**
* Calculates the logarithm (log) base 2 of num
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	try
*	{
*		float retVal = Ace::LogBaseTwo(69.0f);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:6.108524
*
* @param num Number to use to calculate its logarithm base 2.
*
* @return The logarithm base 2 of num. If a mathematical
* 		  error occurs or num is negative, then an exception
* 		  is thrown.
*/
float Ace::LogBaseTwo(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0.0f) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = log2(num);

		//Look for any mathematical errors as a result of calling log2().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: LogBaseTwo() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: LogBaseTwo() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Domain Error: LogBaseTwo() num must be greater than or equal to 0.";
	}

	return result;
}

/**
* Calculates the logarithm (log) base 2 of num
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int retVal = Ace::LogBaseTwo(69);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:6
*
* @param num Number to use to calculate its logarithm base 2.
*
* @return The logarithm base 2 of num. If a mathematical
* 		  error occurs or num is negative, then an exception
* 		  is thrown.
*/
int Ace::LogBaseTwo(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = log2(num);

		//Look for any mathematical errors as a result of calling log2().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: LogBaseTwo() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: LogBaseTwo() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Domain Error: LogBaseTwo() num must be greater than or equal to 0.";
	}

	return result;
}

/**
* Computes the floating-point remainder of the
* division operation numerator/denominator in
* compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		double result = Ace::Mod(9.0, 5.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4.000000
*
* @param numerator Number to be divided.
* @param denominator Number to divide the numerator by.
*
* @return The remainder of the division operation. If a
* 		  mathematical error occurs or if denominator equals 0,
* 		  then an exception is thrown.
*/
double Ace::Mod(double numerator, double denominator)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	if (denominator != 0)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = fmod(numerator, denominator);

		//Look for any mathematical errors as a result of calling fmod().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: Mod() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: Mod() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Domain Error: Mod() denominator cannot be 0.";
	}

	return result;
}

/**
* Computes the floating-point remainder of the
* division operation numerator/denominator in
* compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float result = Ace::Mod(9.0f, 5.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4.000000
*
* @param numerator Number to be divided.
* @param denominator Number to divide the numerator by.
*
* @return The remainder of the division operation. If a
* 		  mathematical error occurs or if denominator equals 0,
* 		  then an exception is thrown.
*/
float Ace::Mod(float numerator, float denominator)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	if (denominator != 0)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = fmod(numerator, denominator);

		//Look for any mathematical errors as a result of calling fmod().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: Mod() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: Mod() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Domain Error: Mod() denominator cannot be 0.";
	}

	return result;
}

/**
* Computes the floating-point remainder of the
* division operation numerator/denominator in
* compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int result = Ace::Mod(9, 5);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4
*
* @param numerator Number to be divided.
* @param denominator Number to divide the numerator by.
*
* @return The remainder of the division operation. If a
* 		  mathematical error occurs or if denominator equals 0,
* 		  then an exception is thrown.
*/
int Ace::Mod(int numerator, int denominator)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	if (denominator != 0)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = fmod(numerator, denominator);

		//Look for any mathematical errors as a result of calling fmod().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: Mod() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: Mod() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Domain Error: Mod() denominator cannot be 0.";
	}

	return result;
}

/**
* Calculates the product of multOne * multTwo, then
* adds add to that product (i.e. (multOne * multTwo) + add).
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		double result = Ace::MultiplyThenAdd(9.0, 3.0, 15.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:42.000000
*
* @param multOne Number to multiply.
* @param multTwo Other number to multiply.
* @param add Number to add to the product of multOne * multTwo.
*
* @return The result of (multOne * multTwo) + add. If a
* 		  mathematical error occurs then an exception is thrown.
*/
double Ace::MultiplyThenAdd(double multOne, double multTwo, double add)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	//(multOne * multTwo) + add
	result = fma(multOne, multTwo, add);

	//Look for any mathematical errors as a result of calling fma().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: MultiplyThenAdd() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: MultiplyThenAdd() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the product of multOne * multTwo, then
* adds add to that product (i.e. (multOne * multTwo) + add).
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*	try
*	{
*		float result = Ace::MultiplyThenAdd(9.0f, 3.0f, 15.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:42.000000
*
* @param multOne Number to multiply.
* @param multTwo Other number to multiply.
* @param add Number to add to the product of multOne * multTwo.
*
* @return The result of (multOne * multTwo) + add. If a
* 		  mathematical error occurs then an exception is thrown.
*/
float Ace::MultiplyThenAdd(float multOne, float multTwo, float add)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	//(multOne * multTwo) + add
	result = fma(multOne, multTwo, add);

	//Look for any mathematical errors as a result of calling fma().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: MultiplyThenAdd() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: MultiplyThenAdd() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the product of multOne * multTwo, then
* adds add to that product (i.e. (multOne * multTwo) + add).
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int result = Ace::MultiplyThenAdd(9, 3, 15);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:42
*
* @param multOne Number to multiply.
* @param multTwo Other number to multiply.
* @param add Number to add to the product of multOne * multTwo.
*
* @return The result of (multOne * multTwo) + add. If a
* 		  mathematical error occurs then an exception is thrown.
*/
int Ace::MultiplyThenAdd(int multOne, int multTwo, int add)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	//(multOne * multTwo) + add
	result = fma(multOne, multTwo, add);

	//Look for any mathematical errors as a result of calling fma().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: MultiplyThenAdd() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: MultiplyThenAdd() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates the Natural Logarithm (ln) of num
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	try
*	{
*		double retVal = Ace::NaturalLog(69.0);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4.234107
*
* @param num Number to use to calculate its Natural Logarithm.
*
* @return The Natural Logarithm of num. If a mathematical
* 		  error occurs or num is negative, then an exception
* 		  is thrown.
*/
double Ace::NaturalLog(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0.0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = log(num);

		//Look for any mathematical errors as a result of calling log().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: NaturalLog() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: NaturalLog() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Domain Error: NaturalLog() num must be greater than or equal to 0.";
	}

	return result;
}

/**
* Calculates the Natural Logarithm (ln) of num
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	try
*	{
*		float retVal = Ace::NaturalLog(69.0f);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4.234107
*
* @param num Number to use to calculate its Natural Logarithm.
*
* @return The Natural Logarithm of num. If a mathematical
* 		  error occurs or num is negative, then an exception
* 		  is thrown.
*/
float Ace::NaturalLog(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0.0f) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = log(num);

		//Look for any mathematical errors as a result of calling log().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: NaturalLog() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: NaturalLog() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Domain Error: NaturalLog() num must be greater than or equal to 0.";
	}

	return result;
}

/**
* Calculates the Natural Logarithm (ln) of num
* in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int retVal = Ace::NaturalLog(69);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4
*
* @param num Number to use to calculate its Natural Logarithm.
*
* @return The Natural Logarithm of num. If a mathematical
* 		  error occurs or num is negative, then an exception
* 		  is thrown.
*/
int Ace::NaturalLog(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = log(num);

		//Look for any mathematical errors as a result of calling log().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: NaturalLog() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: NaturalLog() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Domain Error: NaturalLog() num must be greater than or equal to 0.";
	}

	return result;
}

/**
* Calculates The Natural Log (ln) of The Gamma Function
* (ln | (0)integral(Inf)[t^num-1 * e^-t dt] |).
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		double result = Ace::NaturalLogGammaFunction(5.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:3.178054
*
* @param num Number to use for t^num-1 in the equation.
*
* @return The result of calculating The Gamma Function for t^num-1.
* 		  If a mathematical error occurs or if num is less than
* 		  or equal to 0.0, then an exception is thrown.
*/
double Ace::NaturalLogGammaFunction(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//is functions prevent an exception from being raised.
	if (isgreater(num, 0.0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = lgamma(num);

		//Look for any mathematical errors as a result of calling lgamma().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: NaturalLogGammaFunction() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: NaturalLogGammaFunction() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Domain Error: NaturalLogGammaFunction() num must be greater than 0.";
	}

	return result;
}

/**
* Calculates The Natural Log (ln) of The Gamma Function
* (ln | (0)integral(Inf)[t^num-1 * e^-t dt] |).
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float result = Ace::NaturalLogGammaFunction(5.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:3.178054
*
* @param num Number to use for t^num-1 in the equation.
*
* @return The result of calculating The Gamma Function for t^num-1.
* 		  If a mathematical error occurs or if num is less than
* 		  or equal to 0.0, then an exception is thrown.
*/
float Ace::NaturalLogGammaFunction(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	if (isgreater(num, 0.0f) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = lgamma(num);

		//Look for any mathematical errors as a result of calling lgamma().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: NaturalLogGammaFunction() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: NaturalLogGammaFunction() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Domain Error: NaturalLogGammaFunction() num must be greater than 0.";
	}

	return result;
}

/**
* Calculates The Natural Log (ln) of The Gamma Function
* (ln | (0)integral(Inf)[t^num-1 * e^-t dt] |).
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int result = Ace::NaturalLogGammaFunction(5);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:3
*
* @param num Number to use for t^num-1 in the equation.
*
* @return The result of calculating The Gamma Function for t^num-1.
* 		  If a mathematical error occurs or if num is less than
* 		  or equal to 0, then an exception is thrown.
*/
int Ace::NaturalLogGammaFunction(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	if (isgreater(num, 0) == true)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = lgamma(num);

		//Look for any mathematical errors as a result of calling lgamma().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: NaturalLogGammaFunction() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: NaturalLogGammaFunction() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Domain Error: NaturalLogGammaFunction() num must be greater than 0.";
	}

	return result;
}

/**
* Retrieves the first occurrence of charToLookFor in strToSearch
* starting at startIndex onward.
*
* @author Katianie
* @date 10/10/2016
*
* @code
*	try
*	{
*		int retVal = Ace::NextIndexOf("I wish I blew it up instead...wicked.", 'I', 1);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:7
*
* @param strToSearch String to search through.
* @param charToLookFor Character to look for in strToSearch.
* @param startIndex Index to start looking.
*
* @return The index of the first occurrence of charToLookFor
* 		  starting from startIndex. If not found, -1 is returned.
* 		  Throws an exception if strToSearch is NULL.
*/
int Ace::NextIndexOf(char* strToSearch, char charToLookFor, int startIndex)
{
	int foundIndex = -1;

	if (strToSearch != NULL)
	{
		for (int i = startIndex; i < strlen(strToSearch); i++)
		{
			if (strToSearch[i] == charToLookFor)
			{
				foundIndex = i;
				break;
			}
		}
	}
	else
	{
		throw "Ace Error: NextIndexOf() strToSearch cannot be NULL.";
	}

	return foundIndex;
}

/**
* Retrieves the first occurrence of charToLookFor in strToSearch
* starting at startIndex onward.
*
* @author Katianie
* @date 10/10/2016
*
* @code
*	try
*	{
*		int retVal = Ace::NextIndexOf(L"I wish I blew it up instead...wicked.", L'I', 1);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:7
*
* @param strToSearch Wide String to search through.
* @param charToLookFor Wide Character to look for in strToSearch.
* @param startIndex Index to start looking.
*
* @return The index of the first occurrence of charToLookFor
* 		  starting from startIndex. If not found, -1 is returned.
* 		  Throws an exception if strToSearch is NULL.
*/
int Ace::NextIndexOf(wchar_t* strToSearch, wchar_t charToLookFor, int startIndex)
{
	int foundIndex = -1;

	if (strToSearch != NULL)
	{
		for (int i = startIndex; i < wcslen(strToSearch); i++)
		{
			if (strToSearch[i] == charToLookFor)
			{
				foundIndex = i;
				break;
			}
		}
	}
	else
	{
		throw "Ace Error: NextIndexOf() strToSearch cannot be NULL.";
	}

	return foundIndex;
}

/**
* Retrieves the first occurrence of strToLookFor in strToSearch
* starting at startIndex onward.
*
* @author Katianie
* @date 10/10/2016
*
* @code
*	try
*	{
*		int retVal = Ace::NextIndexOf("I wish I blew it up instead...wicked.", "wicked", 1);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:30
*
* @param strToSearch String to search through.
* @param strToLookFor String to look for in strToSearch.
* @param startIndex Index to start looking.
*
* @return The index of the first occurrence of strToLookFor
* 		  starting from startIndex. If not found, -1 is returned.
* 		  Throws an exception if an error occurs.
*/
int Ace::NextIndexOf(char* strToSearch, char* strToLookFor, int startIndex)
{
	int strToLookForLen;
	int strToSearchLen;
	char* currSubstring;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling strlen().
		strToLookForLen = strlen(strToLookFor);
		strToSearchLen = strlen(strToSearch);

		//The item to look for must be smaller then the word.
		if (strToLookForLen < strToSearchLen)
		{
			//Search the string starting at startIndex for the first occurrence of strToLookFor.
			for (int currIndex = startIndex; currIndex <= (strToSearchLen - strToLookForLen); currIndex++)
			{
				try
				{
					currSubstring = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);
				}
				catch (char* exceptionMessage)
				{
					//Create a stack trace of the exception.
					char combinedMessage[1024];
					if (SecureZeroMemory(combinedMessage, 1024) != NULL)
					{
						if (sprintf_s(combinedMessage, 1024, "Ace::NextIndexOf()->%s\n", exceptionMessage) > 0)
						{
							throw combinedMessage;
							return -1;
						}
					}

					throw "Ace Error: NextIndexOf() Encountered an error.";
					return -1;
				}

				//Check to see if currSubstring is equal to what we are looking for.
				if (strcmp(currSubstring, strToLookFor) == 0)
				{
					//Cleanup before returning.
					_aligned_free(currSubstring);

					//Found it!
					return currIndex;
				}

				//Cleanup the currSubstring since SubString allocates memory.
				_aligned_free(currSubstring);
			}
		}
		else
		{
			throw "Ace Error: NextIndexOf() strToLookFor is smaller then strToSearch.";
		}
	}
	else
	{
		throw "Ace Error: NextIndexOf() Parameters cannot be NULL.";
	}

	//Couldn't find it.
	return -1;
}

/**
* Retrieves the first occurrence of strToLookFor in strToSearch
* starting at startIndex onward.
*
* @author Katianie
* @date 10/10/2016
*
* @code
*	try
*	{
*		int retVal = Ace::NextIndexOf(L"I wish I blew it up instead...wicked.", L"wicked", 1);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:30
*
* @param strToSearch Wide String to search through.
* @param strToLookFor Wide String to look for in strToSearch.
* @param startIndex Index to start looking.
*
* @return The index of the first occurrence of strToLookFor
* 		  starting from startIndex. If not found, -1 is returned.
* 		  Throws an exception if an error occurs.
*/
int Ace::NextIndexOf(wchar_t* strToSearch, wchar_t* strToLookFor, int startIndex)
{
	int strToLookForLen;
	int strToSearchLen;
	wchar_t* currSubstring;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling wcslen().
		strToLookForLen = wcslen(strToLookFor);
		strToSearchLen = wcslen(strToSearch);

		//The item to look for must be smaller then the word.
		if (strToLookForLen < strToSearchLen)
		{
			//Search the string starting at startIndex for the first occurrence of strToLookFor.
			for (int currIndex = startIndex; currIndex <= (strToSearchLen - strToLookForLen); currIndex++)
			{
				try
				{
					currSubstring = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);
				}
				catch (char* exceptionMessage)
				{
					//Create a stack trace of the exception.
					char combinedMessage[1024];
					if (SecureZeroMemory(combinedMessage, 1024) != NULL)
					{
						if (sprintf_s(combinedMessage, 1024, "Ace::NextIndexOf()->%s\n", exceptionMessage) > 0)
						{
							throw combinedMessage;
							return -1;
						}
					}

					throw "Ace Error: NextIndexOf() Encountered an error.";
					return -1;
				}

				//Check to see if currSubstring is equal to what we are looking for.
				if (wcscmp(currSubstring, strToLookFor) == 0)
				{
					//Cleanup before returning.
					_aligned_free(currSubstring);

					//Found it!
					return currIndex;
				}

				//Cleanup the currSubstring since SubString allocates memory.
				_aligned_free(currSubstring);
			}
		}
		else
		{
			throw "Ace Error: NextIndexOf() strToLookFor is smaller then strToSearch.";
		}
	}
	else
	{
		throw "Ace Error: NextIndexOf() Parameters cannot be NULL.";
	}

	//Couldn't find it.
	return -1;
}

/**
* Calculates num * 2^power in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/5/2016
*
* @code
*	try
*	{
*		double result = Ace::NumTimesTwoToTheXPower(1.0 / 3.0, 5.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:10.666667
*
* @param num Number to multiply 2^power by.
* @param power Number to raise 2 to.
*
* @return The result of num * 2^power. If a mathematical error occurs
* 		  then an exception is thrown.
*/
double Ace::NumTimesTwoToTheXPower(double num, double power)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	//num * 2^power.
	result = ldexp(num, power);

	//Check for possible errors as a result of calling ldexp()
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: NumTimesTwoToTheXPower() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: NumTimesTwoToTheXPower() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates num * 2^power in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float result = Ace::NumTimesTwoToTheXPower(1.0f / 3.0f, 5.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:10.666667
*
* @param num Number to multiply 2^power by.
* @param power Number to raise 2 to.
*
* @return The result of num * 2^power. If a mathematical error occurs
* 		  then an exception is thrown.
*/
float Ace::NumTimesTwoToTheXPower(float num, float power)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	//num * 2^power.
	result = ldexp(num, power);

	//Check for possible errors as a result of calling ldexp()
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: NumTimesTwoToTheXPower() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: NumTimesTwoToTheXPower() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates num * 2^power in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int result = Ace::NumTimesTwoToTheXPower(3, 5);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:96
*
* @param num Number to multiply 2^power by.
* @param power Number to raise 2 to.
*
* @return The result of num * 2^power. If a mathematical error occurs
* 		  then an exception is thrown.
*/
int Ace::NumTimesTwoToTheXPower(int num, int power)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	//num * 2^power.
	result = ldexp(num, power);

	//Check for possible errors as a result of calling ldexp()
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: NumTimesTwoToTheXPower() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: NumTimesTwoToTheXPower() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates the difference between numOne and numTwo only
* if numOne > numTwo. If numOne <= numTwo then 0 is returned.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/5/2016
*
* @code
*	try
*	{
*		double result = Ace::PositiveDifference(6.3, 2.7);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:3.600000
*
* @param numOne Number to use for subtraction.
* @param numTwo Other number to use for subtraction.
*
* @return If numOne is larger then numTwo; then numOne - numTwo is returned,
* 		  else 0 is returned. If a mathematical error occurs then an exception
* 		  is thrown.
*/
double Ace::PositiveDifference(double numOne, double numTwo)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = fdim(numOne, numTwo);

	//Check for possible errors as a result of calling fdim()
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: PositiveDifference() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: PositiveDifference() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the difference between numOne and numTwo only
* if numOne > numTwo. If numOne <= numTwo then 0 is returned.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/5/2016
*
* @code
*	try
*	{
*		float result = Ace::PositiveDifference(6.3f, 2.7f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:3.600000
*
* @param numOne Number to use for subtraction.
* @param numTwo Other number to use for subtraction.
*
* @return If numOne is larger then numTwo; then numOne - numTwo is returned,
* 		  else 0 is returned. If a mathematical error occurs then an exception
* 		  is thrown.
*/
float Ace::PositiveDifference(float numOne, float numTwo)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = fdim(numOne, numTwo);

	//Check for possible errors as a result of calling fdim()
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: PositiveDifference() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: PositiveDifference() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the difference between numOne and numTwo only
* if numOne > numTwo. If numOne <= numTwo then 0 is returned.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/5/2016
*
* @code
*	try
*	{
*		int result = Ace::PositiveDifference(6, 2);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4
*
* @param numOne Number to use for subtraction.
* @param numTwo Other number to use for subtraction.
*
* @return If numOne is larger then numTwo; then numOne - numTwo is returned,
* 		  else 0 is returned. If a mathematical error occurs then an exception
* 		  is thrown.
*/
int Ace::PositiveDifference(int numOne, int numTwo)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = fdim(numOne, numTwo);

	//Check for possible errors as a result of calling fdim()
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: PositiveDifference() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: PositiveDifference() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Takes the provided base and raises it to the exponent (power).
*
* @author Katianie
* @date 9/29/2016
*
* @code
* 	try
* 	{
* 		double powResult = Ace::Power(2.0, 5.0);
* 		printf("%f\n", powResult);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:32.000000
*
* @param base Number to raise.
* @param exponent Number of times to multiply base by itself.
*
* @return The base raised to the exponent is returned. If a
* 		  mathematical error occurs or base is 0.0 and exponent
* 		  is less than or equal to 0.0 OR base is negative,
* 		  then an exception is thrown.
*/
double Ace::Power(double base, double exponent)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Ensure the base and exponent are acceptable values for pow().
	//is functions prevent mathematical exception from being thrown.
	if (((base == 0.0) && islessequal(exponent, 0.0)) || isless(base, 0.0))
	{
		throw "Ace Domain Error: Power() Parameters are not in valid range.";
		return -1.0;
	}
	else
	{
		//Clear all previous errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = pow(base, exponent);

		//Check for possible errors as a result of calling pow()
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: Power() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: Power() Calculation resulted in a range error.";
			return -1.0;
		}

		return result;
	}
}

/**
* Takes the provided base and raises it to the exponent (power).
*
* @author Katianie
* @date 10/6/2016
*
* @code
* 	try
* 	{
* 		float powResult = Ace::Power(2.0f, 5.0f);
* 		printf("%f\n", powResult);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:32.000000
*
* @param base Number to raise.
* @param exponent Number of times to multiply base by itself.
*
* @return The base raised to the exponent is returned. If a
* 		  mathematical error occurs or base is 0.0 and exponent
* 		  is less than or equal to 0.0 OR base is negative,
* 		  then an exception is thrown.
*/
float Ace::Power(float base, float exponent)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Ensure the base and exponent are acceptable values for pow().
	//is functions prevent mathematical exception from being thrown.
	if (((base == 0.0f) && islessequal(exponent, 0.0f)) || isless(base, 0.0f))
	{
		throw "Ace Domain Error: Power() Parameters are not in valid range.";
		return -1.0f;
	}
	else
	{
		//Clear all previous errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = pow(base, exponent);

		//Check for possible errors as a result of calling pow()
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: Power() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: Power() Calculation resulted in a range error.";
			return -1.0f;
		}

		return result;
	}
}

/**
* Takes the provided base and raises it to the exponent (power).
*
* @author Katianie
* @date 10/20/2016
*
* @code
* 	try
* 	{
* 		int powResult = Ace::Power(2, 5);
* 		printf("%d\n", powResult);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:32
*
* @param base Number to raise.
* @param exponent Number of times to multiply base by itself.
*
* @return The base raised to the exponent is returned. If a
* 		  mathematical error occurs or base is 0 and exponent
* 		  is less than or equal to 0 OR base is negative,
* 		  then an exception is thrown.
*/
int Ace::Power(int base, int exponent)
{
	//Allows access to mathematical errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Ensure the base and exponent are acceptable values for pow().
	//is functions prevent mathematical exception from being thrown.
	if (((base == 0) && islessequal(exponent, 0)) || isless(base, 0))
	{
		throw "Ace Domain Error: Power() Parameters are not in valid range.";
		return -1;
	}
	else
	{
		//Clear all previous errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = pow(base, exponent);

		//Check for possible errors as a result of calling pow()
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: Power() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: Power() Calculation resulted in a range error.";
			return -1;
		}

		return result;
	}
}

/**
* Converts a number from radians to degrees.
*
* @author Katianie
* @date 5/16/2016
*
* @code
* 	double rad = 1.570796;
* 	double deg = Ace::RadiansToDegrees(rad);
* 	printf("%f\n", deg);
*
* @endcode
* Output:89.999981
*
* @param [in] radians Radians to convert.
*
* @return Degrees.
*/
double Ace::RadiansToDegrees(double radians)
{
	return (radians * 180.0) / PI;
}

/**
* Converts a number from radians to degrees.
*
* @author Katianie
* @date 5/16/2016
*
* @code
* 	float rad = 1.570796f;
* 	float deg = Ace::RadiansToDegrees(rad);
* 	printf("%f\n", deg);
*
* @endcode
* Output:89.999981
*
* @param [in] radians Radians to convert.
*
* @return Degrees.
*/
float Ace::RadiansToDegrees(float radians)
{
	return (radians * 180.0f) / PI;
}

/**
* Converts a number from radians to degrees.
*
* @author Katianie
* @date 5/16/2016
*
* @code
* 	int rad = 1;
* 	int deg = Ace::RadiansToDegrees(rad);
* 	printf("%d\n", deg);
*
* @endcode
* Output:57
*
* @param [in] radians Radians to convert.
*
* @return Degrees.
*/
int Ace::RadiansToDegrees(int radians)
{
	return (radians * 180) / PI;
}

/**
* Opens a file for reading and returns a char buffer containing
* some or all of the data in the file. Created in compliance with
* the CERT Coding Standard.
*
* @author Katianie
* @date 10/12/2016
*
* @code
* 	try
* 	{
* 		char* retVal = Ace::ReadTextFile("C:\\dicks.txt");
* 		printf("%s\n", retVal);
* 		_aligned_free(retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:dicksdicksdicksdicksdicksdicksdick8=>
*
* @param [in] filePath The path to the file.
* @param numBytesToRead The size of the buffer (the number bytes read in). If 0; numBytesToRead = maxBufferSize.
* @param maxBufferSize Maximum number of bytes to read.
* @param [in] mode The mode for fopen_s ("r" for read, "r+" for read/write, etc).
* @param seekOffset Either zero, or a value returned by ftell().
* @param seekOrigin Position used as reference for the offset (SEEK_SET represents beginning of file).
* @param [out] outEndCursorPos The value returned by ftell() after doing the read.
*
* @return The buffer containing the file data of size numBytesToRead.
* 		  Throws an exception if error occurs.
*/
char* Ace::ReadTextFile(char* filePath, size_t numBytesToRead, size_t maxBufferSize, const char* mode, int seekOffset, int seekOrigin, long* outEndCursorPos)
{
	size_t numItems = 0;
	size_t bufferSize = 0;
	char* buffer = NULL;
	FILE* file = NULL;

	//0 indicates to read as much as possible (the max specified).
	if (numBytesToRead == 0)
	{
		numBytesToRead = maxBufferSize;
	}

	if (filePath != NULL && mode != NULL)
	{
		//Ensure the filePath does NOT lead to a device.
		if (Ace::IsPathADevice(filePath) == false)
		{
			//Ensure there are no errors in opening the file.
			if (fopen_s(&file, filePath, mode) == 0)
			{
				//Set the cursor location (back to the beginning of the file by default).
				if (fseek(file, seekOffset, seekOrigin) != 0)
				{
					fclose(file);
					throw "Ace Error: ReadTextFile() Could not change file cursor position.";
					return NULL;
				}

				//Calculate the size of the buffer in bytes.
				bufferSize = numBytesToRead * sizeof(char);

				//Create the buffer to store file data in.
				buffer = (char*)_aligned_malloc(bufferSize, BYTE_ALIGNMENT);

				//Ensure the buffer was allocated.
				if (buffer == NULL)
				{
					fclose(file);
					throw "Ace Error: ReadTextFile() Buffer could not be allocated.";
					return NULL;
				}

				//Clear any garbage data in the buffer.
				if (SecureZeroMemory(buffer, bufferSize) == NULL)
				{
					_aligned_free(buffer);
					throw "Ace Error: ReadTextFile() Failed to zero out buffer.";
					return NULL;
				}

				//Read the data from the file.
				numItems = fread_s(buffer, bufferSize, sizeof(char), numBytesToRead, file);

				//Check for read errors.
				if (numItems <= 0)
				{
					fclose(file);
					_aligned_free(buffer);
					throw "Ace Error: ReadTextFile() File could not be read.";
					return NULL;
				}

				//Get the value of ftell() if outEndCursorPos is provided.
				if (outEndCursorPos != NULL)
				{
					*outEndCursorPos = ftell(file);
				}

				//Ensure the file is closed without errors.
				if (fclose(file) != 0)
				{
					_aligned_free(buffer);
					throw "Ace Error: ReadTextFile() File did not close properly.";
					return NULL;
				}

			}
			else
			{
				throw "Ace Error: ReadTextFile() Unable to open/locate file.";
			}
		}
		else
		{
			throw "Ace Error: ReadTextFile() File path cannot be a device.";
		}
	}
	else
	{
		throw "Ace Error: ReadTextFile() Parameters cannot be NULL.";
	}

	return buffer;
}

/**
* Opens a file for reading and returns a wide char buffer containing
* some or all of the data in the file. Created in compliance with the
* CERT Coding Standard.
*
* @author Katianie
* @date 10/13/2016
*
* @code
* 	try
* 	{
* 		//By default, read mode is set to "r, ccs=UTF-8".
* 		wchar_t* retVal = Ace::ReadTextFileW(L"C:\\jap.txt");
* 		MessageBoxW(NULL, retVal, NULL, 0);
* 		_aligned_free(retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:わたし　わ　エドワド　オ’ハゲン　です。　これ　は　なん　です　か？
*
* @param [in] filePath The path to the file.
* @param numBytesToRead The size of the buffer (the number bytes read in). If 0; numBytesToRead = maxBufferSize.
* @param maxBufferSize Maximum number of bytes to read.
* @param [in] mode The mode for fopen_s ("r" for read, "r, ccs=UTF-8" for read unicode, "r+" for read/write, etc).
* @param seekOffset Either zero, or a value returned by ftell().
* @param seekOrigin Position used as reference for the offset (SEEK_SET represents beginning of file).
* @param [out] outEndCursorPos The value returned by ftell() after doing the read.
*
* @return The buffer containing the file data of size numBytesToRead.
* 		  Throws an exception if error occurs.
*/
wchar_t* Ace::ReadTextFileW(wchar_t* filePath, size_t numBytesToRead, size_t maxBufferSize, const wchar_t* mode, int seekOffset, int seekOrigin, long* outEndCursorPos)
{
	size_t numItems = 0;
	size_t bufferSize = 0;
	wchar_t* buffer = NULL;
	FILE* file = NULL;

	//0 indicates to read as much as possible (the max specified).
	if (numBytesToRead == 0)
	{
		numBytesToRead = maxBufferSize;
	}

	if (filePath != NULL && mode != NULL)
	{
		//Ensure the filePath does NOT lead to a device.
		if (Ace::IsPathADevice(filePath) == false)
		{
			//Ensure there are no errors in opening the file.
			if (_wfopen_s(&file, filePath, mode) == 0)
			{
				//Set the cursor location (back to the beginning of the file by default).
				if (fseek(file, seekOffset, seekOrigin) != 0)
				{
					fclose(file);
					throw "Ace Error: ReadTextFileW() Could not change file cursor position.";
					return NULL;
				}

				//Calculate the size of the buffer in bytes.
				bufferSize = numBytesToRead * sizeof(wchar_t);

				//Create the buffer to store file data in.
				buffer = (wchar_t*)_aligned_malloc(bufferSize, BYTE_ALIGNMENT);

				//Ensure the buffer was allocated.
				if (buffer == NULL)
				{
					fclose(file);
					throw "Ace Error: ReadTextFileW() Buffer could not be allocated.";
					return NULL;
				}

				//Clear any garbage data in the buffer.
				if (SecureZeroMemory(buffer, bufferSize) == NULL)
				{
					_aligned_free(buffer);
					throw "Ace Error: ReadTextFileW() Failed to zero out buffer.";
					return NULL;
				}

				//Read the data from the file.
				numItems = fread_s(buffer, bufferSize, sizeof(wchar_t), numBytesToRead, file);

				//Check for read errors.
				if (numItems <= 0)
				{
					fclose(file);
					_aligned_free(buffer);
					throw "Ace Error: ReadTextFileW() File could not be read.";
					return NULL;
				}

				//Get the value of ftell() if outEndCursorPos is provided.
				if (outEndCursorPos != NULL)
				{
					*outEndCursorPos = ftell(file);
				}

				//Ensure the file is closed without errors.
				if (fclose(file) != 0)
				{
					_aligned_free(buffer);
					throw "Ace Error: ReadTextFileW() File did not close properly.";
					return NULL;
				}

			}
			else
			{
				throw "Ace Error: ReadTextFileW() Unable to open file.";
			}
		}
		else
		{
			throw "Ace Error: ReadTextFileW() File path cannot be a device.";
		}
	}
	else
	{
		throw "Ace Error: ReadTextFileW() Parameters cannot be NULL.";
	}

	return buffer;
}

/**
* Opens a file for reading and returns a char buffer containing
* some or all of the data in the file. Created in compliance with the
* CERT Coding Standard.
*
* @author Katianie
* @date 10/13/2016
*
* @code
* 	try
* 	{
* 		char* retVal = Ace::ReadBinaryFile("C:\\lookupdata.bin");
* 		printf("%x\0", (unsigned int)retVal);
* 		_aligned_free(retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:(binary file data can't be displayed).
*
* @param [in] filePath The path to the file.
* @param numBytesToRead The size of the buffer (the number bytes read in). If 0; numBytesToRead = maxBufferSize.
* @param maxBufferSize Maximum number of bytes to read.
* @param [in] mode The mode for fopen_s ("r" for read, "r+" for read/write, etc).
* @param seekOffset Number of bytes to offset from origin.
* @param seekOrigin Position used as reference for the offset (SEEK_SET represents beginning of file).
* @param [out] outEndCursorPos The value returned by ftell() after doing the read.
*
* @return The buffer containing the file data of size numBytesToRead.
* 		  Throws an exception if error occurs.
*/
char* Ace::ReadBinaryFile(char* filePath, size_t numBytesToRead, size_t maxBufferSize, const char* mode, int seekOffset, int seekOrigin, long* outEndCursorPos)
{
	return ReadTextFile(filePath, numBytesToRead, maxBufferSize, mode, seekOffset, seekOrigin, outEndCursorPos);
}

/**
* Opens a file for reading and returns a wide char buffer containing
* some or all of the data in the file. Created in compliance with the
* CERT Coding Standard.
*
* @author Katianie
* @date 10/13/2016
*
* @code
* 	try
* 	{
* 		//By default, read mode is set to "r, ccs=UTF-8".
* 		wchar_t* retVal = Ace::ReadBinaryFileW(L"C:\\lookupdata.bin");
* 		printf("%x\0", (unsigned int)retVal);
* 		_aligned_free(retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:(binary file data can't be displayed).
*
* @param [in] filePath The path to the file.
* @param numBytesToRead The size of the buffer (the number bytes read in). If 0; numBytesToRead = maxBufferSize.
* @param maxBufferSize Maximum number of bytes to read.
* @param [in] mode The mode for fopen_s ("r" for read, "r, ccs=UTF-8" for read unicode, "r+" for read/write, etc).
* @param seekOffset Number of bytes to offset from origin.
* @param seekOrigin Position used as reference for the offset (SEEK_SET represents beginning of file).
* @param [out] outEndCursorPos The value returned by ftell() after doing the read.
*
* @return The buffer containing the file data of size numBytesToRead.
* 		  Throws an exception if error occurs.
*/
wchar_t* Ace::ReadBinaryFileW(wchar_t* filePath, size_t numBytesToRead, size_t maxBufferSize, const wchar_t* mode, int seekOffset, int seekOrigin, long* outEndCursorPos)
{
	return ReadTextFileW(filePath, numBytesToRead, maxBufferSize, mode, seekOffset, seekOrigin, outEndCursorPos);
}

/**
* Calculates the Computes The IEEE remainder of the floating point
* division operation numerator/denominator. In contrast to Mod();
* the returned value is not guaranteed to have the same sign as
* the numerator. Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/5/2016
*
* @code
*	try
*	{
*		double result = Ace::Remainder(-5.1, 3.2);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:1.300000
*
* @param numerator Number to be divided.
* @param denominator Number to divide the numerator by.
*
* @return The IEEE remainder of the floating point division.
* 		  If a mathematical error occurs or denominator
* 		  equals 0, then an exception is thrown.
*/
double Ace::Remainder(double numerator, double denominator)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	if (denominator != 0)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = remainder(numerator, denominator);

		//Look for any mathematical errors as a result of calling remainder().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: Remainder() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: Remainder() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Error: Remainder() denominator cannot be 0";
	}

	return result;
}

/**
* Calculates the Computes The IEEE remainder of the floating point
* division operation numerator/denominator. In contrast to Mod();
* the returned value is not guaranteed to have the same sign as
* the numerator. Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float result = Ace::Remainder(-5.1f, 3.2f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:1.300000
*
* @param numerator Number to be divided.
* @param denominator Number to divide the numerator by.
*
* @return The IEEE remainder of the floating point division.
* 		  If a mathematical error occurs or denominator
* 		  equals 0, then an exception is thrown.
*/
float Ace::Remainder(float numerator, float denominator)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	if (denominator != 0)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = remainder(numerator, denominator);

		//Look for any mathematical errors as a result of calling remainder().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: Remainder() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: Remainder() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Error: Remainder() denominator cannot be 0";
	}

	return result;
}

/**
* Calculates the Computes The IEEE remainder of the floating point
* division operation numerator/denominator. In contrast to Mod();
* the returned value is not guaranteed to have the same sign as
* the numerator. Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		int result = Ace::Remainder(-5, 3);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:1
*
* @param numerator Number to be divided.
* @param denominator Number to divide the numerator by.
*
* @return The IEEE remainder of the floating point division.
* 		  If a mathematical error occurs or denominator
* 		  equals 0, then an exception is thrown.
*/
int Ace::Remainder(int numerator, int denominator)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	if (denominator != 0)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = remainder(numerator, denominator);

		//Look for any mathematical errors as a result of calling remainder().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: Remainder() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: Remainder() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Error: Remainder() denominator cannot be 0";
	}

	return result;
}

/**
* Calculates the floating-point remainder of the division operation
* numerator/denominator the same way the Remainder() function does.
* Additionally, the sign and at least the three of the last bits of
* numerator/denominator will be stored in outQuadrant, sufficient to
* determine the quadrant of the result within a period.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/5/2016
*
* @code
*	int quadrant;
*	double result = Ace::RemainderWithQuadrant(-5.1, 3.2, &quadrant);
*	quadrant = (unsigned int)quadrant % 4;
*	printf("%f Quadrant:%d\n", result, quadrant);
*
* @endcode
* Output:1.300000 Quadrant:2
*
* @param numerator Number to be divided.
* @param denominator Number to divide the numerator by.
* @param outQuadrant Sign to indicate the quadrant.
*
* @return The IEEE remainder of the floating point division
* 		  and the quadrant data. If a mathematical error occurs
* 		  or denominator equals 0, then an exception is thrown.
*/
double Ace::RemainderWithQuadrant(double numerator, double denominator, int* outQuadrant)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	if (denominator != 0)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = remquo(numerator, denominator, outQuadrant);

		//Look for any mathematical errors as a result of calling remquo().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: RemainderWithQuadrant() Calculation resulted in a range error.";
			return -1.0;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: RemainderWithQuadrant() Calculation resulted in a range error.";
			return -1.0;
		}
	}
	else
	{
		throw "Ace Error: RemainderWithQuadrant() denominator cannot be 0";
	}

	return result;
}

/**
* Calculates the floating-point remainder of the division operation
* numerator/denominator the same way the Remainder() function does.
* Additionally, the sign and at least the three of the last bits of
* numerator/denominator will be stored in outQuadrant, sufficient to
* determine the quadrant of the result within a period.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	int quadrant;
*	float result = Ace::RemainderWithQuadrant(-5.1f, 3.2f, &quadrant);
*	quadrant = (unsigned int)quadrant % 4;
*	printf("%f Quadrant:%d\n", result, quadrant);
*
* @endcode
* Output:1.300000 Quadrant:2
*
* @param numerator Number to be divided.
* @param denominator Number to divide the numerator by.
* @param outQuadrant Sign to indicate the quadrant.
*
* @return The IEEE remainder of the floating point division
* 		  and the quadrant data. If a mathematical error occurs
* 		  or denominator equals 0, then an exception is thrown.
*/
float Ace::RemainderWithQuadrant(float numerator, float denominator, int* outQuadrant)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	if (denominator != 0)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = remquo(numerator, denominator, outQuadrant);

		//Look for any mathematical errors as a result of calling remquo().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: RemainderWithQuadrant() Calculation resulted in a range error.";
			return -1.0f;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: RemainderWithQuadrant() Calculation resulted in a range error.";
			return -1.0f;
		}
	}
	else
	{
		throw "Ace Error: denominator cannot be 0";
	}

	return result;
}

/**
* Calculates the floating-point remainder of the division operation
* numerator/denominator the same way the Remainder() function does.
* Additionally, the sign and at least the three of the last bits of
* numerator/denominator will be stored in outQuadrant, sufficient to
* determine the quadrant of the result within a period.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	int quadrant;
*	int result = Ace::RemainderWithQuadrant(-5, 3, &quadrant);
*	quadrant = (unsigned int)quadrant % 4;
*	printf("%d Quadrant:%d\n", result, quadrant);
*
* @endcode
* Output:1 Quadrant:2
*
* @param numerator Number to be divided.
* @param denominator Number to divide the numerator by.
* @param outQuadrant Sign to indicate the quadrant.
*
* @return The IEEE remainder of the floating point division
* 		  and the quadrant data. If a mathematical error occurs
* 		  or denominator equals 0, then an exception is thrown.
*/
int Ace::RemainderWithQuadrant(int numerator, int denominator, int* outQuadrant)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	if (denominator != 0)
	{
		//Clear all previous floating point errors.
		if ((math_errhandling & MATH_ERREXCEPT) > 0)
		{
			feclearexcept(FE_ALL_EXCEPT);
		}
		errno = 0;

		result = remquo(numerator, denominator, outQuadrant);

		//Look for any mathematical errors as a result of calling remquo().
		if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
		{
			throw "Ace Range Error: RemainderWithQuadrant() Calculation resulted in a range error.";
			return -1;
		}
		else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
		{
			throw "Ace Range Error: RemainderWithQuadrant() Calculation resulted in a range error.";
			return -1;
		}
	}
	else
	{
		throw "Ace Error: denominator cannot be 0";
	}

	return result;
}

/**
* Removes all whitespace from a given string. A newly allocated string is returned that has all
* whitespace removed from it.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* temp = Ace::RemoveAllWhitespace("E dd\t i  e ");
* 		printf("%s\n", temp);
* 		_aligned_free(temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie
*
* @param [in] str The string to remove whitespace from.
*
* @return A newly created string containing no whitespace.
* 		  Throws an exception if str is NULL.
*/
char* Ace::RemoveAllWhitespace(char* str)
{
	char* buffer = NULL;

	if (str != NULL)
	{
		try
		{
			buffer = Ace::AllocateAndCopyString(str);
			Ace::RemoveAllWhitespaceInPlace(buffer);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RemoveAllWhitespace()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RemoveAllWhitespace() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RemoveAllWhitespace() str cannot be NULL.";
	}

	return buffer;
}

/**
* Removes all whitespace from a given wide string. A newly allocated string is returned that has all
* whitespace removed from it.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* temp = Ace::RemoveAllWhitespace(L"E dd\t i  e ");
* 		printf("%S\n", temp);
* 		_aligned_free(temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie
*
* @param [in] str The string to remove whitespace from.
*
* @return A newly created string containing no whitespace.
* 		  Throws an exception if str is NULL.
*/
wchar_t* Ace::RemoveAllWhitespace(wchar_t* str)
{
	wchar_t* buffer = NULL;

	if (str != NULL)
	{
		try
		{
			buffer = Ace::AllocateAndCopyString(str);
			Ace::RemoveAllWhitespaceInPlace(buffer);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RemoveAllWhitespace()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RemoveAllWhitespace() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RemoveAllWhitespace() str cannot be NULL.";
	}

	return buffer;
}

/**
* Removes all whitespace from a given string in place. This means That str
* is modified directly and as a result, no return value is needed. Assumes str is an
* allocated string and not a string literal.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* temp = Ace::AllocateAndCopyString("E dd\t i  e ");
* 		Ace::RemoveAllWhitespaceInPlace(temp);
* 		printf("%s\n", temp);
* 		_aligned_free(temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie
*
* @param [in,out] str The string to directly modify and delete whitespace from.
* 				  Throws an exception if str is NULL.
*/
void Ace::RemoveAllWhitespaceInPlace(char* str)
{
	int tempWordIndex = 0;
	int numWhiteSpaceSkipped = 0;

	//We will modify the existing string in place without allocating a new one.
	if (str != NULL)
	{
		//Now go through the whole word and filter out anything that's a space.
		for (size_t currIndex = 0; currIndex < strlen(str); currIndex++)
		{
			if (Ace::IsWhitespace(str[currIndex]) == false)
			{
				str[tempWordIndex] = str[currIndex];
				tempWordIndex++;
			}
		}

		//End the string at 'tempWordIndex' since it is smaller then the original.
		str[tempWordIndex] = '\0';
	}
	else
	{
		throw "Ace Error: RemoveAllWhitespaceInPlace() str cannot be NULL.";
	}
}

/**
* Removes all whitespace from a given wide string in place. This means That str
* is modified directly and as a result, no return value is needed. Assumes str is an
* allocated string and not a string literal.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* temp = Ace::AllocateAndCopyString(L"E dd\t i  e ");
* 		Ace::RemoveAllWhitespaceInPlace(temp);
* 		printf("%S\n", temp);
* 		_aligned_free(temp);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie
*
* @param [in,out] str The wide string to directly modify and delete whitespace from.
* 				  Throws an exception if str is NULL.
*/
void Ace::RemoveAllWhitespaceInPlace(wchar_t* str)
{
	int tempWordIndex = 0;
	int numWhiteSpaceSkipped = 0;

	//We will modify the existing string in place without allocating a new one.
	if (str != NULL)
	{
		//Now go through the whole word and filter out anything that's a space.
		for (size_t currIndex = 0; currIndex < wcslen(str); currIndex++)
		{
			if (Ace::IsWhitespace(str[currIndex]) == false)
			{
				str[tempWordIndex] = str[currIndex];
				tempWordIndex++;
			}
		}

		//End the string at 'tempWordIndex' since it is smaller then the original.
		str[tempWordIndex] = L'\0';
	}
	else
	{
		throw "Ace Error: RemoveAllWhitespaceInPlace() str cannot be NULL.";
	}
}

/**
* Removes any occurrence of charToRemove from strToRemoveFrom.
*
* @author Katianie
* @date 6/12/2016
*
* @code
* 	try
* 	{
* 		char* data = "Has anyone ever been as far as decided to be there and then look more like?";
* 		char* removedCharData = Ace::RemoveCharactersFromString(data, 'a');
* 		printf("%s\n", removedCharData);
* 		_aligned_free(removedCharData);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Hs nyone ever been s fr s decided to be there nd then look more like?
*
* @param [in,out] strToRemoveFrom If non-null, to remove from.
* @param charToRemove The character to remove.
*
* @return A newly created string with every occurrence of charToRemove removed.
* 		  Throws an exception if strToRemoveFrom is NULL.
*/
char* Ace::RemoveCharactersFromString(char* strToRemoveFrom, char charToRemove)
{
	char* strToReturn = NULL;

	if (strToRemoveFrom != NULL)
	{
		try
		{
			strToReturn = Ace::AllocateAndCopyString(strToRemoveFrom);
			Ace::RemoveCharactersFromStringInPlace(strToReturn, charToRemove);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RemoveCharactersFromString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RemoveCharactersFromString() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RemoveCharactersFromString() strToRemoveFrom cannot be NULL.";
	}

	return strToReturn;
}

/**
* Removes any occurrence of charToRemove from strToRemoveFrom.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = "Has anyone ever been as far as decided to be there and then look more like?";
* 		wchar_t* removedCharData = Ace::RemoveCharactersFromString(data, L'a');
* 		printf("%S\n", removedCharData);
* 		_aligned_free(removedCharData);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Hs nyone ever been s fr s decided to be there nd then look more like?
*
* @param [in,out] strToRemoveFrom If non-null, to remove from.
* @param charToRemove The character to remove.
*
* @return A newly created string with every occurrence of charToRemove removed.
* 		  Throws an exception if strToRemoveFrom is NULL.
*/
wchar_t* Ace::RemoveCharactersFromString(wchar_t* strToRemoveFrom, wchar_t charToRemove)
{
	wchar_t* strToReturn = NULL;

	if (strToRemoveFrom != NULL)
	{
		try
		{
			strToReturn = Ace::AllocateAndCopyString(strToRemoveFrom);
			Ace::RemoveCharactersFromStringInPlace(strToReturn, charToRemove);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RemoveCharactersFromString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RemoveCharactersFromString() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RemoveCharactersFromString() strToRemoveFrom cannot be NULL.";
	}

	return strToReturn;
}

/**
* Removes any occurrence of charToRemove from strToRemoveFrom.
* strToRemoveFrom must be an allocated string since this function
* works directly on it.
*
* @author Katianie
* @date 6/12/2016
*
* @code
* 	try
* 	{
* 		char* removedCharData = Ace::AllocateAndCopyString("Has anyone ever been as far as decided to be there and then look more like?");
* 		Ace::RemoveCharactersFromStringInPlace(removedCharData, 'a');
* 		printf("%s", removedCharData);
* 		_aligned_free(removedCharData);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Hs nyone ever been s fr s decided to be there nd then look more like?
*
* @param [in,out] strToRemoveFrom If non-null, to remove from.
* @param charToRemove The character to remove.
*
* @return Throws an exception if strToRemoveFrom is NULL.
*/
void Ace::RemoveCharactersFromStringInPlace(char* strToRemoveFrom, char charToRemove)
{
	//We will modify the existing string in place without allocating a new one.
	int wordLength;
	int tempWordIndex;

	if (strToRemoveFrom != NULL)
	{
		wordLength = strlen(strToRemoveFrom) + 1;
		tempWordIndex = 0;

		//Now go through the whole word and filter out anything that's a new line.
		for (int i = 0; i < wordLength; i++)
		{
			//Only copy over the character if it is NOT charToRemove.
			if (*(strToRemoveFrom + i) != charToRemove)
			{
				*(strToRemoveFrom + tempWordIndex) = *(strToRemoveFrom + i);
				tempWordIndex++;
			}
		}

		//End the string at 'tempWordIndex' since it is smaller than the original.
		*(strToRemoveFrom + tempWordIndex) = '\0';
	}
	else
	{
		throw "Ace Error: RemoveCharactersFromStringInPlace() strToRemoveFrom cannot be NULL.";
	}
}

/**
* Removes any occurrence of charToRemove from strToRemoveFrom.
* strToRemoveFrom must be an allocated string since this function
* works directly on it.
*
* @author Katianie
* @date 9/7/2016
*
* @code
* 	try
* 	{
* 		wchar_t* removedCharData = Ace::AllocateAndCopyString(L"Has anyone ever been as far as decided to be there and then look more like?");
* 		Ace::RemoveCharactersFromStringInPlace(removedCharData, L'a');
* 		printf("%S", removedCharData);
* 		_aligned_free(removedCharData);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Hs nyone ever been s fr s decided to be there nd then look more like?
*
* @param [in,out] strToRemoveFrom If non-null, to remove from.
* @param charToRemove The character to remove.
*
* @return Throws an exception if strToRemoveFrom is NULL.
*/
void Ace::RemoveCharactersFromStringInPlace(wchar_t* strToRemoveFrom, wchar_t charToRemove)
{
	//We will modify the existing string in place without allocating a new one.
	int wordLength;
	int tempWordIndex;

	if (strToRemoveFrom != NULL)
	{
		wordLength = wcslen(strToRemoveFrom) + 1;
		tempWordIndex = 0;

		//Now go through the whole word and filter out anything that's a new line.
		for (int i = 0; i < wordLength; i++)
		{
			//Only copy over the character if it is NOT charToRemove.
			if (*(strToRemoveFrom + i) != charToRemove)
			{
				*(strToRemoveFrom + tempWordIndex) = *(strToRemoveFrom + i);
				tempWordIndex++;
			}
		}

		//End the string at 'tempWordIndex' since it is smaller than the original.
		*(strToRemoveFrom + tempWordIndex) = L'\0';
	}
	else
	{
		throw "Ace Error: RemoveCharactersFromStringInPlace() strToRemoveFrom cannot be NULL.";
	}
}

/**
* Removes the specified string (strToLookFor) from strToSearch
* and returns a newly allocated buffer containing the result.
*
* @author Katianie
* @date 11/2/2016
*
* @code
* 	try
* 	{
* 		char* retVal = Ace::RemoveStringFromString("Eddie has a small small dick.", "small ");
* 		printf("%s\n", retVal);
* 		_aligned_free(retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie has a dick.
*
* @param [in,out] strToSearch The string to directly parse and modify.
* @param [in] strToLookFor The substring to look for.
* @param removeOnlyFirstOccurrence True if only the first occurrence
* 								   of strToLookFor should be removed.
*
* @return Throws an exception if an error occurs.
*/
char* Ace::RemoveStringFromString(char* strToRemoveFrom, char* strToRemove, bool removeOnlyFirstOccurrence)
{
	char* buffer = NULL;

	if (strToRemoveFrom != NULL && strToRemove != NULL)
	{
		try
		{
			buffer = Ace::AllocateAndCopyString(strToRemoveFrom);
			Ace::RemoveStringFromStringInPlace(buffer, strToRemove);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RemoveStringFromString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RemoveStringFromString() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RemoveStringFromString() Parameters cannot be NULL.";
	}

	return buffer;
}

/**
* Removes the specified string (strToLookFor) from strToSearch
* and returns a newly allocated buffer containing the result.
*
* @author Katianie
* @date 11/2/2016
*
* @code
* 	try
* 	{
* 		wchar_t* retVal = Ace::RemoveStringFromString(L"Eddie has a small small dick.", L"small ");
* 		printf("%S\n", retVal);
* 		_aligned_free(retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie has a dick.
*
* @param [in,out] strToSearch The string to directly parse and modify.
* @param [in] strToLookFor The substring to look for.
* @param removeOnlyFirstOccurrence True if only the first occurrence
* 								   of strToLookFor should be removed.
*
* @return Throws an exception if an error occurs.
*/
wchar_t* Ace::RemoveStringFromString(wchar_t* strToRemoveFrom, wchar_t* strToRemove, bool removeOnlyFirstOccurrence)
{
	wchar_t* buffer = NULL;

	if (strToRemoveFrom != NULL && strToRemove != NULL)
	{
		try
		{
			buffer = Ace::AllocateAndCopyString(strToRemoveFrom);
			Ace::RemoveStringFromStringInPlace(buffer, strToRemove);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RemoveStringFromString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RemoveStringFromString() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RemoveStringFromString() Parameters cannot be NULL.";
	}

	return buffer;
}

/**
* Removes the specified string (strToLookFor) by directly modifying
* strToSearch.
*
* @author Katianie
* @date 11/2/2016
*
* @code
* 	try
* 	{
* 		char* buffer = Ace::AllocateAndCopyString("Eddie has a small small dick.");
* 		Ace::RemoveStringFromStringInPlace(buffer, "small ");
* 		printf("%s\n", buffer);
* 		_aligned_free(buffer);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie has a dick.
*
* @param [in,out] strToSearch The string to directly parse and modify.
* @param [in] strToLookFor The substring to look for.
* @param removeOnlyFirstOccurrence True if only the first occurrence
* 								   of strToLookFor should be removed.
*
* @return Throws an exception if an error occurs.
*/
void Ace::RemoveStringFromStringInPlace(char*& strToSearch, char* strToLookFor, bool removeOnlyFirstOccurrence)
{
	char* leftHalf;
	char* rightHalf;
	char* tempSub;
	int strToLookForLen;
	int strToSearchLen;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling strlen().
		strToLookForLen = strlen(strToLookFor);
		strToSearchLen = strlen(strToSearch) + 1;

		try
		{
			for (int currIndex = 0; currIndex < (strToSearchLen - strToLookForLen) + 1; currIndex++)
			{
				tempSub = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);

				//Found the substring to replace.
				if (strcmp(tempSub, strToLookFor) == 0)
				{
					//Take the portion of the string that comes before our found substring.
					leftHalf = Ace::SubString(strToSearch, 0, currIndex);

					//Take the portion of the string that comes after our found substring.
					rightHalf = Ace::SubString(strToSearch, currIndex + strToLookForLen);

					//Free the current strToSearch since we are overwriting it.
					_aligned_free(strToSearch);

					//Combine the left and right parts which effectively deletes the word.
					strToSearch = Ace::Concatenate(2, leftHalf, rightHalf);

					//Go back one spot since we just combined.
					currIndex--;

					//Re-get the length since it changed with concatenate.
					strToSearchLen = strlen(strToSearch) + 1;

					_aligned_free(leftHalf);
					_aligned_free(rightHalf);
					_aligned_free(tempSub);

					if (removeOnlyFirstOccurrence == true)
					{
						break; //Kick Out The Epic Mother Fucker!
					}
				}
				else
				{
					//No match found.
					_aligned_free(tempSub);
				}
			}
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RemoveStringFromStringInPlace()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RemoveStringFromStringInPlace() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RemoveStringFromStringInPlace() Parameters cannot be NULL.";
	}
}

/**
* Removes the specified string (strToLookFor) by directly modifying
* strToSearch.
*
* @author Katianie
* @date 11/2/2016
*
* @code
* 	try
* 	{
* 		wchar_t* buffer = Ace::AllocateAndCopyString(L"Eddie has a small small dick.");
* 		Ace::RemoveStringFromStringInPlace(buffer, L"small ");
* 		printf("%S\n", buffer);
* 		_aligned_free(buffer);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie has a dick.
*
* @param [in,out] strToSearch The string to directly parse and modify.
* @param [in] strToLookFor The substring to look for.
* @param removeOnlyFirstOccurrence True if only the first occurrence
* 								   of strToLookFor should be removed.
*
* @return Throws an exception if an error occurs.
*/
void Ace::RemoveStringFromStringInPlace(wchar_t*& strToSearch, wchar_t* strToLookFor, bool removeOnlyFirstOccurrence)
{
	wchar_t* leftHalf;
	wchar_t* rightHalf;
	wchar_t* tempSub;
	int strToLookForLen;
	int strToSearchLen;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling strlen().
		strToLookForLen = wcslen(strToLookFor);
		strToSearchLen = wcslen(strToSearch) + 1;

		try
		{
			for (int currIndex = 0; currIndex < (strToSearchLen - strToLookForLen) + 1; currIndex++)
			{
				tempSub = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);

				//Found the substring to replace.
				if (wcscmp(tempSub, strToLookFor) == 0)
				{
					//Take the portion of the string that comes before our found substring.
					leftHalf = Ace::SubString(strToSearch, 0, currIndex);

					//Take the portion of the string that comes after our found substring.
					rightHalf = Ace::SubString(strToSearch, currIndex + strToLookForLen);

					//Free the current strToSearch since we are overwriting it.
					_aligned_free(strToSearch);

					//Combine the left and right parts which effectively deletes the word.
					strToSearch = Ace::ConcatenateW(2, leftHalf, rightHalf);

					//Go back one spot since we just combined.
					currIndex--;

					//Re-get the length since it changed with concatenate.
					strToSearchLen = wcslen(strToSearch) + 1;

					_aligned_free(leftHalf);
					_aligned_free(rightHalf);
					_aligned_free(tempSub);

					if (removeOnlyFirstOccurrence == true)
					{
						break; //Kick Out The Epic Mother Fucker!
					}
				}
				else
				{
					//No match found.
					_aligned_free(tempSub);
				}
			}
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RemoveStringFromStringInPlace()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RemoveStringFromStringInPlace() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RemoveStringFromStringInPlace() Parameters cannot be NULL.";
	}
}

/**
* Inserts strToReplaceWith in the position strToLookFor is located, then
* strToLookFor is removed. A newly allocated string is then returned.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = Ace::ReplaceString("Eddie is cool.", "is", "is not");
* 		printf("%s\n", data);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie is not cool.
*
* @param [in] strToSearch The string to look through.
* @param [in] strToLookFor The substring to look for.
* @param [in] strToReplaceWith The substring to replace with.
* @param removeOnlyFirstOccurrence True if only the first occurrence
* 								   of strToLookFor should be removed.
*
* @return A newly created string with the change. Throws an exception
* 		  if strToSearch or strToLookFor are NULL.
*/
char* Ace::ReplaceString(char* strToSearch, char* strToLookFor, char* strToReplaceWith, bool removeOnlyFirstOccurrence)
{
	char* strSearch = NULL;

	if (strToSearch != NULL && strToLookFor != NULL && strToReplaceWith != NULL)
	{
		try
		{
			strSearch = Ace::AllocateAndCopyString(strToSearch);
			Ace::ReplaceStringInPlace(strSearch, strToLookFor, strToReplaceWith, removeOnlyFirstOccurrence);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::ReplaceString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: ReplaceString() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: ReplaceString() Parameters cannot be NULL.";
	}

	return strSearch;
}

/**
* Inserts strToReplaceWith in the position strToLookFor is located, then
* strToLookFor is removed. A newly allocated wide string is then returned.
*
* @author Katianie
* @date 9/8/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = Ace::ReplaceString(L"Eddie is cool.", L"is", L"is not");
* 		printf("%S\n", data);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie is not cool.
*
* @param [in] strToSearch The wide string to look through.
* @param [in] strToLookFor The substring to look for.
* @param [in] strToReplaceWith The substring to replace with.
* @param removeOnlyFirstOccurrence True if only the first occurrence
* 								   of strToLookFor should be removed.
*
* @return A newly created wide string with the change. Throws an exception
* 		  if strToSearch or strToLookFor are NULL.
*/
wchar_t* Ace::ReplaceString(wchar_t* strToSearch, wchar_t* strToLookFor, wchar_t* strToReplaceWith, bool removeOnlyFirstOccurrence)
{
	wchar_t* strSearch = NULL;

	if (strToSearch != NULL && strToLookFor != NULL && strToReplaceWith != NULL)
	{
		try
		{
			strSearch = Ace::AllocateAndCopyString(strToSearch);
			Ace::ReplaceStringInPlace(strSearch, strToLookFor, strToReplaceWith, removeOnlyFirstOccurrence);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::ReplaceString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: ReplaceString() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: ReplaceString() Parameters cannot be NULL.";
	}

	return strSearch;
}

/**
* Directly modifies and inserts strToReplaceWith in the position
* strToLookFor is located, then strToLookFor is removed. Since strToSearch
* is modified directly, there is no return value. data is re-allocated in order to hold
* the replacement string.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = Ace::AllocateAndCopyString("Eddie is cool.");
* 		Ace::ReplaceStringInPlace(data, "is", "is not");
* 		printf("%s\n", data);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie is not cool.
*
* @param [in,out] strToSearch The string to directly parse and modify.
* @param [in] strToLookFor The substring to look for.
* @param [in] strToReplaceWith The substring to replace with.
* @param removeOnlyFirstOccurrence True if only the first occurrence
* 								   of strToLookFor should be removed.
*
* @return Throws an exception if an error occurs.
*/
void Ace::ReplaceStringInPlace(char*& strToSearch, char* strToLookFor, char* strToReplaceWith, bool removeOnlyFirstOccurrence)
{
	char* leftHalf;
	char* rightHalf;
	char* tempSub;
	int strToLookForLen;
	int strToSearchLen;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling strlen().
		strToLookForLen = strlen(strToLookFor);
		strToSearchLen = strlen(strToSearch) + 1;

		for (int currIndex = 0; currIndex < (strToSearchLen - strToLookForLen) + 1; currIndex++)
		{
			try
			{
				tempSub = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);

				//Found the substring to replace.
				if (strcmp(tempSub, strToLookFor) == 0)
				{
					//Take the portion of the string that comes before our found substring.
					leftHalf = Ace::SubString(strToSearch, 0, currIndex);

					//Take the portion of the string that comes after our found substring.
					rightHalf = Ace::SubString(strToSearch, currIndex + strToLookForLen);

					//Free the current strToSearch since we are overwriting it.
					_aligned_free(strToSearch);

					//Combine the two halves with the replacement in the middle, this effectively removes the previous substring.
					strToSearch = Ace::Concatenate(3, leftHalf, strToReplaceWith, rightHalf);

					//Re-get the length since it changed with concatenate.
					strToSearchLen = strlen(strToSearch) + 1;

					_aligned_free(leftHalf);
					_aligned_free(rightHalf);
					_aligned_free(tempSub);

					if (removeOnlyFirstOccurrence == true)
					{
						break; //Kick Out The Epic Mother Fucker!
					}
				}
				else
				{
					//No match found.
					_aligned_free(tempSub);
				}
			}
			catch (char* exceptionMessage)
			{
				//Create a stack trace of the exception.
				char combinedMessage[1024];
				if (SecureZeroMemory(combinedMessage, 1024) != NULL)
				{
					if (sprintf_s(combinedMessage, 1024, "Ace::ReplaceStringInPlace()->%s\n", exceptionMessage) > 0)
					{
						throw combinedMessage;
					}
				}

				throw "Ace Error: ReplaceStringInPlace() Encountered an error.";
			}
		}
	}
	else
	{
		throw "Ace Error: ReplaceStringInPlace() Parameters cannot be NULL.";
	}
}

/**
* Directly modifies and inserts strToReplaceWith in the position
* strToLookFor is located, then strToLookFor is removed. Since strToSearch
* is modified directly, there is no return value. data is re-allocated in order to hold
* the replacement wide string.
*
* @author Katianie
* @date 9/8/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = Ace::AllocateAndCopyString(L"Eddie is cool.");
* 		Ace::ReplaceStringInPlace(data, L"is", L"is not");
* 		printf("%S\n", data);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Eddie is not cool.
*
* @param [in,out] strToSearch The wide string to directly parse and modify.
* @param [in] strToLookFor The substring to look for.
* @param [in] strToReplaceWith The substring to replace with.
* @param removeOnlyFirstOccurrence True if only the first occurrence
* 								   of strToLookFor should be removed.
*
* @return Throws an exception if an error occurs.
*/
void Ace::ReplaceStringInPlace(wchar_t*& strToSearch, wchar_t* strToLookFor, wchar_t* strToReplaceWith, bool removeOnlyFirstOccurrence)
{
	wchar_t* leftHalf;
	wchar_t* rightHalf;
	wchar_t* tempSub;
	int strToLookForLen;
	int strToSearchLen;

	if (strToSearch != NULL && strToLookFor != NULL)
	{
		//Record the lengths so we don't have to keep calling wcslen().
		strToLookForLen = wcslen(strToLookFor);
		strToSearchLen = wcslen(strToSearch) + 1;

		for (int currIndex = 0; currIndex < (strToSearchLen - strToLookForLen) + 1; currIndex++)
		{
			try
			{
				tempSub = Ace::SubString(strToSearch, currIndex, currIndex + strToLookForLen);

				//Found the substring to replace.
				if (wcscmp(tempSub, strToLookFor) == 0)
				{
					//Take the portion of the string that comes before our found substring.
					leftHalf = Ace::SubString(strToSearch, 0, currIndex);

					//Take the portion of the string that comes after our found substring.
					rightHalf = Ace::SubString(strToSearch, currIndex + strToLookForLen);

					//Free the current strToSearch since we are overwriting it.
					_aligned_free(strToSearch);

					//Combine the two halves with the replacement in the middle, this effectively removes the previous substring.
					strToSearch = Ace::ConcatenateW(3, leftHalf, strToReplaceWith, rightHalf);

					//Re-get the length since it changed with concatenate.
					strToSearchLen = wcslen(strToSearch) + 1;

					_aligned_free(leftHalf);
					_aligned_free(rightHalf);
					_aligned_free(tempSub);

					if (removeOnlyFirstOccurrence == true)
					{
						break; //Kick Out The Epic Mother Fucker!
					}
				}
				else if (tempSub != NULL)
				{
					//No match found.
					_aligned_free(tempSub);
				}
			}
			catch (char* exceptionMessage)
			{
				//Create a stack trace of the exception.
				char combinedMessage[1024];
				if (SecureZeroMemory(combinedMessage, 1024) != NULL)
				{
					if (sprintf_s(combinedMessage, 1024, "Ace::ReplaceStringInPlace()->%s\n", exceptionMessage) > 0)
					{
						throw combinedMessage;
					}
				}

				throw "Ace Error: ReplaceStringInPlace() Encountered an error.";
			}
		}
	}
	else
	{
		throw "Ace Error: ReplaceStringInPlace() Parameters cannot be NULL.";
	}
}

/**
* Finds the XML data between the startTag and endTag and returns it.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
*	{
*		char* data = Ace::ReadTextFile("C:\\Config.xml");
*		char* tagData = Ace::RetrieveDataBetweenTags(data, "<position>", "</position>");
*		printf("Data between tags:%s\n", tagData);
*		_aligned_free(tagData);
*		_aligned_free(data);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.1, 2.3, 2.2
*
* @param [in] fileData A string containing the data to be parsed.
* @param [in] startTag A string representing the tag to start from.
* @param [in] endTag A string representing the tag to end on.
*
* @return A Buffer containing the data between startTag + startTag.length() and
*		  endTag. Throws an exception if error occurs.
*/
char* Ace::RetrieveDataBetweenTags(char* fileData, char* startTag, char* endTag)
{
	int startIndex = -1;
	int	endIndex = -1;
	char* data = NULL;

	if (fileData != NULL && startTag != NULL && endTag != NULL)
	{
		try
		{
			startIndex = Ace::IndexOf(fileData, startTag);
			endIndex = Ace::IndexOf(fileData, endTag);

			//Add the length to the startIndex so we begin at the end of the startTag.
			startIndex = startIndex + strlen(startTag);

			//Get the data from the end of the start tag up to (but not including) the endIndex.
			data = Ace::RetrieveDataBetweenTags(fileData, startIndex, endIndex);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RetrieveDataBetweenTags()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RetrieveDataBetweenTags() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RetrieveDataBetweenTags() Parameters cannot be NULL.";
	}

	return data;
}

/**
* Finds the XML data between the startTag and endTag and returns it.
*
* @author Katianie
* @date 9/8/2016
*
* @code
* 	try
*	{
*		wchar_t* data = Ace::ReadTextFileW(L"C:\\Config.xml");
*		wchar_t* tagData = Ace::RetrieveDataBetweenTags(data, L"<position>", L"</position>");
*		printf("Data between tags:%S\n", tagData);
*		_aligned_free(tagData);
*		_aligned_free(data);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.1, 2.3, 2.2
*
* @param [in] fileData A string containing the data to be parsed.
* @param [in] startTag A string representing the tag to start from.
* @param [in] endTag A string representing the tag to end on.
*
* @return A Buffer containing the data between startTag + startTag.length() and
*		  endTag. Throws an exception if error occurs.
*/
wchar_t* Ace::RetrieveDataBetweenTags(wchar_t* fileData, wchar_t* startTag, wchar_t* endTag)
{
	int startIndex = -1;
	int	endIndex = -1;
	wchar_t* data = NULL;

	if (fileData != NULL && startTag != NULL && endTag != NULL)
	{
		try
		{
			startIndex = Ace::IndexOf(fileData, startTag);
			endIndex = Ace::IndexOf(fileData, endTag);

			//Add the length to the startIndex so we begin at the end of the startTag.
			startIndex = startIndex + wcslen(startTag);

			//Get the data from the end of the start tag up to (but not including) the endIndex.
			data = Ace::RetrieveDataBetweenTags(fileData, startIndex, endIndex);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RetrieveDataBetweenTags()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RetrieveDataBetweenTags() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RetrieveDataBetweenTags() Parameters cannot be NULL.";
	}

	return data;
}

/**
* Finds the XML data between the startTagIndex and endTagIndex and returns it.
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = Ace::ReadTextFile("C://Config.xml");
* 		int startIndex = Ace::IndexOf(data, "<position>") + strlen("<position>");
* 		int endIndex = Ace::IndexOf(data, "</position>");
* 		char* tagData = Ace::RetrieveDataBetweenTags(data, startIndex, endIndex);
* 		printf("Data between tags:%s\n", tagData);
* 		_aligned_free(tagData);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.1, 2.3, 2.2
*
* @param [in] fileData A string containing the data to be parsed.
* @param [in] startTagIndex Index of where to start reading in.
* @param [in] endTagIndex Index of where to end reading data.
*
* @return A Buffer containing the data between startTagIndex and endTagIndex.
* 		  Throws an exception if error occurs.
*/
char* Ace::RetrieveDataBetweenTags(char* fileData, int startTagIndex, int endTagIndex)
{
	if (fileData != NULL && startTagIndex >= 0 && endTagIndex >= 0 && endTagIndex > startTagIndex)
	{
		try
		{
			return Ace::SubString(fileData, startTagIndex, startTagIndex + (endTagIndex - startTagIndex));
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RetrieveDataBetweenTags()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RetrieveDataBetweenTags() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RetrieveDataBetweenTags() Parameters cannot be NULL.";
	}

	return NULL;
}

/**
* Finds the XML data between the startTagIndex and endTagIndex and returns it.
*
* @author Katianie
* @date 9/8/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = Ace::ReadTextFileW(L"C://Config.xml");
* 		int startIndex = Ace::IndexOf(data, L"<position>") + wcslen(L"<position>");
* 		int endIndex = Ace::IndexOf(data, L"</position>");
* 		wchar_t* tagData = Ace::RetrieveDataBetweenTags(data, startIndex, endIndex);
* 		printf("Data between tags:%S\n", tagData);
* 		_aligned_free(tagData);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.1, 2.3, 2.2
*
* @param [in] fileData A wide string containing the data to be parsed.
* @param [in] startTagIndex Index of where to start reading in.
* @param [in] endTagIndex Index of where to end reading data.
*
* @return A Buffer containing the data between startTagIndex and endTagIndex.
* 		  Throws an exception if error occurs.
*/
wchar_t* Ace::RetrieveDataBetweenTags(wchar_t* fileData, int startTagIndex, int endTagIndex)
{
	if (fileData != NULL && startTagIndex >= 0 && endTagIndex >= 0 && endTagIndex > startTagIndex)
	{
		try
		{
			return Ace::SubString(fileData, startTagIndex, startTagIndex + (endTagIndex - startTagIndex));
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RetrieveDataBetweenTags()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RetrieveDataBetweenTags() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RetrieveDataBetweenTags() Parameters cannot be NULL.";
	}

	return NULL;
}

/**
* Finds a series of XML data between startTag and endTag. For example, if we were reading in an
* XML file that looked like this:
* 	<Models>
* 		<Model>
*   			<position>0.1, 2.3, 2.2</position>
*   			<rotation>20.0, 0.0, 0.0, 1.0</rotation>
* 		</Model>
* 		<Model>
*   			<position>3.0, 4.0, 5.0</position>
*   			<rotation>0.0, 30.0, 0.0, 1.0</rotation>
* 		</Model>
* 	</Models>
*
* And the start and end tags were "<Model>" and "</Model>" respectively; then the
* function will return an array containing:
* 	Array[0]="<position>0.1, 2.3, 2.2</position>
*   		  <rotation>20.0, 0.0, 0.0, 1.0</rotation>"
*
* 	Array[1]="<position>1.0, 2.0, 0.0</position>
*   		  <rotation>20.0, 0.0, 0.0, 1.0</rotation>"
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = Ace::ReadTextFile("C://Config.xml");
* 		int tagDataSize = 0;
* 		char** tagData = Ace::RetrieveDataChunksBetweenTags(data, "<Model>", "</Model>", tagDataSize);
* 		for (int currIndex = 0; currIndex < tagDataSize; currIndex++)
* 		{
* 			printf("Array[%d]:%s\n", currIndex, tagData[currIndex]);
* 			_aligned_free(tagData[currIndex]);
* 		}
* 		_aligned_free(tagData);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:(Displayed Above)
*
* @param [in] fileData A string containing the data to be parsed.
* @param [in] startTag A string representing the tag to start from.
* @param [in] endTag A string representing the tag to end on.
* @param [out] outFinalBufferSize Size of the out final buffer.
*
* @return A newly allocated string array containing the data between each startTag and
*		  endTag found. Throws an exception if error occurs.
*/
char** Ace::RetrieveDataChunksBetweenTags(char* fileData, char* startTag, char* endTag, int& outFinalBufferSize)
{
	vector<char*> dataChunksVectorArray;
	char** dataChunksArray = NULL;
	char* data = fileData;
	int startIndex = -1;
	int endIndex = -1;

	if (data != NULL && startTag != NULL && endTag != NULL)
	{
		try
		{
			//Grab the data in the file between each <ClassName> and </ClassName> tag.
			while (Ace::IndexOf(data, endTag) >= 0)
			{
				startIndex = Ace::IndexOf(data, startTag);
				endIndex = Ace::IndexOf(data, endTag);

				if (startIndex >= 0 && endIndex > startIndex)
				{
					//Add the length to the startIndex so we begin at the end of the startTag.
					startIndex = startIndex + strlen(startTag);

					dataChunksVectorArray.push_back(Ace::RetrieveDataBetweenTags(data, startIndex, endIndex));

					//Go past the end tag to the next item.
					data += endIndex + strlen(endTag);
				}
				else
				{
					//Couldn't find the tag(s) so break out of the loop.
					break;
				}
			}

			outFinalBufferSize = dataChunksVectorArray.size();
			dataChunksArray = Ace::ConvertStringVectorArrayToStringArray(dataChunksVectorArray, outFinalBufferSize);

			//Clean up the vector array.
			for (size_t currIndex = 0; currIndex < dataChunksVectorArray.size(); currIndex++)
			{
				_aligned_free(dataChunksVectorArray[currIndex]);
			}
			dataChunksVectorArray.clear();
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RetrieveDataChunksBetweenTags()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RetrieveDataChunksBetweenTags() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RetrieveDataChunksBetweenTags() Parameters cannot be NULL.";
	}

	return dataChunksArray;
}

/**
* Finds a series of XML data between startTag and endTag. For example, if we were reading in an
* XML file that looked like this:
* 	<Models>
* 		<Model>
*   			<position>0.1, 2.3, 2.2</position>
*   			<rotation>20.0, 0.0, 0.0, 1.0</rotation>
* 		</Model>
* 		<Model>
*   			<position>3.0, 4.0, 5.0</position>
*   			<rotation>0.0, 30.0, 0.0, 1.0</rotation>
* 		</Model>
* 	</Models>
*
* And the start and end tags were "<Model>" and "</Model>" respectively; then the
* function will return an array containing:
* 	Array[0]="<position>0.1, 2.3, 2.2</position>
*   		  <rotation>20.0, 0.0, 0.0, 1.0</rotation>"
*
* 	Array[1]="<position>1.0, 2.0, 0.0</position>
*   		  <rotation>20.0, 0.0, 0.0, 1.0</rotation>"
*
* @author Katianie
* @date 9/8/2016
*
* @code
*	try
* 	{
* 		wchar_t* data = Ace::ReadTextFileW(L"C://Config.xml");
* 		int tagDataSize = 0;
* 		wchar_t** tagData = Ace::RetrieveDataChunksBetweenTags(data, L"<Model>", L"</Model>", tagDataSize);
* 		for (int currIndex = 0; currIndex < tagDataSize; currIndex++)
* 		{
* 			printf("Array[%d]:%S\n", currIndex, tagData[currIndex]);
* 			_aligned_free(tagData[currIndex]);
* 		}
* 		_aligned_free(tagData);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:(Displayed Above)
*
* @param [in] fileData A wide string containing the data to be parsed.
* @param [in] startTag A wide string representing the tag to start from.
* @param [in] endTag A wide string representing the tag to end on.
* @param [out] outFinalBufferSize Size of the out final buffer.
*
* @return A newly allocated wide string array containing the data between each startTag and
*		  endTag found. Throws an exception if error occurs.
*/
wchar_t** Ace::RetrieveDataChunksBetweenTags(wchar_t* fileData, wchar_t* startTag, wchar_t* endTag, int& outFinalBufferSize)
{
	vector<wchar_t*> dataChunksVectorArray;
	wchar_t** dataChunksArray = NULL;
	wchar_t* data = fileData;
	int startIndex = -1;
	int endIndex = -1;

	if (data != NULL && startTag != NULL && endTag != NULL)
	{
		try
		{
			//Grab the data in the file between each <ClassName> and </ClassName> tag.
			while (Ace::IndexOf(data, endTag) >= 0)
			{
				startIndex = Ace::IndexOf(data, startTag);
				endIndex = Ace::IndexOf(data, endTag);

				if (startIndex >= 0 && endIndex > startIndex)
				{
					//Add the length to the startIndex so we begin at the end of the startTag.
					startIndex = startIndex + wcslen(startTag);

					dataChunksVectorArray.push_back(Ace::RetrieveDataBetweenTags(data, startIndex, endIndex));

					//Go past the end tag to the next item.
					data += endIndex + wcslen(endTag);
				}
				else
				{
					//Couldn't find the tag(s) so break out of the loop.
					break;
				}
			}

			outFinalBufferSize = dataChunksVectorArray.size();
			dataChunksArray = Ace::ConvertStringVectorArrayToStringArray(dataChunksVectorArray, outFinalBufferSize);

			//Clean up the vector array.
			for (size_t currIndex = 0; currIndex < dataChunksVectorArray.size(); currIndex++)
			{
				_aligned_free(dataChunksVectorArray[currIndex]);
			}
			dataChunksVectorArray.clear();
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RetrieveDataChunksBetweenTags()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RetrieveDataChunksBetweenTags() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RetrieveDataChunksBetweenTags() Parameters cannot be NULL.";
	}

	return dataChunksArray;
}

/**
* Returns all the names of the files inside a folder, Ignores non files like folders.
*
* @author Katianie
* @date 7/9/2016
*
* @code
* 	try
* 	{
* 		char* imageFilePath = "C:\\Images\\180Panoramic";
* 		vector<char*> fileNames = Ace::RetrieveFileNames(imageFilePath);
* 		for (size_t i = 0; i < fileNames.size(); i++)
* 		{
* 			printf("%s\n", fileNames[i]);
* 			_aligned_free(fileNames[i]);
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:sky.jpg
*		 mountans.png
*		 tits.gtfo
*		 dontread.txt
*
* @param [in] fullFilePath Directory to look inside.
*
* @return A vector array containing all the file names in the folder.
* 		  Throws an exception if fullFilePath is NULL.
*/
vector<char*> Ace::RetrieveFileNames(char* fullFilePath)
{
	char* currFileName = NULL;
	char* filePath = fullFilePath;
	vector<char*> fileNames;
	WIN32_FIND_DATAA fd;
	HANDLE hFind;

	if (fullFilePath != NULL)
	{
		try
		{
			//Check if the path ends with a slash.
			if (Ace::LastIndexOf(fullFilePath, "\\") != strlen(fullFilePath) - 1)
			{
				//Append the final slash to the end of the path since it's missing.
				filePath = Ace::Concatenate(2, fullFilePath, "\\*.*");
			}
			else
			{
				filePath = Ace::Concatenate(2, fullFilePath, "*.*");
			}

			if ((hFind = FindFirstFileA((LPCSTR)filePath, &fd)) != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						currFileName = Ace::AllocateAndCopyString((char*)fd.cFileName);

						//Filter out the . and .. file names.
						if (Ace::Equals(currFileName, ".") == false && Ace::Equals(currFileName, "..") == false)
						{
							fileNames.push_back(currFileName);
						}
						else
						{
							_aligned_free(currFileName);
						}

					}
				} while (FindNextFileA(hFind, &fd));

				FindClose(hFind);
			}
			_aligned_free(filePath);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RetrieveFileNames()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RetrieveFileNames() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RetrieveFileNames() fullFilePath cannot be NULL.";
	}

	return fileNames;
}

/**
* Returns all the names of the files inside a folder, Ignores non files like folders.
*
* @author Katianie
* @date 9/8/2016
*
* @code
* 	try
* 	{
* 		wchar_t* imageFilePath = L"C:\\Images\\180Panoramic";
* 		vector<wchar_t*> fileNames = Ace::RetrieveFileNames(imageFilePath);
* 		for (size_t i = 0; i < fileNames.size(); i++)
* 		{
* 			printf("%S\n", fileNames[i]);
* 			_aligned_free(fileNames[i]);
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:sky.jpg
*		 mountans.png
*		 tits.gtfo
*		 dontread.txt
*
* @param [in] fullFilePath Directory to look inside.
*
* @return A vector array containing all the file names in the folder.
* 		  Throws an exception if fullFilePath is NULL.
*/
vector<wchar_t*> Ace::RetrieveFileNames(wchar_t* fullFilePath)
{
	wchar_t* currFileName = NULL;
	wchar_t* filePath = fullFilePath;
	vector<wchar_t*> fileNames;
	WIN32_FIND_DATAW fd;
	HANDLE hFind;

	if (fullFilePath != NULL)
	{
		try
		{
			//Check if the path ends with a slash.
			if (Ace::LastIndexOf(fullFilePath, L"\\") != wcslen(fullFilePath) - 1)
			{
				//Append the final slash to the end of the path since it's missing.
				filePath = Ace::ConcatenateW(2, fullFilePath, L"\\*.*");
			}
			else
			{
				filePath = Ace::ConcatenateW(2, fullFilePath, L"*.*");
			}

			if ((hFind = FindFirstFileW((LPCWSTR)filePath, &fd)) != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						currFileName = Ace::AllocateAndCopyString((wchar_t*)fd.cFileName);

						//Filter out the . and .. file names.
						if (Ace::Equals(currFileName, L".") == false && Ace::Equals(currFileName, L"..") == false)
						{
							fileNames.push_back(currFileName);
						}
						else
						{
							_aligned_free(currFileName);
						}

					}
				} while (FindNextFileW(hFind, &fd));

				FindClose(hFind);
			}
			_aligned_free(filePath);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RetrieveFileNames()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RetrieveFileNames() Encountered an error.";
		}

	}
	else
	{
		throw "Ace Error: RetrieveFileNames() fullFilePath cannot be NULL.";
	}

	return fileNames;
}

/**
* Returns all the names of the sub-folder(s) in a folder.
*
* @author Katianie
* @date 6/1/2016
*
* @code
* 	try
* 	{
* 		char* imageFilePath = "C:\\Images";
* 		vector<char*> folderNames = Ace::RetrieveFolderNames(imageFilePath);
* 		for (size_t i = 0; i < folderNames.size(); i++)
* 		{
* 			printf("%s\n", folderNames[i]);
* 			_aligned_free(folderNames[i]);
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:180 panoramics
* 		 360 panoramics
* 		 photospheres
*
* @param [in] fullFilePath Directory to look inside.
*
* @return A vector array containing all the sub-folder names in the folder.
* 		  Throws an exception if fullFilePath is NULL.
*/
vector<char*> Ace::RetrieveFolderNames(char* fullFilePath)
{
	char* currFileName = NULL;
	char* filePath = NULL;
	vector<char*> fileNames;
	WIN32_FIND_DATAA fd;
	HANDLE hFind;

	if (fullFilePath != NULL)
	{
		try
		{
			//Check if the path ends with a slash.
			if (Ace::LastIndexOf(fullFilePath, "\\") != strlen(fullFilePath) - 1)
			{
				//Append the final slash to the end of the path since it's missing.
				filePath = Ace::Concatenate(2, fullFilePath, "\\*");
			}
			else
			{
				filePath = Ace::Concatenate(2, fullFilePath, "*");
			}

			if ((hFind = FindFirstFileA((LPCSTR)filePath, &fd)) != INVALID_HANDLE_VALUE)
			{
				do
				{
					//This if statement is the only difference bestrewn RetrieveFileNames.
					if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0)
					{
						currFileName = Ace::AllocateAndCopyString((char*)fd.cFileName);

						//Filter out the . and .. file names.
						if (Ace::Equals(currFileName, ".") == false && Ace::Equals(currFileName, "..") == false)
						{
							fileNames.push_back(currFileName);
						}
						else
						{
							_aligned_free(currFileName);
						}

					}
				} while (FindNextFileA(hFind, &fd));

				FindClose(hFind);
			}
			_aligned_free(filePath);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RetrieveFolderNames()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RetrieveFolderNames() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RetrieveFolderNames() fullFilePath cannot be NULL.";
	}

	return fileNames;
}

/**
* Returns all the names of the sub-folder(s) in a folder.
*
* @author Katianie
* @date 9/8/2016
*
* @code
* 	try
* 	{
* 		wchar_t* imageFilePath = L"C:\\Images";
* 		vector<wchar_t*> folderNames = Ace::RetrieveFolderNames(imageFilePath);
* 		for (size_t i = 0; i < folderNames.size(); i++)
* 		{
* 			printf("%S\n", folderNames[i]);
* 			_aligned_free(folderNames[i]);
* 		}
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:180 panoramics
* 		 360 panoramics
* 		 photospheres
*
* @param [in] fullFilePath Directory to look inside.
*
* @return A vector array containing all the sub-folder names in the folder.
* 		  Throws an exception if fullFilePath is NULL.
*/
vector<wchar_t*> Ace::RetrieveFolderNames(wchar_t* fullFilePath)
{
	wchar_t* currFileName = NULL;
	wchar_t* filePath = NULL;
	vector<wchar_t*> fileNames;
	WIN32_FIND_DATAW fd;
	HANDLE hFind;

	if (fullFilePath != NULL)
	{
		try
		{
			//Check if the path ends with a slash.
			if (Ace::LastIndexOf(fullFilePath, L"\\") != wcslen(fullFilePath) - 1)
			{
				//Append the final slash to the end of the path since it's missing.
				filePath = Ace::ConcatenateW(2, fullFilePath, "\\*");
			}
			else
			{
				filePath = Ace::ConcatenateW(2, fullFilePath, "*");
			}

			if ((hFind = FindFirstFileW((LPCWSTR)filePath, &fd)) != INVALID_HANDLE_VALUE)
			{
				do
				{
					//This if statement is the only difference bestrewn RetrieveFileNames.
					if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0)
					{
						currFileName = Ace::AllocateAndCopyString((wchar_t*)fd.cFileName);

						//Filter out the . and .. file names.
						if (Ace::Equals(currFileName, L".") == false && Ace::Equals(currFileName, L"..") == false)
						{
							fileNames.push_back(currFileName);
						}
						else
						{
							_aligned_free(currFileName);
						}

					}
				} while (FindNextFileW(hFind, &fd));

				FindClose(hFind);
			}
			_aligned_free(filePath);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::RetrieveFolderNames()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: RetrieveFolderNames() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: RetrieveFolderNames() fullFilePath cannot be NULL.";
	}

	return fileNames;
}

/**
* Creates a new string containing all the characters from strToReverse in reverse order.
* For example: "ABCDEF" will return "FEDCBA".
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = Ace::ReverseString("ABCDEF");
* 		printf("%s\n", data);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:FEDCBA
*
* @param [in] strToReverse A string to reverse.
*
* @return A newly created string containing the reverse of strToReverse.
* 		  Throws an exception if strToReverse is NULL.
*/
char* Ace::ReverseString(char* strToReverse)
{
	char* strReverse = NULL;

	if (strToReverse != NULL)
	{
		try
		{
			strReverse = Ace::AllocateAndCopyString(strToReverse);
			Ace::ReverseStringInPlace(strReverse);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::ReverseString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: ReverseString() Encountered an error.";
		}

	}
	else
	{
		throw "Ace Error: ReverseString() strToReverse cannot be NULL.";
	}

	return strReverse;
}

/**
* Creates a new wide string containing all the characters from strToReverse in reverse order.
* For example: "ABCDEF" will return "FEDCBA".
*
* @author Katianie
* @date 9/8/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = Ace::ReverseString(L"ABCDEF");
* 		printf("%S\n", data);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:FEDCBA
*
* @param [in] strToReverse A wide string to reverse.
*
* @return A newly created wide string containing the reverse of strToReverse.
* 		  Throws an exception of strToReverse is NULL.
*/
wchar_t* Ace::ReverseString(wchar_t* strToReverse)
{
	wchar_t* strReverse = NULL;

	if (strToReverse != NULL)
	{
		try
		{
			strReverse = Ace::AllocateAndCopyString(strToReverse);
			Ace::ReverseStringInPlace(strReverse);
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::ReverseString()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: ReverseString() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: ReverseString() strToReverse cannot be NULL.";
	}

	return strReverse;
}

/**
* Directly modifies strToReverse to have all its characters in reverse order.
* For example: "ABCDEF" will return "FEDCBA".
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* data = Ace::AllocateAndCopyString("ABCDEF");
* 		Ace::ReverseStringInPlace(data);
* 		printf("%s\n", data);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:(Displayed above)
*
* @param [in,out] strToReverse A preallocated string to reverse.
* 				  Throws an exception of strToReverse is NULL.
*/
void Ace::ReverseStringInPlace(char* strToReverse)
{
	char juggler = '\0';
	int leftIndex = 0;
	int rightIndex = 0;

	if (strToReverse != NULL)
	{
		rightIndex = strlen(strToReverse) - 1;

		while (leftIndex < rightIndex)
		{
			juggler = strToReverse[leftIndex];
			strToReverse[leftIndex] = strToReverse[rightIndex];
			strToReverse[rightIndex] = juggler;

			leftIndex++;
			rightIndex--;
		}
	}
	else
	{
		throw "Ace Error: ReverseStringInPlace() strToReverse cannot be NULL.";
	}
}

/**
* Directly modifies strToReverse to have all its characters in reverse order.
* For example: "ABCDEF" will return "FEDCBA".
*
* @author Katianie
* @date 9/8/2016
*
* @code
* 	try
* 	{
* 		wchar_t* data = Ace::AllocateAndCopyString(L"ABCDEF");
* 		Ace::ReverseStringInPlace(data);
* 		printf("%S\n", data);
* 		_aligned_free(data);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:(Displayed above)
*
* @param [in,out] strToReverse A preallocated string to reverse.
* 				  Throws an exception of strToReverse is NULL.
*/
void Ace::ReverseStringInPlace(wchar_t* strToReverse)
{
	wchar_t juggler = L'\0';
	int leftIndex = 0;
	int rightIndex = 0;

	if (strToReverse != NULL)
	{
		rightIndex = wcslen(strToReverse) - 1;

		while (leftIndex < rightIndex)
		{
			juggler = strToReverse[leftIndex];
			strToReverse[leftIndex] = strToReverse[rightIndex];
			strToReverse[rightIndex] = juggler;

			leftIndex++;
			rightIndex--;
		}
	}
	else
	{
		throw "Ace Error: ReverseStringInPlace() strToReverse cannot be NULL.";
	}
}

/**
* Rounds the floating-point argument num to an integer value,
* using the current rounding mode. Created in compliance with
* CERT Coding Standard.
*
* @author Katianie
* @date 10/5/2016
*
* @code
*	try
*	{
*		long result = Ace::RoundToNearestInteger(3.5);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4
*
* @param num Number to round.
*
* @return The num rounded using the current rounding mode.
* 		  If a mathematical error occurs then an exception is
* 		  thrown.
*/
long Ace::RoundToNearestInteger(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	long result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = lrint(num);

	//Look for any mathematical errors as a result of calling lrint().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: RoundToNearestInteger() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: RoundToNearestInteger() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Rounds the floating-point argument num to an integer value,
* using the current rounding mode. Created in compliance with
* CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		long result = Ace::RoundToNearestInteger(3.5f);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4
*
* @param num Number to round.
*
* @return The num rounded using the current rounding mode.
* 		  If a mathematical error occurs then then an exception is
* 		  thrown.
*/
long Ace::RoundToNearestInteger(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	long result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = lrint(num);

	//Look for any mathematical errors as a result of calling lrint().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: RoundToNearestInteger() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: RoundToNearestInteger() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Rounds the floating-point argument num to an integer value,
* using the current rounding mode. This function is somewhat
* useless but provided for completeness. Created in compliance
* with CERT Coding Standard.
*
* @author Katianie
* @date 10/19/2016
*
* @code
*	try
*	{
*		long result = Ace::RoundToNearestInteger(3);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:3
*
* @param num Number to round.
*
* @return The num rounded using the current rounding mode.
* 		  If a mathematical error occurs then an exception is
* 		  thrown.
*/
long Ace::RoundToNearestInteger(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	long result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = lrint(num);

	//Look for any mathematical errors as a result of calling lrint().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: RoundToNearestInteger() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: RoundToNearestInteger() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates the nearest integer value to num (in integer format),
* rounding halfway cases away from zero, regardless of the current
* rounding mode. Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/5/2016
*
* @code
*	try
*	{
*		long result = Ace::RoundUp(3.5);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4
*
* @param num Number to round.
*
* @return The nearest integer value to num, rounding halfway
* 		  cases away from zero. If a mathematical error occurs
* 		  then an exception is thrown.
*/
long Ace::RoundUp(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	long result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = lround(num);

	//Look for any mathematical errors as a result of calling lround().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: RoundUp() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: RoundUp() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates the nearest integer value to num (in integer format),
* rounding halfway cases away from zero, regardless of the current
* rounding mode. Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		long result = Ace::RoundUp(3.5f);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:4
*
* @param num Number to round.
*
* @return The nearest integer value to num, rounding halfway
* 		  cases away from zero. If a mathematical error occurs
* 		  then an exception is thrown.
*/
long Ace::RoundUp(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	long result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = lround(num);

	//Look for any mathematical errors as a result of calling lround().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: RoundUp() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: RoundUp() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates the nearest integer value to num (in integer format),
* rounding halfway cases away from zero, regardless of the current
* rounding mode. This function is somewhat useless but provided for
* completeness. Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		long result = Ace::RoundUp(3);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:3
*
* @param num Number to round.
*
* @return The nearest integer value to num, rounding halfway
* 		  cases away from zero. If a mathematical error occurs
* 		  then an exception is thrown.
*/
long Ace::RoundUp(int num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	long result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = lround(num);

	//Look for any mathematical errors as a result of calling lround().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: RoundUp() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: RoundUp() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Displays a standard message box with a message, title, and
* an OK button.
*
* @author Katianie
* @date 10/31/2016
*
* @code
*	try
*	{
*		Ace::ShowMessageBox(NULL, "Hello Eddie!", "Message Box Name");
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:(Shows a message box with the message).
*
* @param hWindow The parent window to the message box (Can be NULL).
* @param message The message to display in the message box (Can be NULL).
* @param title The title to use in the message box window (Can be NULL).
*
* @return Throws an exception if an error is encountered.
*/
void Ace::ShowMessageBox(HWND hWindow, char* message, char* title)
{
	if (MessageBoxA(hWindow, message, title, MB_OK) == 0)
	{
		throw "Ace Error: ShowMessageBox() Encountered an error on execution.";
	}
}

/**
* Displays a standard message box with a message, title, and
* an OK button.
*
* @author Katianie
* @date 10/31/2016
*
* @code
*	try
*	{
*		Ace::ShowMessageBox(NULL, L"Hello Eddie!", L"Message Box Name");
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:(Shows a message box with the message).
*
* @param hWindow The parent window to the message box (Can be NULL).
* @param message The message to display in the message box (Can be NULL).
* @param title The title to use in the message box window (Can be NULL).
*
* @return Throws an exception if an error is encountered.
*/
void Ace::ShowMessageBox(HWND hWindow, wchar_t* message, wchar_t* title)
{
	if (MessageBoxW(hWindow, message, title, MB_OK) == 0)
	{
		throw "Ace Error: ShowMessageBox() Encountered an error on execution.";
	}
}

/**
* Calculates the Sin in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		const double PI = 3.14159265;
*		double result = Ace::Sin(PI / 4.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.707107
*
* @param numInRadians Number in radians to calculate Sin for.
*
* @return The Sin of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
double Ace::Sin(double numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if (math_errhandling & MATH_ERREXCEPT)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = sin(numInRadians);

	//Look for any mathematical errors as a result of calling sin().
	if ((math_errhandling & MATH_ERRNO) && errno != 0)
	{
		throw "Ace Range Error: Sin() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Sin() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the Sin in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		const float PI = 3.14159265f;
*		float result = Ace::Sin(PI / 4.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.707107
*
* @param numInRadians Number in radians to calculate Sin for.
*
* @return The Sin of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
float Ace::Sin(float numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if (math_errhandling & MATH_ERREXCEPT)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = sin(numInRadians);

	//Look for any mathematical errors as a result of calling sin().
	if ((math_errhandling & MATH_ERRNO) && errno != 0)
	{
		throw "Ace Range Error: Sin() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Sin() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the Sin in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		const float PI = 3.14159265f;
*		int result = Ace::Sin((int)(PI / 4));
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.707107
*
* @param numInRadians Number in radians to calculate Sin for.
*
* @return The Sin of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
int Ace::Sin(int numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if (math_errhandling & MATH_ERREXCEPT)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = sin(numInRadians);

	//Look for any mathematical errors as a result of calling sin().
	if ((math_errhandling & MATH_ERRNO) && errno != 0)
	{
		throw "Ace Range Error: Sin() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Sin() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates the SinH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/4/2016
*
* @code
*	try
*	{
*		const double PI = 3.14159265;
*		double result = Ace::SinH(PI / 4.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.868671
*
* @param numInRadians Number in radians to calculate SinH for.
*
* @return The SinH of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
double Ace::SinH(double numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if (math_errhandling & MATH_ERREXCEPT)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = sinh(numInRadians);

	//Look for any mathematical errors as a result of calling sinh().
	if ((math_errhandling & MATH_ERRNO) && errno != 0)
	{
		throw "Ace Range Error: SinH() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: SinH() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the SinH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		const float PI = 3.14159265f;
*		float result = Ace::SinH(PI / 4.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0.868671
*
* @param numInRadians Number in radians to calculate SinH for.
*
* @return The SinH of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
float Ace::SinH(float numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = sinh(numInRadians);

	//Look for any mathematical errors as a result of calling sinh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: SinH() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: SinH() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the SinH in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		const float PI = 3.14159265f;
*		int result = Ace::SinH((int)(PI / 4));
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:0
*
* @param numInRadians Number in radians to calculate SinH for.
*
* @return The SinH of numInRadians. If a mathematical error occurs
* 		  then an exception is thrown.
*/
int Ace::SinH(int numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = sinh(numInRadians);

	//Look for any mathematical errors as a result of calling sinh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: SinH() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: SinH() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates the smaller of the two values
* and returns the smaller one.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	try
*	{
*		double retVal = Ace::Smallest(5.002, 5.01);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:5.002000
*
* @param num1 Number to compare.
* @param num2 Other number to compare.
*
* @return The smaller of the two values.
*/
double Ace::Smallest(double num1, double num2)
{
	return fmin(num1, num2);
}

/**
* Calculates the smaller of the two values
* and returns the smaller one.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	try
*	{
*		float retVal = Ace::Smallest(5.002f, 5.01f);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:5.002000
*
* @param num1 Number to compare.
* @param num2 Other number to compare.
*
* @return The smaller of the two values.
*/
float Ace::Smallest(float num1, float num2)
{
	return fmin(num1, num2);
}

/**
* Calculates the smaller of the two values
* and returns the smaller one.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int retVal = Ace::Smallest(5, 7);
*		printf("%d\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:5
*
* @param num1 Number to compare.
* @param num2 Other number to compare.
*
* @return The smaller of the two values.
*/
int Ace::Smallest(int num1, int num2)
{
	return fmin(num1, num2);
}

/**
* Returns the Square Root of the provided number. If the number provided
* is negative, -1 is returned.
*
* @author Katianie
* @date 9/29/2016
*
* @code
*	try
*	{
*		double squareRoot = Ace::SquareRoot(144.0);
*		printf("%f\n", squareRoot);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:12.000000000
*
* @param [in] num The number to take the Square Root of.
*
* @return If num is negative; an exception is thrown,
* 		  else the Square Root is returned.
*/
double Ace::SquareRoot(double num)
{
	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0.0) == true)
	{
		return sqrt(num);
	}
	else
	{
		throw "Ace Error: SquareRoot() Cannot take square root of a negative number.";
		return -1.0;
	}
}

/**
* Returns the Square Root of the provided number. If the number provided
* is negative, -1 is returned.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float squareRoot = Ace::SquareRoot(144.0f);
*		printf("%f\n", squareRoot);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:12.000000000
*
* @param [in] num The number to take the Square Root of.
*
* @return If num is negative; an exception is thrown,
* 		  else the Square Root is returned.
*/
float Ace::SquareRoot(float num)
{
	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0.0f) == true)
	{
		return sqrt(num);
	}
	else
	{
		throw "Ace Error: SquareRoot() Cannot take square root of a negative number.";
		return -1.0f;
	}
}

/**
* Returns the Square Root of the provided number. If the number provided
* is negative, -1 is returned.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int squareRoot = Ace::SquareRoot(144);
*		printf("%d\n", squareRoot);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:12
*
* @param [in] num The number to take the Square Root of.
*
* @return If num is negative; an exception is thrown,
* 		  else the Square Root is returned.
*/
int Ace::SquareRoot(int num)
{
	//is functions prevent an exception from being raised.
	if (isgreaterequal(num, 0) == true)
	{
		return sqrt(num);
	}
	else
	{
		throw "Ace Error: SquareRoot() Cannot take square root of a negative number.";
		return -1;
	}
}

/**
* Returns a string containing the characters between startIndex and
* upToButNotIncludingIndex. Unlike strstr(), this function creates a new string
* and returns it. If upToButNotIncludingIndex is not provided, then the string returned
* is everything from startIndex to the end of the string.
*
* Here's an example:
*	str = "Happiness"
*	startIndex = 3
*	upToButNotIncludingIndex = 8
*	then the output would be "pines".
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		char* substr = Ace::SubString("Happiness", 3, 8);
* 		printf("%s\n", substr);
* 		_aligned_free(substr);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:pines
*
* @param [in] str The string to parse.
* @param [in] startIndex The (0 based) index of where to start parsing str.
* @param [in] upToButNotIncludingIndex The (0 based) index of where to stop.
*
* @return A newly created string containing the characters from
*		  startIndex to upToButNotIncludingIndex. Throws an exception
*		  if error occurs.
*/
char* Ace::SubString(char* str, int startIndex, int upToButNotIncludingIndex)
{
	int strToParseLength = -1;
	size_t bufferSize = 0;
	char* strToParse = str;
	char* buffer = NULL;
	char* buffHead = NULL;

	if (strToParse != NULL)
	{
		strToParseLength = strlen(strToParse);
		if (startIndex >= 0 && startIndex <= strToParseLength)
		{
			if (upToButNotIncludingIndex < 0 || upToButNotIncludingIndex > strToParseLength)
			{
				//Prevent invalid values.
				upToButNotIncludingIndex = strToParseLength;
			}

			if ((upToButNotIncludingIndex - startIndex) > 0)
			{
				//+1 for Null character at the end.
				bufferSize = (upToButNotIncludingIndex - startIndex + 1) * sizeof(char);
				buffer = (char*)Ace::_Aligned_Calloc(bufferSize);

				buffHead = buffer;

				for (int currCharIndex = startIndex; currCharIndex < upToButNotIncludingIndex && *strToParse != NULL; currCharIndex++)
				{
					*buffer = *(strToParse + currCharIndex);
					buffer++;
				}
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			throw "Ace Error: SubString() startIndex must be in valid range.";
		}
	}
	else
	{
		throw "Ace Error: SubString() strToParse cannot be NULL.";
	}

	return buffHead;
}

/**
* Returns a wide string containing the characters between startIndex and
* upToButNotIncludingIndex. Unlike strstr(), this function creates a new string
* and returns it. If upToButNotIncludingIndex is not provided, then the string returned
* is everything from startIndex to the end of the string.
*
* Here's an example:
*	str = "Happiness"
*	startIndex = 3
*	upToButNotIncludingIndex = 8
*	then the output would be "pines".
*
* @author Katianie
* @date 9/5/2016
*
* @code
* 	try
* 	{
* 		wchar_t* substr = Ace::SubString(L"Happiness", 3, 8);
* 		printf("%S\n", substr);
* 		_aligned_free(substr);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:pines
*
* @param [in] str The wide string to parse.
* @param [in] startIndex The (0 based) index of where to start parsing str.
* @param [in] upToButNotIncludingIndex The (0 based) index of where to stop.
*
* @return A newly created wide string containing the characters from
*         startIndex to upToButNotIncludingIndex. Throws an exception
*		  if error occurs.
*/
wchar_t* Ace::SubString(wchar_t* str, int startIndex, int upToButNotIncludingIndex)
{
	int strToParseLength = -1;
	size_t bufferSize = 0;
	wchar_t* strToParse = str;
	wchar_t* buffer = NULL;
	wchar_t* buffHead = NULL;

	if (strToParse != NULL)
	{
		strToParseLength = wcslen(strToParse);
		if (startIndex >= 0 && startIndex <= strToParseLength)
		{
			if (upToButNotIncludingIndex < 0 || upToButNotIncludingIndex > strToParseLength)
			{
				//Prevent invalid values.
				upToButNotIncludingIndex = strToParseLength;
			}

			if ((upToButNotIncludingIndex - startIndex) > 0)
			{
				//+1 for Null character at the end.
				bufferSize = (upToButNotIncludingIndex - startIndex + 1) * sizeof(wchar_t);
				buffer = (wchar_t*)Ace::_Aligned_Calloc(bufferSize);

				buffHead = buffer;

				for (int currCharIndex = startIndex; currCharIndex < upToButNotIncludingIndex && *strToParse != NULL; currCharIndex++)
				{
					*buffer = *(strToParse + currCharIndex);
					buffer++;
				}
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			throw "Ace Error: SubString() startIndex must be in valid range.";
		}
	}
	else
	{
		throw "Ace Error: SubString() strToParse cannot be NULL.";
	}

	return buffHead;
}

/**
* Calculates the Tangent (Tan) of numInRadians in
* compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
* 	try
* 	{
* 		const double PI = 3.14159265358979;
* 		double retVal = Ace::Tan(PI / 4.0);
* 		printf("%f\n", retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:1.000000
*
* @param numInRadians Number to use to calculate its tangent.
*
* @return The Tangent (tan) of numInRadians. If a mathematical
* 		  error occurs then an exception is thrown.
*/
double Ace::Tan(double numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = tan(numInRadians);

	//Look for any mathematical errors as a result of calling tan().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Tan() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Tan() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates the Tangent (Tan) of numInRadians in
* compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.141592654f;
* 		float retVal = Ace::Tan(PI / 4.0f);
* 		printf("%f\n", retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:1.000000
*
* @param numInRadians Number to use to calculate its tangent.
*
* @return The Tangent (tan) of numInRadians. If a mathematical
* 		  error occurs then an exception is thrown.
*/
float Ace::Tan(float numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = tan(numInRadians);

	//Look for any mathematical errors as a result of calling tan().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Tan() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Tan() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates the Tangent (Tan) of numInRadians in
* compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
* 	try
* 	{
* 		const double PI = 3.14159265358979;
* 		int retVal = Ace::Tan((int)(PI / 4));
* 		printf("%d\n", retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0
*
* @param numInRadians Number to use to calculate its tangent.
*
* @return The Tangent (tan) of numInRadians. If a mathematical
* 		  error occurs then exception is thrown.
*/
int Ace::Tan(int numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = tan(numInRadians);

	//Look for any mathematical errors as a result of calling tan().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Tan() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Tan() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Calculates The Hyperbolic Tangent (TanH) of
* numInRadians in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
* 	try
* 	{
* 		const double PI = 3.14159265358979;
* 		double retVal = Ace::TanH(PI / 4.0);
* 		printf("%f\n", retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.655794
*
* @param numInRadians Number to use to calculate its hyperbolic tangent.
*
* @return The Hyperbolic Tangent (tan) of numInRadians.
* 		  If a mathematical error occurs then an exception is thrown.
*/
double Ace::TanH(double numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = tanh(numInRadians);

	//Look for any mathematical errors as a result of calling tanh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: TanH() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: TanH() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates The Hyperbolic Tangent (TanH) of
* numInRadians in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
* 	try
* 	{
* 		const float PI = 3.141592654f;
* 		float retVal = Ace::TanH(PI / 4.0f);
* 		printf("%f\n", retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0.655794
*
* @param numInRadians Number to use to calculate its hyperbolic tangent.
*
* @return The Hyperbolic Tangent (tan) of numInRadians.
* 		  If a mathematical error occurs then an exception is thrown.
*/
float Ace::TanH(float numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = tanh(numInRadians);

	//Look for any mathematical errors as a result of calling tanh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: TanH() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: TanH() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates The Hyperbolic Tangent (TanH) of
* numInRadians in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
* 	try
* 	{
* 		const double PI = 3.14159265358979;
* 		int retVal = Ace::TanH((int)(PI / 4));
* 		printf("%d\n", retVal);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:0
*
* @param numInRadians Number to use to calculate its hyperbolic tangent.
*
* @return The Hyperbolic Tangent (tan) of numInRadians.
* 		  If a mathematical error occurs then an exception is thrown.
*/
int Ace::TanH(int numInRadians)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = tanh(numInRadians);

	//Look for any mathematical errors as a result of calling tanh().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: TanH() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: TanH() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Breaks up a string and returns an array of all the strings between each delimiter.
*
* For example: "Shit,Piss, Cunt,Cock-sucker,Mother_Fucker,and tits" And the delimiter was ","
* then the function would return:
*		Array[0] = "Shit"
*		Array[1] = "Piss"
*		Array[2] = " Cunt"
*		Array[3] = "Cock-sucker"
*		Array[4] = "Mother_Fucker"
*		Array[5] = "and tits"
*
* @author Katianie
* @date 5/7/2016
*
* @code
* 	try
* 	{
* 		int numItems = 0;
* 		char* data = "Shit, Piss, Cunt, Cock - sucker, Mother_Fucker, and tits";
* 		char** items = Ace::Tokenize(data, ",", numItems);
* 		for (int currIndex = 0; currIndex < numItems; currIndex++)
* 		{
* 			printf("Array[%d]:%s\n", currIndex, items[currIndex]);
* 	 		_aligned_free(items[currIndex]);
* 		}
* 		_aligned_free(items);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Array[0] = "Shit"
* 		 Array[1] = "Piss"
* 		 Array[2] = " Cunt"
* 		 Array[3] = "Cock-sucker"
* 		 Array[4] = "Mother_Fucker"
* 		 Array[5] = "and tits"
*
* @param [in,out] strToTokenize A string to break up into an array.
* @param [in,out] delimiter A string/character used to separate each string in strToTokenize.
* @param [in,out] outNumTokensFound Number of strings added to the final array.
*
* @return An array of strings broken up from strToTokenize. Throws an exception if
* 		  an error occurs.
*/
char** Ace::Tokenize(char* strToTokenize, char* delimiter, int& outNumTokensFound)
{
	char** tokensArray = NULL;
	char* endSubStr = NULL;
	int strToTokenizeLength = 0;
	int delimiterLength = 0;
	int numDelims = 0;
	int delimiterIndex = -1;

	if (strToTokenize != NULL && delimiter != NULL)
	{
		try
		{
			//Find the number of delimiters in the string.
			numDelims = Ace::CountNumOccurrences(strToTokenize, delimiter);

			//The number of tokens will be equal to the number of delimiters + 1.
			outNumTokensFound = numDelims + 1;

			//Get the length so we don't have to keep calling strlen().
			strToTokenizeLength = strlen(strToTokenize);
			delimiterLength = strlen(delimiter);

			//Check if there is a delimiter at the very end of the string.
			endSubStr = Ace::SubString(strToTokenize, strToTokenizeLength - delimiterLength);
			if (strcmp(delimiter, endSubStr) == 0)
			{
				//It doesn't count as a token if nothing comes after the delimiter at the end.
				outNumTokensFound--;
			}
			_aligned_free(endSubStr);

			//Allocate the array of tokens and zero it out.
			tokensArray = (char**)_aligned_malloc(outNumTokensFound * sizeof(char*), BYTE_ALIGNMENT);

			//Ensure the buffer was allocated successfully.
			if (tokensArray == NULL)
			{
				throw "Ace Error: Tokenize() Buffer could not be allocated.";
				return NULL;
			}

			//Fuck it, I'll roll my own. I was gonna use strtok(), but it's broken.
			for (int currIndex = 0; currIndex < outNumTokensFound; currIndex++)
			{
				delimiterIndex = Ace::IndexOf(strToTokenize, delimiter);
				if (delimiterIndex < 0)
				{
					//Grab everything from start to the end of 'strToTokenize'.
					tokensArray[currIndex] = Ace::SubString(strToTokenize, 0);
				}
				else
				{
					tokensArray[currIndex] = Ace::SubString(strToTokenize, 0, delimiterIndex);

					//Move past the delimiter to get to the next token.
					strToTokenize += (delimiterIndex + 1);
				}
			}
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::Tokenize()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: Tokenize() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: Tokenize() parameters cannot be NULL.";
	}

	return tokensArray;
}

/**
* Breaks up a wide string and returns an array of all the wide strings between each delimiter.
*
* For example: "Shit,Piss, Cunt,Cock-sucker,Mother_Fucker,and tits" And the delimiter was ","
* then the function would return:
*		Array[0] = "Shit"
*		Array[1] = "Piss"
*		Array[2] = " Cunt"
*		Array[3] = "Cock-sucker"
*		Array[4] = "Mother_Fucker"
*		Array[5] = "and tits"
*
* @author Katianie
* @date 9/8/2016
*
* @code
* 	try
* 	{
* 		int numItems = 0;
* 		wchar_t* data = L"Shit, Piss, Cunt, Cock - sucker, Mother_Fucker, and tits";
* 		wchar_t** items = Ace::Tokenize(data, L",", numItems);
* 		for (int currIndex = 0; currIndex < numItems; currIndex++)
* 		{
* 			printf("Array[%d]:%S\n", currIndex, items[currIndex]);
* 	 		_aligned_free(items[currIndex]);
* 		}
* 		_aligned_free(items);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:Array[0] = "Shit"
* 		 Array[1] = "Piss"
* 		 Array[2] = " Cunt"
* 		 Array[3] = "Cock-sucker"
* 		 Array[4] = "Mother_Fucker"
* 		 Array[5] = "and tits"
*
* @param [in,out] strToTokenize A wide string to break up into an array.
* @param [in,out] delimiter A wide string/character used to separate each wide string in strToTokenize.
* @param [in,out] outNumTokensFound Number of wide strings added to the final array.
*
* @return An array of strings broken up from strToTokenize. Throws an exception if
* 		  an error occurs.
*/
wchar_t** Ace::Tokenize(wchar_t* strToTokenize, wchar_t* delimiter, int& outNumTokensFound)
{
	wchar_t** tokensArray = NULL;
	wchar_t* endSubStr = NULL;
	int strToTokenizeLength = 0;
	int delimiterLength = 0;
	int numDelims = 0;
	int delimiterIndex = -1;

	if (strToTokenize != NULL && delimiter != NULL)
	{
		try
		{
			//Find the number of delimiters in the string.
			numDelims = Ace::CountNumOccurrences(strToTokenize, delimiter);

			//The number of tokens will be equal to the number of delimiters + 1.
			outNumTokensFound = numDelims + 1;

			//Get the length so we don't have to keep calling strlen().
			strToTokenizeLength = wcslen(strToTokenize);
			delimiterLength = wcslen(delimiter);

			//Check if there is a delimiter at the very end of the string.
			endSubStr = Ace::SubString(strToTokenize, strToTokenizeLength - delimiterLength);
			if (wcscmp(delimiter, endSubStr) == 0)
			{
				//It doesn't count as a token if nothing comes after the delimiter at the end.
				outNumTokensFound--;
			}
			_aligned_free(endSubStr);

			//Allocate the array of tokens and zero it out.
			tokensArray = (wchar_t**)_aligned_malloc(outNumTokensFound * sizeof(wchar_t*), BYTE_ALIGNMENT);

			//Ensure the buffer was allocated successfully.
			if (tokensArray == NULL)
			{
				throw "Ace Error: Tokenize() Buffer could not be allocated.";
				return NULL;
			}

			//Fuck it, I'll roll my own. I was gonna use strtok(), but it's broken.
			for (int currIndex = 0; currIndex < outNumTokensFound; currIndex++)
			{
				delimiterIndex = Ace::IndexOf(strToTokenize, delimiter);
				if (delimiterIndex < 0)
				{
					//Grab everything from start to the end of 'strToTokenize'.
					tokensArray[currIndex] = Ace::SubString(strToTokenize, 0);
				}
				else
				{
					tokensArray[currIndex] = Ace::SubString(strToTokenize, 0, delimiterIndex);

					//Move past the delimiter to get to the next token.
					strToTokenize += (delimiterIndex + 1);
				}
			}
		}
		catch (char* exceptionMessage)
		{
			//Create a stack trace of the exception.
			char combinedMessage[1024];
			if (SecureZeroMemory(combinedMessage, 1024) != NULL)
			{
				if (sprintf_s(combinedMessage, 1024, "Ace::Tokenize()->%s\n", exceptionMessage) > 0)
				{
					throw combinedMessage;
				}
			}

			throw "Ace Error: Tokenize() Encountered an error.";
		}
	}
	else
	{
		throw "Ace Error: Tokenize() parameters cannot be NULL.";
	}

	return tokensArray;
}

/**
* Removes everything after the decimal point.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	try
*	{
*		double retVal = Ace::Truncate(2.849543);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:2.000000
*
* @param num Number to truncate.
*
* @return The truncated version of num. If a mathematical
* 		  error occurs then an exception is thrown.
*/
double Ace::Truncate(double num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = trunc(num);

	//Look for any mathematical errors as a result of calling exp2().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Truncate() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Truncate() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Removes everything after the decimal point.
* Created in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/9/2016
*
* @code
*	try
*	{
*		float retVal = Ace::Truncate(2.849543f);
*		printf("%f\n", retVal);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:2.000000
*
* @param num Number to truncate.
*
* @return The truncated version of num. If a mathematical
* 		  error occurs then an exception is thrown.
*/
float Ace::Truncate(float num)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = trunc(num);

	//Look for any mathematical errors as a result of calling exp2().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: Truncate() Calculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: Truncate() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates 2^power in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/5/2016
*
* @code
*	try
*	{
*		double result = Ace::TwoToTheXPower(6.0);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:64.000000
*
* @param power Number to raise 2 to.
*
* @return The value of 2^power. If a mathematical error occurs
* 		  then an exception is thrown.
*/
double Ace::TwoToTheXPower(double power)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	double result = -1.0;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = exp2(power);

	//Look for any mathematical errors as a result of calling exp2().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: TwoToTheXPower() Calculation resulted in a range error.";
		return -1.0;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: TwoToTheXPower() Calculation resulted in a range error.";
		return -1.0;
	}

	return result;
}

/**
* Calculates 2^power in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/6/2016
*
* @code
*	try
*	{
*		float result = Ace::TwoToTheXPower(6.0f);
*		printf("%f\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:64.000000
*
* @param power Number to raise 2 to.
*
* @return The value of 2^power. If a mathematical error occurs
* 		  then an exception is thrown.
*/
float Ace::TwoToTheXPower(float power)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	float result = -1.0f;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = exp2(power);

	//Look for any mathematical errors as a result of calling exp2().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: CTwoToTheXPower() alculation resulted in a range error.";
		return -1.0f;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: TwoToTheXPower() Calculation resulted in a range error.";
		return -1.0f;
	}

	return result;
}

/**
* Calculates 2^power in compliance with CERT Coding Standard.
*
* @author Katianie
* @date 10/20/2016
*
* @code
*	try
*	{
*		int result = Ace::TwoToTheXPower(4);
*		printf("%d\n", result);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:16
*
* @param power Number to raise 2 to.
*
* @return The value of 2^power. If a mathematical error occurs
* 		  then an exception is thrown.
*/
int Ace::TwoToTheXPower(int power)
{
	//Allows access to floating point errors.
#ifdef STDC
#pragma STDC FENV_ACCESS ON
#endif
	int result = -1;

	//Clear all previous floating point errors.
	if ((math_errhandling & MATH_ERREXCEPT) > 0)
	{
		feclearexcept(FE_ALL_EXCEPT);
	}
	errno = 0;

	result = exp2(power);

	//Look for any mathematical errors as a result of calling exp2().
	if ((math_errhandling & MATH_ERRNO) > 0 && errno != 0)
	{
		throw "Ace Range Error: TwoToTheXPower() Calculation resulted in a range error.";
		return -1;
	}
	else if ((math_errhandling & MATH_ERREXCEPT) > 0 && fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0)
	{
		throw "Ace Range Error: TwoToTheXPower() Calculation resulted in a range error.";
		return -1;
	}

	return result;
}

/**
* Returns false and displays an error window
* if result is an error (i.e. FAILED). Otherwise
* returns true.
*
* @author Katianie
* @date 10/18/2016
*
* @code
*	try
*	{
*		HRESULT hResult = -1;
*		bool result = Ace::Validate(NULL, hResult, "Ace Error: Windows function encountered error.", "Error Window");
*		if (result == true)
*		{
*			printf("%s\n", "Windows function succeeded.");
*		}
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:(Message box appears and displays the provided ERROR message.)
*
* @param hWnd			Handle of the window.
* @param result			The result to validate.
* @param [in] message	The message to display if failure.
* @param [in] title		The title for the display window if failure.
*
* @return false if error, true otherwise. Throws an exception if
* 		  ShowMessageBox() encounters an error.
*/
bool Ace::Validate(HWND hWnd, HRESULT result, char* message, char* title)
{
	if (FAILED(result) == true)
	{
		Ace::ShowMessageBox(hWnd, message, title);
		return false;
	}

	return true;
}

/**
* Returns false and displays an error window
* if result is an error (i.e. FAILED). Otherwise
* returns true.
*
* @author Katianie
* @date 10/18/2016
*
* @code
*	try
*	{
*		HRESULT hResult = -1;
*		bool result = Ace::Validate(NULL, hResult, L"Ace Error: Windows function encountered error.", L"Error Window");
*		if (result == true)
*		{
*			printf("%s\n", "Windows function succeeded.");
*		}
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:(Message box appears and displays the provided error message.)
*
* @param hWnd			Handle of the window.
* @param result			The result to validate.
* @param [in] message	The message to display if failure.
* @param [in] title		The title for the display window if failure.
*
* @return false if error, true otherwise. Throws an exception if
* 		  ShowMessageBox() encounters an error.
*/
bool Ace::Validate(HWND hWnd, HRESULT result, wchar_t* message, wchar_t* title)
{
	if (FAILED(result) == true)
	{
		Ace::ShowMessageBox(hWnd, message, title);
		return false;
	}

	return true;
}

/**
* Opens a file for writing and returns the number of items/elements
* written to the file. Created in compliance with the CERT Coding Standard.
*
* @author Katianie
* @date 10/13/2016
*
* @code
* 	try
* 	{
* 		char* buffer = Ace::AllocateAndCopyString("In a row?!");
* 		size_t bufferSize = strlen(buffer) * sizeof(char);
* 		size_t retVal = Ace::WriteTextFile("C:\\dicks.txt", buffer, bufferSize, "a");
* 		_aligned_free(buffer);
* 		buffer = Ace::ReadTextFile("C:\\dicks.txt");
* 		printf("%s\n", buffer);
* 		_aligned_free(buffer);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:dicksdicksdicksdicksdicksdicksdick8=>In a row?!
*
* @param [in] filePath The path to the file.
* @param [in] bufferToWrite The data to be written.
* @param bufferSize Size of the buffer in bytes.
* @param [in] mode The mode for fopen_s ("w" for write, "w+" for read/write, etc).
* @param seekOffset Either zero, or a value returned by ftell().
* @param seekOrigin Position used as reference for the offset (SEEK_SET represents beginning of file).
* @param [out] outEndCursorPos The value returned by ftell() after doing the read.
*
* @return The number of items/elements written to the file. Throws an exception if
* 		  error occurs.
*/
size_t Ace::WriteTextFile(char* filePath, char* bufferToWrite, size_t bufferSize, const char* mode, int seekOffset, int seekOrigin, long* outEndCursorPos)
{
	size_t numItems = 0;
	FILE* file = NULL;

	if (filePath != NULL && bufferToWrite != NULL && mode != NULL)
	{
		//Ensure the filePath does NOT lead to a device.
		if (Ace::IsPathADevice(filePath) == false)
		{
			//Ensure there are no errors in opening the file.
			if (fopen_s(&file, filePath, mode) == 0)
			{
				//Set the cursor location (back to the beginning of the file by default).
				if (fseek(file, seekOffset, seekOrigin) != 0)
				{
					fclose(file);
					throw "Ace Error: WriteTextFile() Could not change file cursor position.";
					return 0;
				}

				//Write the data to the file.
				numItems = fwrite(bufferToWrite, sizeof(char), bufferSize / sizeof(char), file);

				//Check for write errors.
				if (numItems <= 0 || numItems != bufferSize / sizeof(char))
				{
					fclose(file);
					throw "Ace Error: WriteTextFile() File could not be written to properly.";
					return 0;
				}

				//Get the value of ftell() if outEndCursorPos is provided.
				if (outEndCursorPos != NULL)
				{
					*outEndCursorPos = ftell(file);
				}

				//Ensure the file is closed without errors.
				if (fclose(file) != 0)
				{
					throw "Ace Error: WriteTextFile() File did not close properly.";
					return 0;
				}
			}
			else
			{
				throw "Ace Error: WriteTextFile() cannot open file.";
			}
		}
		else
		{
			throw "Ace Error: WriteTextFile() filePath cannot be a device.";
		}
	}
	else
	{
		throw "Ace Error: WriteTextFile() Parameters cannot be NULL.";
	}

	return numItems;
}

/**
* Opens a file for writing and returns the number of items/elements
* written to the file. Created in compliance with the CERT Coding Standard.
*
* @author Katianie
* @date 10/13/2016
*
* @code
* 	try
* 	{
* 		wchar_t* buffer = Ace::AllocateAndCopyString(L"いい　ですよ。");
* 		size_t bufferSize = wcslen(buffer) * sizeof(wchar_t);
* 		size_t retVal = Ace::WriteTextFileW(L"C:\\jap.txt", buffer, bufferSize, L"a, ccs=UTF-8");
* 		_aligned_free(buffer);
* 		buffer = Ace::ReadTextFileW(L"C:\\jap.txt");
* 		MessageBoxW(NULL, buffer, NULL, 0);
* 		_aligned_free(buffer);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:わたし　わ　エドワド　オ’ハゲン　です。　これ　は　なん　です　か？いい　ですよ。
*
* @param [in] filePath The path to the file.
* @param [in] bufferToWrite The data to be written.
* @param bufferSize Size of the buffer in bytes.
* @param [in] mode The mode for fopen_s ("w" for write, "w+" for read/write, etc).
* @param seekOffset Either zero, or a value returned by ftell().
* @param seekOrigin Position used as reference for the offset (SEEK_SET represents beginning of file).
* @param [out] outEndCursorPos The value returned by ftell() after doing the read.
*
* @return The number of items/elements written to the file. Throws an exception if
* 		  error occurs.
*/
size_t Ace::WriteTextFileW(wchar_t* filePath, wchar_t* bufferToWrite, size_t bufferSize, const wchar_t* mode, int seekOffset, int seekOrigin, long* outEndCursorPos)
{
	size_t numItems = 0;
	FILE* file = NULL;

	if (filePath != NULL && bufferToWrite != NULL && mode != NULL)
	{
		//Ensure the filePath does NOT lead to a device.
		if (Ace::IsPathADevice(filePath) == false)
		{
			//Ensure there are no errors in opening the file.
			if (_wfopen_s(&file, filePath, mode) == 0)
			{
				//Set the cursor location (back to the beginning of the file by default).
				if (fseek(file, seekOffset, seekOrigin) != 0)
				{
					fclose(file);
					throw "Ace Error: WriteTextFileW() Could not change file cursor position.";
					return 0;
				}

				//Write the data to the file.
				numItems = fwrite(bufferToWrite, sizeof(wchar_t), bufferSize / sizeof(wchar_t), file);

				//Check for write errors.
				if (numItems <= 0 || numItems != bufferSize / sizeof(wchar_t))
				{
					fclose(file);
					throw "Ace Error: WriteTextFileW() File could not be written to properly.";
					return 0;
				}

				//Get the value of ftell() if outEndCursorPos is provided.
				if (outEndCursorPos != NULL)
				{
					*outEndCursorPos = ftell(file);
				}

				//Ensure the file is closed without errors.
				if (fclose(file) != 0)
				{
					throw "Ace Error: WriteTextFileW() File did not close properly.";
					return 0;
				}

			}
			else
			{
				throw "Ace Error: WriteTextFileW() cannot open file.";
			}
		}
		else
		{
			throw "Ace Error: WriteTextFileW() filePath cannot be a device.";
		}
	}
	else
	{
		throw "Ace Error: WriteTextFileW() Parameters cannot be NULL.";
	}

	return numItems;
}

/**
* Opens a file for writing and returns the number of items/elements
* written to the file. Created in compliance with the CERT Coding Standard.
*
* @author Katianie
* @date 10/13/2016
*
* @code
* 	try
* 	{
* 		size_t numItemsWritten = Ace::WriteBinaryFile("C:\\lookupdata.bin", "0xFFFFFFFF", 11);
* 		printf("%d\0", numItemsWritten);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:11
*
* @param [in] filePath The path to the file.
* @param bufferToWrite The data to be written.
* @param bufferSize Maximum number of bytes to read.
* @param [in] mode The mode for fopen_s ("w" for write, "w+" for read/write, etc).
* @param seekOffset Number of bytes to offset from origin.
* @param seekOrigin Position used as reference for the offset (SEEK_SET represents beginning of file).
* @param [out] outEndCursorPos The value returned by ftell() after doing the read.
*
* @return The buffer containing the file data of size numBytesToRead. Throws an exception if
* 		  error occurs.
*/
size_t Ace::WriteBinaryFile(char* filePath, char* bufferToWrite, size_t bufferSize, const char* mode, int seekOffset, int seekOrigin, long* outEndCursorPos)
{
	return Ace::WriteTextFile(filePath, bufferToWrite, bufferSize, mode, seekOffset, seekOrigin, outEndCursorPos);
}

/**
* Opens a file for writing and returns the number of items/elements
* written to the file. Created in compliance with the CERT Coding Standard.
*
* @author Katianie
* @date 10/13/2016
*
* @code
* 	try
* 	{
* 		size_t numItemsWritten = Ace::WriteBinaryFileW(L"C:\\lookupdata.bin", L"0xFFFFFFFF", 11);
* 		printf("%d\0", numItemsWritten);
* 	}
* 	catch (char* exceptionMessage)
* 	{
* 		printf("%s\n", exceptionMessage);
* 	}
*
* @endcode
* Output:11
*
* @param [in] filePath The path to the file.
* @param bufferToWrite The data to be written.
* @param bufferSize Maximum number of bytes to read.
* @param [in] mode The mode for fopen_s ("w" for write, "w+" for read/write, etc).
* @param seekOffset Number of bytes to offset from origin.
* @param seekOrigin Position used as reference for the offset (SEEK_SET represents beginning of file).
* @param [out] outEndCursorPos The value returned by ftell() after doing the read.
*
* @return The buffer containing the file data of size numBytesToRead. Throws an exception if
* 		  error occurs.
*/
size_t Ace::WriteBinaryFileW(wchar_t* filePath, wchar_t* bufferToWrite, size_t bufferSize, const wchar_t* mode, int seekOffset, int seekOrigin, long* outEndCursorPos)
{
	return Ace::WriteTextFileW(filePath, bufferToWrite, bufferSize, mode, seekOffset, seekOrigin, outEndCursorPos);
}