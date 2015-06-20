#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

/*
*	class to convert the primitives data types
*/

class _NativeFrameworkPrimitivesConverter{
	public:
		// to string conversion
		static string toString(int);
		static string toString(long);
		static string toString(double);
		// to int conversion
		static int toInt(string);
		static int toInt(char*);
		static int toInt(double);
		//to long conversion
		static long toLong(string);
		static long toLong(char*);
		static long toNumber(string);
		static long toNumber(char*);
		// to double conversion	->	needs implementation
		static double toDouble(string);
		static double toDouble(char*);
		static double toDouble(long);
		static double toDecimal(string);
		static double toDecimal(char*);
		static double toDecimal(long);
};