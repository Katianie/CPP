#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <random>
#include <math.h>
#include <fenv.h>
#include <errno.h>
#include <Windows.h>

#ifndef BYTE_ALIGNMENT
static const size_t BYTE_ALIGNMENT = 2 << 3;
#endif

#ifndef MAX_SEEDS
static const size_t MAX_SEEDS = 200;
#endif

#ifndef MAX_STRING_BUFFER_SIZE
static const size_t MAX_STRING_BUFFER_SIZE = 4 << 20;
#endif

#ifndef PI
static const double PI = 3.1415926535897932384626433832795;
#endif

using namespace std;

class Ace
{
public:
	static void*			_Aligned_Calloc(size_t size, size_t alignment = BYTE_ALIGNMENT);
	static double			AbsoluteValue(double num);
	static float			AbsoluteValue(float num);
	static int				AbsoluteValue(int num);
	static char*			AllocateAndCopyString(char* strToCopy);
	static wchar_t*			AllocateAndCopyString(wchar_t* wideStrToCopy);
	static double			ArcCos(double numInRadians);
	static float			ArcCos(float numInRadians);
	static int				ArcCos(int numInRadians);
	static double			ArcCosH(double numInRadians);
	static float			ArcCosH(float numInRadians);
	static int				ArcCosH(int numInRadians);
	static double			ArcSin(double numInRadians);
	static float			ArcSin(float numInRadians);
	static int				ArcSin(int numInRadians);
	static double			ArcSinH(double numInRadians);
	static float			ArcSinH(float numInRadians);
	static int				ArcSinH(int numInRadians);
	static double			ArcTan(double numInRadians);
	static float			ArcTan(float numInRadians);
	static int				ArcTan(int numInRadians);
	static double			ArcTanH(double numInRadians);
	static float			ArcTanH(float numInRadians);
	static int				ArcTanH(int numInRadians);
	static double			ArcTan2(double yInRadians, double xInRadians);
	static float			ArcTan2(float yInRadians, float xInRadians);
	static int				ArcTan2(int yInRadians, int xInRadians);
	static void				CalculateAspectRatio(int newWidth, int newHeight, int originalWidth, int originalHeight, int& outAdjustedWidth, int& outAdjustedHeight);
	static LONGLONG			CalculateFileSize(char* theFilePath);
	static LONGLONG			CalculateFileSize(wchar_t* theFilePath);
	static int				CalculateNumLength(int num);
	static long				CalculateNumLength(long num);
	static double			Ceiling(double num);
	static float			Ceiling(float num);
	static int				Ceiling(int num);
	static char				CharAt(char* str, int index);
	static wchar_t			CharAt(wchar_t* str, int index);
	static double			ComplementaryErrorFunction(double num);
	static float			ComplementaryErrorFunction(float num);
	static int				ComplementaryErrorFunction(int num);
	static char*			ConcatenateStrings(int numStrings, char** stringArray);
	static wchar_t*			ConcatenateStrings(int numStrings, wchar_t** stringArray);
	static char*			ConcatenateStrings(vector<char*> stringVectorArray);
	static wchar_t*			ConcatenateStrings(vector<wchar_t*> stringVectorArray);
	static char*			Concatenate(int numStrings, ...);
	static wchar_t*			ConcatenateW(int numWideStrings, ...);
	static bool				Contains(char* strToSearch, char* strToLookFor);
	static bool				Contains(wchar_t* strToSearch, wchar_t* strToLookFor);
	static bool				ContainsIgnoreCase(char* strToSearch, char* strToLookFor);
	static bool				ContainsIgnoreCase(wchar_t* strToSearch, wchar_t* strToLookFor);
	static bool				ConvertDoubleToBool(double num);
	static bool				ConvertFloatToBool(float num);
	static bool				ConvertIntToBool(int num);
	static char*			ConvertIntToString(int integerToConvert);
	static wchar_t*			ConvertIntToWideString(int integerToConvert);
	static vector<char*>	ConvertStringArrayToStringVectorArray(char** stringArray);
	static vector<wchar_t*> ConvertStringArrayToStringVectorArray(wchar_t** stringArray);
	static bool				ConvertStringToBool(char* str);
	static bool				ConvertStringToBool(wchar_t* str);
	static float*			ConvertStringToFloatArray(char* strToConvert, char* delimiter, int& outNumItems);
	static float*			ConvertStringToFloatArray(wchar_t* strToConvert, wchar_t* delimiter, int& outNumItems);
	static unsigned int		ConvertStringToHex(char* str);
	static unsigned int		ConvertStringToHex(wchar_t* str);
	static char*			ConvertStringToOneLine(char* strToConvert);
	static wchar_t*			ConvertStringToOneLine(wchar_t* strToConvert);
	static void				ConvertStringToOneLineInPlace(char* strToConvert);
	static void				ConvertStringToOneLineInPlace(wchar_t* strToConvert);
	static wchar_t*			ConvertStringToWCharString(const char* strToConvert);
	static char**			ConvertStringVectorArrayToStringArray(vector<char*> stringVectorArray, int& outStringArrayLength);
	static wchar_t**		ConvertStringVectorArrayToStringArray(vector<wchar_t*> stringVectorArray, int& outStringArrayLength);
	static char				ConvertToLowerCase(char character);
	static wchar_t			ConvertToLowerCase(wchar_t character);
	static char				ConvertToUpperCase(char character);
	static wchar_t			ConvertToUpperCase(wchar_t character);
	static char*			ConvertToLowerCase(char* str);
	static wchar_t*			ConvertToLowerCase(wchar_t* str);
	static void				ConvertToLowerCaseInPlace(char* str);
	static void				ConvertToLowerCaseInPlace(wchar_t* str);
	static char*			ConvertToUpperCase(char* str);
	static wchar_t*			ConvertToUpperCase(wchar_t* str);
	static void				ConvertToUpperCaseInPlace(char* str);
	static void				ConvertToUpperCaseInPlace(wchar_t* str);
	static double			Cos(double numInRadians);
	static float			Cos(float numInRadians);
	static int				Cos(int numInRadians);
	static double			CosH(double numInRadians);
	static float			CosH(float numInRadians);
	static int				CosH(int numInRadians);
	static int				CountNumOccurrences(char* strToSearch, char charToLookFor);
	static int				CountNumOccurrences(wchar_t* strToSearch, wchar_t charToLookFor);
	static int				CountNumOccurrences(char* strToSearch, char* strToLookFor);
	static int				CountNumOccurrences(wchar_t* strToSearch, wchar_t* strToLookFor);
	static double			CubeRoot(double num);
	static float			CubeRoot(float num);
	static int				CubeRoot(int num);
	static double			DegreesToRadians(double degrees);
	static float			DegreesToRadians(float degrees);
	static int				DegreesToRadians(int degrees);
	static bool				DoesFileExist(char* fullFilePathWithNameAndExt);
	static bool				DoesFileExist(wchar_t* fullFilePathWithNameAndExt);
	static double			ErrorFunction(double num);
	static float			ErrorFunction(float num);
	static int				ErrorFunction(int num);
	static bool				Equals(char* strToCompOne, char* strToCompTwo);
	static bool				Equals(wchar_t* strToCompOne, wchar_t* strToCompTwo);
	static bool				EqualsIgnoreCase(char* strToCompOne, char* strToCompTwo);
	static bool				EqualsIgnoreCase(wchar_t* strToCompOne, wchar_t* strToCompTwo);
	static double			EToTheXPower(double power);
	static float			EToTheXPower(float power);
	static int				EToTheXPower(int power);
	static double			EToTheXPowerMinusOne(double power);
	static float			EToTheXPowerMinusOne(float power);
	static int				EToTheXPowerMinusOne(int power);
	static char				FirstChar(char* str);
	static wchar_t			FirstChar(wchar_t* str);
	static double			Floor(double num);
	static float			Floor(float num);
	static int				Floor(int num);
	static double			GammaFunction(double num);
	static float			GammaFunction(float num);
	static int				GammaFunction(int num);
	static int				GenerateRandomNum(int min, int max);
	static char*			GetHResultDescription(HRESULT result);
	static wchar_t*			GetHResultDescriptionW(HRESULT result);
	static void				GetImageSize(char* fileName, int* outWidth, int* outHeight);
	static void				GetImageSizeW(wchar_t* fileName, int* outWidth, int* outHeight);
	static double			Hypotenuse(double x, double y);
	static float			Hypotenuse(float x, float y);
	static int				Hypotenuse(int x, int y);
	static int				IndexOf(char* strToSearch, char charToLookFor);
	static int				IndexOf(wchar_t* strToSearch, wchar_t charToLookFor);
	static int				IndexOf(char* strToSearch, char* strToLookFor);
	static int				IndexOf(wchar_t* strToSearch, wchar_t* strToLookFor);
	static int				IndexOfFirstLetter(char* strToSearch);
	static int				IndexOfFirstLetter(wchar_t* strToSearch);
	static int				IndexOfIgnoreCase(char* strToSearch, char charToLookFor);
	static int				IndexOfIgnoreCase(wchar_t* strToSearch, wchar_t charToLookFor);
	static int				IndexOfIgnoreCase(char* strToSearch, char* strToLookFor);
	static int				IndexOfIgnoreCase(wchar_t* strToSearch, wchar_t* strToLookFor);
	static char*			InsertString(char* strToReceive, char* strToInsert, int startIndex);
	static wchar_t*			InsertString(wchar_t* strToReceive, wchar_t* strToInsert, int startIndex);
	static bool				IsAlphabetic(char character);
	static bool				IsAlphabetic(wchar_t character);
	static bool				IsDigit(char character);
	static bool				IsDigit(wchar_t character);
	static bool				IsEmpty(char* str);
	static bool				IsEmpty(wchar_t* str);
	static bool				IsNumber(char* str);
	static bool				IsNumber(wchar_t* str);
	static bool				IsPathADevice(const char* path);
	static bool				IsPathADevice(const wchar_t* path);
	static bool				IsWhitespace(char character);
	static bool				IsWhitespace(wchar_t character);
	static double			Largest(double num1, double num2);
	static float			Largest(float num1, float num2);
	static int				Largest(int num1, int num2);
	static char				LastChar(char* str);
	static wchar_t			LastChar(wchar_t* str);
	static int				LastIndexOf(char* strToSearch, char* strToLookFor);
	static int				LastIndexOf(wchar_t* strToSearch, wchar_t* strToLookFor);
	static double			LogBaseTen(double num);
	static float			LogBaseTen(float num);
	static int				LogBaseTen(int num);
	static double			LogBaseTwo(double num);
	static float			LogBaseTwo(float num);
	static int				LogBaseTwo(int num);
	static double			Mod(double numerator, double denominator);
	static float			Mod(float numerator, float denominator);
	static int				Mod(int numerator, int denominator);
	static double			MultiplyThenAdd(double multOne, double multTwo, double add);
	static float			MultiplyThenAdd(float multOne, float multTwo, float add);
	static int				MultiplyThenAdd(int multOne, int multTwo, int add);
	static double			NaturalLog(double num);
	static float			NaturalLog(float num);
	static int				NaturalLog(int num);
	static double			NaturalLogGammaFunction(double num);
	static float			NaturalLogGammaFunction(float num);
	static int				NaturalLogGammaFunction(int num);
	static int				NextIndexOf(char* strToSearch, char charToLookFor, int startIndex);
	static int				NextIndexOf(wchar_t* strToSearch, wchar_t charToLookFor, int startIndex);
	static int				NextIndexOf(char* strToSearch, char* strToLookFor, int startIndex);
	static int				NextIndexOf(wchar_t* strToSearch, wchar_t* strToLookFor, int startIndex);
	static double			NumTimesTwoToTheXPower(double num, double power);
	static float			NumTimesTwoToTheXPower(float num, float power);
	static int				NumTimesTwoToTheXPower(int num, int power);
	static double			Power(double base, double exponent);
	static float			Power(float base, float exponent);
	static int				Power(int base, int exponent);
	static double			PositiveDifference(double numOne, double numTwo);
	static float			PositiveDifference(float numOne, float numTwo);
	static int				PositiveDifference(int numOne, int numTwo);
	static double			RadiansToDegrees(double radians);
	static float			RadiansToDegrees(float radians);
	static int				RadiansToDegrees(int radians);
	static char*			ReadTextFile(char* filePath, size_t numBytesToRead = 0, size_t maxBufferSize = MAX_STRING_BUFFER_SIZE, const char* mode = "r", int seekOffset = 0, int seekOrigin = SEEK_SET, long* outEndCursorPos = NULL);
	static wchar_t*			ReadTextFileW(wchar_t* filePath, size_t numBytesToRead = 0, size_t maxBufferSize = MAX_STRING_BUFFER_SIZE, const wchar_t* mode = L"r, ccs=UTF-8", int seekOffset = 0, int seekOrigin = SEEK_SET, long* outEndCursorPos = NULL);
	static char*			ReadBinaryFile(char* filePath, size_t numBytesToRead = 0, size_t maxBufferSize = MAX_STRING_BUFFER_SIZE, const char* mode = "rb", int seekOffset = 0, int seekOrigin = SEEK_SET, long* outEndCursorPos = NULL);
	static wchar_t*			ReadBinaryFileW(wchar_t* filePath, size_t numBytesToRead = 0, size_t maxBufferSize = MAX_STRING_BUFFER_SIZE, const wchar_t* mode = L"rb, ccs=UTF-8", int seekOffset = 0, int seekOrigin = SEEK_SET, long* outEndCursorPos = NULL);
	static vector<char*>	RetrieveFileNames(char* fullFilePath);
	static double			Remainder(double numerator, double denominator);
	static float			Remainder(float numerator, float denominator);
	static int				Remainder(int numerator, int denominator);
	static double			RemainderWithQuadrant(double numerator, double denominator, int* outQuadrant);
	static float			RemainderWithQuadrant(float numerator, float denominator, int* outQuadrant);
	static int				RemainderWithQuadrant(int numerator, int denominator, int* outQuadrant);
	static char*			RemoveAllWhitespace(char* str);
	static wchar_t*			RemoveAllWhitespace(wchar_t* str);
	static void				RemoveAllWhitespaceInPlace(char* str);
	static void				RemoveAllWhitespaceInPlace(wchar_t* str);
	static char*			RemoveCharactersFromString(char* strToRemoveFrom, char charToRemove);
	static wchar_t*			RemoveCharactersFromString(wchar_t* strToRemoveFrom, wchar_t charToRemove);
	static void				RemoveCharactersFromStringInPlace(char* strToRemoveFrom, char charToRemove);
	static void				RemoveCharactersFromStringInPlace(wchar_t* strToRemoveFrom, wchar_t charToRemove);
	static char*			RemoveStringFromString(char* strToRemoveFrom, char* strToRemove, bool removeOnlyFirstOccurrence = false);
	static wchar_t*			RemoveStringFromString(wchar_t* strToRemoveFrom, wchar_t* strToRemove, bool removeOnlyFirstOccurrence = false);
	static void				RemoveStringFromStringInPlace(char*& strToRemoveFrom, char* strToRemove, bool removeOnlyFirstOccurrence = false);
	static void				RemoveStringFromStringInPlace(wchar_t*& strToRemoveFrom, wchar_t* strToRemove, bool removeOnlyFirstOccurrence = false);
	static char*			ReplaceString(char* strToSearch, char* strToLookFor, char* strToReplaceWith, bool removeOnlyFirstOccurrence = false);
	static wchar_t*			ReplaceString(wchar_t* strToSearch, wchar_t* strToLookFor, wchar_t* strToReplaceWith, bool removeOnlyFirstOccurrence = false);
	static void				ReplaceStringInPlace(char*& strToSearch, char* strToLookFor, char* strToReplaceWith, bool removeOnlyFirstOccurrence = false);
	static void				ReplaceStringInPlace(wchar_t*& strToSearch, wchar_t* strToLookFor, wchar_t* strToReplaceWith, bool removeOnlyFirstOccurrence = false);
	static char*			RetrieveDataBetweenTags(char* fileData, char* startTag, char* endTag);
	static wchar_t*			RetrieveDataBetweenTags(wchar_t* fileData, wchar_t* startTag, wchar_t* endTag);
	static char*			RetrieveDataBetweenTags(char* fileData, int startTagIndex, int endTagIndex);
	static wchar_t*			RetrieveDataBetweenTags(wchar_t* fileData, int startTagIndex, int endTagIndex);
	static char**			RetrieveDataChunksBetweenTags(char* fileData, char* startTag, char* endTag, int& outFinalBufferSize);
	static wchar_t**		RetrieveDataChunksBetweenTags(wchar_t* fileData, wchar_t* startTag, wchar_t* endTag, int& outFinalBufferSize);
	static vector<wchar_t*> RetrieveFileNames(wchar_t* fullFilePath);
	static vector<char*>	RetrieveFolderNames(char* fullFilePath);
	static vector<wchar_t*> RetrieveFolderNames(wchar_t* fullFilePath);
	static char*			ReverseString(char* strToReverse);
	static wchar_t*			ReverseString(wchar_t* strToReverse);
	static void				ReverseStringInPlace(char* strToReverse);
	static void				ReverseStringInPlace(wchar_t* strToReverse);
	static long				RoundToNearestInteger(double num);
	static long				RoundToNearestInteger(float num);
	static long				RoundToNearestInteger(int num);
	static long				RoundUp(double num);
	static long				RoundUp(float num);
	static long				RoundUp(int num);
	static void				ShowMessageBox(HWND hWindow, char* message, char* title = "Error");
	static void				ShowMessageBox(HWND hWindow, wchar_t* message, wchar_t* title = L"Error");
	static double			Sin(double numInRadians);
	static float			Sin(float numInRadians);
	static int				Sin(int numInRadians);
	static double			SinH(double numInRadians);
	static float			SinH(float numInRadians);
	static int				SinH(int numInRadians);
	static double			Smallest(double num1, double num2);
	static float			Smallest(float num1, float num2);
	static int				Smallest(int num1, int num2);
	static double			SquareRoot(double num);
	static float			SquareRoot(float num);
	static int				SquareRoot(int num);
	static char*			SubString(char* str, int startIndex, int upToButNotIncludingIndex = INT_MAX);
	static wchar_t*			SubString(wchar_t* str, int startIndex, int upToButNotIncludingIndex = INT_MAX);
	static double			Tan(double numInRadians);
	static float			Tan(float numInRadians);
	static int				Tan(int numInRadians);
	static double			TanH(double numInRadians);
	static float			TanH(float numInRadians);
	static int				TanH(int numInRadians);
	static char**			Tokenize(char* strToTokenize, char* delimiter, int& outNumTokensFound);
	static wchar_t**		Tokenize(wchar_t* strToTokenize, wchar_t* delimiter, int& outNumTokensFound);
	static double			Truncate(double num);
	static float			Truncate(float num);
	static double			TwoToTheXPower(double power);
	static float			TwoToTheXPower(float power);
	static int				TwoToTheXPower(int power);
	static bool				Validate(HWND hWnd, HRESULT result, char* message, char* title = "ERROR");
	static bool				Validate(HWND hWnd, HRESULT result, wchar_t* message, wchar_t* title = L"ERROR");
	static size_t			WriteTextFile(char* filePath, char* bufferToWrite, size_t bufferSize, const char* mode = "w", int seekOffset = 0, int seekOrigin = SEEK_SET, long* outEndCursorPos = NULL);
	static size_t			WriteTextFileW(wchar_t* filePath, wchar_t* bufferToWrite, size_t bufferSize, const wchar_t* mode = L"w, ccs=UTF-8", int seekOffset = 0, int seekOrigin = SEEK_SET, long* outEndCursorPos = NULL);
	static size_t			WriteBinaryFile(char* filePath, char* bufferToWrite, size_t bufferSize, const char* mode = "wb", int seekOffset = 0, int seekOrigin = SEEK_SET, long* outEndCursorPos = NULL);
	static size_t			WriteBinaryFileW(wchar_t* filePath, wchar_t* bufferToWrite, size_t bufferSize, const wchar_t* mode = L"wb, ccs=UTF-8", int seekOffset = 0, int seekOrigin = SEEK_SET, long* outEndCursorPos = NULL);
};