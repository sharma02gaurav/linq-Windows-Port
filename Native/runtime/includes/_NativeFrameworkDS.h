#pragma once

#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <stdbool.h>
#include <algorithm>

using namespace std;

#ifndef EXCEPTION_DEFS
	#define EXCEPTION_DEFS

	#define __ERRFILE __FILE__
	#define __ERRFUNC __func__
	#define __ERRLINE __LINE__
#endif

#define _DSException(Error_Message) _NativeFrameworkDSException(Error_Message, __ERRFILE, __ERRFUNC, __ERRLINE)

// the execption handler for these classes
class _NativeFrameworkDSException;
class _NativeFrameworkDSException{
	private:
		string errorMessage;
		string filename;
		string function;
		int line;

		void _printError(){
			cerr << "ERROR: " ;
			cerr << errorMessage << " \nin " << filename << " on line " << line << " in function " << function << endl;
			cerr << "Aborting" << endl;
		}
	public:
		_NativeFrameworkDSException(string, string, string, int);
		string toString();
};

//defining the string class
#define EmptyString  "\0"
// the virtual operators
#define equals ==
#define append +
#define mul *
#define assign =

using namespace std;

class _NativeFrameworkDSString{
	private:
		string variableString;

		// converting to upper struct
		struct Upper{
			void operator()(char& character){
				character = toupper(character);
			}
		};
		// converting to lower struct
		struct Lower{
			void operator()(char& character){
				character = tolower(character);
			}
		};

	public:
		_NativeFrameworkDSString(char*);
		_NativeFrameworkDSString(string);
		_NativeFrameworkDSString(const _NativeFrameworkDSString&);
		_NativeFrameworkDSString();

		/*
		*	utility methods implementation
		*/
		// get the length of the string
		int length();
		
		// get the character the specified index
		char charAt(int);
		
		// get the first occurence index of the charcter represented by the char
		int firstIndexOf(char);
		int firstIndexOf(char*);
		int firstIndexOf(string);
		int firstIndexOf(_NativeFrameworkDSString);	// _linqString is an alias for this class
		// defined in V1.1
		int indexOf(char);
		int indexOf(char*);
		int indexOf(string);
		int indexOf(_NativeFrameworkDSString);
		
		// get the last index of the string/char
		int lastIndexOf(char);
		int lastIndexOf(char*);
		int lastIndexOf(string);
		int lastIndexOf(_NativeFrameworkDSString);
		
		/*	create a substring of this class
		*	@Param 1 refers to the start position
		*	@Param 2 refers to the end position
		*/
		char* substring(int, int);
		//	creates a substring of a string from index 0 to the offset position
		char* substring(int);

		//checks for the equality of two strings
		bool equalsTo(char*);
		bool equalsTo(string);
		bool equalsTo(_NativeFrameworkDSString);

		//checks for equality ignoring the case
		bool equalsIgnoringCase(char*);
		bool equalsIgnoringCase(string);
		bool equalsIgnoringCase(_NativeFrameworkDSString);

		// checks if the string is empty
		bool isEmpty();
		// checks if the string only contains whitespaces
		// V1.1
		bool isWhitespaceOnly();

		// get the reverse of the string
		char* getReverse();

		// check if the string is palindrome
		bool isPalindrome();

		// convert the string to lowercase
		char* toLowerCase();
		// V1.1
		char* toLowercase();
		// convert the string to uppercase
		char* toUpperCase();
		// V1.1
		char* toUppercase();

		// counts the occurance of a character in the string
		int countOccurance(char);

		// check of the string contains char
		bool contains(char);
		bool contains(char*);
		bool contains(_NativeFrameworkDSString);
		bool contains(string);

		/*
		*	replace all the param1 characters with param2
		*/
		void replace(char, char);
		/*	repplace one string blocks with another
		*/
		void replace(char*, char*);
		void replace(_NativeFrameworkDSString, _NativeFrameworkDSString);
		void replace(string, string);

		// return true if the string starts with the character in parameter
		bool startWith(char);
		// V1.1
		bool startWith(char*);
		bool startWith(string);
		bool startWith(_NativeFrameworkDSString);
		bool startsWith(char);
		bool startsWith(char*);
		bool startsWith(string);
		bool startsWith(_NativeFrameworkDSString);
		// returns true if the string ends with the character in parameter
		bool endWith(char);
		// V1.1
		bool endWith(char*);
		bool endWith(string);
		bool endWith(_NativeFrameworkDSString);
		bool endsWith(char);
		bool endsWith(char*);
		bool endsWith(string);
		bool endsWith(_NativeFrameworkDSString);

		string getString() const;
		char* getCString() const;
		/*string setting functions*/
		void setString(string);
		void setString(char*);
		void setString(_NativeFrameworkDSString);

		// type-binding to native type
		operator char*() const{return (char*)variableString.c_str();}
		//operators
		/*
		*	appending strings
		*/
		_NativeFrameworkDSString operator+(_NativeFrameworkDSString);
		_NativeFrameworkDSString operator+=(_NativeFrameworkDSString);
		_NativeFrameworkDSString operator+(char);
		_NativeFrameworkDSString operator+=(char);
		_NativeFrameworkDSString operator+(char*);
		_NativeFrameworkDSString operator+=(char*);
		_NativeFrameworkDSString operator+(string);
		_NativeFrameworkDSString operator+=(string);
		/*
		*	appending the primitives
		*/
		_NativeFrameworkDSString operator+(int);
		_NativeFrameworkDSString operator+=(int);
		_NativeFrameworkDSString operator+(long);
		_NativeFrameworkDSString operator+=(long);
		_NativeFrameworkDSString operator+(double);
		_NativeFrameworkDSString operator+=(double);

		/*
		*	The realtime functions
		*/
		/*	String multiply function.
		*	e.g. if string is "hello" then "hello"*2 would return
		*	"hello hello"
		*/
		_NativeFrameworkDSString operator*(int);
		_NativeFrameworkDSString operator*=(int);
		_NativeFrameworkDSString operator*(long);
		_NativeFrameworkDSString operator*=(long);

		/*
		*	equality operators
		*/
		bool operator==(_NativeFrameworkDSString);
		bool operator==(char*);
		bool operator==(string);
		bool operator!=(_NativeFrameworkDSString);
		bool operator!=(char*);
		bool operator!=(string);
};

/*
*	The integer wrapper class
*/
class _NativeFrameworkDSNumber{
	private:
		long number;
	public:
		_NativeFrameworkDSNumber();
		_NativeFrameworkDSNumber(int);
		_NativeFrameworkDSNumber(long);

		long getNumber();
		string toString();

		//operators
		_NativeFrameworkDSNumber operator+(_NativeFrameworkDSNumber);
		_NativeFrameworkDSNumber operator-(_NativeFrameworkDSNumber);
		_NativeFrameworkDSNumber operator*(_NativeFrameworkDSNumber);
		_NativeFrameworkDSNumber operator/(_NativeFrameworkDSNumber);
		_NativeFrameworkDSNumber operator%(_NativeFrameworkDSNumber);
		void operator+=(_NativeFrameworkDSNumber);
		void operator-=(_NativeFrameworkDSNumber);
		void operator*=(_NativeFrameworkDSNumber);
		void operator/=(_NativeFrameworkDSNumber);
		
		_NativeFrameworkDSNumber operator++();
		_NativeFrameworkDSNumber operator++(int);
		_NativeFrameworkDSNumber operator--();
		_NativeFrameworkDSNumber operator--(int);

		_NativeFrameworkDSNumber operator+(long);
		_NativeFrameworkDSNumber operator-(long);
		_NativeFrameworkDSNumber operator*(long);
		_NativeFrameworkDSNumber operator/(long);
		_NativeFrameworkDSNumber operator%(long);
		void operator+=(long);
		void operator-=(long);
		void operator*=(long);
		void operator/=(long);

		_NativeFrameworkDSNumber operator+(int);
		_NativeFrameworkDSNumber operator-(int);
		_NativeFrameworkDSNumber operator*(int);
		_NativeFrameworkDSNumber operator/(int);
		_NativeFrameworkDSNumber operator%(int);
		void operator+=(int);
		void operator-=(int);
		void operator*=(int);
		void operator/=(int);

		_NativeFrameworkDSNumber operator+(double);
		_NativeFrameworkDSNumber operator-(double);
		_NativeFrameworkDSNumber operator*(double);
		_NativeFrameworkDSNumber operator/(double);
		void operator+=(double);
		void operator-=(double);
		void operator*=(double);
		void operator/=(double);

		// V1.1
		_NativeFrameworkDSString operator+(const char*);
		_NativeFrameworkDSString operator+(_NativeFrameworkDSString);

		// comparison operators
		bool operator==(_NativeFrameworkDSNumber);
		bool operator==(int);
		bool operator==(long);
		bool operator!=(_NativeFrameworkDSNumber);
		bool operator!=(int);
		bool operator!=(long);
		// v1.1
		bool operator<(int);
		bool operator<(long);
		bool operator>(int);
		bool operator>(long);
		bool operator<=(int);
		bool operator<=(long);
		bool operator>=(int);
		bool operator>=(long);

		operator long() const{return static_cast<long>(number);}
};

/*
*	The double wrapper class
*/
class _NativeFrameworkDSDecimal{
	private:
		double value;
	public:
		_NativeFrameworkDSDecimal();
		_NativeFrameworkDSDecimal(double);
		_NativeFrameworkDSDecimal(int);

		double getDecimal();
		string toString();

		// operators
		_NativeFrameworkDSDecimal operator+(_NativeFrameworkDSDecimal);
		_NativeFrameworkDSDecimal operator-(_NativeFrameworkDSDecimal);
		_NativeFrameworkDSDecimal operator*(_NativeFrameworkDSDecimal);
		_NativeFrameworkDSDecimal operator/(_NativeFrameworkDSDecimal);
		void operator+=(_NativeFrameworkDSDecimal);
		void operator-=(_NativeFrameworkDSDecimal);
		void operator*=(_NativeFrameworkDSDecimal);
		void operator/=(_NativeFrameworkDSDecimal);
		
		_NativeFrameworkDSDecimal operator++();
		_NativeFrameworkDSDecimal operator--();

		_NativeFrameworkDSDecimal operator+(long);
		_NativeFrameworkDSDecimal operator-(long);
		_NativeFrameworkDSDecimal operator*(long);
		_NativeFrameworkDSDecimal operator/(long);
		void operator+=(long);
		void operator-=(long);
		void operator*=(long);
		void operator/=(long);

		_NativeFrameworkDSDecimal operator+(double);
		_NativeFrameworkDSDecimal operator-(double);
		_NativeFrameworkDSDecimal operator*(double);
		_NativeFrameworkDSDecimal operator/(double);
		void operator+=(double);
		void operator-=(double);
		void operator*=(double);
		void operator/=(double);

		//V1.1
		_NativeFrameworkDSString operator+(const char*);
		_NativeFrameworkDSString operator+(_NativeFrameworkDSString);

		/*comparison operator*/
		bool operator==(_NativeFrameworkDSDecimal);
		bool operator==(double);
		bool operator!=(_NativeFrameworkDSDecimal);
		bool operator!=(double);

		// v1.1 functions
		bool operator<(double);
		bool operator<=(double);
		bool operator>(double);
		bool operator>=(double);

		operator double() const{return value;}
};

#ifndef DATATYPES_ENUM
	#define DATATYPES_ENUM
	typedef enum DataTypes{
		LONG,
		DOUBLE,
		CHAR,
		C_STR,
		STRING,
		BOOLEAN
	};
#endif

class _NativeFrameworkDSVariant{
private:
		long longValue;
		double doubleValue;
		char charValue;
		string stringValue;
		bool booleanValue;

		DataTypes activeData;
	public:
		_NativeFrameworkDSVariant();
		/*the valid constructor for this class*/
		_NativeFrameworkDSVariant(int);
		_NativeFrameworkDSVariant(short);
		_NativeFrameworkDSVariant(long);
		_NativeFrameworkDSVariant(double);
		_NativeFrameworkDSVariant(float);
		_NativeFrameworkDSVariant(char);
		_NativeFrameworkDSVariant(string);
		_NativeFrameworkDSVariant(char*);
		_NativeFrameworkDSVariant(bool);

		//getter methods for this variant
		long getInteger();
		long getNumber();
		long getShort();
		long getLong();
		double getDecimal();
		double getFloat();
		char getChar();
		string getString();
		bool getBool();
		DataTypes getType();

		// operator overloading
		bool operator==(_NativeFrameworkDSVariant);
		bool operator!=(_NativeFrameworkDSVariant);

		//type convesions
		operator char*() const{
			ostringstream output;
			switch(activeData){
				case LONG:	output << longValue; break;
				case DOUBLE: output << doubleValue; break;
				case CHAR: output << charValue; break;
				case STRING: return (char*)stringValue.c_str();
				case C_STR: return (char*)stringValue.c_str();
				case BOOLEAN: output << booleanValue;
			}
			return (char*)output.str().c_str();
		}
};