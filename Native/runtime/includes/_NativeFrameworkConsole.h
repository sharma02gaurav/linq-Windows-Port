#pragma once

#include <iostream>
#include <string>
#include <stdbool.h>
#include <sstream>
#include <stdlib.h>	// for system

#include "_NativeFrameworkDS.h"
#include "_NativeFrameworkObjectFacade.h"

using namespace std;

#define output(__VALUE) _NativeFrameworkConsole::print((_NativeFrameworkDSString)__VALUE)
#define outputLine(__VALUE) _NativeFrameworkConsole::printLine((_NativeFrameworkDSString)__VALUE)

// to define the console color
enum __FrameworkColors{
	Black, 
	Blue, 
	Green, 
	Aqua, 
	Red, 
	Purple, 
	Yellow, 
	Silver, 
	Gray, 
	LightBlue, 
	LightGreen, 
	LightAqua,
	LightRed, 
	LightPurple,
	LightYellow,
	White
};


// this class is used by the _NativeFrameworkConsole class (only).
class _NativeFrameworkConsoleColors{
	private:
		/*
		*	returns the color code for the requested color. The color codes are defined in defines
		*/
		static string _getColorCode(__FrameworkColors);
		// create the final executable statement
		static char* _compileStatement(string, string);
	public:
		/*
		*	set the console colors
		*	@Param1 for background
		*	@Param2 for text
		*/
		static void setColor(__FrameworkColors, __FrameworkColors);
		/*
		*	change to default colors
		*/
		static void defaultColor();
};


class _NativeFrameworkConsole{
	private:
		/*
		*	private overriden read messages to be used by read and read****() methods.
		*/
		static void _readDefault(string&);
		static void _readDefault(char*);
		static void _readDefault(long&);
		static void _readDefault(double&);
		static void _readDefault(char&);
	public:
		/*
		*	printing functions
		*/
		static void print(string);
		static void print(char*);
		static void print(long);
		static void print(double);
		static void print(char);
		// V1.1
		static void print(_NativeFrameworkObject);
		/*
		*	printing line functions
		*/
		static void printLine();
		static void printLine(string);
		static void printLine(char*);
		static void printLine(long);
		static void printLine(double);
		static void printLine(char);
		// V1.1
		static void printLine(_NativeFrameworkObject);

		// the explicit read functions
		static void read(string&);
		static void read(char*);
		static void read(long&);
		static void read(double&);
		static void read(char&);

		// the dynamic read functions
		static long readLong();
		static long readNumber();
		static double readDouble();
		static double readDecimal();
		static string readString();
		static char* readCString();
		static char readChar();

		//change the console colors
		static void setConsoleColor(__FrameworkColors, __FrameworkColors);
		static void setDefaultColor();
};