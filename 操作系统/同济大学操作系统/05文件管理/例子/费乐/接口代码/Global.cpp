#include "StdAfx.h"
#include "Global.h"
char MainName[FILENAME] = "MAINDIR";

int MaxValue(int value1, int value2)
{
	if (value1 > value2)
		return value1;
	return value2;
}

int MinValue(int value1, int value2)
{
	if (value1 < value2)
		return value1;
	return value2;
}